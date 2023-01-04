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
static console* terminal=nullptr;

std::mutex console::io_mtx;
std::mutex console::updates_mtx;
console::updates_queu::stack console::updates;

console::console()
{
    if(terminal)
        throw rem::push_fatal() < rem::exist < " static console instance already exists.";

    terminal = this;
}

bool console::get_term_size()
{

    if(!terminal)
        return false;
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
    terminal->wh = { {1,1},{static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)}, static_cast<int>(win.ws_col), static_cast<int>(win.ws_row) };
#endif // Windows/Linux

    rem::push_debug(source_fl) < " ScreenSize: " < color::Yellow < (std::string)terminal->wh;
    return true;
}




console::~console()
{

}

result<> console::init()
{

    terminal = new console;
    auto R = terminal->get_term_size();
    if (!R)
        throw rem::push_fatal(source_fl) < rem::rejected < " Getting screen/console dimensions...";

    //std::cout << "\033[?1049h";
    terminal->gotoxy({});
    console::crs_hide();

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
    if(!rect({},wh).in(pt_))
    {
        rem::push_error() < rem::rejected < " coord '" < pt_ < "' is out of console's geometry.";
        return *this;
    }
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

console& console::operator<<(color::type c)
{
    auto s = scrat::attr<textattr::format::ansi256>::bg(c);
    write(1,s.c_str(),s.length());
    std::flush(std::cout);
    return *this;
}




/**
 * @brief Output to the terminal the contents of the vdc row given by xy_.y, starting at column xy_.x .
 *
 * @param mem_  pointer to the vdc.
 * @param xy_   xy (vdc's coords) coordinates into the vdc mem bloc. ( xy_.y is the row # to render on the terminal) Thus, xy_ must be vdc's inner computed offset.
 * @param w_    length of the row relative to xy_x. If 0, then from column xy_.x to the end of the vdc row ( w_: { w_ >= 0 || w_<= vdc::width() - xy_.x} )
 * @return console& (ref to self )
 * @note To be called only when one line of a vdc bloc is needed to be rendered on the terminal.
 * @author &copy; 2022, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
console &console::render_vdc_row(vdc *mem_, point xy_, int w_)
{
    vdc::type p = mem_->peek(xy_);
    w_ = w_ == 0 ? mem_->width() - xy_.x : w_;
    w_ = w_ > mem_->width() - xy_.x ? mem_->width() - xy_.x : w_;
    vdc::cell cell=p;
    vdc::cell prev_cell=p;
    using ansi = scrat::attr<textattr::format::ansi256>;
    for(int x = 0; x< w_; x++)
    {

        if (prev_cell.bg() != cell.bg()) (*terminal) << ansi::bg(cell.bg());
        if (prev_cell.fg() != cell.fg()) (*terminal) << ansi::fg(cell.fg());
        if (cell.mem & vdc::cell::UGlyph)
        {
            auto Ic =  cell.icon_id();
            write(1, Icon::Data[Ic], std::strlen(Icon::Data[Ic]));
        }
        else
            write(1,&cell.mem,1);
        prev_cell = p++;
        cell = p;
    }
    return *this;
}



console &console::render_vdc(vdc *mem_, const rect &r_)
{
    rect me = rect({}, wh);
    rect r = me & r_;
    if(!r)
    {
        rem::push_error() < rem::rejected < " rect is out of bounds in the terminal: " < r;
        return *this;
    }
    using ansi = scrat::attr<textattr::format::ansi256>;
    for(int y = 0; y < r.height(); y++)
    {
        vdc::type p = mem_->peek({r.a.x, r.a.y+y});
        auto w_ = r.width();
        vdc::cell cell=p;
        vdc::cell prev_cell=p;
        for(int x = 0; x< w_; x++)
        {
            if (prev_cell.bg() != cell.bg()) (*terminal) << ansi::bg(cell.bg());
            if (prev_cell.fg() != cell.fg()) (*terminal) << ansi::fg(cell.fg());
            if (cell.mem & vdc::cell::UGlyph)
            {
                auto Ic =  cell.icon_id();
                write(1, Icon::Data[Ic], std::strlen(Icon::Data[Ic]));
            }
            else
                write(1,&cell.mem,1);
            prev_cell = p++;
            cell = p;
        }
    }
    return *this;
}


/*!
    @brief no checks yet...
 */
void console::update(vdc* dc_, const rect& area_)
{
    console::updates_mtx.lock(); // blocs until unlocked....?
    console::updates.push({dc_,area_});
    console::updates_mtx.unlock();

}


}

scrat::ui::console & scrat::ui::console::me()
{

    return *terminal;
}
void scrat::ui::console::terminate()
{
    //std::cout << "\033[0m\033[?1049l";
    crs_show();
}
