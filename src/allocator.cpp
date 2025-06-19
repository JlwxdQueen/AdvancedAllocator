#include "../include/allocator.h"
#include "../internal/utils.h"

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

void *Allocator::allocate(size_t size, size_t alignment) {
    if (!utils::is_power_of_2(alignment)) {
        throw std::invalid_argument("Alignment must be a power of 2");
    }

    void *ptr = nullptr;
    int result = posix_memalign(&ptr, alignment, size);
    if (result != 0) {
        throw std::bad_alloc();
    }
    return ptr;
}

void Allocator::deallocate(void *ptr, [[maybe_unused]] size_t size) noexcept {
    if (ptr == nullptr) {
        return;
    }
    free(ptr);
}
