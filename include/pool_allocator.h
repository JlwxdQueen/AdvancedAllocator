#pragma once
#include "allocator.h"
#include <vector>
#include <mutex>

class PoolAllocator : public Allocator {
public:
    explicit PoolAllocator(size_t block_size, size_t num_blocks);
    
    ~PoolAllocator() override;

    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) override;

    void deallocate(void* ptr, size_t size = 0) noexcept override;

private:
    size_t block_size_;
    std::vector<void*> free_list_; 
    void* pool_;
    size_t pool_size_;
    std::mutex mutex_;
    
    PoolAllocator(const PoolAllocator&) = delete;
    PoolAllocator& operator=(const PoolAllocator&) = delete;
}; 