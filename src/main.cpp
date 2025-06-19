#include <iostream>
#include <vector>
#include "../include/buddy_allocator.h"
#include "../include/pool_allocator.h"
#include "../include/linear_allocator.h"

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

        std::cout << "\n--- LinearAllocator Demo ---\n";
        LinearAllocator linear(2048);  // 2KB pool
        
        std::cout << "Created Linear Allocator with 2KB pool\n";
        
        std::cout << "\nTesting different alignments:\n";
        void* ptr1 = linear.allocate(100, 4);
        void* ptr2 = linear.allocate(200, 8);  
        void* ptr3 = linear.allocate(150, 16);
        
        std::cout << "Allocated 100 bytes (4-byte aligned) at " << ptr1 << "\n";
        std::cout << "Allocated 200 bytes (8-byte aligned) at " << ptr2 << "\n";
        std::cout << "Allocated 150 bytes (16-byte aligned) at " << ptr3 << "\n";

        std::cout << "\nAlignment verification:\n";
        std::cout << "ptr1 address: " << (uintptr_t)ptr1 << " (divisible by 4: " 
                  << ((uintptr_t)ptr1 % 4 == 0 ? "YES" : "NO") << ")\n";
        std::cout << "ptr2 address: " << (uintptr_t)ptr2 << " (divisible by 8: " 
                  << ((uintptr_t)ptr2 % 8 == 0 ? "YES" : "NO") << ")\n";
        std::cout << "ptr3 address: " << (uintptr_t)ptr3 << " (divisible by 16: " 
                  << ((uintptr_t)ptr3 % 16 == 0 ? "YES" : "NO") << ")\n";

        std::cout << "\nAttempting individual deallocation (should do nothing):\n";
        linear.deallocate(ptr1, 100);
        linear.deallocate(ptr2, 200);
        linear.deallocate(ptr3, 150);
        std::cout << "Individual deallocation completed (but memory is still 'used')\n";

        void* ptr4 = linear.allocate(300);
        std::cout << "Allocated 300 more bytes at " << ptr4 << "\n";

        std::cout << "\nResetting the allocator...\n";
        linear.reset();

        void* ptr5 = linear.allocate(500);
        std::cout << "After reset, allocated 500 bytes at " << ptr5 << "\n";
        std::cout << "Note: ptr5 should be at the same address as ptr1 was!\n";

        std::cout << "\nTesting allocation beyond pool size:\n";
        try {
            void* ptr6 = linear.allocate(3000);
            std::cout << "Unexpectedly allocated 3000 bytes at " << ptr6 << "\n";
        } catch (const std::bad_alloc& e) {
            std::cout << "Correctly caught std::bad_alloc: " << e.what() << "\n";
        }
        
        std::cout << "\nLinear Allocator demo completed successfully!\n";
        
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
