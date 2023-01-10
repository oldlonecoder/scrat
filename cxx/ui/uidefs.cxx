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
                    {State::Active, {color::Grey70,color::Grey35}},
                    //...
                }
            },
            {
                "label",
                {
                    {State::Active, {color::CadetBlue2, color::Grey39}},// Active label base coherance with composite/complex ui
                    {State::Disable,{color::Grey19, color::Grey35}},    // Keep coherance with widget base
                    {State::Normal,{color::Grey70,color::Grey35}},      // Keep coherance with widget base
                    //...
                }
            },
            {
                "shortkey",
                {
                    {State::Active, {color::HotPink4,color::Grey35}},
                    {State::Disable,{color::Grey19, color::Grey35}},    // Keep coherance with widget base
                    {State::Focus,{color::Red4, color::Grey35}}
                }
            },
            {
                "icon",
                {
                    {State::Active, {color::Blue1, color::Grey35}},    // Keep coherance with widget base
                    {State::Disable,{color::Grey19, color::Grey35}},
                    {State::Normal,{color::Blue1, color::Grey35}},
                    {State::Focus,{color::Red4, color::Grey35}}

                    //...
                }
            },
            {
                "input-area",
                {
                    {State::Active, {color::Grey70, color::Grey30}},    // Keep coherance with widget base
                    {State::Disable,{color::Grey39, color::Grey35}},
                    {State::Normal,{color::Grey70, color::Grey30}},
                    {State::Focus,{color::White, color::Grey30}}
                }
            },
            {
                "text-input",
                    {State::Active, {color::Grey70, color::Grey30}},    // Keep coherance with widget base
                    {State::Disable,{color::Grey39, color::Grey35}},
                    {State::Normal,{color::Grey70, color::Grey30}},
                    {State::Focus,{color::White, color::Grey30}}
            }
        }
    }
};

}
