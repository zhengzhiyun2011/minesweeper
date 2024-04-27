#pragma once
#ifndef SETTING_H_
#  define SETTING_H_
// #  define ENGLISH_LANGUAGE
#  define CHINESE_LANGUAGE

namespace minesweeper
{
    constexpr double difficulty = 0.1;

    namespace messages
    {
#  ifdef ENGLISH_LANGUAGE
        constexpr const char* welcome =
            "Welcome to Minesweeper.\n"
            "This is a console version of Minesweeper written by ZZY.\n"
            "Have fun!\n"
            "\n"
            "Open source with GPL3.0.\n"
            "\n"
            "Please enter the size of the game board and the first open position to start the "
            "game...\n";
        constexpr const char* congratulatory_words =
            "You win!"
            "Press Enter to end the game.\n";
        constexpr const char* requires_an_action =
            "Please enter one of 'o' (open), 'm' (mark), 'u' (unmark).\n";
        constexpr const char* stream_error = "Please enter the correct parameters\n";
        constexpr const char* invalid_input = "The input is invalid!\n";
        constexpr const char* invalid_position = "The location is invalid.\n";
        constexpr const char* unknown_operation =
            "Unknown operation. Please enter one of 'o' (open), 'm' (mark), 'u' (unmark).\n";
        constexpr const char* open_repeatly = "Open repeatedly!";
        constexpr const char* mark_repeatly = "Mark repeatedly!";
        constexpr const char* unmark_repeatly = "This block has not been marked!";
        constexpr const char* open_failed =
            "Blocks that have already been marked cannot be opened.";
        constexpr const char* mark_failed =
            "It is not possible to mark a block that has already been opened.";
        constexpr const char* unmark_failed =
            "It is not possible to unmark a block that has already been opened";
        constexpr const char* mine_hit = "The block is a mine!";
#  elif defined(CHINESE_LANGUAGE)
        constexpr const char* welcome =
            "欢迎来到扫雷。\n"
            "这是 ZZY 编写的 Minesweeper 的控制台版本。\n"
            "玩得愉快！\n"
            "\n"
            "使用 GPL3.0 开源。\n"
            "\n"
            "请输入游戏板的大小和第一个打开的位置以开始游戏...\n";
        constexpr const char* congratulatory_words =
            "你赢了!\n"
            "按Enter键以结束游戏...";
        constexpr const char* requires_an_action =
            "请输入‘o’（打开），‘m’ (标记)，‘u’（取消标记）中的其中一个。\n";
        constexpr const char* stream_error = "请输入正确的参数。\n";
        constexpr const char* invalid_input = "输入无效！\n";
        constexpr const char* invalid_position = "坐标无效。\n";
        constexpr const char* unknown_operation =
            "未知的操作。请输入‘o’（打开），‘m’ （标记），‘u’（取消标记）中的其中一个。\n";
        constexpr const char* open_repeatly = "这一个块已经被打开了！";
        constexpr const char* mark_repeatly = "这一个块已经被标记了！";
        constexpr const char* unmark_repeatly = "这一个块没有被标记过！";
        constexpr const char* open_failed = "被标记的块不能被打开。";
        constexpr const char* mark_failed = "被打开的块不能被标记。";
        constexpr const char* unmark_failed = "被打开的块不能被取消标记。";
        constexpr const char* mine_hit = "这个块是一个雷！";
#  endif
    }  // namespace messages
}  // namespace minesweeper
#endif  // !SETTING_H_

