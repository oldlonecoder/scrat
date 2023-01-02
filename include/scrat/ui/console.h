#pragma once
#include <scrat/dlconfig.h>
//#include <scrat/object.h>
#include <scrat/ui/vdc.h>
#include <scrat/object>
#include <map>
#include <scrat/ui/uidefs.h>



namespace scrat::ui
{
class SCRAT_API console : object
{
    _decl_objname
    dim wh;
    std::string _id;

    vdc mem;

    bool get_term_size();
public:

    using list = std::map<std::string, console*>;


    console();
    console(console* parent_, scrat::dim consize_, const std::string& console_id_);
    ~console();
    //...
    result<> init();

    static void crs_hide();
    static void crs_show();

    console& gotoxy(const point& pt_);

    console& operator<<(const std::string& aStr);
    console& operator<<(char C);
    console& operator<<(Icon::Type C);
    console& operator<<(Accent::Type C);

    console& render_vdc_row(vdc* mem_, point xy_, int w_);



};

}