#include "../include/linear_allocator.h"
#include "../internal/utils.h"
#include <cstdlib>
#include <stdexcept>
#include <cstring>

LinearAllocator::LinearAllocator(size_t pool_size)
    : total_size_(pool_size), current_offset_(0) {
    if (pool_size == 0) {
        throw std::invalid_argument("Pool size must be greater than 0");
    }

    memory_pool_ = std::malloc(pool_size);
    if (!memory_pool_) {
        throw std::bad_alloc();
    }

    std::memset(memory_pool_, 0, pool_size);
}

LinearAllocator::~LinearAllocator() {
    if (memory_pool_) {
        std::free(memory_pool_);
        memory_pool_ = nullptr;
    }
}

void* LinearAllocator::allocate(size_t size, size_t alignment) {
    if (size == 0) {
        return nullptr;
    }

    if (!utils::is_power_of_2(alignment)) {
        throw std::invalid_argument("Alignment must be a power of 2");
    }

    std::lock_guard<std::mutex> lock(mutex_);

    size_t aligned_offset = (current_offset_ + alignment - 1) & ~(alignment - 1);

    if (aligned_offset + size > total_size_) {
        throw std::bad_alloc();
    }

    void* ptr = static_cast<char*>(memory_pool_) + aligned_offset;
    current_offset_ = aligned_offset + size;
    return ptr;
}

void LinearAllocator::deallocate(void* ptr, size_t size) noexcept {
    if (size == 0 || ptr == nullptr) {
        return;
    }
}

void LinearAllocator::reset() noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    current_offset_ = 0;
}