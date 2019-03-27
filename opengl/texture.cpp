#include "../rendering/gl_utility.hpp"
#include "texture.h"

namespace gl
{
	int format_to_texture(image::Format format)
	{
		switch (format)
		{
		case image::ALPHA:
		case image::GRAYSCALE:
			return GL_RED;
		case image::GRAYSCALE_ALPHA:
			ASSERT("Unkown format 'GRAYSCALE_ALPHA'!!!!");
		case image::RGB:
			return GL_RGB;
		case image::BGR:
			return GL_BGR;
		case image::RGBA:
			return GL_RGBA;
		case image::BGRA:
			return GL_BGRA;
		default:
            ASSERT("Unkown format '" + image::FormatStrings[format] + "'.");
		}
	}

	int internal_texture_format(image::Format format)
	{
		switch (format)
		{
		case image::ALPHA:
		case image::GRAYSCALE:
			return GL_RED;
		case image::GRAYSCALE_ALPHA:
			ASSERT("Unkown format 'GRAYSCALE_ALPHA'!!!!");
		case image::RGB:
		case image::BGR:
			return GL_RGB;
		case image::RGBA:
			return GL_RGBA;
		case image::BGRA:
			return GL_BGRA;
		default:
            ASSERT("Unkown format '" + image::FormatStrings[format] + "'.");
		}
	}

	Texture::Texture(image::Format  format, const glm::ivec2 & size, TextureFilter texture_filter, TextureWrapping texture_wrapping) :
		id_(0u), format_(format), size_(size)
	{
		GL_CHECK(glGenTextures(1, &id_));
		ASSERT_IF_FALSE(id_, "glGenTextures() failed to generate texture id.");

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrapping);

			std::vector<uint8_t> pixels(size_.x * size_.y * format_size(format_), 0);
			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format_to_texture(format_), size_.x, size_.y, 0, internal_texture_format(format_), GL_UNSIGNED_BYTE, pixels.data()));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(image::Bitmap const & bitmap, TextureFilter texture_filter, TextureWrapping texture_wrapping) :
		id_(0u), format_(bitmap.format), size_(bitmap.size)
	{
		GL_CHECK(glGenTextures(1, &id_));
		ASSERT_IF_FALSE(id_, "glGenTextures() failed to generate texture id.");

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrapping);

			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format_to_texture(format_), size_.x, size_.y, 0, internal_texture_format(format_), GL_UNSIGNED_BYTE, bitmap.buffer.data()));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(image::Format  format, const glm::ivec2 & size, const std::vector<uint8_t>& pixels, TextureFilter texture_filter, TextureWrapping texture_wrapping) :
		id_(0u), format_(format), size_(size)
	{
		if (id_ != 0)
			glDeleteTextures(1, &id_);
		GL_CHECK(glGenTextures(1, &id_));
		ASSERT_IF_FALSE(id_, "glGenTextures() failed to generate texture id.");

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrapping);

			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format_to_texture(format_), size_.x, size_.y, 0, internal_texture_format(format_), GL_UNSIGNED_BYTE, pixels.data()));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(image::Format format, const glm::ivec2 & size, const uint8_t * data, TextureFilter texture_filter, TextureWrapping texture_wrapping) :
		id_(0u), format_(format), size_(size)
	{
		if (id_ != 0)
			glDeleteTextures(1, &id_);
		GL_CHECK(glGenTextures(1, &id_));
		ASSERT_IF_FALSE(id_, "glGenTextures() failed to generate texture id.");

		GL_CHECK(glBindTexture(GL_TEXTURE_2D, id_));
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrapping);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrapping);

			GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format_to_texture(format_), size_.x, size_.y, 0, internal_texture_format(format_), GL_UNSIGNED_BYTE, data));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{
		if (id_ != 0)
			glDeleteTextures(1, &id_);
	}

	const glm::ivec2 & Texture::size()
	{
		return size_;
	}

	void Texture::set_data(const std::vector<uint8_t>& data)
	{
		glBindTexture(GL_TEXTURE_2D, id_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, internal_texture_format(format_), GL_UNSIGNED_BYTE, data.data()));
	}

	void Texture::set_data(const glm::ivec4 & rect, const std::vector<uint8_t>& data)
	{
		glBindTexture(GL_TEXTURE_2D, id_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], rect[2], rect[3], internal_texture_format(format_), GL_UNSIGNED_BYTE, data.data()));
	}

	void Texture::set_data(const std::uint8_t* data)
	{
		glBindTexture(GL_TEXTURE_2D, id_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size_.x, size_.y, internal_texture_format(format_), GL_UNSIGNED_BYTE, data));
	}

	void Texture::set_data(const glm::ivec4 & rect, const std::uint8_t* data)
	{
		glBindTexture(GL_TEXTURE_2D, id_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, rect[0], rect[1], rect[2], rect[3], internal_texture_format(format_), GL_UNSIGNED_BYTE, data));
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id_);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	
}
