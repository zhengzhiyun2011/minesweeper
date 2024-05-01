#pragma once
#ifndef OPERATION_H_
#  define OPERATION_H_
#  include <cctype>

#  include <chrono>
#  include <functional>
#  include <ios>
#  include <limits>
#  include <string_view>
#  include <thread>
#  include <unordered_map>

#  include "game_board.h"
#  include "tools.h"

namespace minesweeper
{
    // 操作表
    extern const std::unordered_map<char, std::function<void(GameBoard*, std::size_t, std::size_t)>>
        operations_table;

    // 清空控制台
    template <typename CharT>
    void clear_console(std::basic_ostream<CharT>& out_stream);
    // 重新打印游戏板
    template <typename CharT>
    void refresh_console(const GameBoard& board, std::basic_ostream<CharT>& out_stream);
    // 重复要求输入，直到输入合法
    template <typename CharT, typename... Args>
    void input_safely(
        const CharT* message,
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream,
        Args&... args
    );
    // 输出欢迎页面，并开始游戏
    template <typename CharT>
    GameBoard welcome(
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream
    );
    // 庆祝胜利
    template <typename CharT>
    void congratulate(std::basic_istream<CharT>& in_stream, std::basic_ostream<CharT>& out_stream);
    // 接受操作并执行
    template <typename CharT>
    bool receive_and_do_operation(
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream,
        GameBoard& board
    );

    template <typename CharT>
    void clear_console(std::basic_ostream<CharT>& out_stream)
    {
        out_stream << "\033c";
    }

    template <typename CharT>
    void refresh_console(const GameBoard& board, std::basic_ostream<CharT>& out_stream)
    {
        clear_console(out_stream);
        out_stream << std::format("{}", board);
    }

    template <typename CharT, typename... Args>
    void input_safely(
        const CharT* message,
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream,
        Args&... args
    )
    {
        for (;;) {
            out_stream << message;
            (in_stream >> ... >> args);
            if (in_stream.fail()) {
                in_stream.clear();
                in_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                err_stream << messages::stream_error;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                clear_console(err_stream);
            } else {
                break;
            }
        }
    }

    template <typename CharT>
    GameBoard welcome(
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream
    )
    {
        std::size_t height, width, x, y;
        for (;;) {
            input_safely(messages::welcome, in_stream, out_stream, err_stream, height, width, x, y);
            if (x < 1 || y < 1 || x > height || y > width) {
                err_stream << messages::stream_error;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                clear_console(err_stream);
            } else {
                break;
            }
        }

        return { height, width, difficulty, x - 1, y - 1 };
    }

    template <typename CharT>
    void congratulate(std::basic_istream<CharT>& in_stream, std::basic_ostream<CharT>& out_stream)
    {
        out_stream << messages::congratulatory_words;
        in_stream.get();
        in_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    template <typename CharT>
    bool receive_and_do_operation(
        std::basic_istream<CharT>& in_stream,
        std::basic_ostream<CharT>& out_stream,
        std::basic_ostream<CharT>& err_stream,
        GameBoard& board
    )
    {
        char operation_type;
        std::size_t x, y;
        input_safely(
            messages::requires_an_action,
            in_stream,
            out_stream,
            err_stream,
            operation_type,
            x,
            y
        );
        operation_type = static_cast<char>(std::tolower(operation_type));
        if (operations_table.contains(operation_type)) {
            try {
                operations_table.at(operation_type)(&board, x - 1, y - 1);
                goto SUCCESS;
            } catch (const MineHitException& e) {
                err_stream << std::format("{}\n", e.what());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                goto MINE_HIT;
            } catch (const std::runtime_error& e) {
                err_stream << std::format("{}\n", e.what());
            } catch (const std::out_of_range&) {
                err_stream << messages::invalid_position;
            }
        } else {
            out_stream << messages::invalid_input;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
SUCCESS:
        return true;
MINE_HIT:
        return false;
    }
}  // namespace minesweeper
#endif  // !OPERATION_H_
