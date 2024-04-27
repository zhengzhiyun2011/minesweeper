#pragma once
#ifndef TOOLS_H_
#  define TOOLS_H_
#  include <iostream>
#  include <random>

namespace minesweeper
{
    extern std::random_device get_random_seed;
    extern std::default_random_engine random_engine;
}  // namespace minesweeper
#endif  // !TOOLS_H_

