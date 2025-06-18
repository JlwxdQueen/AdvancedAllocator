#include <iostream>
#include <vector>
#include "../include/buddy_allocator.h"
#include "../include/pool_allocator.h"

int main() {
    try {
        std::cout << "--- PoolAllocator Demo ---\n";
        PoolAllocator pool(1024, 4);
        std::vector<void *> pool_ptrs;
        for (int i = 0; i < 4; ++i) {
            void *ptr = pool.allocate(512);
            std::cout << "Allocated block " << i << " at " << ptr << "\n";
            pool_ptrs.push_back(ptr);
        }
        for (int i = 0; i < 4; ++i) {
            pool.deallocate(pool_ptrs[i]);
            std::cout << "Deallocated block " << i << " at " << pool_ptrs[i] << "\n";
        }

        std::cout << "\n--- BuddyAllocator Demo ---\n";
        BuddyAllocator buddy(4096);
        std::vector<void *> buddy_ptrs;
        size_t sizes[] = {100, 200, 300, 400};
        for (int i = 0; i < 4; ++i) {
            void *ptr = buddy.allocate(sizes[i]);
            std::cout << "Allocated " << sizes[i] << " bytes at " << ptr << "\n";
            buddy_ptrs.push_back(ptr);
        }
        for (int i = 0; i < 4; ++i) {
            buddy.deallocate(buddy_ptrs[i], sizes[i]);
            std::cout << "Deallocated " << sizes[i] << " bytes at " << buddy_ptrs[i] << "\n";
        }
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
