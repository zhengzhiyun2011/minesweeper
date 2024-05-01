#include "game_board.h"

#include "tools.h"
using std::array;
using std::bernoulli_distribution;
using std::make_pair;
using std::pair;
using std::ptrdiff_t;
using std::size_t;
using std::vector;

namespace minesweeper
{
    GameBoard::GameBoard(
        size_t height,
        size_t width,
        double probability,
        size_t selected_x,
        size_t selected_y
    )
        : m_board(height, vector<Block>(width))
        , m_surrounding_mines_table(height, vector<int>(width))
        , m_blocks_remaining_to_open(0)
    {
        bernoulli_distribution whether_or_not_is_a_mine(probability);
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                if (!(i == selected_x && j == selected_y) &&
                    whether_or_not_is_a_mine(random_engine)) {
                    m_board[i][j].is_mine = true;
                } else {
                    ++m_blocks_remaining_to_open;
                }
            }
        }

        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                m_surrounding_mines_table[i][j] = count_surrounding_mines(i, j);
            }
        }

        open(selected_x, selected_y);
    }

    int GameBoard::count_surrounding_mines(size_t x, size_t y) const noexcept
    {
        int number_of_surrounding_mines = 0;
        for (const auto& offset : Block::offsets_to_next_blocks) {
            pair<size_t, size_t> next_pos{ static_cast<ptrdiff_t>(x) + offset.first,
                                           static_cast<ptrdiff_t>(y) + offset.second };
            if (is_valid_pos(next_pos.first, next_pos.second) &&
                is_mine(next_pos.first, next_pos.second)) {
                ++number_of_surrounding_mines;
            }
        }

        return number_of_surrounding_mines;
    }

    void GameBoard::open(size_t x, size_t y)
    {
        if (is_open(x, y)) {
            throw DuplicateOperationException(messages::open_repeatly);
        } else if (is_marked(x, y)) {
            throw OperationFailureException(messages::open_failed);
        } else if (is_mine(x, y)) {
            throw MineHitException(messages::mine_hit);
        } else [[likely]] {
            m_board.at(x).at(y).is_open = true;
            --m_blocks_remaining_to_open;
            if (m_surrounding_mines_table[x][y] == 0) {
                for (const auto& offset : Block::offsets_to_next_blocks) {
                    pair<size_t, size_t> next_pos{ static_cast<ptrdiff_t>(x) + offset.first,
                                                   static_cast<ptrdiff_t>(y) + offset.second };
                    if (is_valid_pos(next_pos.first, next_pos.second)) {
                        try {
                            open(next_pos.first, next_pos.second);
                        } catch (...) {
                            // do nothing
                        }
                    }
                }
            }
        }
    }
}  // namespace minesweeper
