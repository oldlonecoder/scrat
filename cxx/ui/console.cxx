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

#include <scrat/accents.fr.h>


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
    auto R = get_term_size();
    if (!R)
        throw rem::push_fatal(source_fl) < rem::rejected < " Getting screen/console dimensions...";

    std::cout << "\033[?1049h";
    console::GotoXY({});
    csr_hide();

    //... To be continued

    return rem::ok;
}

void console::crs_hide()
{
    write(1,"\033[?25l",6);
}

void console::crs_show()
{
    write(1, "\033[?25h",6);
}



/**
 * @brief set terminal cursor position.
 *
 * @param pt_
 * @return console&
 * @note As of this version, std::cout is the simplest and fastest I have found.
 */
console&  console::gotoxy(const point &pt_)
{
    std::cout << "\033[" << pt_.y << ';' << pt_.x << "H";
    std::flush(std::cout);

    return *this;
}


console& console::operator<<(const std::string& aStr)
{
    write(1, aStr.c_str(), aStr.length());
    return *this;
}


console& console::operator<<(char C)
{
    write(1, &C, 1);
    return *this;
}

console& console::operator<<(Icon::Type C)
{
    std::string str = Icon::Data[C];
    write(1, str.c_str(), str.length());
    return *this;
}

console& console::operator<<(Accent::Type A)
{
    std::string str = Accent::Data[A];
    write(1, str.c_str(), str.length());
    return *this;
}


/**
 * @brief Output to the terminal the contents of the vdc row given by xy_.y, starting at column xy_.x .
 *
 * @param mem_  pointer to the vdc.
 * @param xy_   xy coordinates into the vdm mem bloc. ( xy_.y is the row # to render on the terminal)
 * @param w_    length of the row relative to xy_x. If 0, then from column xy_.x to the end of the vdc row ( w_: { w_ >= 0 || w_<= vdc::width() - xy_.x} )
 * @return console& (ref to self )
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
console &console::render_vdc_row(vdc *mem_, point xy_, int w_=0)
{

    return *this;
}
}
