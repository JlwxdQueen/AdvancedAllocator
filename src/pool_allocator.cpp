#include "../include/pool_allocator.h"
#include "../include/utils.h"
#include <stdexcept>
#include <cstring>
#include <cstdint>
#include <cstdlib>

PoolAllocator::PoolAllocator(size_t block_size, size_t num_blocks)
    : block_size_(block_size)
    , pool_size_(block_size * num_blocks)
{
    if (block_size == 0 || num_blocks == 0) {
        throw std::invalid_argument("Block size and number of blocks must be positive and not zero");
    }

    pool_ = std::aligned_alloc(alignof(std::max_align_t), pool_size_);
    if (!pool_) {
        throw std::bad_alloc();
    }

    free_list_.reserve(num_blocks);
    char* current = static_cast<char*>(pool_);
    for (size_t i = 0; i < num_blocks; ++i) {
        free_list_.push_back(current);
        current += block_size_;
    }
}

PoolAllocator::~PoolAllocator() {
    std::free(pool_);
}

void* PoolAllocator::allocate(size_t size, size_t alignment) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (size > block_size_) {
        throw std::invalid_argument("Requested size is larger than block size");
    }

    if (!utils::is_power_of_2(alignment)) {
        throw std::invalid_argument("Alignment must be a power of 2");
    }

    if (free_list_.empty()) {
        throw std::bad_alloc();
    }

    void* block = free_list_.back();
    free_list_.pop_back();

    uintptr_t addr = reinterpret_cast<uintptr_t>(block);
    if (addr % alignment != 0) {
        free_list_.push_back(block);
        throw std::bad_alloc();
    }

    return block;
}

void PoolAllocator::deallocate(void* ptr, [[maybe_unused]] size_t size) noexcept {
    if (!ptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    auto ptr_addr = reinterpret_cast<uintptr_t>(ptr);
    auto pool_start = reinterpret_cast<uintptr_t>(pool_);
    auto pool_end = pool_start + pool_size_;

    if (ptr_addr < pool_start || ptr_addr >= pool_end) {
        return;
    }

    free_list_.push_back(ptr);
} 