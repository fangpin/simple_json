#pragma once

#include <atomic>
#include <memory>

namespace json {
class SpinLock {
private:
    std::atomic<bool> lock_ = {false};
public:
    void lock() {
        while (true) {
            if (!lock_.exchange(true, std::memory_order_acquire)) {
                break;
            }
            while (lock_.load(std::memory_order_relaxed)) {
                #if defined(__x86_64__) || defined(_M_AMD64)
                    __builtin_ia32_pause();
                #elif defined(__aarch64__) || defined(_M_ARM64)
                    asm volatile("yield");
                #endif   
            }
        }
    }

    void unlock() {
        lock_.store(false, std::memory_order_release);
    }
};
} // namespace json


namespace cxx
{
using size_t = std::size_t;

struct defaultAllocateTraits 
{
    static void* _Allocate(size_t const bytes)
    {
        return ::operator new(bytes);
    }
};

template <size_t TypeSize>
constexpr size_t GetSizeOfBlock(size_t const count)
  {
    constexpr bool overFlowPossible = TypeSize > 1;
    if constexpr(overFlowPossible)
    {
        constexpr size_t maxPossible = static_cast<size_t>(-1)/ TypeSize;
        if(count > maxPossible)
        {
            throw 1;
        }
    }
    return count * TypeSize;
}

template <class Type>
inline constexpr size_t NewAlignOf = alignof(Type);

template<size_t align, class Traits = defaultAllocateTraits>
void * Allocate(size_t const bytes)
{
    if (bytes != 0) 
    {
        return Traits::_Allocate(bytes);
    }
    return nullptr;
}

template<size_t Align>
void Deallocate(void * ptr, size_t bytes) noexcept
{
    ::operator delete(ptr, bytes);
}

template <class Type>
class allocator{
    public:

    using value_type = Type;
    using size_type = size_t;

    constexpr allocator() noexcept{}
    constexpr allocator(allocator const&) noexcept = default;

    void deallocate(Type * ptr, size_t const count){
        Deallocate<NewAlignOf<Type>>(ptr, sizeof(Type) * count);
    }

    Type* allocate(size_t const count){
        return static_cast<Type*>
           (Allocate<NewAlignOf<Type>>
            (GetSizeOfBlock<sizeof(Type)>(count))
           );
    }
};
}
