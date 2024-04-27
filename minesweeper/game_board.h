#pragma once
#ifndef GAME_BOARD_H_
#  define GAME_BOARD_H_
#  include <array>
#  include <format>
#  include <stdexcept>
#  include <utility>
#  include <vector>

#  include "setting.h"

namespace minesweeper
{
    constexpr char closed_block_display_char = '?';
    constexpr char marked_block_display_char = '$';

    // 块结构体
    struct Block
    {
        bool is_open;
        bool is_marked;
        bool is_mine;
        // 前往周围块的偏移量数组
        static constexpr std::array<std::pair<ptrdiff_t, ptrdiff_t>, 8> offsets_to_next_blocks{
            std::make_pair(1, 0),  std::make_pair(1, 1),  std::make_pair(1, -1),
            std::make_pair(0, 1),  std::make_pair(0, -1), std::make_pair(-1, 1),
            std::make_pair(-1, 0), std::make_pair(-1, -1)
        };
    };

    // 重复操作异常
    class DuplicateOperationException : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    // 触雷异常
    class MineHitException : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    // 操作失败异常
    class OperationFailureException : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    // 游戏板类
    class GameBoard
    {
    public:
        GameBoard() = delete;
        // 指定长宽、雷出现的几率和第一次选择的坐标构造一个游戏板
        // 在第一次选择的坐标上不会出现雷
        GameBoard(
            std::size_t height,
            std::size_t width,
            double probability,
            std::size_t selected_x,
            std::size_t selected_y
        );

        // 判断坐标是否有效
        inline bool is_valid_pos(std::size_t x, std::size_t y) const noexcept;
        // 判断坐标指定的块是否被标记过
        inline bool is_marked(std::size_t x, std::size_t y) const;
        // 判断坐标指定的块是否被打开过
        inline bool is_open(std::size_t x, std::size_t y) const;
        // 判断坐标指定的块是否是雷
        inline bool is_mine(std::size_t x, std::size_t y) const;
        inline bool is_won() const noexcept;
        int count_surrounding_mines(std::size_t x, std::size_t y) const noexcept;
        inline void mark(std::size_t x, std::size_t y);
        inline void unmark(std::size_t x, std::size_t y);
        void open(std::size_t x, std::size_t y);
    private:
        std::vector<std::vector<Block>> m_board;
        std::vector<std::vector<int>> m_surrounding_mines_table;
        int m_blocks_remaining_to_open;
        friend struct std::formatter<GameBoard>;
    };
}  // namespace minesweeper

namespace std
{
    template <typename CharT>
    struct formatter<minesweeper::GameBoard, CharT> : public formatter<bool, CharT>
    {
        template <typename FormatContext>
        auto format(const minesweeper::GameBoard& board, FormatContext& context) const;
    };
}  // namespace std

namespace minesweeper
{

    inline bool GameBoard::is_valid_pos(std::size_t x, std::size_t y) const noexcept
    {
        return x < m_board.size() && y < m_board[0].size();
    }

    inline bool GameBoard::is_marked(std::size_t x, std::size_t y) const
    {
        return m_board.at(x).at(y).is_marked;
    }

    inline bool GameBoard::is_open(std::size_t x, std::size_t y) const
    {
        return m_board.at(x).at(y).is_open;
    }

    inline bool GameBoard::is_mine(std::size_t x, std::size_t y) const
    {
        return m_board.at(x).at(y).is_mine;
    }

    inline bool GameBoard::is_won() const noexcept
    {
        return m_blocks_remaining_to_open == 0;
    }

    inline void GameBoard::mark(std::size_t x, std::size_t y)
    {
        if (is_open(x, y)) {
            throw OperationFailureException(messages::mark_failed);
        } else if (is_marked(x, y)) {
            throw DuplicateOperationException(messages::mark_repeatly);
        } else {
            m_board.at(x).at(y).is_marked = true;
        }
    }

    inline void GameBoard::unmark(std::size_t x, std::size_t y)
    {
        if (is_open(x, y)) {
            throw OperationFailureException(messages::unmark_failed);
        } else if (!is_marked(x, y)) {
            throw DuplicateOperationException(messages::unmark_repeatly);
        } else {
            m_board.at(x).at(y).is_marked = false;
        }
    }
}  // namespace minesweeper

namespace std
{
    template <typename CharT>
    template <typename FormatContext>
    auto formatter<minesweeper::GameBoard, CharT>::format(
        const minesweeper::GameBoard& board,
        FormatContext& context
    ) const
    {
        auto out_iter = context.out();
        (out_iter = '%') = '|';
        for (std::size_t i = 0; i < board.m_board[0].size(); ++i) {
            out_iter = static_cast<char>(i + 1 + '0');
        }

        ((out_iter = '\n') = '-') = '|';
        for (std::size_t i = 0; i < board.m_board[0].size(); ++i) {
            out_iter = '-';
        }

        out_iter = '\n';
        for (std ::size_t i = 0; i < board.m_board.size(); ++i) {
            out_iter = static_cast<char>(i + 1 + '0');
            out_iter = '|';
            for (std::size_t j = 0; j < board.m_board[0].size(); ++j) {
                if (board.is_open(i, j)) {
                    out_iter = static_cast<char>(board.m_surrounding_mines_table[i][j] + '0');
                } else if (board.is_marked(i, j)) {
                    out_iter = minesweeper::marked_block_display_char;
                } else {
                    out_iter = minesweeper::closed_block_display_char;
                }
            }

            out_iter = '\n';
        }

        return out_iter;
    }
}  // namespace std
#endif  // !GAME_BOARD_H_
