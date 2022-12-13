#pragma once

#include <scrat/dlconfig.h>
#include <vector>
#include <string>


/*

    https://www.instructables.com/Programming--how-to-detect-and-read-UTF-8-charact/
// utf8 points to a byte of a text string
// Uni  points to a variable which will store the Unicode
// the function returns how many byte have been read

int UTF8_to_Unicode ( char * utf8, unsigned int * Uni )
{
    if ( utf8 == NULL ) return 0;
    if ( Uni  == NULL ) return 0;
    // U-00000000 - U-0000007F
    // ASCII code ?
    if (*utf8 >= 0)
    {
        *Uni= *utf8; return 1;
    }

    int len=0;
    unsigned char * u = (unsigned char *)utf8;
    *Uni = 0;

    // U-00000080 - U-000007FF : 110xxxxx 10xxxxxx
    if ( (u[0]&0xE0) == 0xC0 )
    {
        len = 2; *Uni = u[0]&0x1F;
    }
    else
    // U-00000800 - U-0000FFFF : 1110xxxx 10xxxxxx 10xxxxxx
        if ( (u[0]&0xF0) == 0xE0 )
        {
            len = 3;
            *Uni = u[0]&0x0F;
        }
        else
        // U-00010000 - U-001FFFFF : 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if ( (u[0]&0xF8) == 0xF0 )
            {
                len = 4;
                *Uni = u[0]&0x07;
            }
            else
            {
            // our UTF-8 character is malformed
            // let&aposs return it as an extended-ASCII
                *Uni = u[0];
                return 1;
            }
            // we&aposre going to read the following bytes
        int a;
        for ( a=1; a<len; a++ )
        {
            if ( ( u[a] >=0 ) || ( (u[a]&0xC0) != 0x80 ) )
            {
            // our UTF-8 code is malformed ...
            // let&aposs return it as an extended-ASCII
            *Uni = u[0];
            return 1;
        }
        // so far, everything seems Ok.
        // we safely build our Unicode
        *Uni = (*Uni<<6) | (u[a]&0x3F);
    }
    // According to Unicode 5.0
    // codes in the range 0xD800 to 0xDFFF
    // are not allowed.
    if ( ( (*Uni) >= 0xD800 ) || ( (*Uni) <= 0xDFFF ) )
    {
        // In this case, our UTF-8 code was well formed.
        // So, or we break it into 2 extended ASCII codes,
        // or we display an other symbol instead ...
        // We should read the Unicode 5.0 book to
        // to know their official recommendations though ...
        *Uni = &apos?&apos;
        return 1;
    }
    // it&aposs done !
    // *Uni contains our unicode.
    // we simply return how many bytes
    // it was stored in.
    return len;
}

Good explanation!

I know this is old, but there are a couple of bugs in your sample code.

* u[a] >= 0 always evaluates to true if u is unsigned. You need a cast to char.
if ((char)u[a] > 0) ... )

* Checking the range U+D800 0 U+DFFF should use && not ||
if ((*Uni >= 0xD800) && (*Uni <= 0xDFFF))

* There's a superfluous semicolon at the end of the while loop's closing brace in UTF8_strlen
Also, Uni should be defined as unsigned int in the same function (to match the other function's argument.

It would be nice if you could reformat the code so that line breaks appear correctly. I suppose it got mangled by the editor...

For anyone interested, here's the fixed version (I hope you don't mind, I'll remove it if any objections).



*/
namespace scrat
{


struct SCRAT_API Accent final
{
    using T = const char*;

    enum Type : uint8_t
    {
        agrave ,
        acirc  ,
        cedile ,
        eacute ,
        egrave ,
        ecirc  ,
        etrema ,
        itrema ,
        ocirc  ,
        ugrave ,
        ucirc  ,
        icirc   ,
        Agrave  ,
        Acirc   ,
        Cedile  ,
        Eacute  ,
        Egrave  ,
        Ecirc   ,
        Etrema  ,
        Itrema  ,
        Ocirc   ,
        Ugrave  ,
        Ucirc   ,
        Icirc   ,
        Err

    };

    static constexpr T c_agrave = "à";
    static constexpr T c_acirc  = "â";
    static constexpr T c_cedile = "ç";
    static constexpr T c_eacute = "é";
    static constexpr T c_egrave = "è";
    static constexpr T c_ecirc  = "ê";
    static constexpr T c_etrema = "ë";
    static constexpr T c_itrema = "ï";
    static constexpr T c_ocirc  = "ô";
    static constexpr T c_ugrave = "ù";
    static constexpr T c_ucric  = "û";
    static constexpr T c_icirc  = "Î";

    static constexpr T c_Agrave = "À";
    static constexpr T c_Acirc  = "Â";
    static constexpr T c_Cedile = "Ç";
    static constexpr T c_Eacute = "É";
    static constexpr T c_Egrave = "È";
    static constexpr T c_Ecirc  = "Ê";
    static constexpr T c_Etrema = "Ë";
    static constexpr T c_Itrema = "Ï";
    static constexpr T c_Ocirc  = "Ô";
    static constexpr T c_Ugrave = "Ù";
    static constexpr T c_Ucric  = "Û";
    static constexpr T c_Icirc  = "Î";

    static std::vector<Accent::T> Data;
    static std::vector<std::string_view> Name;
    static Accent::Type Code(std::string_view aTypeName);
};

}
