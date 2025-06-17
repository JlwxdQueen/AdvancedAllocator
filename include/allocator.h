#pragma once
#include <cstddef>

class Allocator {
public:
    virtual ~Allocator() = default;
    
    virtual void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
    
    virtual void deallocate(void* ptr, size_t size = 0) noexcept;

    template <typename T>
    struct Rebind {
        using other = Allocator;
    };
};