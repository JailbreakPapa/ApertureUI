#pragma once

#include <APHTML/V8Engine/V8EngineDLL.h>
#include <Foundation/Types/Uuid.h>
namespace aperture::v8::internal
{
    // use the only one field if possible, otherwise it's not straightforward in implementing the same thing with QuickJS
    enum EInternalFields : nsUInt32
    {
        IF_Pointer = 0, // pointer to object (used by object/variant both)
        IF_ObjectT = 1, // placeholder for non-valuetype objects

        IF_VariantFieldCount = 1,
        IF_ObjectFieldCount = 2,
    };

    namespace EBindingPolicy
    {
        enum Type : nsUInt8
        {
            External, // managed by c++ (the javascript counterpart will be strong-referenced)
            Managed,  // managed by javascript gc (c++ waits gc to finalize the native object)
        };
    }

    // Object classes or c++ native wrapped classes are registered in an object registry in Environment.
    // nsVariant (valuetype) DO NOT have it's ObjectHandle.
    struct ObjectHandle
    {
        nsUuid class_id;

        // primitive pointer to the native object.
        // must be a real pointer which implies that different objects have different addresses.
        void* pointer;

        // this reference is initially weak and hooked on v8 gc callback.
        // it becomes a strong reference after the `ref_count_` explicitly increased.
        ::v8::Global<::v8::Object> ref_;

        nsUInt32 ref_count_;
    };
}