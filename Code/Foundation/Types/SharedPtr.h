#pragma once

#include <Foundation/Types/RefCounted.h>
#include <Foundation/Types/UniquePtr.h>

/// \brief A Shared ptr manages a shared object and destroys that object when no one references it anymore. The managed object must derive
/// from nsRefCounted.
template <typename T>
class nsSharedPtr
{
public:
  NS_DECLARE_MEM_RELOCATABLE_TYPE();

  /// \brief Creates an empty shared ptr.
  nsSharedPtr();

  /// \brief Creates a shared ptr from a freshly created instance through NS_NEW or NS_DEFAULT_NEW.
  template <typename U>
  nsSharedPtr(const nsInternal::NewInstance<U>& instance);

  /// \brief Creates a shared ptr from a pointer and an allocator. The passed allocator will be used to destroy the instance when the shared
  /// ptr goes out of scope.
  template <typename U>
  nsSharedPtr(U* pInstance, nsAllocator* pAllocator);

  /// \brief Copy constructs a shared ptr from another. Both will hold a reference to the managed object afterwards.
  nsSharedPtr(const nsSharedPtr<T>& other);

  /// \brief Copy constructs a shared ptr from another. Both will hold a reference to the managed object afterwards.
  template <typename U>
  nsSharedPtr(const nsSharedPtr<U>& other);

  /// \brief Move constructs a shared ptr from another. The other shared ptr will be empty afterwards.
  template <typename U>
  nsSharedPtr(nsSharedPtr<U>&& other);

  /// \brief Move constructs a shared ptr from a unique ptr. The unique ptr will be empty afterwards.
  template <typename U>
  nsSharedPtr(nsUniquePtr<U>&& other);

  /// \brief Initialization with nullptr to be able to return nullptr in functions that return shared ptr.
  nsSharedPtr(std::nullptr_t);

  /// \brief Destroys the managed object using the stored allocator if no one else references it anymore.
  ~nsSharedPtr();

  /// \brief Sets the shared ptr from a freshly created instance through NS_NEW or NS_DEFAULT_NEW.
  template <typename U>
  nsSharedPtr<T>& operator=(const nsInternal::NewInstance<U>& instance);

  /// \brief Sets the shared ptr from another. Both will hold a reference to the managed object afterwards.
  nsSharedPtr<T>& operator=(const nsSharedPtr<T>& other);

  /// \brief Sets the shared ptr from another. Both will hold a reference to the managed object afterwards.
  template <typename U>
  nsSharedPtr<T>& operator=(const nsSharedPtr<U>& other);

  /// \brief Move assigns a shared ptr from another. The other shared ptr will be empty afterwards.
  template <typename U>
  nsSharedPtr<T>& operator=(nsSharedPtr<U>&& other);

  /// \brief Move assigns a shared ptr from a unique ptr. The unique ptr will be empty afterwards.
  template <typename U>
  nsSharedPtr<T>& operator=(nsUniquePtr<U>&& other);

  /// \brief Assigns a nullptr to the shared ptr. Same as Reset.
  nsSharedPtr<T>& operator=(std::nullptr_t);

  /// \brief Borrows the managed object. The shared ptr stays unmodified.
  T* Borrow() const;

  /// \brief Destroys the managed object if no one else references it anymore and resets the shared ptr.
  void Clear();

  /// \brief Provides access to the managed object.
  T& operator*() const;

  /// \brief Provides access to the managed object.
  T* operator->() const;

  /// \brief Provides access to the managed object.
  operator const T*() const;

  /// \brief Provides access to the managed object.
  operator T*();

  /// \brief Returns true if there is managed object and false if the shared ptr is empty.
  explicit operator bool() const;

  /// \brief Compares the shared ptr against another shared ptr.
  bool operator==(const nsSharedPtr<T>& rhs) const;
  bool operator!=(const nsSharedPtr<T>& rhs) const;
  bool operator<(const nsSharedPtr<T>& rhs) const;
  bool operator<=(const nsSharedPtr<T>& rhs) const;
  bool operator>(const nsSharedPtr<T>& rhs) const;
  bool operator>=(const nsSharedPtr<T>& rhs) const;

  /// \brief Compares the shared ptr against nullptr.
  bool operator==(std::nullptr_t) const;
  bool operator!=(std::nullptr_t) const;
  bool operator<(std::nullptr_t) const;
  bool operator<=(std::nullptr_t) const;
  bool operator>(std::nullptr_t) const;
  bool operator>=(std::nullptr_t) const;

  /// \brief Returns a copy of this, as an nsSharedPtr<DERIVED>. Downcasts the stored pointer (using static_cast).
  ///
  /// Does not check whether the cast would be valid, that is all your responsibility.
  template <typename DERIVED>
  nsSharedPtr<DERIVED> Downcast() const
  {
    return nsSharedPtr<DERIVED>(static_cast<DERIVED*>(m_pInstance), m_pAllocator);
  }

private:
  template <typename U>
  friend class nsSharedPtr;

  void AddReferenceIfValid();
  void ReleaseReferenceIfValid();

  T* m_pInstance;
  nsAllocator* m_pAllocator;
};

/**
 * @brief Creates a shared pointer of type T by forwarding the provided arguments to T's constructor.
 *
 * @tparam T The type of the object to be managed.
 * @tparam Args The types of arguments to forward to T's constructor.
 * @param args The arguments to forward to T's constructor.
 * @return nsSharedPtr<T> A shared pointer managing the newly created object.
 */
template <typename T, typename... Args>
NS_ALWAYS_INLINE nsSharedPtr<T> nsMakeShared(Args&&... args)
{
  return nsSharedPtr<T>(new T(std::forward<Args>(args)...), nullptr);
}

#include <Foundation/Types/Implementation/SharedPtr_inl.h>
