//
// Created by oldlonecoder on 24-01-27.
//

#include "scrat/rt/TToken.h"

namespace scrat
{

std::string_view TToken::TLocation::operator()() const
{
    return {Begin,End};
}

[[maybe_unused]] std::string TToken::TLocation::Position() const
{
    StrAcc Txt = "%d,%d";

    return { (Txt << Line << Column)() };
}


} // scrat