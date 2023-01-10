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

    std::cout << "\033[?1049h";
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

    std::cout << "\033[" << pt_.y + 1 << ';' << pt_.x + 1 << 'H';
    fflush(stdout);

    return *this;
}


console& console::operator<<(const std::string& aStr)
{
    write(STDOUT_FILENO,  aStr.c_str(), aStr.length());
    return *this;
}


console& console::operator<<(char C)
{
    write(STDOUT_FILENO, &C, 1);
    return *this;
}

console& console::operator<<(Icon::Type C)
{
    std::string str = Icon::Data[C];
    write(STDOUT_FILENO, str.c_str(), str.length());
    return *this;
}

console& console::operator<<(Accent::Type A)
{
    std::string str = Accent::Data[A];
    write(STDOUT_FILENO, str.c_str(), str.length());
    return *this;
}

console& console::operator<<(color::type c)
{
    auto s = scrat::attr<textattr::format::ansi256>::bg(c);
    write(STDOUT_FILENO,s.c_str(),s.length());
    fflush(stdout);
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
    rect cr = rect({}, wh);
    rect ar = r_ ? r_ : mem_->geometry();
    ar += mem_->location();
    rect r = cr & ar;
    if(!r)
    {
        rem::push_error() < rem::rejected < " rect is out of bounds in the terminal: " < r;
        return *this;
    }
    rem::push_debug(source_fnl) < " vdc geometry:" < mem_->geometry() < " :";
    rem::push_output() < " console geometry:" < cr;
    rem::push_output() < " vdc's exposed geometry:" < ar;
    rem::push_output() < "intersection geometry: " < r;

    using ansi = scrat::attr<textattr::format::ansi256>;
    for(int y = 0; y < r.height(); y++)
    {
        vdc::type p = mem_->peek({r.a.x, r.a.y+y});
        auto w_ = r.width();
        vdc::cell cell=p;
        vdc::cell prev_cell=p;
        terminal->gotoxy({r.a.x,r.a.y+y});
        (*terminal) << ansi::bg(cell.bg()) << ansi::fg(cell.fg());
        point pt = r.a;
        pt += {0,y};
        rem::push_output() < " vdc line#" < y < " " < pt;

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
        write(1,"\033[0m",4);
    }
    return *this;
}


/*!
    @brief no checks yet...
 */
void console::update(vdc* dc_, const point& cxy_, const rect& area_)
{
    // not exposition check/compute yet. (too much load if rejected, so it shall be rejected during the exposure loop /(thread?))

    console::updates_mtx.lock(); // blocs until unlocked....?
    console::updates.push_back({dc_,area_});
    console::updates_mtx.unlock();

    // --- Just call console::draw(...) as of current dev status does not use threads yet :
    (void) console::draw(); // Kind of screen::refresh
}

void console::draw_vdc(const console::updates_queu& q)
{
    using ansi = attr<textattr::format::ansi256>;
    rem::push_debug(source_fnl) < " vdc geometry: " < q.r ;

    for(int y = 0; y < q.r.height(); y++)
    {
        auto vr = q.r;
        vr -= q.r.a;
        vdc::type p = q.dc->peek({vr.a.x, vr.a.y + y});
        vdc::cell cell=p;
        vdc::cell prev_cell=p;
        terminal->gotoxy({q.r.a.x, q.r.a.y + y});
        (*terminal) << ansi::bg(cell.bg()) << ansi::fg(cell.fg());
        point pt = q.r.a;
        pt += {0,y};
        //rem::push_output(source_ffl) < " vdc line#" < y < " " < pt < '(' < cell.details() < ')';
        auto w_ = q.r.width();
        for(int x = 0; x< w_; x++)
        {
            if (prev_cell.bg() != cell.bg()) (*terminal) << ansi::bg(cell.bg());
            if (prev_cell.fg() != cell.fg()) (*terminal) << ansi::fg(cell.fg());
            if(cell.mem & vdc::cell::UTFMASK)
            {
                if (cell.mem & vdc::cell::UGlyph)
                {
                    auto Ic =  cell.icon_id();
                    write(STDOUT_FILENO, Icon::Data[Ic], std::strlen(Icon::Data[Ic]));
                    terminal->gotoxy({q.r.a.x+x+1, q.r.a.y + y});
                    rem::push_debug(source_pffl) < color::White < "@ " < point{q.r.a.x+x, q.r.a.y + y} < color::White < " Glyph: " < cell.details();
                }
                if (cell.mem & vdc::cell::Accent)
                {
                    //w_ -= 1;
                    auto Ic =  cell.accent_id();
                    write(STDOUT_FILENO, Accent::Data[Ic], std::strlen(Accent::Data[Ic]));
                }
            }
            else
                write(STDOUT_FILENO,&cell.mem,1);
            prev_cell = p++;
            cell = p;
        }
        write(STDOUT_FILENO,"\033[0m",4);
        fflush(stdout);
    }
}




result<int> console::draw()
{
    console::updates_mtx.lock(); // blocs until unlocked....?
    for( auto upd : console::updates)
    {
        draw_vdc(upd);
    }
    console::updates_mtx.unlock();
    return 0;
}


console &console::me()
{
    if(!terminal)
        throw rem::push_exception(source_fnl) < " nullptr - using console that is not initialized.";
    return *terminal;
}

void console::terminate()
{
    std::cout << "\033[0m\033[?1049l";
    console::updates.clear();
    crs_show();
}

rect console::geometry()
{
    return {{0,0},terminal->wh};
}

}