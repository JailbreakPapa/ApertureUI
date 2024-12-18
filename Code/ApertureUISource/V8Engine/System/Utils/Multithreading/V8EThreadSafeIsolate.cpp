#include <V8Engine/System/Utils/Multithreading/V8EThreadSafeIsolate.h>

bool aperture::v8::V8EThreadSafeIsolate::CreateIsolate(::v8::Isolate::CreateParams& params)
{
  mangaged_isolate = ::v8::Isolate::New(params);
  if(mangaged_isolate.isValid())
  {
    return true;
  }
  return false;
}