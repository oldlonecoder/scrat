//
// Created by oldlonecoder on 24-01-27.
//

#include "Tests.h"
#include <AppBook/Book/AppBook.h>
#include <AppBook/Book/StmlText.h>

#include <csignal>
#include "scrat/Compiler/TLexer.h"


namespace test
{


size_t MyConfig::DeclareTable()
{
    return TTokenTable::DeclareTable();
}

MyConfig::MyConfig(Util::Object *ParenObj, const std::string &ID) : TTokenTable(ParenObj, ID)
{
    //TTokenTable::DeclareTable();
}


} // test








void SigInt(int s )
{
    //rem::push_interrupted() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sig break caught...\n";
    AppBook::Close();
    exit(3);
}

void SigFault(int s)
{
    //rem::push_segfault() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << " sigfault caught...\n";
    AppBook::Close();
    exit(127);
}

void SigAbort(int s)
{
    //rem::push_aborted() << " Dump messages stream and exit:";
    //rem::clear(nullptr);
    std::cerr << "abort signal caught...\n" << '\n';
    AppBook::Close();
    exit(127);
}

// --------------------------------------------------------------------------





auto main(int argc, char** argv) -> int
{

    ::signal(SIGINT, SigInt);
    ::signal(SIGSEGV, SigFault);
    ::signal(SIGABRT, SigAbort);

    try
    {
        AppBook& Livre = AppBook::Init("ScratTests");
        AppBook::Open();
        Livre.Descriptions =
R"(
<Icon:School; fg:Yellow>This is the development of the <fg:White>[scrat API & its explorations]<fg/>
------------------------------------------------------------------------
)";

        std::string head;
        Book::STMLText ml_description;
        auto c = ml_description << Livre.Descriptions >> head;
        if(c != Book::Enums::Code::Success)
            std::cerr << " text processing failed.";

        auto & SppDev= AppBook::CreateSection("scrat.dev");
        SppDev.Open().CreateSectionContents("TextCursor.tests");

        Livre["scrat.dev"]["TextCursor.tests"];
        AppBook::Out() << Book::Enums::Fn::Endl << head;

        test::MyConfig Conf(nullptr,"My Config");
        scrat::TLexer Lexer;
        Lexer.Config() =
        {
            .Source     = R"(
                                145.0333
)",
            .TokenTable = &Conf,
        };

        auto R = Lexer();
        AppBook::Debug() << R;

    }
    catch(AppBook::Exception& be)
    {
        std::cerr << " Caught Book::exception: " << be.what() << '\n';
        AppBook::Close();
        return 127;
    }
        /// xio and interpreter compiling stuff:
    catch(AppBook::Section::Contents::Element& bel)
    {
        std::cerr << " Caught event exception held in Book element contents file. Closing the Book now";
    }

    AppBook::Close();

    return 0;
}

