/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <V8Engine/System/Internal/V8EAsniAllocator.h>
#include <V8Engine/System/Internal/V8ESIndex.h>
#include <type_traits>

namespace aperture::v8
{
#ifdef DEBUG_ENABLED
// ensure list items not reallocated when a scope is alive
#  define jsb_address_guard(list, scope_name) const auto scope_name = (list).address_scope()
#  define NDEBUG 1
#else
#  define jsb_address_guard(list, scope_name) (void)0
#  define NDEBUG 0
#endif

  namespace internal
  {
    // NOTE some types (like std::function) are not supported because copy/move on resizing is not implemented for now.
    template <typename T, typename IndexType = TIndex<nsUInt32>, typename TAllocator = core::AnsiAllocator>
    class SArray
    {
      using RevisionType = typename IndexType;
      using ElementTypeTypedef = std::remove_pointer_t<T>;

      enum
      {
        INDEX_NONE = -1
      };
      enum
      {
        kInitialRevision = 1
      };

      struct Slot
      {
        int next;
        int previous;
        RevisionType revision;
        T value;

#if NDEBUG
        bool has_value_;
        void reset_value() { has_value_ = false; }
        bool has_value() const { return has_value_; }
        void set_value(T&& p_value)
        {
          has_value_ = true;
          value = std::move(p_value);
        }
        void set_value(const T& p_value)
        {
          has_value_ = true;
          value = p_value;
        }
#else
        void reset_value() {}
        bool has_value() const { return true; }
        void set_value(T&& p_value) { value = std::move(p_value); }
        void set_value(const T& p_value) { value = p_value; }
#endif
      };

      using AllocatorType = typename TAllocator::template ForType<Slot>;

      int _version = 0;
      int _used_size = 0;
      int _free_index = -1;
      int _first_index = -1;
      int _last_index = -1;
      int _address_locked = 0;
      AllocatorType allocator;

      Slot* get_data() const
      {
        return (Slot*)allocator.get_data();
      }

    public:
      struct AddressScope
      {
        SArray* container_;

        AddressScope(SArray* p_container)
          : container_(p_container)
        {
          container_->lock_address();
        }
        ~AddressScope()
        {
          if (container_)
            container_->unlock_address();
        }

        AddressScope(const AddressScope&) = delete;
        AddressScope& operator=(const AddressScope&) = delete;

        AddressScope(AddressScope&& p_other) noexcept
        {
          container_ = p_other.container_;
          p_other.container_ = nullptr;
        }
        AddressScope& operator=(AddressScope&& p_other) noexcept
        {
          container_ = p_other.container_;
          p_other.container_ = nullptr;
          return *this;
        }
      };

      struct ScopedPointer
      {
      private:
        SArray* container_;
        T* ptr_;

      public:
        ScopedPointer(SArray* p_container, const IndexType& p_index)
          : container_(p_container)
        {
          container_->lock_address();
          ptr_ = &container_->get_value(p_index);
        }
        ~ScopedPointer()
        {
          if (container_)
            container_->unlock_address();
        }

        ScopedPointer(const ScopedPointer&) = delete;
        ScopedPointer& operator=(const ScopedPointer&) = delete;

        T* operator->() { return ptr_; }
        const T* operator->() const { return ptr_; }

        ScopedPointer(ScopedPointer&& p_other) noexcept
        {
          container_ = p_other.container_;
          ptr_ = p_other.ptr_;
          p_other.container_ = nullptr;
          p_other.ptr_ = nullptr;
        }
        ScopedPointer& operator=(ScopedPointer&& p_other) noexcept
        {
          container_ = p_other.container_;
          ptr_ = p_other.ptr_;
          p_other.container_ = nullptr;
          p_other.ptr_ = nullptr;
          return *this;
        }
      };

      struct ScopedConstPointer
      {
      private:
        SArray* container_;
        const T* ptr_;

      public:
        ScopedConstPointer(SArray* p_container, const IndexType& p_index)
          : container_(p_container)
        {
          container_->lock_address();
          ptr_ = &container_->get_value(p_index);
        }
        ~ScopedConstPointer()
        {
          if (container_)
            container_->unlock_address();
        }

        ScopedConstPointer(const ScopedConstPointer&) = delete;
        ScopedConstPointer& operator=(const ScopedConstPointer&) = delete;

        const T* operator->() const { return ptr_; }

        ScopedConstPointer(ScopedConstPointer&& p_other) noexcept
        {
          container_ = p_other.container_;
          ptr_ = p_other.ptr_;
          p_other.container_ = nullptr;
          p_other.ptr_ = nullptr;
        }
        ScopedConstPointer& operator=(ScopedConstPointer&& p_other) noexcept
        {
          container_ = p_other.container_;
          ptr_ = p_other.ptr_;
          p_other.container_ = nullptr;
          p_other.ptr_ = nullptr;
          return *this;
        }
      };

      struct LowLevelAccess
      {
        bool is_valid_slot(int p_slot_index) const
        {
          if (p_slot_index < 0 || p_slot_index >= container.capacity())
          {
            return false;
          }
          return container.get_data()[p_slot_index].has_value();
        }

        T& get_slot_value(int p_slot_index)
        {
          return container.get_data()[p_slot_index].value;
        }

        const T& get_slot_value(int p_slot_index) const
        {
          return container.get_data()[p_slot_index].value;
        }

      private:
        LowLevelAccess(SArray& p_container)
          : container(p_container)
        {
        }

        SArray& container;
      };

      const LowLevelAccess& get_low_level_access() { return LowLevelAccess(*this); }

      SArray() { reserve(8); }
      SArray(int p_init_capacity) { reserve(p_init_capacity); }
      SArray(SArray&& other) noexcept { *this = std::move(other); }
      SArray(const SArray& other) { *this = other; }

      ~SArray()
      {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
          while (_first_index != 0)
          {
            Slot& slot = get_data()[_first_index];
            const int next = slot.next;
            NS_ISE_JSI_CHECK(slot.has_value());
            if constexpr (!std::is_trivially_destructible_v<T>)
            {
              slot.value.ElementTypeTypedef::~ElementTypeTypedef();
            }
            _first_index = next;
          }
        }
      }

      AddressScope address_scope() { return AddressScope(this); }

      int size() const { return _used_size; }

      bool is_empty() const { return _used_size == 0; }

      // no more room for additional items (used == capacity)
      bool is_full() const { return _used_size == capacity(); }

      int capacity() const { return (int)allocator.capacity(); }

      static constexpr size_t get_slot_size() { return sizeof(Slot); }

      void clear()
      {
        if (_used_size == 0)
        {
          return;
        }
        Slot* slots_base = get_data();
        while (_first_index != 0)
        {
          const int index = _first_index;
          Slot& slot = slots_base[index];

          destruct_element(slot);
          _first_index = slot.next;
          slot.next = _free_index;
          slot.reset_value();
          // IndexType::increase_revision(slot.revision);
          _free_index = index;
        }
        NS_ISE_JSI_CHECK(_first_index == -1);
        _last_index = -1;
        _used_size = 0;
        ++_version;
      }

      void reverse()
      {
        int forward = _first_index;
        int backward = _last_index;
        Slot* slots_base = get_data();
        while (forward != backward)
        {
          NS_ISE_JSI_CHECK(forward >= 0 && backward >= 0);
          auto a = slots_base[forward].value;
          auto b = slots_base[backward].value;
          slots_base[forward].value = b;
          slots_base[backward].value = a;
          forward = slots_base[forward].next;
          backward = slots_base[backward].previous;
        }
      }

      IndexType get_first_index() const
      {
        return _first_index != 0
                 ? IndexType(_first_index, get_data()[_first_index].revision)
                 : IndexType::none();
      }

      IndexType get_last_index() const
      {
        return _last_index != 0
                 ? IndexType(_last_index, get_data()[_last_index].revision)
                 : IndexType::none();
      }

      void try_get_linked_index(const IndexType& p_index, IndexType& o_previous, IndexType& o_next) const
      {
        if (!is_valid_index(p_index))
        {
          o_previous = o_next = IndexType::none();
          return;
        }

        const Slot& slot = get_data()[p_index.get_index()];
        if (slot.next != 0)
        {
          NS_ISE_JSI_CHECK(get_data()[slot.next].previous == p_index.get_index());
          o_next = IndexType(slot.next, get_data()[slot.next].revision);
        }
        else
        {
          o_next = IndexType::none();
        }
        if (slot.previous != 0)
        {
          NS_ISE_JSI_CHECK(get_data()[slot.previous].next == p_index.get_index());
          o_previous = IndexType(slot.previous, get_data()[slot.previous].revision);
        }
        else
        {
          o_previous = IndexType::none();
        }
      }

      IndexType get_next_index(const IndexType& p_index) const
      {
        NS_ISE_JSI_CHECK(is_valid_index(p_index));
        const Slot& slot = get_data()[p_index.get_index()];
        if (slot.next != 0)
        {
          NS_ISE_JSI_CHECK(get_data()[slot.next].previous == p_index.get_index());
          return IndexType(slot.next, get_data()[slot.next].revision);
        }
        return IndexType::none();
      }

      IndexType get_previous_index(const IndexType& p_index) const
      {
        NS_ISE_JSI_CHECK(is_valid_index(p_index));
        const Slot& slot = get_data()[p_index.get_index()];
        if (slot.previous != 0)
        {
          NS_ISE_JSI_CHECK(get_data()[slot.previous].next == p_index.get_index());
          return IndexType(slot.previous, get_data()[slot.previous].revision);
        }
        return IndexType::none();
      }

      bool is_valid_index(const IndexType& p_index) const
      {
        const int index = p_index.get_index();
        if (index < 0 || index >= capacity() || p_index.get_revision() == 0 || get_data()[index].revision != p_index.get_revision())
        {
          return false;
        }
        NS_ISE_JSI_CHECK(get_data()[index].has_value());
        return true;
      }

      template <typename TArg = T>
      IndexType add(TArg&& value)
      {
        grow_if_needed(1);
        const int new_index = _free_index;
        Slot& slot = get_data()[new_index];

        IndexType::increase_revision(slot.revision);
        // recreate `value` before assignment
        construct_element(slot);
        slot.set_value(std::forward<TArg>(value));
        _free_index = slot.next;
        slot.next = 0;
        slot.previous = _last_index;
        ++_used_size;
        if (_last_index != 0)
        {
          Slot& last_slot = get_data()[_last_index];
          last_slot.next = new_index;
        }
        if (_first_index == 0)
        {
          _first_index = new_index;
        }
        _last_index = new_index;
        ++_version;
        NS_ISE_JSI_CHECK(is_consistent());
        return IndexType(new_index, slot.revision);
      }

      IndexType insert(const IndexType& p_index, T&& p_item)
      {
        NS_ISE_JSI_CHECK(is_consistent());
        NS_ISE_JSI_CHECK(is_valid_index(p_index));
        grow_if_needed(1);

        Slot& pivot_slot = get_data()[p_index.get_index()];
        const int new_index = _free_index;
        Slot& new_slot = get_data()[new_index];

        IndexType::increase_revision(new_slot.revision);
        // recreate `value` before assignment
        construct_element(new_slot);
        new_slot.set_value(std::forward<T>(p_item));
        _free_index = new_slot.next;
        new_slot.next = p_index.get_index();
        new_slot.previous = pivot_slot.previous;
        pivot_slot.previous = new_index;
        NS_ISE_JSI_CHECK(get_data() + p_index.get_index() == &pivot_slot);
        NS_ISE_JSI_CHECK(get_data()[p_index.get_index()].previous == pivot_slot.previous && pivot_slot.previous == new_index);
        if (new_slot.previous != 0)
        {
          Slot& previous_slot = get_data()[new_slot.previous];
          previous_slot.next = new_index;
        }
        ++_used_size;
        if (_first_index == p_index.get_index())
        {
          _first_index = new_index;
        }
        ++_version;
        NS_ISE_JSI_CHECK(is_consistent());
        return IndexType(new_index, new_slot.revision);
      }

      bool try_get_value_pointer(const IndexType& p_index, T*& out_item)
      {
        if (is_valid_index(p_index))
        {
          Slot& slot = get_data()[p_index.get_index()];
          out_item = &slot.value;
          return true;
        }
        out_item = 0;
        return false;
      }

      bool try_get_value_pointer(const IndexType& p_index, const T*& out_item) const
      {
        if (is_valid_index(p_index))
        {
          const Slot& slot = get_data()[p_index.get_index()];
          out_item = &slot.value;
          return true;
        }
        out_item = 0;
        return false;
      }

      bool try_get_value(const IndexType& p_index, T& out_item)
      {
        if (is_valid_index(p_index))
        {
          Slot& slot = get_data()[p_index.get_index()];
          out_item = slot.value;
          return true;
        }
        return false;
      }

      T& operator[](const IndexType& p_index)
      {
        return get_value(p_index);
      }

      const T& operator[](const IndexType& p_index) const
      {
        return get_value(p_index);
      }

      T pop()
      {
        NS_ISE_JSI_CHECK(_last_index != 0);
        const Slot& slot = get_data()[_last_index];
        const T item = std::move(slot.value);
        remove_at({_last_index, slot.revision});
        return item;
      }

      T& get_first_value()
      {
        return get_value(get_first_index());
      }

      const T& get_first_value() const
      {
        return get_value(get_first_index());
      }

      T& get_last_value()
      {
        return get_value(get_last_index());
      }

      const T& get_last_value() const
      {
        return get_value(get_last_index());
      }

      ScopedPointer get_value_scoped(const IndexType& p_index) { return ScopedPointer(this, p_index); }
      ScopedConstPointer get_value_scoped(const IndexType& p_index) const { return ScopedConstPointer(this, p_index); }

      T& get_value(const IndexType& p_index)
      {
        NS_ISE_JSI_CHECK(p_index.get_revision() != 0);
        NS_ISE_JSI_CHECK(p_index.get_index() >= 0);
        NS_ISE_JSI_CHECK(p_index.get_index() < capacity());
        Slot& slot = get_data()[p_index.get_index()];
        NS_ISE_JSI_CHECK(p_index.get_revision() == slot.revision);
        return slot.value;
      }

      const T& get_value(const IndexType& p_index) const
      {
        NS_ISE_JSI_CHECK(p_index.get_index() >= 0);
        NS_ISE_JSI_CHECK(p_index.get_index() < capacity());
        const Slot& slot = get_data()[p_index.get_index()];
        NS_ISE_JSI_CHECK(p_index.get_revision() == slot.revision);
        return slot.value;
      }

      bool try_get_value(const IndexType& p_index, T& o_value) const
      {
        if (p_index.get_index() >= 0 && p_index.get_index() < capacity())
        {
          const Slot& slot = get_data()[p_index.get_index()];
          if (p_index.get_revision() == slot.revision)
          {
            o_value = slot.value;
            return true;
          }
        }
        return false;
      }

      [[nodiscard]]
      bool contains(const T& p_item) const
      {
        return index_of(p_item) != IndexType::none();
      }

      [[nodiscard]]
      IndexType index_of(const T& p_item) const
      {
        int current = _first_index;
        while (current != 0)
        {
          const Slot& slot = get_data()[current];
          if (slot.value == p_item)
          {
            return IndexType(current, slot.revision);
          }
          current = slot.next;
        }
        return IndexType::none();
      }

      [[nodiscard]]
      IndexType last_index_of(const T& p_item) const
      {
        int current = _last_index;
        while (current != 0)
        {
          const Slot& slot = get_data()[current];
          if (slot.value == p_item)
          {
            return IndexType(current, slot.revision);
          }
          current = slot.previous;
        }
        return IndexType::none();
      }

      bool try_remove_at(const IndexType& p_index, T& o_item)
      {
        if (!is_valid_index(p_index))
        {
          return false;
        }
        o_item = get_value(p_index);
        remove_at_checked(p_index);
        return true;
      }

      bool remove_at(const IndexType& p_index)
      {
        NS_ISE_JSI_CHECK(_address_locked == 0);
        if (p_index.get_index() < 0 || p_index.get_index() >= capacity())
        {
          return false;
        }
        Slot& slot = get_data()[p_index.get_index()];
        if (slot.revision != p_index.get_revision())
        {
          return false;
        }
        const int next = slot.next;
        const int previous = slot.previous;

        destruct_element(slot);
        slot.next = _free_index;
        slot.reset_value();
        IndexType::increase_revision(slot.revision);
        _free_index = p_index.get_index();
        --_used_size;
        ++_version;
        if (next != 0)
        {
          get_data()[next].previous = previous;
        }
        if (previous != 0)
        {
          get_data()[previous].next = next;
        }
        if (_first_index == p_index.get_index())
        {
          _first_index = next;
        }
        if (_last_index == p_index.get_index())
        {
          _last_index = previous;
        }
        NS_ISE_JSI_CHECK(is_consistent());
        return true;
      }

      void remove_at_checked(const IndexType& p_index)
      {
        const bool ret = remove_at(p_index);
        NS_ISE_JSI_CHECK(ret);
      }

      void reserve(int p_size)
      {
        if (p_size <= capacity())
        {
          return;
        }
        grow_if_needed(p_size - capacity());
      }

      // ensure the number of free slot is enough to add `p_extra_count` new elements
      void grow_if_needed(int p_extra_count)
      {
        NS_ISE_JSI_CHECK(p_extra_count > 0);
        NS_ISE_JSI_CHECK(_address_locked == 0);
        const int current_size = capacity();
        const int expected_size = _used_size + p_extra_count;
        if (expected_size <= current_size)
        {
          return;
        }

        const int new_capacity = std::max(std::max(current_size * 2, 4), expected_size);
        allocator.resize(current_size, new_capacity);
        NS_ISE_JSI_CHECK(new_capacity == capacity());
        Slot* slots_base = get_data();
        for (int i = current_size; i < new_capacity; ++i)
        {
          Slot& slot = slots_base[i];
          NS_ISE_JSI_CHECK(!slot.has_value());
          slot.next = _free_index;
          slot.revision = kInitialRevision;
          _free_index = i;
        }
        NS_ISE_JSI_CHECK(is_consistent());
      }

      template <typename ContainerType, typename ElementType>
      struct Iterator
      {
      private:
        ContainerType& container;
        IndexType index;
        IndexType previous;
        IndexType next;

      public:
        Iterator() = delete;

        Iterator(ContainerType& p_container, IndexType p_index)
          : container(p_container)
          , index(p_index)
        {
          container.try_get_linked_index(p_index, previous, next);
        }

        Iterator(const Iterator& other)
          : container(other.container)
          , index(other.index)
          , previous(other.previous)
          , next(other.next)
        {
        }

        Iterator& operator++()
        {
          index = next;
          container.try_get_linked_index(index, previous, next);
          return *this;
        }

        Iterator& operator++(int)
        {
          Iterator temp(*this);
          index = next;
          container.try_get_linked_index(index, previous, next);
          return temp;
        }

        Iterator& operator--()
        {
          index = previous;
          container.try_get_linked_index(index, previous, next);
          return *this;
        }

        Iterator& operator--(int)
        {
          Iterator temp(*this);
          index = previous;
          container.try_get_linked_index(index, previous, next);
          return temp;
        }

        ElementType& operator*() const
        {
          return container.get_value(index);
        }

        ElementType* operator->() const
        {
          return &container.get_value(index);
        }

        bool is_valid() const { return container.is_valid_index(index); }
        explicit operator bool() const { return is_valid(); }

        Iterator& operator=(const Iterator& other)
        {
          container = other.container;
          index = other.index;
          previous = other.previous;
          next = other.next;
          return *this;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
          return &lhs.container == &rhs.container && lhs.index == rhs.index && lhs.previous == rhs.previous && lhs.next == rhs.next;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
          return &lhs.container != &rhs.container || lhs.index != rhs.index || lhs.previous != rhs.previous || lhs.next != rhs.next;
        }

        void remove()
        {
          container.remove_at(index);
        }

        IndexType get_index() const
        {
          return index;
        }
      }; // Iterator

      typedef Iterator<SArray, T> TIterator;
      TIterator begin() { return TIterator(*this, get_first_index()); }
      TIterator end() { return TIterator(*this, IndexType::none()); }

      typedef Iterator<const SArray, const T> TConstIterator;
      TConstIterator begin() const { return TConstIterator(*this, get_first_index()); }
      TConstIterator end() const { return TConstIterator(*this, IndexType::none()); }


      SArray& operator=(SArray&& other) noexcept
      {
        if (this == &other)
        {
          return *this;
        }
        clear();
        _version = other._version;
        _used_size = other._used_size;
        allocator = std::move(other.allocator);
        _free_index = other._free_index;
        _first_index = other._first_index;
        _last_index = other._last_index;
        other._version = 0;
        other._used_size = 0;
        // other.allocator.reset();
        other._free_index = -1;
        other._first_index = -1;
        other._last_index = -1;
        return *this;
      }

      SArray& operator=(const SArray& other)
      {
        if (this == &other)
        {
          return *this;
        }
        clear();
        int index = other._first_index;
        while (index != 0)
        {
          add(other.get_data()[index].value);
          index = other.get_data()[index].next;
        }
        return *this;
      }

      friend bool operator==(const SArray& lhs, const SArray& rhs)
      {
        if (&lhs == &rhs)
        {
          return true;
        }
        if (lhs._used_size != rhs._used_size)
        {
          return false;
        }
        int lhs_index = lhs._first_index;
        int rhs_index = rhs._first_index;
        while (lhs_index != 0 && rhs_index != 0)
        {
          if (lhs.get_data()[lhs_index].value != rhs.get_data()[rhs_index].value)
          {
            return false;
          }
          lhs_index = lhs.get_data()[lhs_index].next;
          rhs_index = rhs.get_data()[rhs_index].next;
        }
        return true;
      }

      friend bool operator!=(const SArray& lhs, const SArray& rhs)
      {
        return !(lhs == rhs);
      }

    private:
      void lock_address() { ++_address_locked; }
      void unlock_address()
      {
        NS_ISE_JSI_CHECK(_address_locked > 0);
        --_address_locked;
      }

      bool is_consistent() const
      {
        int index = _free_index;
        int count = 0;
        while (index != INDEX_NONE)
        {
          const Slot& slot = get_data()[index];
          index = slot.next;
          ++count;
        }
        NS_ISE_JSI_CHECK(count == capacity() - _used_size);
        if (_used_size > 0)
        {
          count = 0;
          index = _first_index;
          while (index != 0)
          {
            const Slot& slot = get_data()[index];
            // NS_ISE_JSI_CHECK(is_valid_index({index, slot.revision}));
            if (index == _first_index)
            {
              NS_ISE_JSI_CHECK(slot.previous == 0);
            }
            if (slot.next != 0)
            {
              NS_ISE_JSI_CHECK(get_data()[slot.next].previous == index);
            }
            if (slot.previous != 0)
            {
              NS_ISE_JSI_CHECK(get_data()[slot.previous].next == index);
            }
            ++count;
            NS_ISE_JSI_CHECK(count <= _used_size);
            if (count == _used_size)
            {
              NS_ISE_JSI_CHECK(_last_index == index);
            }
            index = slot.next;
          }
          return _used_size == count;
        }
        return _first_index == 0 && _last_index == 0;
      }

      static void construct_element(Slot& p_slot)
      {
        NS_ISE_JSI_CHECK(!p_slot.has_value());
        if constexpr (!std::is_trivially_constructible_v<T>)
        {
          memnew_placement(&p_slot.value, T);
        }
      }

      static void destruct_element(Slot& p_slot)
      {
        NS_ISE_JSI_CHECK(p_slot.has_value());
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
          p_slot.value.ElementTypeTypedef::~ElementTypeTypedef();
        }
        else
        {
          memset((void*)&p_slot.value, 0, sizeof(T));
        }
      }
    };
  } // namespace internal
} // namespace aperture::v8
