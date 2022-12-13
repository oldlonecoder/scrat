/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <scrat/dlconfig.h>

#include <string>

#include <vector>

namespace scrat {
struct SCRAT_API Icon final
{
    using T = const char*;
    using Type = uint8_t;
    using DataArray = std::vector<Icon::T>;
    static constexpr T CNullPtr              = "  ";
    static constexpr T CArrowRight           = "➜ ";
    static constexpr T CArrowLeft            = "⬅ ";
    static constexpr T CArrowUp              = "⬆ ";
    static constexpr T CArrowDown            = "⬇ ";
    static constexpr T CArrowUpRight         = "⬈ ";
    static constexpr T CArrowUpLeft          = "⬉ ";
    static constexpr T CArrowDownRight       = "⬊ ";
    static constexpr T CArrowDownLeft        = "⬋ ";
    static constexpr T CArrowHeadRight       = "➤ ";
    static constexpr T CArrowDheadRight      = "⨠ ";
    static constexpr T CPencilDr             = "✎ ";
    static constexpr T CPencilUr             = "✐ ";
    static constexpr T CErr1                 = "✘ ";
    static constexpr T CFlag                 = "⚑ ";
    static constexpr T CBolt                 = "⚡ ";
    static constexpr T CSuccess              = "✔ ";
    static constexpr T CSmallDot             = "⚫ ";
    static constexpr T CBigDot               = "⬤ ";
    static constexpr T CBall1                = "❶ ";
    static constexpr T CBall2                = "❷ ";
    static constexpr T CDeadHead             = "☠ ";
    static constexpr T CYinYang              = "☯ ";
    static constexpr T CSadface              = "☹ ";
    static constexpr T CHappy1               = "☺ ";
    static constexpr T CHappy2               = "☻ ";
    static constexpr T CCircleX              = "⨂ ";
    static constexpr T CFunction             = "⨍ ";
    static constexpr T CCut1                 = "✂ ";
    static constexpr T CCut2                 = "✁ ";
    static constexpr T CCut3                 = "✂ ";
    static constexpr T CCMarkX               = "☒ ";
    static constexpr T CBaseline             = "⩡ ";
    static constexpr T CStar5                = "✭ ";
    static constexpr T CCMarkCheck           = "☑ ";
    static constexpr T CSwordCross           = "⚔ ";
    static constexpr T CHammerCross          = "⚒ ";
    static constexpr T CSmallDot2            = "⚫ ";
    static constexpr T CErr2                 = "✖ ";
    static constexpr T CErr3                 = "✗ ";
    static constexpr T CSQuote1              = "❜ ";
    static constexpr T CDQuoteLeft           = "❝ ";
    static constexpr T CDuoteRight           = "❞ ";
    static constexpr T CDowntobottomleft     = "⟀ ";
    static constexpr T CUndo                 = "⟲ ";
    static constexpr T CEnter                = "⤵ ";
    static constexpr T CKeyEnter             = "⤶ ";
    static constexpr T CLinux                = " ";
    static constexpr T CArch                 = " ";
    static constexpr T CHome                 = " ";
    static constexpr T CFile                 = " ";
    static constexpr T CThumbDown            = " ";
    static constexpr T CGithubCircle         = " ";
    static constexpr T CGithubFolder         = " ";
    static constexpr T CWindows              = " ";
    static constexpr T CPi                   = " ";
    static constexpr T CFolder               = " ";
    static constexpr T CBug                  = " ";
    static constexpr T CInfo                 = "כֿ ";
    static constexpr T CWarning              = " ";

    static constexpr Type NullPtr            = 0;
    static constexpr Type ArrowRight         = 1;
    static constexpr Type ArrowLeft          = 2;
    static constexpr Type ArrowUp            = 3;
    static constexpr Type ArrowDown          = 4;
    static constexpr Type ArrowUpRight       = 5;
    static constexpr Type ArrowUpLeft        = 6;
    static constexpr Type ArrowDownRight     = 7;
    static constexpr Type ArrowDownLeft      = 8;
    static constexpr Type ArrowHeadRight     = 9;
    static constexpr Type ArrowDheadRight    = 10;
    static constexpr Type PencilDr           = 11;
    static constexpr Type PencilUr           = 12;
    static constexpr Type Err1               = 13;
    static constexpr Type Flag               = 14;
    static constexpr Type Bolt               = 15;
    static constexpr Type Success            = 16;
    static constexpr Type SmallDot           = 17;
    static constexpr Type BigDot             = 18;
    static constexpr Type Ball1              = 19;
    static constexpr Type Ball2              = 20;
    static constexpr Type DeadHead           = 21;
    static constexpr Type YinYang            = 22;
    static constexpr Type Sadface            = 23;
    static constexpr Type Happy1             = 24;
    static constexpr Type Happy2             = 25;
    static constexpr Type CircleX            = 26;
    static constexpr Type Function           = 27;
    static constexpr Type Cut1               = 28;
    static constexpr Type Cut2               = 29;
    static constexpr Type Cut3               = 30;
    static constexpr Type CMarkX             = 31;
    static constexpr Type Baseline           = 32;
    static constexpr Type Star5              = 33;
    static constexpr Type CMarkCheck         = 34;
    static constexpr Type SwordCross         = 35;
    static constexpr Type HammerCross        = 36;
    static constexpr Type SmallDot2          = 37;
    static constexpr Type Err2               = 38;
    static constexpr Type Err3               = 39;
    static constexpr Type SQuote1            = 40;
    static constexpr Type DQuoteLeft         = 41;
    static constexpr Type DQuoteRight        = 42;
    static constexpr Type Downtobottomleft   = 43;
    static constexpr Type Undo               = 44;
    static constexpr Type Enter              = 45;
    static constexpr Type KeyEnter           = 46;
    static constexpr Type Linux              = 47;
    static constexpr Type Arch               = 48;
    static constexpr Type Home               = 49;
    static constexpr Type File               = 50;
    static constexpr Type ThumbDown          = 51;
    static constexpr Type GithubCircle       = 52;
    static constexpr Type GithubFolder       = 53;
    static constexpr Type Windows            = 54;
    static constexpr Type Pi                 = 55;
    static constexpr Type Folder             = 56;
    static constexpr Type Bug                = 57;
    static constexpr Type Info               = 58;
    static constexpr Type Warning            = 59;
    static Icon::DataArray Data;

    static Icon::Type Scan(std::string IcName);
};
}
