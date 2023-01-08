#include <scrat/ui/uidefs.h>



namespace scrat::ui::colors
{

db::themes db::data =
{
    {
        "default",
        {
            {
                "widget",
                {
                    {State::Active, {color::Grey70,color::NavyBlue}},
                    //...
                }
            },
            {
                "label",
                {
                    {State::Active, {color::CadetBlue2, color::Grey39}},
                    {State::Disable,{color::Grey19, color::Grey30}},
                    {State::Normal,{color::Grey70,color::Grey30}},
                    //...
                }
            },
            {
                "icon",
                {
                    {State::Active, {color::Blue1, color::Grey30}},
                    {State::Disable,{color::Grey19, color::Grey30}},
                    {State::Normal,{color::Blue1, color::Grey30}},
                    //...
                }
            }
        }
    }
};

}
