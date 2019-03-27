#pragma once
#ifndef FONT_TO_BITMAP_H
#define FONT_TO_BITMAP_H

#include "tarja/core.hpp"
#include "tarja/mathematics.hpp"

#include "gl_enums.h"
#include "tarja/image.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

struct CharacterData {
public:
	std::vector<uint8_t> buffer;
	glm::ivec2 size;

	CharacterData() {}
	CharacterData(const std::vector<uint8_t>& bitmap_buffer, const glm::ivec2& bitmap_size) :
		buffer(bitmap_buffer), size(bitmap_size)
	{
	}
	~CharacterData() {}
};

class FontToBitmap : public Singleton<FontToBitmap>
{
	friend class Singleton<FontToBitmap>;
private:
	struct FontInfo
	{
		using CharacterDataMap = std::array<CharacterData, 128>;
		using CharacterSizeMap = std::unordered_map<uint16_t, CharacterDataMap>;

		FT_Face face;
		CharacterSizeMap characters;

		~FontInfo() { }//FT_Done_Face(face);
	};

private:
	FT_Library freetype_lib_;
	std::unordered_map<core::FontName, FontInfo> fonts_;

public:
	FontToBitmap();
	~FontToBitmap();

	CharacterData& get_character(core::FontName font, const uint16_t& size, const int8_t& character);
	image::Bitmap text_to_bitmap(core::FontName, const uint16_t& font_size, const std::string& text);
	image::Bitmap text_to_bitmap(core::FontName, const uint16_t& font_size, const std::string& text, uint16_t max_length);

private:
	void load_font(core::FontName font, const uint16_t& size);
};

#endif // !CHARACTER_GENERATOR_H



