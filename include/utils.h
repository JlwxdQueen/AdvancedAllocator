#pragma once
#include <cstddef>
#include <list>
#include <vector>

namespace utils {

    bool is_power_of_2(size_t n);

    std::vector<std::list<void *>> break_into_blocks(unsigned int block_to_break, unsigned int target_size);

} // namespace utils
