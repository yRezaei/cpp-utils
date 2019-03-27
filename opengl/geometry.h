#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "tarja/core.hpp"
#include "tarja/mathematics.hpp"
#include "gl_enums.h"

namespace gl
{
	using GlBufferData = std::map<gl::Attribute, std::vector<float>>;
	using GlIndexData = std::vector<size_t>;
}
namespace geo
{
	inline std::tuple<gl::GlBufferData, gl::GlIndexData> rect_2D()
	{
		return std::make_tuple(gl::GlBufferData{ { gl::VERTICES, std::vector<float>{ -0.5f, 0.5f , 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f }},
		{ gl::TEXTURE_COORD, std::vector<float>{ 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f }} }, gl::GlIndexData{ 0, 1, 2, 2, 1, 3 });
	}
}

#endif // !GEOMETRY_H

