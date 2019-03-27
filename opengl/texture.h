#pragma once
#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "tarja/core.hpp"
#include "tarja/mathematics.hpp"
#include "tarja/image.hpp"

namespace gl
{
	enum TextureFilter
	{
		FILTER_NEAREST				= 0x2600,
		FILTER_LINEAR				= 0x2601
	};

	enum TextureWrapping
	{
		WRAPPING_REPEAT				= 0x2901,
		WRAPPING_MIRRORED_REPEAT	= 0x8370,
		WRAPPING_CLAMP_TO_EDGE		= 0x812F,
		WRAPPING_CLAMP_TO_BORDER	= 0x812D
	};

	class Texture
	{
	private:
		uint32_t id_;
	protected:
		image::Format format_;
		glm::ivec2 size_;
		std::vector<uint8_t> data_;

	public:
		Texture(image::Format format, const glm::ivec2& size, TextureFilter texture_filter = FILTER_LINEAR,
			TextureWrapping texture_wrapping = WRAPPING_CLAMP_TO_BORDER);
		Texture(image::Bitmap const& bitmap, TextureFilter texture_filter = FILTER_LINEAR,
			TextureWrapping texture_wrapping = WRAPPING_CLAMP_TO_BORDER);
		Texture(image::Format format, const glm::ivec2& size, const std::vector<uint8_t>& data, TextureFilter texture_filter = FILTER_LINEAR,
			TextureWrapping texture_wrapping = WRAPPING_CLAMP_TO_BORDER);
		Texture(image::Format format, const glm::ivec2& size, const uint8_t* data, TextureFilter texture_filter = FILTER_LINEAR,
			TextureWrapping texture_wrapping = WRAPPING_CLAMP_TO_BORDER);
		virtual ~Texture();
		const glm::ivec2& size();
		void set_data(const std::vector<uint8_t>& data);
		void set_data(const glm::ivec4& rect, const std::vector<uint8_t>& data);
		void set_data(const std::uint8_t* data);
		void set_data(const glm::ivec4& rect, const std::uint8_t* data);

		void bind();
		void unbind();
	};
}

#endif // !ABSTRACT_TEXTURE_H

