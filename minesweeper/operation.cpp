#include "operation.h"
using std::function;
using std::size_t;
using std::unordered_map;

namespace minesweeper
{
    extern const unordered_map<char, function<void(GameBoard*, size_t, size_t)>> operations_table{
        { 'o', &GameBoard::open },
        { 'm', &GameBoard::mark },
        { 'u', &GameBoard::unmark }
    };
}  // namespace minesweeper
