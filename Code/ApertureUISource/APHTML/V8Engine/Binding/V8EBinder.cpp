#include <APHTML/V8Engine/Binding/V8EBinder.h>

template <typename ReturnType, typename... Args>
void aperture::v8::binding::V8EBinder::BindFunctionToJS(const char* name, ReturnType (*func)(Args...))
{
  ::v8::Local<::v8::ObjectTemplate> global = global_template_.Get(isolate_);
  global->Set(
    ::v8::String::NewFromUtf8(isolate_, name.c_str()).ToLocalChecked(),
    ::v8::FunctionTemplate::New(isolate_, func));
}

template <typename T>
void aperture::v8::binding::V8EBinder::BindVariable(const char* name, const T& value)
{
  ::v8::Local<::v8::ObjectTemplate> global = global_template_.Get(isolate_);
  global->Set(
    ::v8::String::NewFromUtf8(isolate_, name.c_str()).ToLocalChecked(),
    ::v8::Number::New(isolate_, value));
}

template <typename ClassType>
void aperture::v8::binding::V8EBinder::BindClass(const char* name)
{
  ::v8::Local<::v8::FunctionTemplate> classTemplate = ::v8::FunctionTemplate::New(isolate_);
  classTemplate->SetClassName(::v8::String::NewFromUtf8(isolate_, name.c_str()).ToLocalChecked());
  class_templates_[name].Reset(isolate_, classTemplate);
}

template <typename ClassType, typename BaseType>
void aperture::v8::binding::V8EBinder::InheritBoundClass()
{
  auto it = class_templates_.find(typeid(ClassType).name());
  auto base_it = class_templates_.find(typeid(BaseType).name());

  if (it != class_templates_.end() && base_it != class_templates_.end())
  {
    ::v8::Local<::v8::FunctionTemplate> derived = it->second.Get(isolate_);
    ::v8::Local<::v8::FunctionTemplate> base = base_it->second.Get(isolate_);
    derived->Inherit(base);
  }
}

template <typename ClassType, typename MemberType>
void aperture::v8::binding::V8EBinder::BindClassVariable(const char* className, const char* memberName, MemberType ClassType::*member)
{
  auto it = class_templates_.find(className);
  if (it != class_templates_.end())
  {
    ::v8::Local<::v8::FunctionTemplate> classTemplate = it->second.Get(isolate_);
    classTemplate->InstanceTemplate()->SetAccessor(
      ::v8::String::NewFromUtf8(isolate_, memberName.c_str()).ToLocalChecked(),
      [](::v8::Local<::v8::String> property, const ::v8::PropertyCallbackInfo<::v8::Value>& info)
      {
        ClassType* self = static_cast<ClassType*>(info.Holder()->GetAlignedPointerFromInternalField(0));
        info.GetReturnValue().Set(self->*member);
      },
      [](::v8::Local<::v8::String> property, ::v8::Local<::v8::Value> value, const ::v8::PropertyCallbackInfo<void>& info)
      {
        ClassType* self = static_cast<ClassType*>(info.Holder()->GetAlignedPointerFromInternalField(0));
        ::v8::Local<::v8::Value> value = info[0]; // Assuming value is the first argument
        self->*member = value->NumberValue(info.GetIsolate()->GetCurrentContext()).ToChecked();
      });
  }
}

template <typename ClassType, typename ReturnType, typename... Args>
void aperture::v8::binding::V8EBinder::BindClassFunction(const char* className, const std::string& functionName, ReturnType (ClassType::*func)(Args...))
{
  auto it = class_templates_.find(className);
  if (it != class_templates_.end())
  {
    ::v8::Local<::v8::FunctionTemplate> classTemplate = it->second.Get(isolate_);
    classTemplate->PrototypeTemplate()->Set(
      ::v8::String::NewFromUtf8(isolate_, functionName.c_str()).ToLocalChecked(),
      ::v8::FunctionTemplate::New(isolate_, [func](const ::v8::FunctionCallbackInfo<::v8::Value>& args)
        {
                ClassType* self = static_cast<ClassType*>(args.Holder()->GetAlignedPointerFromInternalField(0));
                if constexpr (std::is_void_v<ReturnType>) {
                    (self->*func)(/* Extract args */);
                } else {
                    args.GetReturnValue().Set((self->*func)(/* Extract args */));
                } }));
              
  }
}

void aperture::v8::binding::V8EBinder::BindFunctionToJS(const char* name, const std::function<void()>& func)
{
  ::v8::HandleScope handle_scope(isolate_);

  // Wrap the std::function in a lambda that forwards to the original function
  auto js_func = ::v8::Function::New(
    isolate_->GetCurrentContext(),
    [](const ::v8::FunctionCallbackInfo<::v8::Value>& args)
    {
      auto* func = static_cast<std::function<void()>*>(args.Data().As<::v8::External>()->Value());
      if (func)
      {
        (*func)(); // Invoke the original function
      }
    },
    ::v8::External::New(isolate_, new std::function<void()>(func)))
                   .ToLocalChecked();

  // Assign the function to the global object in V8
  isolate_->GetCurrentContext()
    ->Global()
    ->Set(isolate_->GetCurrentContext(),
      ::v8::String::NewFromUtf8(isolate_, name).ToLocalChecked(),
      js_func)
    .Check();
}

::v8::Local<::v8::Object> aperture::v8::binding::V8EBinder::CreateJSModule(const char* name)
{
  ::v8::Local<::v8::ObjectTemplate> global = global_template_.Get(isolate_);
  ::v8::Local<::v8::Object> module = global->NewInstance(isolate_->GetCurrentContext()).ToLocalChecked();
  return module;
}

::v8::Isolate* aperture::v8::binding::V8EBinder::GetIsolate() const
{
  return isolate_;
}

void aperture::v8::binding::V8EBinder::SetIsolate(::v8::Isolate* isolate)
{
  std::scoped_lock lock(mutex_);
  isolate_ = isolate;
}
