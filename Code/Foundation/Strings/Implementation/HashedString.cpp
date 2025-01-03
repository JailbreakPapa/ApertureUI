#include <Foundation/FoundationPCH.h>

#include <Foundation/Logging/Log.h>
#include <Foundation/Strings/HashedString.h>
#include <Foundation/Threading/Lock.h>
#include <Foundation/Threading/Mutex.h>

struct HashedStringData
{
  nsMutex m_Mutex;
  nsHashedString::StringStorage m_Storage;
  nsHashedString::HashedType m_Empty;
};

static HashedStringData* s_pHSData;

NS_MSVC_ANALYSIS_WARNING_PUSH
NS_MSVC_ANALYSIS_WARNING_DISABLE(6011) // Disable warning for null pointer dereference as InitHashedString() will ensure that s_pHSData is set

// static
nsHashedString::HashedType nsHashedString::AddHashedString(nsStringView sString, nsUInt64 uiHash)
{
  if (s_pHSData == nullptr)
    InitHashedString();

  NS_LOCK(s_pHSData->m_Mutex);

  // try to find the existing string
  bool bExisted = false;
  auto ret = s_pHSData->m_Storage.FindOrAdd(uiHash, &bExisted);

  // if it already exists, just increase the refcount
  if (bExisted)
  {
#if NS_ENABLED(NS_COMPILE_FOR_DEVELOPMENT)
    if (ret.Value().m_sString != sString)
    {
      // TODO: I think this should be a more serious issue
      nsLog::Error("Hash collision encountered: Strings \"{}\" and \"{}\" both hash to {}.", nsArgSensitive(ret.Value().m_sString), nsArgSensitive(sString), uiHash);
    }
#endif

#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
    ret.Value().m_iRefCount.Increment();
#endif
  }
  else
  {
    nsHashedString::HashedData& d = ret.Value();
#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
    d.m_iRefCount = 1;
#endif
    d.m_sString = sString;
  }

  return ret;
}

NS_MSVC_ANALYSIS_WARNING_POP

// static
void nsHashedString::InitHashedString()
{
  if (s_pHSData != nullptr)
    return;

  alignas(alignof(HashedStringData)) static nsUInt8 HashedStringDataBuffer[sizeof(HashedStringData)];
  s_pHSData = new (HashedStringDataBuffer) HashedStringData();

  // makes sure the empty string exists for the default constructor to use
  s_pHSData->m_Empty = AddHashedString("", nsHashingUtils::StringHash(""));

#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
  // this one should never get deleted, so make sure its refcount is 2
  s_pHSData->m_Empty.Value().m_iRefCount.Increment();
#endif
}

#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
nsUInt32 nsHashedString::ClearUnusedStrings()
{
  NS_LOCK(s_pHSData->m_Mutex);

  nsUInt32 uiDeleted = 0;

  for (auto it = s_pHSData->m_Storage.GetIterator(); it.IsValid();)
  {
    if (it.Value().m_iRefCount == 0)
    {
      it = s_pHSData->m_Storage.Remove(it);
      ++uiDeleted;
    }
    else
      ++it;
  }

  return uiDeleted;
}
#endif

NS_MSVC_ANALYSIS_WARNING_PUSH
NS_MSVC_ANALYSIS_WARNING_DISABLE(6011) // Disable warning for null pointer dereference as InitHashedString() will ensure that s_pHSData is set

nsHashedString::nsHashedString()
{
  static_assert(sizeof(m_Data) == sizeof(void*), "The hashed string data should only be as large as one pointer.");
  static_assert(sizeof(*this) == sizeof(void*), "The hashed string data should only be as large as one pointer.");

  // only insert the empty string once, after that, we can just use it without the need for the mutex
  if (s_pHSData == nullptr)
    InitHashedString();

  m_Data = s_pHSData->m_Empty;
#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
  m_Data.Value().m_iRefCount.Increment();
#endif
}

NS_MSVC_ANALYSIS_WARNING_POP

bool nsHashedString::IsEmpty() const
{
  return m_Data == s_pHSData->m_Empty;
}

void nsHashedString::Clear()
{
#if NS_ENABLED(NS_HASHED_STRING_REF_COUNTING)
  if (m_Data != s_pHSData->m_Empty)
  {
    HashedType tmp = m_Data;

    m_Data = s_pHSData->m_Empty;
    m_Data.Value().m_iRefCount.Increment();

    tmp.Value().m_iRefCount.Decrement();
  }
#else
  m_Data = s_pHSData->m_Empty;
#endif
}

nsResult nsHashedString::LookupStringHash(nsUInt64 uiHash, nsStringView& out_sResult)
{
  NS_LOCK(s_pHSData->m_Mutex);
  auto it = s_pHSData->m_Storage.Find(uiHash);

  if (!it.IsValid())
    return NS_FAILURE;

  out_sResult = it.Value().m_sString;
  return NS_SUCCESS;
}
