#include <scrat/ui/console.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#   include <Windows.h>
#   include <stdlib.h>
#   include <io.h>
#   define write _write
#elif defined(__linux__)
#   include <sys/ioctl.h>
#   include <unistd.h>
#endif // Windows/Linux


namespace scrat::ui
{

    _object_name(console)


bool console::get_term_size()
{
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    WH = {
            {1,1},
            {static_cast<int>(csbi.srWindow.Right - csbi.srWindow.Left + 1), static_cast<int>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)},
            static_cast<int>(csbi.srWindow.Right - csbi.srWindow.Left + 1), static_cast<int>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
    };
#elif defined(__linux__)
    struct winsize win;
    ioctl(fileno(stdout), TIOCGWINSZ, &win);
    wh = { {1,1},{static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)}, static_cast<int>(win.ws_col), static_cast<int>(win.ws_row) };
#endif // Windows/Linux

    rem::push_debug(source_fl) < " ScreenSize: " < color::Yellow < (std::string)wh;
    return rem::ok;
}



console::console():object()
{
    _id = "null";
}

console::console(console *parent_, scrat::dim consize_, const std::string &console_id_):object(parent_)
{
    wh = consize_;
    _id = console_id_;
}

console::~console()
{
    _id.clear();
}

result<> console::init()
{
    return rem::notimplemented;
}

}

