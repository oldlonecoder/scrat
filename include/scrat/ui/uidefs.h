#pragma once


#include <cstdint>
#include <map>
#include <scrat/textattr>

namespace scrat::ui
{


/**
 * @brief Widget ui justification enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace Justify
{
using Type = uint8_t;
static constexpr uint8_t Left 		= 0x01;
static constexpr uint8_t Right 		= 0x02;
static constexpr uint8_t HCenter 	= 0x04;
static constexpr uint8_t VCenter 	= 0x08;
static constexpr uint8_t Center 	= 0x0C;
static constexpr uint8_t WordWRap 	= 0x10;
static constexpr uint8_t Prefix 	= 0x20;
static constexpr uint8_t Suffix 	= 0x40;
}


/**
 * @brief Direction enums values
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace Direction
{
using Type = int8_t;
static constexpr int8_t Left = -1;
static constexpr int8_t Right = 1;
static constexpr int8_t Up = 2;
static constexpr int8_t Down = -2;
}

/**
 * @brief Widget ui class enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace wclass
{
using type = uint16_t;
static constexpr type TopLevel 	= 0x0001;
static constexpr type Floating 	= 0x0002;
static constexpr type Child 	= 0x0004;
static constexpr type Parent 	= 0x0008;
static constexpr type Caption 	= 0x0010;
static constexpr type Frame 	= 0x0020;
static constexpr type Input 	= 0x0040;
//...
}
/**
 * @brief Widget ui state enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace State
{
	using Type = uint8_t;
	static constexpr Type Normal  = 0x00;
	static constexpr Type Disable = 0x01;
	static constexpr Type Active  = 0x02;
	static constexpr Type Focus   = 0x04;
	static constexpr Type Error   = 0x08;
	static constexpr Type Warning = 0x10;
	static constexpr Type Illegal = 0x20;
    static constexpr Type Invalid = 0x40;
	static constexpr Type Visible = 0x80;
}

namespace colors
{

struct SCRAT_API db
{
    using components= std::map<State::Type,textattr::pair>;
    using elements  = std::map<std::string_view, db::components>;
    using themes    = std::map<std::string_view, db::elements>;
    //using theme     = std::map<std::string_view, db::dictionary>;
    static db::themes data;
};
}




}
