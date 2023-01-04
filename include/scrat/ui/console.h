#pragma once
#include <scrat/dlconfig.h>
//#include <scrat/object.h>
#include <scrat/ui/vdc.h>
#include <scrat/object>
#include <map>
#include <scrat/ui/uidefs.h>
#include <mutex>



namespace scrat::ui
{
class SCRAT_API console : object
{
    _decl_objname
    dim wh;
    std::string _id;
    vdc mem;
    static std::mutex io_mtx;
    static std::mutex updates_mtx;
    bool get_term_size();
    console();
public:

    struct updates_queu
    {
        vdc* dc;
        rect r;
        using stack = std::stack<console::updates_queu>;
    };

    ~console();
    //...
    static result<> init();

    static void crs_hide();
    static void crs_show();

    static void update(vdc* dc_, const rect& area_ = {});

    console& gotoxy(const point& pt_);

    console& operator<<(const std::string& aStr);
    console& operator<<(char C);
    console& operator<<(Icon::Type C);
    console& operator<<(Accent::Type C);
    console& operator<<(color::type c);

    console& render_vdc_row(vdc* mem_, point xy_, int w_);
    console& render_vdc(vdc* mem_, const rect& r_ = {});
    static console& me();
    static void terminate();
private:
    static console::updates_queu::stack updates;


};

}
