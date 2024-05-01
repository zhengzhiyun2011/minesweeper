#include <cstdlib>

#include <iostream>

#include "operation.h"
using minesweeper::clear_console;
using minesweeper::congratulate;
using minesweeper::GameBoard;
using minesweeper::receive_and_do_operation;
using minesweeper::refresh_console;
using minesweeper::welcome;
using std::cerr;
using std::cin;
using std::cout;
using std::system;

int main()
{
#ifdef _WIN32
    system("chcp 65001");  // 更改字符集，防止乱码
    clear_console(cout);
#endif  // _WIN32
    GameBoard board = welcome(cin, cout, cerr);
    while (!board.is_won()) {
        refresh_console(board, cout);
        if (!receive_and_do_operation(cin, cout, cerr, board)) {
            goto END;
        }
    }

    refresh_console(board, cout);
    congratulate(cin, cout);
END:
    return 0;
}
