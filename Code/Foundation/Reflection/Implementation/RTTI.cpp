#include <Foundation/FoundationPCH.h>

#include <Foundation/Reflection/Implementation/AbstractProperty.h>
#include <Foundation/Reflection/Implementation/MessageHandler.h>

#include <Foundation/Communication/Message.h>
#include <Foundation/Configuration/Startup.h>
#include <Foundation/Containers/HashTable.h>

struct nsTypeData
{
  nsMutex m_Mutex;
  nsHashTable<nsUInt64, nsRTTI*, nsHashHelper<nsUInt64>, nsStaticsAllocatorWrapper> m_TypeNameHashToType;
  nsDynamicArray<nsRTTI*> m_AllTypes;

  bool m_bIterating = false;
};

nsTypeData* GetTypeData()
{
  // Prevent static initialization hazard between first nsRTTI instance
  // and type data and also make sure it is sufficiently sized before first use.
  auto CreateData = []() -> nsTypeData*
  {
    nsTypeData* pData = new nsTypeData();
    pData->m_TypeNameHashToType.Reserve(512);
    pData->m_AllTypes.Reserve(512);
    return pData;
  };
  static nsTypeData* pData = CreateData();
  return pData;
}

// clang-format off
NS_BEGIN_SUBSYSTEM_DECLARATION(Foundation, Reflection)

  //BEGIN_SUBSYSTEM_DEPENDENCIES
  //  "FileSystem"
  //END_SUBSYSTEM_DEPENDENCIES

  ON_CORESYSTEMS_STARTUP
  {
    nsPlugin::Events().AddEventHandler(nsRTTI::PluginEventHandler);
    nsRTTI::AssignPlugin("Static");
  }

  ON_CORESYSTEMS_SHUTDOWN
  {
    nsPlugin::Events().RemoveEventHandler(nsRTTI::PluginEventHandler);
  }

NS_END_SUBSYSTEM_DECLARATION;
// clang-format on

nsRTTI::nsRTTI(nsStringView sName, const nsRTTI* pParentType, nsUInt32 uiTypeSize, nsUInt32 uiTypeVersion, nsUInt8 uiVariantType,
  nsBitflags<nsTypeFlags> flags, nsRTTIAllocator* pAllocator, nsArrayPtr<const nsAbstractProperty*> properties, nsArrayPtr<const nsAbstractFunctionProperty*> functions,
  nsArrayPtr<const nsPropertyAttribute*> attributes, nsArrayPtr<nsAbstractMessageHandler*> messageHandlers, nsArrayPtr<nsMessageSenderInfo> messageSenders,
  const nsRTTI* (*fnVerifyParent)())
  : m_sTypeName(sName)
  , m_Properties(properties)
  , m_Functions(functions)
  , m_Attributes(attributes)
  , m_pAllocator(pAllocator)
  , m_VerifyParent(fnVerifyParent)
  , m_MessageHandlers(messageHandlers)
  , m_MessageSenders(messageSenders)
{
  UpdateType(pParentType, uiTypeSize, uiTypeVersion, uiVariantType, flags);

  // This part is not guaranteed to always work here!
  // pParentType is (apparently) always the correct pointer to the base class BUT it is not guaranteed to have been constructed at this
  // point in time! Therefore the message handler hierarchy is initialized delayed in DispatchMessage
  //
  // However, I don't know where we could do these debug checks where they are guaranteed to be executed.
  // For now they are executed here and one might also do that in e.g. the game application
  {
#if NS_ENABLED(NS_COMPILE_FOR_DEVELOPMENT)
    VerifyCorrectness();
#endif
  }

  if (!m_sTypeName.IsEmpty())
  {
    RegisterType();
  }
}

nsRTTI::~nsRTTI()
{
  if (!m_sTypeName.IsEmpty())
  {
    UnregisterType();
  }
}

void nsRTTI::GatherDynamicMessageHandlers()
{
  // This cannot be done in the constructor, because the parent types are not guaranteed to be initialized at that point

  if (m_uiMsgIdOffset != nsSmallInvalidIndex)
    return;

  m_uiMsgIdOffset = 0;

  nsUInt16 uiMinMsgId = nsSmallInvalidIndex;
  nsUInt16 uiMaxMsgId = 0;

  const nsRTTI* pInstance = this;
  while (pInstance != nullptr)
  {
    for (nsUInt32 i = 0; i < pInstance->m_MessageHandlers.GetCount(); ++i)
    {
      nsUInt16 id = pInstance->m_MessageHandlers[i]->GetMessageId();
      uiMinMsgId = nsMath::Min(uiMinMsgId, id);
      uiMaxMsgId = nsMath::Max(uiMaxMsgId, id);
    }

    pInstance = pInstance->m_pParentType;
  }

  if (uiMinMsgId != nsSmallInvalidIndex)
  {
    m_uiMsgIdOffset = uiMinMsgId;
    nsUInt16 uiNeededCapacity = uiMaxMsgId - uiMinMsgId + 1;

    m_DynamicMessageHandlers.SetCount(uiNeededCapacity);

    pInstance = this;
    while (pInstance != nullptr)
    {
      for (nsUInt32 i = 0; i < pInstance->m_MessageHandlers.GetCount(); ++i)
      {
        nsAbstractMessageHandler* pHandler = pInstance->m_MessageHandlers[i];
        nsUInt16 uiIndex = pHandler->GetMessageId() - m_uiMsgIdOffset;

        // this check ensures that handlers in base classes do not override the derived handlers
        if (m_DynamicMessageHandlers[uiIndex] == nullptr)
        {
          m_DynamicMessageHandlers[uiIndex] = pHandler;
        }
      }

      pInstance = pInstance->m_pParentType;
    }
  }
}

void nsRTTI::SetupParentHierarchy()
{
  m_ParentHierarchy.Clear();

  for (const nsRTTI* rtti = this; rtti != nullptr; rtti = rtti->m_pParentType)
  {
    m_ParentHierarchy.PushBack(rtti);
  }
}

void nsRTTI::VerifyCorrectness() const
{
  if (m_VerifyParent != nullptr)
  {
    NS_ASSERT_DEV(m_VerifyParent() == m_pParentType, "Type '{0}': The given parent type '{1}' does not match the actual parent type '{2}'",
      m_sTypeName, (m_pParentType != nullptr) ? m_pParentType->GetTypeName() : "null",
      (m_VerifyParent() != nullptr) ? m_VerifyParent()->GetTypeName() : "null");
  }

  {
    nsSet<nsStringView> Known;

    const nsRTTI* pInstance = this;

    while (pInstance != nullptr)
    {
      for (nsUInt32 i = 0; i < pInstance->m_Properties.GetCount(); ++i)
      {
        const bool bNewProperty = !Known.Find(pInstance->m_Properties[i]->GetPropertyName()).IsValid();
        Known.Insert(pInstance->m_Properties[i]->GetPropertyName());

        NS_IGNORE_UNUSED(bNewProperty);
        NS_ASSERT_DEV(bNewProperty, "{0}: The property with name '{1}' is already defined in type '{2}'.", m_sTypeName,
          pInstance->m_Properties[i]->GetPropertyName(), pInstance->GetTypeName());
      }

      pInstance = pInstance->m_pParentType;
    }
  }

  {
    for (const nsAbstractProperty* pFunc : m_Functions)
    {
      NS_IGNORE_UNUSED(pFunc);
      NS_ASSERT_DEV(pFunc->GetCategory() == nsPropertyCategory::Function, "Invalid function property '{}'", pFunc->GetPropertyName());
    }
  }
}

void nsRTTI::VerifyCorrectnessForAllTypes()
{
  nsRTTI::ForEachType([](const nsRTTI* pRtti)
    { pRtti->VerifyCorrectness(); });
}


void nsRTTI::UpdateType(const nsRTTI* pParentType, nsUInt32 uiTypeSize, nsUInt32 uiTypeVersion, nsUInt8 uiVariantType, nsBitflags<nsTypeFlags> flags)
{
  m_pParentType = pParentType;
  m_uiVariantType = uiVariantType;
  m_uiTypeSize = uiTypeSize;
  m_uiTypeVersion = uiTypeVersion;
  m_TypeFlags = flags;
  m_ParentHierarchy.Clear();
}

void nsRTTI::RegisterType()
{
  m_uiTypeNameHash = nsHashingUtils::StringHash(m_sTypeName);

  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);
  pData->m_TypeNameHashToType.Insert(m_uiTypeNameHash, this);

  m_uiTypeIndex = pData->m_AllTypes.GetCount();
  pData->m_AllTypes.PushBack(this);
}

void nsRTTI::UnregisterType()
{
  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);
  pData->m_TypeNameHashToType.Remove(m_uiTypeNameHash);

  NS_ASSERT_DEV(pData->m_bIterating == false, "Unregistering types while iterating over types might cause unexpected behavior");
  pData->m_AllTypes.RemoveAtAndSwap(m_uiTypeIndex);
  if (m_uiTypeIndex != pData->m_AllTypes.GetCount())
  {
    pData->m_AllTypes[m_uiTypeIndex]->m_uiTypeIndex = m_uiTypeIndex;
  }
}

void nsRTTI::GetAllProperties(nsDynamicArray<const nsAbstractProperty*>& out_properties) const
{
  out_properties.Clear();

  if (m_pParentType)
    m_pParentType->GetAllProperties(out_properties);

  out_properties.PushBackRange(GetProperties());
}

const nsRTTI* nsRTTI::FindTypeByName(nsStringView sName)
{
  nsUInt64 uiNameHash = nsHashingUtils::StringHash(sName);

  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  nsRTTI* pType = nullptr;
  pData->m_TypeNameHashToType.TryGetValue(uiNameHash, pType);
  return pType;
}

const nsRTTI* nsRTTI::FindTypeByNameHash(nsUInt64 uiNameHash)
{
  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  nsRTTI* pType = nullptr;
  pData->m_TypeNameHashToType.TryGetValue(uiNameHash, pType);
  return pType;
}

const nsRTTI* nsRTTI::FindTypeByNameHash32(nsUInt32 uiNameHash)
{
  return FindTypeIf([=](const nsRTTI* pRtti)
    { return (nsHashingUtils::StringHashTo32(pRtti->GetTypeNameHash()) == uiNameHash); });
}

const nsRTTI* nsRTTI::FindTypeIf(PredicateFunc func)
{
  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  for (const nsRTTI* pRtti : pData->m_AllTypes)
  {
    if (func(pRtti))
    {
      return pRtti;
    }
  }

  return nullptr;
}

const nsAbstractProperty* nsRTTI::FindPropertyByName(nsStringView sName, bool bSearchBaseTypes /* = true */) const
{
  const nsRTTI* pInstance = this;

  do
  {
    for (nsUInt32 p = 0; p < pInstance->m_Properties.GetCount(); ++p)
    {
      if (pInstance->m_Properties[p]->GetPropertyName() == sName)
      {
        return pInstance->m_Properties[p];
      }
    }

    if (!bSearchBaseTypes)
      return nullptr;

    pInstance = pInstance->m_pParentType;
  } while (pInstance != nullptr);

  return nullptr;
}

bool nsRTTI::DispatchMessage(void* pInstance, nsMessage& ref_msg) const
{
  NS_ASSERT_DEBUG(m_uiMsgIdOffset != nsSmallInvalidIndex, "Message handler table should have been gathered at this point.\n"
                                                          "If this assert is triggered for a type loaded from a dynamic plugin,\n"
                                                          "you may have forgotten to instantiate an nsPlugin object inside your plugin DLL.");

  const nsUInt32 uiIndex = ref_msg.GetId() - m_uiMsgIdOffset;

  // m_DynamicMessageHandlers contains all message handlers of this type and all base types
  if (uiIndex < m_DynamicMessageHandlers.GetCount())
  {
    nsAbstractMessageHandler* pHandler = m_DynamicMessageHandlers.GetData()[uiIndex];
    if (pHandler != nullptr)
    {
      (*pHandler)(pInstance, ref_msg);
      return true;
    }
  }

  return false;
}

bool nsRTTI::DispatchMessage(const void* pInstance, nsMessage& ref_msg) const
{
  NS_ASSERT_DEBUG(m_uiMsgIdOffset != nsSmallInvalidIndex, "Message handler table should have been gathered at this point.\n"
                                                          "If this assert is triggered for a type loaded from a dynamic plugin,\n"
                                                          "you may have forgotten to instantiate an nsPlugin object inside your plugin DLL.");

  const nsUInt32 uiIndex = ref_msg.GetId() - m_uiMsgIdOffset;

  // m_DynamicMessageHandlers contains all message handlers of this type and all base types
  if (uiIndex < m_DynamicMessageHandlers.GetCount())
  {
    nsAbstractMessageHandler* pHandler = m_DynamicMessageHandlers.GetData()[uiIndex];
    if (pHandler != nullptr && pHandler->IsConst())
    {
      (*pHandler)(pInstance, ref_msg);
      return true;
    }
  }

  return false;
}

void nsRTTI::ForEachType(VisitorFunc func, nsBitflags<ForEachOptions> options /*= ForEachOptions::Default*/)
{
  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  pData->m_bIterating = true;
  // Can't use ranged based for loop here since we might add new types while iterating and the m_AllTypes array might re-allocate.
  for (nsUInt32 i = 0; i < pData->m_AllTypes.GetCount(); ++i)
  {
    auto pRtti = pData->m_AllTypes.GetData()[i];
    if (options.IsSet(ForEachOptions::ExcludeNonAllocatable) && (pRtti->GetAllocator() == nullptr || pRtti->GetAllocator()->CanAllocate() == false))
      continue;

    if (options.IsSet(ForEachOptions::ExcludeAbstract) && pRtti->GetTypeFlags().IsSet(nsTypeFlags::Abstract))
      continue;

    func(pRtti);
  }
  pData->m_bIterating = false;
}

void nsRTTI::ForEachDerivedType(const nsRTTI* pBaseType, VisitorFunc func, nsBitflags<ForEachOptions> options /*= ForEachOptions::Default*/)
{
  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  pData->m_bIterating = true;
  // Can't use ranged based for loop here since we might add new types while iterating and the m_AllTypes array might re-allocate.
  for (nsUInt32 i = 0; i < pData->m_AllTypes.GetCount(); ++i)
  {
    auto pRtti = pData->m_AllTypes.GetData()[i];
    if (!pRtti->IsDerivedFrom(pBaseType))
      continue;

    if (options.IsSet(ForEachOptions::ExcludeNonAllocatable) && (pRtti->GetAllocator() == nullptr || pRtti->GetAllocator()->CanAllocate() == false))
      continue;

    if (options.IsSet(ForEachOptions::ExcludeAbstract) && pRtti->GetTypeFlags().IsSet(nsTypeFlags::Abstract))
      continue;

    func(pRtti);
  }
  pData->m_bIterating = false;
}

void nsRTTI::AssignPlugin(nsStringView sPluginName)
{
  // assigns the given plugin name to every nsRTTI instance that has no plugin assigned yet

  auto pData = GetTypeData();
  NS_LOCK(pData->m_Mutex);

  for (nsRTTI* pRtti : pData->m_AllTypes)
  {
    if (pRtti->m_sPluginName.IsEmpty())
    {
      pRtti->m_sPluginName = sPluginName;
      SanityCheckType(pRtti);

      pRtti->SetupParentHierarchy();
      pRtti->GatherDynamicMessageHandlers();
    }
  }
}

#if NS_ENABLED(NS_COMPILE_FOR_DEBUG)

static bool IsValidIdentifierName(nsStringView sIdentifier)
{
  // empty strings are not valid
  if (sIdentifier.IsEmpty())
    return false;

  // digits are not allowed as the first character
  nsUInt32 uiChar = sIdentifier.GetCharacter();
  if (uiChar >= '0' && uiChar <= '9')
    return false;

  for (auto it = sIdentifier.GetIteratorFront(); it.IsValid(); ++it)
  {
    const nsUInt32 c = it.GetCharacter();

    if (c >= 'a' && c <= 'z')
      continue;
    if (c >= 'A' && c <= 'Z')
      continue;
    if (c >= '0' && c <= '9')
      continue;
    if (c >= '_')
      continue;
    if (c >= ':')
      continue;

    return false;
  }

  return true;
}

#endif

void nsRTTI::SanityCheckType(nsRTTI* pType)
{
  NS_ASSERT_DEV(pType->GetTypeFlags().IsSet(nsTypeFlags::StandardType) + pType->GetTypeFlags().IsSet(nsTypeFlags::IsEnum) +
                    pType->GetTypeFlags().IsSet(nsTypeFlags::Bitflags) + pType->GetTypeFlags().IsSet(nsTypeFlags::Class) ==
                  1,
    "Types are mutually exclusive!");

  for (auto pProp : pType->m_Properties)
  {
    const nsRTTI* pSpecificType = pProp->GetSpecificType();

    NS_ASSERT_DEBUG(IsValidIdentifierName(pProp->GetPropertyName()), "Property name is invalid: '{0}'", pProp->GetPropertyName());

    if (pProp->GetCategory() != nsPropertyCategory::Function)
    {
      NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::StandardType) + pProp->GetFlags().IsSet(nsPropertyFlags::IsEnum) +
                        pProp->GetFlags().IsSet(nsPropertyFlags::Bitflags) + pProp->GetFlags().IsSet(nsPropertyFlags::Class) <=
                      1,
        "Types are mutually exclusive!");
    }

    switch (pProp->GetCategory())
    {
      case nsPropertyCategory::Constant:
      {
        NS_IGNORE_UNUSED(pSpecificType);
        NS_ASSERT_DEV(pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::StandardType), "Only standard type constants are supported!");
      }
      break;
      case nsPropertyCategory::Member:
      {
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::StandardType) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::StandardType),
          "Property-Type missmatch!");
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::IsEnum) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::IsEnum),
          "Property-Type missmatch! Use NS_BEGIN_STATIC_REFLECTED_ENUM for type and NS_ENUM_MEMBER_PROPERTY / "
          "NS_ENUM_ACCESSOR_PROPERTY for property.");
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::Bitflags) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::Bitflags),
          "Property-Type missmatch! Use NS_BEGIN_STATIC_REFLECTED_ENUM for type and NS_BITFLAGS_MEMBER_PROPERTY / "
          "NS_BITFLAGS_ACCESSOR_PROPERTY for property.");
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::Class) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::Class),
          "If nsPropertyFlags::Class is set, the property type must be nsTypeFlags::Class and vise versa.");
      }
      break;
      case nsPropertyCategory::Array:
      case nsPropertyCategory::Set:
      case nsPropertyCategory::Map:
      {
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::StandardType) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::StandardType),
          "Property-Type missmatch!");
        NS_ASSERT_DEV(pProp->GetFlags().IsSet(nsPropertyFlags::Class) == pSpecificType->GetTypeFlags().IsSet(nsTypeFlags::Class),
          "If nsPropertyFlags::Class is set, the property type must be nsTypeFlags::Class and vise versa.");
      }
      break;
      case nsPropertyCategory::Function:
        NS_REPORT_FAILURE("Functions need to be put into the NS_BEGIN_FUNCTIONS / NS_END_FUNCTIONS; block.");
        break;
    }
  }
}

void nsRTTI::PluginEventHandler(const nsPluginEvent& EventData)
{
  switch (EventData.m_EventType)
  {
    case nsPluginEvent::BeforeLoading:
    {
      // before a new plugin is loaded, make sure all current nsRTTI instances
      // are assigned to the proper plugin
      // all not-yet assigned rtti instances cannot be in any plugin, so assign them to the 'static' plugin
      AssignPlugin("Static");
    }
    break;

    case nsPluginEvent::AfterLoadingBeforeInit:
    {
      // after we loaded a new plugin, but before it is initialized,
      // find all new rtti instances and assign them to that new plugin
      AssignPlugin(EventData.m_sPluginBinary);

#if NS_ENABLED(NS_COMPILE_FOR_DEBUG)
      nsRTTI::VerifyCorrectnessForAllTypes();
#endif
    }
    break;

    default:
      break;
  }
}

nsRTTIAllocator::~nsRTTIAllocator() = default;


NS_STATICLINK_FILE(Foundation, Foundation_Reflection_Implementation_RTTI);
