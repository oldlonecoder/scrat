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
                    {State::Active, {color::Blue1,color::Grey23}},
                    //...
                }
            },
            {
                "label",
                {
                    {State::Active, {color::Blue1, color::White}},
                    {State::Disable,{color::Grey19, color::White}},
                    //...
                }
            }
        }
    }
};

}
