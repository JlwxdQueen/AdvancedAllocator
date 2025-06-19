#include "../include/buddy_allocator.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include "../internal/utils.h"

BuddyAllocator::BuddyAllocator(size_t max_block_size) : max_block_size_(max_block_size) {
    if (!utils::is_power_of_2(max_block_size)) {
        throw std::invalid_argument("Max block size must be a power of 2");
    }

    size_t current_size = max_block_size;
    while (current_size >= alignof(std::max_align_t)) {
        free_lists_.push_back(std::list<void *>());
        current_size /= 2;
    }

    initial_block_ = std::aligned_alloc(alignof(std::max_align_t), max_block_size);
    if (!initial_block_) {
        throw std::bad_alloc();
    }

    free_lists_[0].push_back(initial_block_);
}

BuddyAllocator::~BuddyAllocator() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::free(initial_block_);
}

void *BuddyAllocator::allocate(size_t size, size_t alignment) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (size == 0) {
        return nullptr;
    }

    if (!utils::is_power_of_2(alignment)) {
        throw std::invalid_argument("Alignment must be a power of 2");
    }

    size_t required_size = 1;
    while (required_size < size) {
        required_size *= 2;
    }

    if (required_size > max_block_size_) {
        throw std::bad_alloc();
    }

    size_t list_index = 0;
    size_t current_size = max_block_size_;
    while (current_size > required_size) {
        current_size /= 2;
        list_index++;
    }

    size_t split_index = list_index;
    while (split_index > 0 && free_lists_[split_index].empty()) {
        split_index--;
    }

    if (free_lists_[split_index].empty()) {
        throw std::bad_alloc();
    }

    while (split_index < list_index) {
        void *block = free_lists_[split_index].front();
        free_lists_[split_index].pop_front();
        size_t block_size = max_block_size_ >> split_index;
        void *buddy = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(block) + block_size / 2);
        free_lists_[split_index + 1].push_back(buddy);
        free_lists_[split_index + 1].push_back(block);
        split_index++;
    }

    void *result = free_lists_[list_index].front();
    free_lists_[list_index].pop_front();

    uintptr_t addr = reinterpret_cast<uintptr_t>(result);
    if (addr % alignment != 0) {
        free_lists_[list_index].push_back(result);
        throw std::bad_alloc();
    }

    return result;
}

void BuddyAllocator::deallocate(void *ptr, [[maybe_unused]] size_t size) noexcept {
    if (!ptr) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    size_t block_size = max_block_size_;
    size_t list_index = 0;

    while (list_index < free_lists_.size() - 1) {
        if (block_size / 2 >= size) {
            block_size /= 2;
            list_index++;
        } else {
            break;
        }
    }

    while (list_index > 0) {
        uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
        uintptr_t buddy_addr = addr ^ block_size;

        auto &current_list = free_lists_[list_index];
        auto buddy_it = std::find_if(current_list.begin(), current_list.end(),
                                     [buddy_addr](void *p) { return reinterpret_cast<uintptr_t>(p) == buddy_addr; });

        if (buddy_it == current_list.end()) {
            break;
        }

        current_list.erase(buddy_it);

        ptr = reinterpret_cast<void *>(std::min(addr, buddy_addr));

        block_size *= 2;
        list_index--;
    }

    free_lists_[list_index].push_back(ptr);
}
