#include "tools.h"
using std::default_random_engine;
using std::random_device;

namespace minesweeper
{
    random_device get_random_seed{};
    default_random_engine random_engine{ get_random_seed() };
}  // namespace minesweeper
