#pragma once

#include <scrat/accents.fr.h>
#include <scrat/Icons.h>
#include <scrat/geometry.h>
#include <scrat/object.h>

namespace scrat::ui
{

class SCRAT_API vdc
{
	object* _owner = nullptr;
	point pos = { 0,0 };
	scrat::dim wh;
	
	rect _peer;

	friend class console;
	friend class painter;

public:
	using type = uint32_t*;

	vdc() = default;
	vdc(object* parent_, scrat::dim dimension_ = {});

	struct SCRAT_API cell
	{
		using type = uint32_t;
		cell::type mem = 0;

		// ============== Bitmasks =============================
		static constexpr uint32_t CharMask	= 0x000000FF;
		static constexpr uint32_t FGMask	= 0x0000FF00;
		static constexpr uint32_t BGMask	= 0x00FF0000;
		static constexpr uint32_t CMask		= 0x00FFFF00;
		static constexpr uint32_t AttrMask	= 0xFF000000;
		// ==============Bit Shift ============================
		static constexpr int FGShift = 0x08;
		static constexpr int BGShift = 0x10;
		static constexpr int ATShift = 0x18;
		// ==================Attributes==========================
		static constexpr uint32_t  UGlyph = 0x01000000;
		static constexpr uint32_t  Underline = 0x02000000;
		static constexpr uint32_t  Stroke = 0x04000000;
		static constexpr uint32_t  Blink = 0x08000000;
		static constexpr uint32_t  Accent = 0x10000000;


		cell() = default;
		cell(cell::type c_);
		cell(vdc::type c_);

		~cell() = default;

		vdc::cell& set_fg(color::type fg_);
		vdc::cell& set_bg(color::type bg_);
		
		vdc::cell& set_attribute(vdc::cell::type d_);
		
		vdc::cell& set_color(textattr::pair&& c_);
		vdc::cell& set_color(const textattr::pair& c_);

		vdc::cell& operator=(vdc::cell::type d_);
		vdc::cell& operator=(vdc::type d_);

		color::type fg();
		color::type bg();
		textattr::pair colors();
		
		uint8_t ascii();

		vdc::cell::type& operator*() { return mem; }
		uint16_t attributes();

		vdc::cell& operator << (Icon::Type i_);
		vdc::cell& operator << (Accent::Type a_);
		std::string render_colors();
		operator std::string();



	};


};

}
