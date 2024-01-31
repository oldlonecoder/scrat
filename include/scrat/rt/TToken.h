//
// Created by oldlonecoder on 24-01-27.
//

/******************************************************************************************
 *   Copyright (C) /.../2024 by Serge Lussier                                             *
 *   lussier.serge@gmail.com                                                              *
 *                                                                                        *
 *   Unless otherwise specified, all code in this project is written                      *
 *   by the author (Serge Lussier)                                                        *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here                     *
 *   Open source FREE licences also apply to the code from the same author (Serge Lussier)*
 *   ----------------------------------------------------------------------------------   *
 *****************************************************************************************/

#pragma once


//#ifndef SCRAT_TTOKEN_H
//#define SCRAT_TTOKEN_H
#include "scrat/rt/Components.scrat.h"
#include <string_view>

namespace scrat
{

struct SCRAT_API TToken
{
    using List = std::vector<TToken>;
    using Iterator  = TToken::List::iterator;
    using CIterator = TToken::List::const_iterator;
    using SVIterator  = std::string_view::iterator;

    Type::T     Prim = Type::Null;
    Type::T     Sem = Type::Null;
    Mnemonic    M = Mnemonic::Noop;
    Oper::T     D = Oper::Identifier;
    struct SCRAT_API TLocation
    {
        [[maybe_unused]] size_t Line{0};
        [[maybe_unused]] size_t Column{0};
        [[maybe_unused]] size_t Offset{0};
        [[maybe_unused]] size_t Length{0};
        SVIterator Begin{nullptr};
        SVIterator End{nullptr};
        std::string_view operator()() const;
        [[maybe_unused]] [[nodiscard]] std::string Position() const;
    }Loc;


    struct SCRAT_API TFlags
    {
        uint8_t V : 1;// This token is a usable value into expression.
        uint8_t S:  1;// Post semantic parser: Left-Assignable Object
        uint8_t M : 1;// Virtual multiply operator. ( ex.: "4ac" => 4*a*c )
                      //                                             ^<...
        uint8_t U : 1;// This token is a single utf-8 symbol { ex.:π,➪, ... }
    }Flags;

    //SppToken* back_to_startof_line();

    explicit operator bool() const
    {
        return Loc.Begin != nullptr;
    }
    bool operator||(Type::T Sem_) const
    {
        return Sem || Sem_;
    }

    [[nodiscard]] std::string_view Text() const
    {
        if ((Flags.M) && (M == Mnemonic::Mul)) return Lexem::Multiply; // Overwrite source location.
        return Loc();
    }

    [[nodiscard]] bool IsText() const
    {
        return Sem & Type::Text;
    }
    [[nodiscard]] bool IsBinary() const
    {
        return Sem & Type::Binary;
    }
    [[nodiscard]] bool IsUnary() const
    {
        return Sem & Type::Unary;
    }
    [[nodiscard]] bool IsIdentifier() const
    {
        return Sem & Type::Id;
    }
    [[nodiscard]] bool IsLeaf() const
    {
        return Sem & Type::Leaf;
    }
    [[nodiscard]] bool IsOperator() const
    {
        return Sem & Type::Operator;
    }
    [[nodiscard]] bool IsClosingPair() const
    {
        return Sem & Type::ClosePair;
    }
    [[nodiscard]] bool IsPunctuation() const
    {
        return Sem & Type::Punc;
    }
    [[nodiscard]] bool IsNumber() const
    {
        return Sem & Type::Number;
    }
    [[nodiscard]] bool IsHexadecimal() const
    {
        return Sem & Type::Hex;
    }
    [[nodiscard]] bool IsPrefix() const
    {
        return Sem & Type::Prefix;
    }
    [[nodiscard]] bool IsPostfix() const
    {
        return Sem & Type::Postfix;
    }
    [[nodiscard]] bool IsInstruction() const
    {
        return !(Sem & (Type::Operator | Type::Leaf));
    }
    [[nodiscard]] bool IsValue() const
    {
        return Flags.V;
    }
    [[nodiscard]] bool IsComment() const
    {
        return (Prim == Type::LineComment) || (Prim == Type::BlocComment);
    }
};

}



//#endif //SCRAT_TTOKEN_H
