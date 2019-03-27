#include "font_to_bitmap.h"
#include "../resources.h"

FontToBitmap::FontToBitmap()
{
	ASSERT_IF_TRUE(FT_Init_FreeType(&freetype_lib_) > 0, "'FT_Init_FreeType()' Could not initialize FreeType Library");
}


FontToBitmap::~FontToBitmap()
{
	FT_Done_FreeType(freetype_lib_);
}

CharacterData & FontToBitmap::get_character(core::FontName font, const uint16_t& size, const int8_t & character)
{
	if (fonts_.find(font) == fonts_.end())
		load_font(font, size);
	else
		if(fonts_[font].characters.find(size)  == fonts_[font].characters.end())
			load_font(font, size);
	return fonts_[font].characters[size][character];
}

image::Bitmap FontToBitmap::text_to_bitmap(core::FontName font_name, const uint16_t & font_size, const std::string & text)
{	
	image::Bitmap bitmap;
	bitmap.format = image::ALPHA;

	{
		if (fonts_.find(font_name) == fonts_.end())
			load_font(font_name, font_size);
		else
			if (fonts_[font_name].characters.find(font_size) == fonts_[font_name].characters.end())
				load_font(font_name, font_size);
	}

	bitmap.size = glm::ivec2(0, font_size);
	for (auto c : text)
	{
		bitmap.size.x += fonts_[font_name].characters[font_size][(int)c].size.x;
	}

	size_t xpos = 0;
	bitmap.buffer = std::vector<uint8_t>(bitmap.size.x * bitmap.size.y, 0);
	for (auto c : text)
	{
		auto& ch = fonts_[font_name].characters[font_size][(int)c];
		for (auto h = 0u; h < ch.size.y; ++h)
		{
			for (auto w = 0u; w < ch.size.x; ++w)
			{
				bitmap.buffer[(h * bitmap.size.x) + (xpos + w)] = ch.buffer[(h * ch.size.x) + w];
			}
		}
		xpos += ch.size.x;
	}
	
	return bitmap;
}

image::Bitmap FontToBitmap::text_to_bitmap(core::FontName font_name, const uint16_t & font_size, const std::string & text, uint16_t max_length)
{
	image::Bitmap bitmap;
	bitmap.format = image::ALPHA;
	{
		if (fonts_.find(font_name) == fonts_.end())
			load_font(font_name, font_size);
		else
			if (fonts_[font_name].characters.find(font_size) == fonts_[font_name].characters.end())
				load_font(font_name, font_size);
	}

	bitmap.size = glm::ivec2(max_length, font_size);

	size_t xpos = 0;
	bitmap.buffer = std::vector<uint8_t>(bitmap.size.x * bitmap.size.y, 0);
	for (auto c : text)
	{
		auto& ch = fonts_[font_name].characters[font_size][(int)c];
		for (auto h = 0u; h < ch.size.y; ++h)
		{
			for (auto w = 0u; w < ch.size.x; ++w)
			{
				if (xpos + w >= max_length)
				{
					h = ch.size.y;
					break;
				}
				bitmap.buffer[(h * bitmap.size.x) + (xpos + w)] = ch.buffer[(h * ch.size.x) + w];
			}
		}
		xpos += ch.size.x;
	}

	return bitmap;
}

void FontToBitmap::load_font(core::FontName font, const uint16_t& size)
{
	fonts_[font].face = nullptr;
    std::string font_str = core::FontNameStrings[font];
	std::string font_name = font_str;
	std::transform(font_str.begin(), font_str.end(), font_name.begin(), ::tolower);
	font_name.append(".ttf");
	ASSERT_IF_TRUE(FT_New_Face(freetype_lib_, resources.get_path(font_name).string().c_str(), 0, &fonts_[font].face) > 0,
		std::string("'FT_New_Face()' failed to load font '" + font_name + "'."));

	auto& face = fonts_[font].face;
	//FT_Set_Char_Size(face, 0, size, 117, 117);
	FT_Set_Pixel_Sizes(face, 0, size);

	ASSERT_IF_TRUE(FT_Load_Char(face, 'g', FT_LOAD_RENDER) > 0, "'FT_Load_Char()' failed to load Glyph for character 'g'.");
	auto valid_height = size - (face->glyph->bitmap.rows - face->glyph->bitmap_top);
	
	for (auto c = 0; c < 128; ++c)
	{
		ASSERT_IF_TRUE(FT_Load_Char(face, c, FT_LOAD_RENDER) > 0, "'FT_Load_Char()' failed to load Glyph for character '" + std::string(1, c) + "'.");
		glm::ivec2 bitmap_size(face->glyph->bitmap_left + face->glyph->bitmap.width, size);
		if (bitmap_size.x < (face->glyph->advance.x >> 6))
			bitmap_size.x = (face->glyph->advance.x >> 6);
		std::vector<uint8_t> bitmap_data(bitmap_size.x * bitmap_size.y, 0);

		auto& bitmap = face->glyph->bitmap;
		size_t xpos = 0;
		size_t ypos = (valid_height - face->glyph->bitmap_top);

		if (face->glyph->bitmap_left >= 0)
			xpos = face->glyph->bitmap_left;
		
		for (auto h = 0u; h < bitmap.rows; ++h)
		{
			auto y_ratio = ((ypos + h) * bitmap_size.x);
			for (auto w = 0u; w < bitmap.width && y_ratio + (xpos + w) < bitmap_data.size(); ++w)
			{
				bitmap_data[y_ratio + (xpos + w)] = bitmap.buffer[(h * bitmap.width) + w];
			}
		}

		fonts_[font].characters[size][c] = CharacterData(bitmap_data, bitmap_size);
	}
}
