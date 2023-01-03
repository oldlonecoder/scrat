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
    bool get_term_size();
    console();
public:

    ~console();
    //...
    static result<> init();

    static void crs_hide();
    static void crs_show();

    console& gotoxy(const point& pt_);

    console& operator<<(const std::string& aStr);
    console& operator<<(char C);
    console& operator<<(Icon::Type C);
    console& operator<<(Accent::Type C);
    console& operator<<(color::type c);

    console& render_vdc_row(vdc* mem_, point xy_, int w_);
    console& render_vdc(vdc* mem_, const rect& r_);



};

}