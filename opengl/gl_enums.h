#pragma once
#include "tarja/core/enum_utility.hpp"

namespace gl
{
	DECLARE_ENUM
	(
		Attribute,
		VERTICES, 
		NORMALS, 
		TEXTURE_COORD, 
		COLORS, POSITION_INSTANCE, 
		TRANSFORM_INSTANCE, 
		COLOR_INSTANCE
        );

	DECLARE_ENUM
	(
		Uniform, 
		VIEW, 
		PROJECTION, 
		MODEL, 
		MODEL_VIEW, 
		VIEW_PROJECTION, 
		MODEL_VIEW_PROJECTION, 
		SCALE, 
		COLOR,
		RECT_SIZE,
		RADIUS, 
		OBJECT_ID, 
		DRAW_ID, 
		TEXTURE_0, 
		TEXTURE_1, 
		TEXTURE_2, 
		TEXTURE_3
        );
}


namespace core
{
	DECLARE_ENUM
	(
		FontName,
		ARIAL, 
		VERA, 
		TIMES,
		COUR,
		COURIER_PRIME_SANS
        );
}

