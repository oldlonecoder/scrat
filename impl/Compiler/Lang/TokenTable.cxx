//
// Created by oldlonecoder on 24-01-27.
//

#include "scrat/Compiler/Lang/TokenTable.h"

namespace scrat::Lang
{



TToken TTokenTable::Scan(const char *C)
{
    if(Ref.empty()) return {};
    
    int      unicode = 0;
    if (!*C)
        return {};

    for (auto Token : Ref)
    {
        const char* crs = C;
        const char* rtxt = Token.Loc.Begin;
        unicode = 0; // oops...
        //std::size_t sz = std::strlen(rtxt);

        if(*crs != *Token.Loc.Begin) continue;

        while ((*crs && *rtxt) && (*crs == *rtxt))
        {
            if (*crs < 0)
                ++unicode; ///< @note oui/ yes; je supporte quelques symboles UTF-8 (pi, xor, deref...)
            ++crs;
            ++rtxt;
        }
        if (*rtxt == 0)
        {
            if (*crs && !isspace(*crs))
            {
                if ((isalnum(*crs) || (*crs == '_')) && !Token.IsOperator())
                    continue;
            }

            Token.Loc.Begin = C;
            Token.Loc.End = crs - 1;
            Token.Loc.Length = (Token.Loc.End - Token.Loc.Begin) + 1;
            return Token;
        }
    }
    return {};
}




TTokenTable::TTokenTable(Util::Object *Par, const std::string &TableName) : Util::Object(Par, TableName)
{

}

TTokenTable::~TTokenTable()
{
    Ref.clear();
}


}
