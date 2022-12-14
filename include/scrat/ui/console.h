#pragma once
#include <scrat/dlconfig.h>
//#include <scrat/object.h>
#include <scrat/ui/vdc.h>
#include <scrat/object>
#include <map>
#include <scrat/ui/uidefs.h>
#include <mutex>
#include <termios.h>
#include <unistd.h>


namespace scrat::ui
{
//------------------------------------------------------------------------------------------------
//        from https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html

//#include <termios.h>
//#include <unistd.h>
//void enableRawMode() {
//    struct termios raw;
//    tcgetattr(STDIN_FILENO, &raw);
//    raw.c_lflag &= ~(ECHO);
//    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
//}
//int main() {
//    enableRawMode();
//    char c;
//    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
//    return 0;
//}
//------------------------------------------------------------------------------------------------
class SCRAT_API console : object
{
    _decl_objname
    dim wh;
    std::string _id;
    vdc mem;
    static std::mutex io_mtx;
    static std::mutex updates_mtx;
    bool get_term_size();
    termios raw,con;
    console();
public:

    struct updates_queu
    {
        vdc* dc; ///< video display (cells/context)...
        //point xy; ///< vdc's coords on screen/terminal/console...
        rect r; ///< subregion to 'expose'.
        using stack = std::vector<console::updates_queu>;
    };

    ~console();
    //...
    static result<> init();

    static void crs_hide();
    static void crs_show();

    static void update(vdc* dc_, const point& cxy_, const rect& area_ = {});

    console& gotoxy(const point& pt_);

    console& operator<<(const std::string& aStr);
    console& operator<<(char C);
    console& operator<<(Icon::Type C);
    console& operator<<(Accent::Type C);
    console& operator<<(color::type c);

// ---------- testing/r&d ---------------------------------
    console& render_vdc_row(vdc* mem_, point xy_, int w_);
    console& render_vdc(vdc* mem_, const rect& r_ = {});
//---------------------------------------------------------

    static result<int> draw();

    static console& me();
    static void terminate();
    static rect geometry();
private:
    static console::updates_queu::stack updates;

    static void  draw_vdc(const console::updates_queu& q);



};

}
