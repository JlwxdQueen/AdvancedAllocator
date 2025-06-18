#pragma once
#include <list>
#include <mutex>
#include <vector>
#include "allocator.h"

class BuddyAllocator : public Allocator {
public:
    explicit BuddyAllocator(size_t max_block_size);
    ~BuddyAllocator() override;

    void *allocate(size_t size, size_t alignment = alignof(std::max_align_t)) override;
    void deallocate(void *ptr, size_t size = 0) noexcept override;

private:
    size_t max_block_size_;
    size_t find_free_block_(size_t size);
    void split_block_(size_t size);
    void merge_buddies_(void *ptr, size_t size);
    std::vector<std::list<void *>> free_lists_;
    std::mutex mutex_;
    void *initial_block_ = nullptr;
};
