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

#include <scrat/Icons.h>

namespace scrat
{
Icon::DataArray Icon::Data =
{
    CNullPtr            ,
    CArrowRight         ,
    CArrowLeft          ,
    CArrowUp            ,
    CArrowDown          ,
    CArrowUpRight       ,
    CArrowUpLeft        ,
    CArrowDownRight     ,
    CArrowDownLeft      ,
    CArrowHeadRight     ,
    CArrowDheadRight    ,
    CPencilDr           ,
    CPencilUr           ,
    CErr1               ,
    CFlag               ,
    CBolt               ,
    CSuccess            ,
    CSmallDot           ,
    CBigDot             ,
    CBall1              ,
    CBall2              ,
    CDeadHead           ,
    CYinYang            ,
    CSadface            ,
    CHappy1             ,
    CHappy2             ,
    CCircleX            ,
    CFunction           ,
    CCut1               ,
    CCut2               ,
    CCut3               ,
    CCMarkX             ,
    CBaseline           ,
    CStar5              ,
    CCMarkCheck         ,
    CSwordCross         ,
    CHammerCross        ,
    CSmallDot2          ,
    CErr2               ,
    CErr3               ,
    CSQuote1            ,
    CDQuoteLeft         ,
    CDuoteRight         ,
    CDowntobottomleft   ,
    CUndo               ,
    CEnter              ,
    CKeyEnter           ,
    CLinux              ,
    CArch               ,
    CHome               ,
    CFile               ,
    CThumbDown          ,
    CGithubCircle       ,
    CGithubFolder       ,
    CWindows            ,
    CPi                 ,
    CFolder             ,
    CBug                ,
    CInfo               ,
    CWarning
};

const char* IcNames[] =
{
    "NullPtr","ArrowRight","ArrowLeft","ArrowUp","ArrowDown","ArrowUpRight","ArrowUpLeft","ArrowDownRight","ArrowDownLeft",
    "ArrowHeadRight","ArrowDheadRight","PencilDr","PencilUr","Err1","Flag","Bolt","Success","SmallDot","BigDot","Ball1","Ball2",
    "DeadHead","YinYang","Sadface","Happy1","Happy2","CircleX","Function","Cut1","Cut2","Cut3","CMarkX","Baseline","Star5","CMarkCheck",
    "SwordCross","HammerCross","SmallDot2","Err2","Err3","SQuote1","DQuoteLeft","DQuoteRight","Downtobottomleft","Undo","Enter",
    "KeyEnter","Linux","Arch","Home","File","ThumbDown","GithubCircle","GithubFolder","Windows","Pi","Folder", "Bug", "Info", "Warning"
};

Icon::Type Icon::Scan(std::string IcName)
{
    int N = 0;
    for (auto Str : IcNames)
    {
        if (IcName == Str) return N;
        ++N;
    }
    return 0;
}

}