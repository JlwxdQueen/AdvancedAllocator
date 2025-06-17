#include "../include/utils.h"
#include <stdexcept>
#include <vector>
#include <list>

namespace utils {

bool is_power_of_2(size_t n) {
    return n != 0 && (n & (n - 1)) == 0;
}

std::vector<std::list<void*>> break_into_blocks(unsigned int block_to_break, unsigned int target_size) {
    if (!is_power_of_2(block_to_break)) {
        throw std::invalid_argument("Block size must be a power of 2");
    }
    if (block_to_break == 0 || target_size == 0) {
        throw std::invalid_argument("Block size and target size must be positive");
    }
    if (block_to_break < target_size) {
        throw std::invalid_argument("Initial block size is smaller than target size");
    }

    std::vector<std::list<void*>> result;
    
    if (block_to_break <= target_size) {
        result.push_back(std::list<void*>());
        return result;
    }

    unsigned int half_size = block_to_break / 2;
    
    if (half_size > target_size) {
        auto left_buddies = break_into_blocks(half_size, target_size);
        auto right_buddies = break_into_blocks(half_size, target_size);
        
        result.insert(result.end(), left_buddies.begin(), left_buddies.end());
        result.insert(result.end(), right_buddies.begin(), right_buddies.end());
    } else {
        result.push_back(std::list<void*>());
    }

    return result;
}

}