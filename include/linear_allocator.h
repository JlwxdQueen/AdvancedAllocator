#pragma once
#include <mutex>
#include "allocator.h"

class LinearAllocator : public Allocator {
public:
    explicit LinearAllocator(size_t size);

    ~LinearAllocator() override;

    void *allocate(size_t size, size_t alignment = alignof(std::max_align_t)) override;

    void deallocate(void *ptr, size_t size = 0) noexcept override;

    void reset() noexcept;

private:
    void* memory_pool_;
    size_t total_size_;
    size_t current_offset_;
    std::mutex mutex_;

    LinearAllocator(const LinearAllocator &) = delete;
    LinearAllocator &operator=(const LinearAllocator &) = delete;
};