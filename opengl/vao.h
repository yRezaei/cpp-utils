#pragma once
#ifndef GL_VAO_H
#define GL_VAO_H

#include "tarja/core.hpp"
#include "gl_enums.h"
#include "../geometry/geometry.h"

namespace gl
{
	class VAO
	{
	private:
		uint32_t vao_id_;
		uint32_t ebo_id_;
		std::unordered_map<gl::Attribute, uint32_t> vbo_id_map_;
		std::size_t index_size_;

	public:
		VAO();
		VAO(geo::Mesh const& mesh);
		~VAO();
		void add_attribute(gl::Attribute attrib_type, const void* buffer_data, const std::size_t& buffer_stride, const uint8_t& attrib_size, const uint32_t& shader_location_id, bool is_instance);
		void set_attributes(geo::Mesh const& mesh);
		void add_indices(const void* buffer_data, const std::size_t& buffer_size, std::size_t indexSize);
		std::uint32_t buffer_id(gl::Attribute attrib_type);
		void bind() const;
		void unbind() const;
		const std::size_t& index_size() const { return index_size_; }
	};
}

#endif // !GL_VAO_H
