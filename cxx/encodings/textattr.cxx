#include <scrat/textattr>


namespace scrat
{


    _object_name(color)

    _object_name(textattr)

    static  textattr::format color_format = textattr::format::ansi256;
    // ----------------------color -----------------------
    //#pragma region color_db


    // ----------- Color base code: -------------------------------------

    // color_data
    SCRAT_API  std::vector<color_data> ColorDB = {
        {color::Black,            "#000"/*#000000	*/, 0x00, 0x00, 0x00},//#000000	rgb(128,0,0)	hsl(0,100%,25%)
        {color::Maroon,           "#800000"/*#800000	*/, 0x80, 0x00, 0x00},//#800000	rgb(128,0,0)	hsl(0,100%,25%)
        {color::Green,            "Green"/*#008000	*/, 0x00, 0x80, 0x00},
        {color::Olive,            "Olive"/*#808000	*/, 0x80, 0x80, 0x00},//#808000	rgb(128,128,0)	hsl(60,100%,25%)
        {color::Navy,             "Navy"/*#000080	*/, 0x00, 0x00, 0x80},//#000080	rgb(0,0,128)	hsl(240,100%,25%)
        {color::Purple,           "Purple"/*#800080	*/, 0x80, 0x00, 0x80},//#800080	rgb(128,0,128)	hsl(300,100%,25%)
        {color::Teal,             "Teal"/*#008080	*/, 0x00, 0x80, 0x80},//#008080	rgb(0,128,128)	hsl(180,100%,25%)
        {color::Silver,           "Silver"/*#c0c0c0	*/, 0xc0, 0xc0, 0xc0},//#c0c0c0	rgb(192,192,192)	hsl(0,0%,75%)
        {color::Grey,             "Grey"/*#808080	*/, 0x80, 0x80, 0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
        {color::Red,              "Red"/*#ff0000	rgb(*/, 0xff, 0x00, 0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
        {color::Lime,             "Lime"/*#00ff00	*/, 0x00, 0xff, 0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
        {color::Yellow,           "Yellow"/*#ffff00	*/, 0xff, 0xff, 0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
        {color::Blue,             "Blue"/*#0000ff	*/, 0x00, 0x00, 0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
        {color::Fuchsia,          "Fuchsia"/*#ff00ff	*/, 0xff, 0x00, 0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
        {color::Aqua,             "Aqua"/*#00ffff	*/, 0x00, 0xff, 0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
        {color::White,            "White"/*#ffffff	*/, 0xff, 0xff, 0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
        {color::Grey0,            "Grey0"/*#000000	*/, 0x00, 0x00, 0x00},//#000000	rgb(0,0,0)	hsl(0,0%,0%)
        {color::NavyBlue,         "NavyBlue"/*#00005f*/, 0x00, 0x00, 0x5f},//#00005f	rgb(0,0,95)	hsl(240,100%,18%)
        {color::DarkBlue,         "DarkBlue"/*#000087*/, 0x00, 0x00, 0x87},//#000087	rgb(0,0,135)	hsl(240,100%,26%)
        {color::Blue3,            "Blue3"/*#0000af	*/, 0x00, 0x00, 0xaf},//#0000af	rgb(0,0,175)	hsl(240,100%,34%)
        {color::Blue4,            "Blue4"/*#0000d7	*/, 0x00, 0x00, 0xd7},//#0000d7	rgb(0,0,215)	hsl(240,100%,42%)
        {color::Blue1,            "Blue1"/*#0000ff	*/, 0x00, 0x00, 0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
        {color::DarkGreen,        "DarkGreen"/*#005f0*/, 0x00, 0x5f, 0x00},//#005f00	rgb(0,95,0)	hsl(120,100%,18%)
        {color::DeepSkyBlue4,     "DeepSkyBlue4"/*#00*/, 0x00, 0x5f, 0x5f},//#005f5f	rgb(0,95,95)	hsl(180,100%,18%)
        {color::DeepSkyBlue5,     "DeepSkyBlue5"/*#00*/, 0x00, 0x5f, 0x87},//#005f87	rgb(0,95,135)	hsl(97,100%,26%)
        {color::DeepSkyBlue6,     "DeepSkyBlue6"/*#00*/, 0x00, 0x5f, 0xaf},//#005faf	rgb(0,95,175)	hsl(07,100%,34%)
        {color::DodgerBlue3,      "DodgerBlue3"/*#005*/, 0x00, 0x5f, 0xd7},//#005fd7	rgb(0,95,215)	hsl(13,100%,42%)
        {color::DodgerBlue2,      "DodgerBlue2"/*#005*/, 0x00, 0x5f, 0xff},//#005fff	rgb(0,95,255)	hsl(17,100%,50%)
        {color::Green4,           "Green4"/*#008700	*/, 0x00, 0x87, 0x00},//#008700	rgb(0,135,0)	hsl(120,100%,26%)
        {color::SpringGreen4,     "SpringGreen4"/*#00*/, 0x00, 0x87, 0x5f},//#00875f	rgb(0,135,95)	hsl(62,100%,26%)
        {color::Turquoise4,       "Turquoise4"/*#0087*/, 0x00, 0x87, 0x87},//#008787	rgb(0,135,135)	hsl(180,100%,26%)
        {color::DeepSkyBlue3,     "DeepSkyBlue3"/*#00*/, 0x00, 0x87, 0xaf},//#0087af	rgb(0,135,175)	hsl(93,100%,34%)
        {color::DeepSkyBlue7,     "DeepSkyBlue7"/*#00*/, 0x00, 0x87, 0xd7},//#0087d7	rgb(0,135,215)	hsl(02,100%,42%)
        {color::DodgerBlue1,      "DodgerBlue1"/*#008*/, 0x00, 0x87, 0xff},//#0087ff	rgb(0,135,255)	hsl(08,100%,50%)
        {color::Green3,           "Green3"/*#00af00	*/, 0x00, 0xaf, 0x00},//#00af00	rgb(0,175,0)	hsl(120,100%,34%)
        {color::SpringGreen3,     "SpringGreen3"/*#00*/, 0x00, 0xaf, 0x5f},//#00af5f	rgb(0,175,95)	hsl(52,100%,34%)
        {color::DarkCyan,         "DarkCyan"/*#00af87*/, 0x00, 0xaf, 0x87},//#00af87	rgb(0,175,135)	hsl(66,100%,34%)
        {color::LightSeaGreen,    "LightSeaGreen"/*#0*/, 0x00, 0xaf, 0xaf},//#00afaf	rgb(0,175,175)	hsl(180,100%,34%)
        {color::DeepSkyBlue2,     "DeepSkyBlue2"/*#00*/, 0x00, 0xaf, 0xd7},//#00afd7	rgb(0,175,215)	hsl(91,100%,42%)
        {color::DeepSkyBlue1,     "DeepSkyBlue1"/*#00*/, 0x00, 0xaf, 0xff},//#00afff	rgb(0,175,255)	hsl(98,100%,50%)
        {color::Green5,           "Green5"/*#00d700	*/, 0x00, 0xd7, 0x00},//#00d700	rgb(0,215,0)	hsl(120,100%,42%)
        {color::SpringGreen5,     "SpringGreen5"/*#00*/, 0x00, 0xd7, 0x5f},//#00d75f	rgb(0,215,95)	hsl(46,100%,42%)
        {color::SpringGreen2,     "SpringGreen2"/*#00*/, 0x00, 0xd7, 0x87},//#00d787	rgb(0,215,135)	hsl(57,100%,42%)
        {color::Cyan3,            "Cyan3"/*#00d7af	*/, 0x00, 0xd7, 0xaf},//#00d7af	rgb(0,215,175)	hsl(68,100%,42%)
        {color::DarkTurquoise,    "DarkTurquoise"/*#0*/, 0x00, 0xd7, 0xd7},//#00d7d7	rgb(0,215,215)	hsl(180,100%,42%)
        {color::Turquoise2,       "Turquoise2"/*#00d7*/, 0x00, 0xd7, 0xff},//#00d7ff	rgb(0,215,255)	hsl(89,100%,50%)
        {color::Green1,           "Green1"/*#00ff00	*/, 0x00, 0xff, 0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
        {color::SpringGreen6,     "SpringGreen6"/*#00*/, 0x00, 0xff, 0x5f},//#00ff5f	rgb(0,255,95)	hsl(42,100%,50%)
        {color::SpringGreen1,     "SpringGreen1"/*#00*/, 0x00, 0xff, 0x87},//#00ff87	rgb(0,255,135)	hsl(51,100%,50%)
        {color::MediumSpringGreen,"MediumSpringGreen"/* */, 0x00, 0xff, 0xff},//#00ffaf	rgb(0,255,175)	hsl(61,100%,50%)
        {color::Cyan2,            "Cyan2"/*#00ffd7	*/, 0x00, 0xff, 0xd7},//#00ffd7	rgb(0,255,215)	hsl(70,100%,50%)
        {color::Cyan1,            "Cyan1"/*#00ffff	*/, 0x00, 0xff, 0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
        {color::DarkRed,          "DarkRed"/*#5f0000	*/, 0x5f, 0x00, 0x00},//#5f0000	rgb(95,0,0)	hsl(0,100%,18%)
        {color::DeepPink4,        "DeepPink4"/*#5f005*/, 0x5f, 0x00, 0x5f},//#5f005f	rgb(95,0,95)	hsl(300,100%,18%)
        {color::Purple4,          "Purple4"/*#5f0087	*/, 0x5f, 0x00, 0x87},//#5f0087	rgb(95,0,135)	hsl(82,100%,26%)
        {color::Purple5,          "Purple5"/*#5f00af	*/, 0x5f, 0x00, 0xaf},//#5f00af	rgb(95,0,175)	hsl(72,100%,34%)
        {color::Purple3,          "Purple3"/*#5f00d7	*/, 0x5f, 0x00, 0xd7},//#5f00d7	rgb(95,0,215)	hsl(66,100%,42%)
        {color::BlueViolet,       "BlueViolet"/*#5f00*/, 0x5f, 0x00, 0xff},//#5f00ff	rgb(95,0,255)	hsl(62,100%,50%)
        {color::Orange4,          "Orange4"/*#5f5f00	*/, 0x5f, 0x5f, 0x00},//#5f5f00	rgb(95,95,0)	hsl(60,100%,18%)
        {color::Grey37,           "Grey37"/*#5f5f5f	*/, 0x5f, 0x5f, 0x5f},//#5f5f5f	rgb(95,95,95)	hsl(0,0%,37%)
        {color::MediumPurple4,    "MediumPurple4"/*#5*/, 0x5f, 0x5f, 0x87},//#5f5f87	rgb(95,95,135)	hsl(240,17%,45%)
        {color::SlateBlue3,       "SlateBlue3"/*#5f5f*/, 0x5f, 0x5f, 0xaf},//#5f5faf	rgb(95,95,175)	hsl(240,33%,52%)
        {color::SlateBlue4,       "SlateBlue4"/*#5f5f*/, 0x5f, 0x5f, 0xd7},//#5f5fd7	rgb(95,95,215)	hsl(240,60%,60%)
        {color::RoyalBlue1,       "RoyalBlue1"/*#5f5f*/, 0x5f, 0x5f, 0xff},//#5f5fff	rgb(95,95,255)	hsl(240,100%,68%)
        {color::Chartreuse4,      "Chartreuse4"/*#5f8*/, 0x5f, 0x87, 0x00},//#5f8700	rgb(95,135,0)	hsl(7,100%,26%)
        {color::DarkSeaGreen4,    "DarkSeaGreen4"/*#5*/, 0x5f, 0x87, 0x5f},//#5f875f	rgb(95,135,95)	hsl(120,17%,45%)
        {color::PaleTurquoise4,   "PaleTurquoise4"/*#*/, 0x5f, 0x87, 0x87},//#5f8787	rgb(95,135,135)	hsl(180,17%,45%)
        {color::SteelBlue,        "SteelBlue"/*#5f87a*/, 0x5f, 0x87, 0xaf},//#5f87af	rgb(95,135,175)	hsl(210,33%,52%)
        {color::SteelBlue3,       "SteelBlue3"/*#5f87*/, 0x5f, 0x87, 0xd7},//#5f87d7	rgb(95,135,215)	hsl(220,60%,60%)
        {color::CornflowerBlue,   "CornflowerBlue"/*#*/, 0x5f, 0x87, 0xff},//#5f87ff	rgb(95,135,255)	hsl(225,100%,68%)
        {color::Chartreuse3,      "Chartreuse3"/*#5fa*/, 0x5f, 0xaf, 0x00},//#5faf00	rgb(95,175,0)	hsl(7,100%,34%)
        {color::DarkSeaGreen5,    "DarkSeaGreen5"/*#5*/, 0x5f, 0xaf, 0x5f},//#5faf5f	rgb(95,175,95)	hsl(120,33%,52%)
        {color::CadetBlue,        "CadetBlue"/*#5faf8*/, 0x5f, 0xaf, 0x87},//#5faf87	rgb(95,175,135)	hsl(150,33%,52%)
        {color::CadetBlue2,       "CadetBlue2"/*#5faf*/, 0x5f, 0xaf, 0xaf},//#5fafaf	rgb(95,175,175)	hsl(180,33%,52%)
        {color::SkyBlue3,         "SkyBlue3"/*#5fafd7*/, 0x5f, 0xaf, 0xd7},//#5fafd7	rgb(95,175,215)	hsl(200,60%,60%)
        {color::SteelBlue1,       "SteelBlue1"/*#5faf*/, 0x5f, 0xaf, 0xff},//#5fafff	rgb(95,175,255)	hsl(210,100%,68%)
        {color::Chartreuse5,      "Chartreuse5"/*#5fd*/, 0x5f, 0xd7, 0x00},//#5fd700	rgb(95,215,0)	hsl(3,100%,42%)
        {color::PaleGreen3,       "PaleGreen3"/*#5fd7*/, 0x5f, 0xd7, 0x5f},//#5fd75f	rgb(95,215,95)	hsl(120,60%,60%)
        {color::SeaGreen3,        "SeaGreen3"/*#5fd78*/, 0x5f, 0xd7, 0x87},//#5fd787	rgb(95,215,135)	hsl(140,60%,60%)
        {color::Aquamarine3,      "Aquamarine3"/*#5fd*/, 0x5f, 0xd7, 0xaf},//#5fd7af	rgb(95,215,175)	hsl(160,60%,60%)
        {color::MediumTurquoise,  "MediumTurquoise"/**/, 0x5f, 0xd7, 0xd7},//#5fd7d7	rgb(95,215,215)	hsl(180,60%,60%)
        {color::SteelBlue4,       "SteelBlue4"/*#5fd7*/, 0x5f, 0xd7, 0xff},//#5fd7ff	rgb(95,215,255)	hsl(195,100%,68%)
        {color::Chartreuse2,      "Chartreuse2"/*#5ff*/, 0x5f, 0xff, 0x00},//#5fff00	rgb(95,255,0)	hsl(7,100%,50%)
        {color::SeaGreen2,        "SeaGreen2"/*#5fff5*/, 0x5f, 0xff, 0x5f},//#5fff5f	rgb(95,255,95)	hsl(120,100%,68%)
        {color::SeaGreen1,        "SeaGreen1"/*#5fff8*/, 0x5f, 0xff, 0x87},//#5fff87	rgb(95,255,135)	hsl(135,100%,68%)
        {color::SeaGreen4,        "SeaGreen4"/*#5fffa*/, 0x5f, 0xff, 0xaf},//#5fffaf	rgb(95,255,175)	hsl(150,100%,68%)
        {color::Aquamarine1,      "Aquamarine1"/*#5ff*/, 0x5f, 0xff, 0xd7},//#5fffd7	rgb(95,255,215)	hsl(165,100%,68%)
        {color::DarkSlateGray2,   "DarkSlateGray2"/*#*/, 0x5f, 0xff, 0xff},//#5fffff	rgb(95,255,255)	hsl(180,100%,68%)
        {color::DarkRed2,         "DarkRed2"/*#870000*/, 0x87, 0x00, 0x00},//#870000	rgb(135,0,0)	hsl(0,100%,26%)
        {color::DeepPink5,        "DeepPink5"/*#87005*/, 0x87, 0x00, 0x5f},//#87005f	rgb(135,0,95)	hsl(17,100%,26%)
        {color::DarkMagenta,      "DarkMagenta"/*#870*/, 0x87, 0x00, 0x87},//#870087	rgb(135,0,135)	hsl(300,100%,26%)
        {color::DarkMagenta2,     "DarkMagenta2"/*#87*/, 0x87, 0x00, 0xaf},//#8700af	rgb(135,0,175)	hsl(86,100%,34%)
        {color::DarkViolet,       "DarkViolet"/*#8700*/, 0x87, 0x00, 0xd7},//#8700d7	rgb(135,0,215)	hsl(77,100%,42%)
        {color::Purple2,          "Purple2"/*#8700ff	*/, 0x87, 0x00, 0xff},//#8700ff	rgb(135,0,255)	hsl(71,100%,50%)
        {color::Orange5,          "Orange5"/*#875f00	*/, 0x87, 0x5f, 0x00},//#875f00	rgb(135,95,0)	hsl(2,100%,26%)
        {color::LightPink4,       "LightPink4"/*#875f*/, 0x87, 0x5f, 0x5f},//#875f5f	rgb(135,95,95)	hsl(0,17%,45%)
        {color::Plum4,            "Plum4"/*#875f87	*/, 0x87, 0x5f, 0x87},//#875f87	rgb(135,95,135)	hsl(300,17%,45%)
        {color::MediumPurple3,    "MediumPurple3"/*#8*/, 0x87, 0x5f, 0xaf},//#875faf	rgb(135,95,175)	hsl(270,33%,52%)
        {color::MediumPurple5,    "MediumPurple5"/*#8*/, 0x87, 0x5f, 0xd7},//#875fd7	rgb(135,95,215)	hsl(260,60%,60%)
        {color::SlateBlue1,       "SlateBlue1"/*#875f*/, 0x87, 0x5f, 0xff},//#875fff	rgb(135,95,255)	hsl(255,100%,68%)
        {color::Yellow4,          "Yellow4"/*#878700	*/, 0x87, 0x87, 0x00},//#878700	rgb(135,135,0)	hsl(60,100%,26%)
        {color::Wheat4,           "Wheat4"/*#87875f	*/, 0x87, 0x87, 0x5f},//#87875f	rgb(135,135,95)	hsl(60,17%,45%)
        {color::Grey53,           "Grey53"/*#878787	*/, 0x87, 0x87, 0x87},//#878787	rgb(135,135,135)	hsl(0,0%,52%)
        {color::LighcoreateGrey,   "LighcoreateGrey"/*#*/, 0x87, 0x87, 0xaf},//#8787af	rgb(135,135,175)	hsl(240,20%,60%)
        {color::MediumPurple,     "MediumPurple"/*#87*/, 0x87, 0x87, 0xd7},//#8787d7	rgb(135,135,215)	hsl(240,50%,68%)
        {color::LighcoreateBlue,   "LighcoreateBlue"/*#*/, 0x87, 0x87, 0xff},//#8787ff	rgb(135,135,255)	hsl(240,100%,76%)
        {color::Yellow5,          "Yellow5"/*#87af00	*/, 0x87, 0xaf, 0x00},//#87af00	rgb(135,175,0)	hsl(3,100%,34%)
        {color::DarkOliveGreen3,  "DarkOliveGreen3"/**/, 0x87, 0xaf, 0x5f},//#87af5f	rgb(135,175,95)	hsl(90,33%,52%)
        {color::DarkSeaGreen,     "DarkSeaGreen"/*#87*/, 0x87, 0xaf, 0x87},//#87af87	rgb(135,175,135)	hsl(120,20%,60%)
        {color::LightSkyBlue3,    "LightSkyBlue3"/*#8*/, 0x87, 0xaf, 0xaf},//#87afaf	rgb(135,175,175)	hsl(180,20%,60%)
        {color::LightSkyBlue4,    "LightSkyBlue4"/*#8*/, 0x87, 0xaf, 0xd7},//#87afd7	rgb(135,175,215)	hsl(210,50%,68%)
        {color::SkyBlue2,         "SkyBlue2"/*#87afff*/, 0x87, 0xaf, 0xff},//#87afff	rgb(135,175,255)	hsl(220,100%,76%)
        {color::Chartreuse6,      "Chartreuse6"/*#87d*/, 0x87, 0xd7, 0x00},//#87d700	rgb(135,215,0)	hsl(2,100%,42%)
        {color::DarkOliveGreen4,  "DarkOliveGreen4"/**/, 0x87, 0xd7, 0x5f},//#87d75f	rgb(135,215,95)	hsl(100,60%,60%)
        {color::PaleGreen4,       "PaleGreen4"/*#87d7*/, 0x87, 0xd7, 0x87},//#87d787	rgb(135,215,135)	hsl(120,50%,68%)
        {color::DarkSeaGreen3,    "DarkSeaGreen3"/*#8*/, 0x87, 0xd7, 0xaf},//#87d7af	rgb(135,215,175)	hsl(150,50%,68%)
        {color::DarkSlateGray3,   "DarkSlateGray3"/*#*/, 0x87, 0xd7, 0xd7},//#87d7d7	rgb(135,215,215)	hsl(180,50%,68%)
        {color::SkyBlue1,         "SkyBlue1"/*#87d7ff*/, 0x87, 0xd7, 0xff},//#0x87,0xd7,0xff	rgb(135,215,255)	hsl(200,100%,76%)
        {color::Chartreuse1,      "Chartreuse1"/*#87f*/, 0x87, 0xff, 0x00},//#87ff00	rgb(135,255,0)	hsl(8,100%,50%)
        {color::LightGreen,       "LightGreen"/*#87ff*/, 0x87, 0xff, 0x5f},//#87ff5f	rgb(135,255,95)	hsl(105,100%,68%)
        {color::LightGreen2,      "LightGreen2"/*#87f*/, 0x87, 0xff, 0x87},//#87ff87	rgb(135,255,135)	hsl(120,100%,76%)
        {color::PaleGreen1,       "PaleGreen1"/*#87ff*/, 0x87, 0xff, 0xaf},//#87ffaf	rgb(135,255,175)	hsl(140,100%,76%)
        {color::Aquamarine2,      "Aquamarine2"/*#87f*/, 0x87, 0xff, 0xd7},//#87ffd7	rgb(135,255,215)	hsl(160,100%,76%)
        {color::DarkSlateGray1,   "DarkSlateGray1"/*#*/, 0x87, 0xff, 0xff},//#87ffff	rgb(135,255,255)	hsl(180,100%,76%)
        {color::Red3,             "Red3"/*#af0000	*/, 0xaf, 0x00, 0x00},//#af0000	rgb(175,0,0)	hsl(0,100%,34%)
        {color::DeepPink6,        "DeepPink6"/*#af005*/, 0xaf, 0x00, 0x5f},//#af005f	rgb(175,0,95)	hsl(27,100%,34%)
        {color::MediumVioletRed,  "MediumVioletRed"/**/, 0xaf, 0x00, 0x87},//#af0087	rgb(175,0,135)	hsl(13,100%,34%)
        {color::Magenta3,         "Magenta3"/*#af00af*/, 0xaf, 0x00, 0xaf},//#af00af	rgb(175,0,175)	hsl(300,100%,34%)
        {color::DarkViole2,       "DarkViole2"/*#af00*/, 0xaf, 0x00, 0xd7},//#af00d7	rgb(175,0,215)	hsl(88,100%,42%)
        {color::Purple6,          "Purple6"/*#af00ff	*/, 0xaf, 0x00, 0xff},//#af00ff	rgb(175,0,255)	hsl(81,100%,50%)
        {color::DarkOrange3,      "DarkOrange3"/*#af5*/, 0xaf, 0x5f, 0x00},//#af5f00	rgb(175,95,0)	hsl(2,100%,34%)
        {color::IndianRed,        "IndianRed"/*#af5f5*/, 0xaf, 0x5f, 0x5f},//#af5f5f	rgb(175,95,95)	hsl(0,33%,52%)
        {color::HotPink3,         "HotPink3"/*#af5f87*/, 0xaf, 0x5f, 0x87},//#af5f87	rgb(175,95,135)	hsl(330,33%,52%)
        {color::MediumOrchid3,    "MediumOrchid3"/*#a*/, 0xaf, 0x5f, 0xaf},//#af5faf	rgb(175,95,175)	hsl(300,33%,52%)
        {color::MediumOrchid,     "MediumOrchid"/*#af*/, 0xaf, 0x5f, 0xd7},//#af5fd7	rgb(175,95,215)	hsl(280,60%,60%)
        {color::MediumPurple2,    "MediumPurple2"/*#a*/, 0xaf, 0x5f, 0xff},//#af5fff	rgb(175,95,255)	hsl(270,100%,68%)
        {color::DarkGoldenrod,    "DarkGoldenrod"/*#a*/, 0xaf, 0x87, 0x00},//#af8700	rgb(175,135,0)	hsl(6,100%,34%)
        {color::LightSalmon3,     "LightSalmon3"/*#af*/, 0xaf, 0x87, 0x5f},//#af875f	rgb(175,135,95)	hsl(30,33%,52%)
        {color::RosyBrown,        "RosyBrown"/*#af878*/, 0xaf, 0x87, 0x87},//#af8787	rgb(175,135,135)	hsl(0,20%,60%)
        {color::Grey63,           "Grey63"/*#af87af	*/, 0xaf, 0x87, 0xaf},//#af87af	rgb(175,135,175)	hsl(300,20%,60%)
        {color::MediumPurple6,    "MediumPurple6"/*#a*/, 0xaf, 0x87, 0xd7},//#af87d7	rgb(175,135,215)	hsl(270,50%,68%)
        {color::MediumPurple1,    "MediumPurple1"/*#a*/, 0xaf, 0x87, 0xff},//#af87ff	rgb(175,135,255)	hsl(260,100%,76%)
        {color::Gold3,            "Gold3"/*#afaf00	*/, 0xaf, 0xaf, 0x00},//#afaf00	rgb(175,175,0)	hsl(60,100%,34%)
        {color::DarkKhaki,        "DarkKhaki"/*#afaf5*/, 0xaf, 0xaf, 0x5f},//#afaf5f	rgb(175,175,95)	hsl(60,33%,52%)
        {color::NavajoWhite3,     "NavajoWhite3"/*#af*/, 0xaf, 0xaf, 0x87},//#afaf87	rgb(175,175,135)	hsl(60,20%,60%)
        {color::Grey69,           "Grey69"/*#afafaf	*/, 0xaf, 0xaf, 0xaf},//#afafaf	rgb(175,175,175)	hsl(0,0%,68%)
        {color::LightSteelBlue3,  "LightSteelBlue3"/**/, 0xaf, 0xaf, 0xd7},//#afafd7	rgb(175,175,215)	hsl(240,33%,76%)
        {color::LightSteelBlue,   "LightSteelBlue"/*#*/, 0xaf, 0xaf, 0xff},//#afafff	rgb(175,175,255)	hsl(240,100%,84%)
        {color::Yellow3,          "Yellow3"/*#afd700	*/, 0xaf, 0xd7, 0x00},//#afd700	rgb(175,215,0)	hsl(1,100%,42%)
        {color::DarkOliveGreen5,  "DarkOliveGreen5"/**/, 0xaf, 0xd7, 0x5f},//#afd75f	rgb(175,215,95)	hsl(80,60%,60%)
        {color::DarkSeaGreen6,    "DarkSeaGreen6"/*#a*/, 0xaf, 0xd7, 0x87},//#afd787	rgb(175,215,135)	hsl(90,50%,68%)
        {color::DarkSeaGreen2,    "DarkSeaGreen2"/*#a*/, 0xaf, 0xd7, 0xaf},//#afd7af	rgb(175,215,175)	hsl(120,33%,76%)
        {color::LightCyan3,       "LightCyan3"/*#afd7*/, 0xaf, 0xd7, 0xd7},//#afd7d7	rgb(175,215,215)	hsl(180,33%,76%)
        {color::LightSkyBlue1,    "LightSkyBlue1"/*#a*/, 0xaf, 0xd7, 0xff},//#afd7ff	rgb(175,215,255)	hsl(210,100%,84%)
        {color::GreenYellow,      "GreenYellow"/*#aff*/, 0xaf, 0xff, 0x00},//#afff00	rgb(175,255,0)	hsl(8,100%,50%)
        {color::DarkOliveGreen2,  "DarkOliveGreen2"/**/, 0xaf, 0xff, 0x5f},//#afff5f	rgb(175,255,95)	hsl(90,100%,68%)
        {color::PaleGreen2,       "PaleGreen2"/*#afff*/, 0xaf, 0xff, 0x87},//#afff87	rgb(175,255,135)	hsl(100,100%,76%)
        {color::DarkSeaGreen7,    "DarkSeaGreen7"/*#a*/, 0xaf, 0xff, 0xaf},//#afffaf	rgb(175,255,175)	hsl(120,100%,84%)
        {color::DarkSeaGreen1,    "DarkSeaGreen1"/*#a*/, 0xaf, 0xff, 0xd7},//#afffd7	rgb(175,255,215)	hsl(150,100%,84%)
        {color::PaleTurquoise1,   "PaleTurquoise1"/*#*/, 0xaf, 0xff, 0xff},//#afffff	rgb(175,255,255)	hsl(180,100%,84%)
        {color::Red4,             "Red4"/*#d70000	*/, 0xd7, 0x00, 0x00},//#d70000	rgb(215,0,0)	hsl(0,100%,42%)
        {color::DeepPink3,        "DeepPink3"/*#d7005*/, 0xd7, 0x00, 0x5f},//#d7005f	rgb(215,0,95)	hsl(33,100%,42%)
        {color::DeepPink7,        "DeepPink7"/*#d7008*/, 0xd7, 0x00, 0x87},//#d70087	rgb(215,0,135)	hsl(22,100%,42%)
        {color::Magenta4,         "Magenta4"/*#d700af*/, 0xd7, 0x00, 0xaf},//#d700af	rgb(215,0,175)	hsl(11,100%,42%)
        {color::Magenta5,         "Magenta5"/*#d700d7*/, 0xd7, 0x00, 0xd7},//#d700d7	rgb(215,0,215)	hsl(300,100%,42%)
        {color::Magenta2,         "Magenta2"/*#d700ff*/, 0xd7, 0x00, 0xff},//#d700ff	rgb(215,0,255)	hsl(90,100%,50%)
        {color::DarkOrange4,      "DarkOrange4"/*#d75*/, 0xd7, 0x5f, 0x00},//#d75f00	rgb(215,95,0)	hsl(6,100%,42%)
        {color::IndianRed2,       "IndianRed2"/*#d75f*/, 0xd7, 0x5f, 0x5f},//#d75f5f	rgb(215,95,95)	hsl(0,60%,60%)
        {color::HotPink4,         "HotPink4"/*#d75f87*/, 0xd7, 0x5f, 0x87},//#d75f87	rgb(215,95,135)	hsl(340,60%,60%)
        {color::HotPink2,         "HotPink2"/*#d75faf*/, 0xd7, 0x5f, 0xaf},//#d75faf	rgb(215,95,175)	hsl(320,60%,60%)
        {color::Orchid,           "Orchid"/*#d75fd7	*/, 0xd7, 0x5f, 0xd7},//#d75fd7	rgb(215,95,215)	hsl(300,60%,60%)
        {color::MediumOrchid1,    "MediumOrchid1"/*#d*/, 0xd7, 0x5f, 0xff},//#d75fff	rgb(215,95,255)	hsl(285,100%,68%)
        {color::Orange3,          "Orange3"/*#d78700	*/, 0xd7, 0x87, 0x00},//#d78700	rgb(215,135,0)	hsl(7,100%,42%)
        {color::LightSalmon4,     "LightSalmon4"/*#d7*/, 0xd7, 0x87, 0x5f},//#d7875f	rgb(215,135,95)	hsl(20,60%,60%)
        {color::LightPink3,       "LightPink3"/*#d787*/, 0xd7, 0x87, 0x87},//#d78787	rgb(215,135,135)	hsl(0,50%,68%)
        {color::Pink3,            "Pink3"/*#d787af	*/, 0xd7, 0x87, 0xaf},//#d787af	rgb(215,135,175)	hsl(330,50%,68%)
        {color::Plum3,            "Plum3"/*#d787d7	*/, 0xd7, 0x87, 0xd7},//#d787d7	rgb(215,135,215)	hsl(300,50%,68%)
        {color::Violet,           "Violet"/*#d787ff	*/, 0xd7, 0x87, 0xff},//#d787ff	rgb(215,135,255)	hsl(280,100%,76%)
        {color::Gold4,            "Gold4"/*#d7af00	*/, 0xd7, 0xaf, 0x00},//#d7af00	rgb(215,175,0)	hsl(8,100%,42%)
        {color::LightGoldenrod3,  "LightGoldenrod3"/**/, 0xd7, 0xaf, 0x5f},//#d7af5f	rgb(215,175,95)	hsl(40,60%,60%)
        {color::Tan,              "Tan"/*#d7af87	rgb(*/, 0xd7, 0xaf, 0x87},//#d7af87	rgb(215,175,135)	hsl(30,50%,68%)
        {color::MistyRose3,       "MistyRose3"/*#d7af*/, 0xd7, 0xaf, 0xaf},//#d7afaf	rgb(215,175,175)	hsl(0,33%,76%)
        {color::Thistle3,         "Thistle3"/*#d7afd7*/, 0xd7, 0xaf, 0xd7},//#d7afd7	rgb(215,175,215)	hsl(300,33%,76%)
        {color::Plum2,            "Plum2"/*#d7afff	*/, 0xd7, 0xaf, 0xff},//#d7afff	rgb(215,175,255)	hsl(270,100%,84%)
        {color::Yellow6,          "Yellow6"/*#d7d700	*/, 0xd7, 0xd7, 0x00},//#d7d700	rgb(215,215,0)	hsl(60,100%,42%)
        {color::Khaki3,           "Khaki3"/*#d7d75f	*/, 0xd7, 0xd7, 0x5f},//#d7d75f	rgb(215,215,95)	hsl(60,60%,60%)
        {color::LightGoldenrod2,  "LightGoldenrod2"/**/, 0xd7, 0xd7, 0x87},//#d7d787	rgb(215,215,135)	hsl(60,50%,68%)
        {color::LightYellow3,     "LightYellow3"/*#d7*/, 0xd7, 0xd7, 0xaf},//#d7d7af	rgb(215,215,175)	hsl(60,33%,76%)
        {color::Grey84,           "Grey84"/*#d7d7d7	*/, 0xd7, 0xd7, 0xd7},//#d7d7d7	rgb(215,215,215)	hsl(0,0%,84%)
        {color::LightSteelBlue1,  "LightSteelBlue1"/**/, 0xd7, 0xd7, 0xff},//#d7d7ff	rgb(215,215,255)	hsl(240,100%,92%)
        {color::Yellow2,          "Yellow2"/*#d7ff00	*/, 0xd7, 0xff, 0x00},//#d7ff00	rgb(215,255,0)	hsl(9,100%,50%)
        {color::DarkOliveGreen1,  "DarkOliveGreen1"/**/, 0xd7, 0xff, 0x5f},//#d7ff5f	rgb(215,255,95)	hsl(75,100%,68%)
        {color::DarkOliveGreen6,  "DarkOliveGreen6"/**/, 0xd7, 0xff, 0x87},//#d7ff87	rgb(215,255,135)	hsl(80,100%,76%)
        {color::DarkSeaGreen8,    "DarkSeaGreen8"/*#d*/, 0xd7, 0xff, 0xaf},//#d7ffaf	rgb(215,255,175)	hsl(90,100%,84%)
        {color::Honeydew2,        "Honeydew2"/*#d7ffd*/, 0xd7, 0xff, 0xd7},//#d7ffd7	rgb(215,255,215)	hsl(120,100%,92%)
        {color::LightCyan1,       "LightCyan1"/*#d7ff*/, 0xd7, 0xff, 0xff},//#d7ffff	rgb(215,255,255)	hsl(180,100%,92%)
        {color::Red1,             "Red1"/*#ff0000	*/, 0xff, 0x00, 0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
        {color::DeepPink2,        "DeepPink2"/*#ff005*/, 0xff, 0x00, 0x5f},//#ff005f	rgb(255,0,95)	hsl(37,100%,50%)
        {color::DeepPink1,        "DeepPink1"/*#ff008*/, 0xff, 0x00, 0x87},//#ff0087	rgb(255,0,135)	hsl(28,100%,50%)
        {color::DeepPink8,        "DeepPink8"/*#ff00a*/, 0xff, 0x00, 0xaf},//#ff00af	rgb(255,0,175)	hsl(18,100%,50%)
        {color::Magenta6,         "Magenta6"/*#ff00d7*/, 0xff, 0x00, 0xd7},//#ff00d7	rgb(255,0,215)	hsl(09,100%,50%)
        {color::Magenta1,         "Magenta1"/*#ff00ff*/, 0xff, 0x00, 0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
        {color::OrangeRed1,       "OrangeRed1"/*#ff5f*/, 0xff, 0x5f, 0x00},//#ff5f00	rgb(255,95,0)	hsl(2,100%,50%)
        {color::IndianRed1,       "IndianRed1"/*#ff5f*/, 0xff, 0x5f, 0x5f},//#ff5f5f	rgb(255,95,95)	hsl(0,100%,68%)
        {color::IndianRed3,       "IndianRed3"/*#ff5f*/, 0xff, 0x5f, 0x87},//#ff5f87	rgb(255,95,135)	hsl(345,100%,68%)
        {color::HotPink,          "HotPink"/*#ff5faf	*/, 0xff, 0x5f, 0xaf},//#ff5faf	rgb(255,95,175)	hsl(330,100%,68%)
        {color::HotPin2,          "HotPin2"/*#ff5fd7	*/, 0xff, 0x5f, 0xd7},//#ff5fd7	rgb(255,95,215)	hsl(315,100%,68%)
        {color::MediumOrchid2,    "MediumOrchid2"/*#f*/, 0xff, 0x5f, 0xff},//#ff5fff	rgb(255,95,255)	hsl(300,100%,68%)
        {color::DarkOrange,       "DarkOrange"/*#ff87*/, 0xff, 0x87, 0x00},//#ff8700	rgb(255,135,0)	hsl(1,100%,50%)
        {color::Salmon1,          "Salmon1"/*#ff875f	*/, 0xff, 0x87, 0x5f},//#ff875f	rgb(255,135,95)	hsl(15,100%,68%)
        {color::LightCoral,       "LightCoral"/*#ff87*/, 0xff, 0x87, 0x87},//#ff8787	rgb(255,135,135)	hsl(0,100%,76%)
        {color::PaleVioletRed1,   "PaleVioletRed1"/*#*/, 0xff, 0x87, 0xaf},//#ff87af	rgb(255,135,175)	hsl(340,100%,76%)
        {color::Orchid2,          "Orchid2"/*#ff87d7	*/, 0xff, 0x87, 0xd7},//#ff87d7	rgb(255,135,215)	hsl(320,100%,76%)
        {color::Orchid1,          "Orchid1"/*#ff87ff	*/, 0xff, 0x87, 0xff},//#ff87ff	rgb(255,135,255)	hsl(300,100%,76%)
        {color::Orange1,          "Orange1"/*#ffaf00	*/, 0xff, 0xaf, 0x00},//#ffaf00	rgb(255,175,0)	hsl(1,100%,50%)
        {color::SandyBrown,       "SandyBrown"/*#ffaf*/, 0xff, 0xaf, 0x5f},//#ffaf5f	rgb(255,175,95)	hsl(30,100%,68%)
        {color::LightSalmon1,     "LightSalmon1"/*#ff*/, 0xff, 0xaf, 0x87},//#ffaf87	rgb(255,175,135)	hsl(20,100%,76%)
        {color::LightPink1,       "LightPink1"/*#ffaf*/, 0xff, 0xaf, 0xaf},//#ffafaf	rgb(255,175,175)	hsl(0,100%,84%)
        {color::Pink1,            "Pink1"/*#ffafd7	*/, 0xff, 0xaf, 0xd7},//#ffafd7	rgb(255,175,215)	hsl(330,100%,84%)
        {color::Plum1,            "Plum1"/*#ffafff	*/, 0xff, 0xaf, 0xff},//#ffafff	rgb(255,175,255)	hsl(300,100%,84%)
        {color::Gold1,            "Gold1"/*#ffd700	*/, 0xff, 0xd7, 0x00},//#ffd700	rgb(255,215,0)	hsl(0,100%,50%)
        {color::LightGoldenrod4,  "LightGoldenrod4"/**/, 0xff, 0xd7, 0x5f},//#ffd75f	rgb(255,215,95)	hsl(45,100%,68%)
        {color::LightGoldenrod5,  "LightGoldenrod5"/**/, 0xff, 0xd7, 0x87},//#ffd787	rgb(255,215,135)	hsl(40,100%,76%)
        {color::NavajoWhite1,     "NavajoWhite1"/*#ff*/, 0xff, 0xd7, 0xaf},//#ffd7af	rgb(255,215,175)	hsl(30,100%,84%)
        {color::MistyRose1,       "MistyRose1"/*#ffd7*/, 0xff, 0xd7, 0xd7},//#ffd7d7	rgb(255,215,215)	hsl(0,100%,92%)
        {color::Thistle1,         "Thistle1"/*#ffd7ff*/, 0xff, 0xd7, 0xff},//#ffd7ff	rgb(255,215,255)	hsl(300,100%,92%)
        {color::Yellow1,          "Yellow1"/*#ffff00	*/, 0xff, 0xff, 0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
        {color::LightGoldenrod1,  "LightGoldenrod1"/**/, 0xff, 0xff, 0x5f},//#ffff5f	rgb(255,255,95)	hsl(60,100%,68%)
        {color::Khaki1,           "Khaki1"/*#ffff87	*/, 0xff, 0xff, 0x87},//#ffff87	rgb(255,255,135)	hsl(60,100%,76%)
        {color::Wheat1,           "Wheat1"/*#ffffaf	*/, 0xff, 0xff, 0xaf},//#ffffaf	rgb(255,255,175)	hsl(60,100%,84%)
        {color::Cornsilk1,        "Cornsilk1"/*#ffffd*/, 0xff, 0xff, 0xd7},//#ffffd7	rgb(255,255,215)	hsl(60,100%,92%)
        {color::Grey100,          "Grey100"/*#ffffff	*/, 0xff, 0xff, 0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
        {color::Grey3,            "Grey3"/*#080808	*/, 0x08, 0x08, 0x08},//#080808	rgb(8,8,8)	hsl(0,0%,3%)
        {color::Grey7,            "Grey7"/*#121212	*/, 0x12, 0x12, 0x12},//#121212	rgb(18,18,18)	hsl(0,0%,7%)
        {color::Grey11,           "Grey11"/*#1c1c1c	*/, 0x1c, 0x1c, 0x1c},//#1c1c1c	rgb(28,28,28)	hsl(0,0%,10%)
        {color::Grey15,           "Grey15"/*#262626	*/, 0x26, 0x26, 0x26},//#262626	rgb(38,38,38)
        {color::Grey19,           "Grey19"/*#303030	*/, 0x30, 0x30, 0x30},//#303030	rgb(48,48,48)	hsl(0,0%,18%)
        {color::Grey23,           "Grey23"/*#3a3a3a	*/, 0x3a, 0x3a, 0x3a},//#3a3a3a	rgb(58,58,58)	hsl(0,0%,22%)
        {color::Grey27,           "Grey27"/*#444444	*/, 0x44, 0x44, 0x44},//#444444	rgb(68,68,68)	hsl(0,0%,26%)
        {color::Grey30,           "Grey30"/*#4e4e4e	*/, 0x4e, 0x4e, 0x4e},//#4e4e4e	rgb(78,78,78)	hsl(0,0%,30%)
        {color::Grey35,           "Grey35"/*#585858	*/, 0x58, 0x58, 0x58},//#585858	rgb(88,88,88)	hsl(0,0%,34%)
        {color::Grey39,           "Grey39"/*#626262	*/, 0x62, 0x62, 0x62},//#626262	rgb(98,98,98)	hsl(0,0%,37%)
        {color::Grey42,           "Grey42"/*#6c6c6c	*/, 0x6c, 0x6c, 0x6c},//#6c6c6c	rgb(108,108,108)	hsl(0,0%,40%)
        {color::Grey46,           "Grey46"/*#767676	*/, 0x76, 0x76, 0x76},//#767676	rgb(118,118,118)	hsl(0,0%,46%)
        {color::Grey50,           "Grey50"/*#808080	*/, 0x80, 0x80, 0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
        {color::Grey54,           "Grey54"/*#8a8a8a	*/, 0x8a, 0x8a, 0x8a},//#8a8a8a	rgb(138,138,138)	hsl(0,0%,54%)
        {color::Grey58,           "Grey58"/*#949494	*/, 0x94, 0x94, 0x94},//#949494	rgb(148,148,148)	hsl(0,0%,58%)
        {color::Grey62,           "Grey62"/*#9e9e9e	*/, 0x9e, 0x9e, 0x9e},//#9e9e9e	rgb(158,158,158)	hsl(0,0%,61%)
        {color::Grey66,           "Grey66"/*#a8a8a8	*/, 0xa8, 0xa8, 0xa8},//#a8a8a8	rgb(168,168,168)	hsl(0,0%,65%)
        {color::Grey70,           "Grey70"/*#b2b2b2	*/, 0xb2, 0xb2, 0xb2},//#b2b2b2	rgb(178,178,178)	hsl(0,0%,69%)
        {color::Grey74,           "Grey74"/*#bcbcbc	*/, 0xbc, 0xbc, 0xbc},//#bcbcbc	rgb(188,188,188)	hsl(0,0%,73%)
        {color::Grey78,           "Grey78"/*#c6c6c6	*/, 0xc6, 0xc6, 0xc6},//#c6c6c6	rgb(198,198,198)	hsl(0,0%,77%)
        {color::Grey82,           "Grey82"/*#d0d0d0	*/, 0xd0, 0xd0, 0xd0},//#d0d0d0	rgb(208,208,208)	hsl(0,0%,81%)
        {color::Grey85,           "Grey85"/*#dadada	*/, 0xda, 0xda, 0xda},//#dadada	rgb(218,218,218)	hsl(0,0%,85%)
        {color::Grey89,           "Grey89"/*#e4e4e4	*/, 0xe4, 0xe4, 0xe4},//#e4e4e4	rgb(228,228,228)	hsl(0,0%,89%)
        {color::Reset,            "Reset"/*#eeeeee	*/, 0,    0,    0}
    };


#pragma region Attr

    std::string scrat::attr<textattr::format::ansi256>::fg(color::type C)
    {
        if (C == color::Reset) { return "\033[0m"; }
        std::ostringstream Out;
        Out << "\033[38;5;" << static_cast<int>(C) << 'm';

        return Out.str();
    }

    std::string attr<textattr::format::ansi256>::bg(color::type C)
    {
        if (C == color::Reset) { return "\033[0m"; }
        std::ostringstream Out;
        Out << "\033[48;5;" << static_cast<int>(C) << 'm';

        return Out.str();
    }


    std::string scrat::attr<textattr::format::html>::fg(color::type C)
    {
        if (C == color::Reset) { return "</span>"; }

        std::ostringstream Out;
        const color_data D = ColorDB[(int)C];
        Out << "<span style=\"color: #" << std::hex << std::setw(2) << std::setfill('0') << D.R << D.G << D.B << ";\">";

        return Out.str();
    }

    std::string attr<textattr::format::html>::bg(color::type C)
    {

        std::ostringstream Out;
        const color_data D = ColorDB[(int)C];
        Out << "<span style=\"background-color: #" << std::hex << std::setw(2) << std::setfill('0') << D.R << D.G << D.B << ";\">";

        return Out.str();
    }

    std::string color_data::html() const
    {
        return "";
    }

#pragma endregion Attr



    std::string textattr::pair::operator()()
    {
        // std::string s = textattr::ansi_bk(bg);
        // s += attr<textattr::format::ansi256>::fg(fg);
        return textattr::ansi(*this);
    }

    textattr::pair &textattr::pair::operator>>(std::string &out)
    {
        out = textattr::ansi(*this);
        return *this;
    }

    std::string textattr::ansi(color::type aColorID)
    {
        if (aColorID == color::Reset) { return "\033[0m"; }
        std::ostringstream Out;
        Out << "\033[38;5;" << static_cast<int>(aColorID) << 'm';

        return Out.str();
    }

    std::string textattr::ansi(textattr::pair color_pair_)
    {
        std::ostringstream Out;
        Out << "\033[";
        if ((color_pair_.fg == color::Reset) || (color_pair_.bg == color::Reset)) Out << "0";
        if(color_pair_.fg != color::Reset)  Out << "38;5;" << static_cast<int>(color_pair_.fg);
        if(color_pair_.bg != color::Reset) Out <<  ";48;5;" << static_cast<int>(color_pair_.bg);
        Out << 'm';

        return Out.str();

    }







    color::type textattr::scan(const std::string& aName)
    {
        int N = 0;
        for (auto& Clr : ColorDB)
        {
            if (aName == Clr.Name) return static_cast<color::type>(N);
            ++N;
        }
        return color::Reset;
    }


    std::string textattr::ansi_bk(color::type aColorID)
    {
        if (aColorID == color::Reset) { return "\033[0m"; }
        std::ostringstream Out;
        Out << "\033[48;5;" << static_cast<int>(aColorID) << 'm';

        return Out.str();
    }

    std::string textattr::name(color::type aColorEnum)
    {
        return ColorDB[aColorEnum].Name.data();
    }

    std::string attr_item::operator ()()
    {
        return p();
    }


    attr_item& attribute_list::operator[](const std::string& id_)
    {
        for (auto& item : items)
            if (item.id == id_) return item;
        return *items.begin(); // Toujours mettre default values au debut...
    }

}