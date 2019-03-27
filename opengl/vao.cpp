#include "../rendering/gl_utility.hpp"
#include "vao.h"

namespace gl
{

	VAO::VAO() : vao_id_(0u), ebo_id_(0u)
	{
		GL_CHECK(glGenVertexArrays(1, &vao_id_));
		ASSERT_IF_TRUE(vao_id_ == 0, "glGenVertexArrays() failed to generate vertex array object id !!!");
	}

	VAO::VAO(geo::Mesh const & mesh) : vao_id_(0u), ebo_id_(0u)
	{
		GL_CHECK(glGenVertexArrays(1, &vao_id_));
		ASSERT_IF_TRUE(vao_id_ == 0, "glGenVertexArrays() failed to generate vertex array object id !!!");
		bind();
		auto i = 0u;
		for (auto& attrib : mesh.attributes)
		{
            ASSERT_IF_FALSE(vbo_id_map_.find(attrib.first) == vbo_id_map_.end(), "An attribute with name \"" + AttributeStrings[attrib.first] + "\" already exist.");
			uint32_t vbo(0u);
			GL_CHECK(glGenBuffers(1, &vbo));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, attrib.second.byte_size, (const void*)attrib.second.buffer.data(), GL_STATIC_DRAW));
			GL_CHECK(glVertexAttribPointer(i, attrib.second.element_size, GL_FLOAT, GL_FALSE, 0, nullptr));
			GL_CHECK(glEnableVertexAttribArray(i));
			if (attrib.second.is_instance)
				GL_CHECK(glVertexAttribDivisor(i, 1));
			vbo_id_map_[attrib.first] = vbo;
			i++;
		}

		GL_CHECK(glGenBuffers(1, &ebo_id_));
		ASSERT_IF_TRUE(ebo_id_ == 0, "glGenBuffers() failed to generate buffer id for index buffer.");
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(mesh.indices[0]), (const void*)mesh.indices.data(), GL_STATIC_DRAW));
		index_size_ = mesh.indices.size();
		unbind();
	}

	VAO::~VAO()
	{
		if (vao_id_ != 0)
			glDeleteVertexArrays(1, &vao_id_);
		for (auto& vbo : vbo_id_map_)
			if (vbo.second != 0)
				glDeleteBuffers(1, &vbo.second);

		if (ebo_id_ != 0)
			glDeleteBuffers(1, &ebo_id_);
	}

	void VAO::add_attribute(gl::Attribute attrib_type, const void* buffer_data, const std::size_t& buffer_size, const uint8_t& attrib_size, const uint32_t& shader_location_id, bool is_instance)
	{
        ASSERT_IF_FALSE(vbo_id_map_.find(attrib_type) == vbo_id_map_.end(), "An attribute with name \"" + AttributeStrings[attrib_type] + "\" already exist.");
		uint32_t vbo(0u);
		GL_CHECK(glGenBuffers(1, &vbo));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW));
		GL_CHECK(glVertexAttribPointer(shader_location_id, attrib_size, GL_FLOAT, GL_FALSE, 0, nullptr));
		GL_CHECK(glEnableVertexAttribArray(shader_location_id));
		if (is_instance)
			GL_CHECK(glVertexAttribDivisor(shader_location_id, 1));
		vbo_id_map_[attrib_type] = vbo;
	}

	void VAO::set_attributes(geo::Mesh const& mesh)
	{
		auto i = 0u;
		for (auto& attrib : mesh.attributes)
		{
            ASSERT_IF_FALSE(vbo_id_map_.find(attrib.first) == vbo_id_map_.end(), "An attribute with name \"" + AttributeStrings[attrib.first] + "\" already exist.");
			uint32_t vbo(0u);
			GL_CHECK(glGenBuffers(1, &vbo));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, attrib.second.byte_size, (const void*)attrib.second.buffer.data(), GL_STATIC_DRAW));
			GL_CHECK(glVertexAttribPointer(i, attrib.second.element_size, GL_FLOAT, GL_FALSE, 0, nullptr));
			GL_CHECK(glEnableVertexAttribArray(i));
			if (attrib.second.is_instance)
				GL_CHECK(glVertexAttribDivisor(i, 1));
			vbo_id_map_[attrib.first] = vbo;
			i++;
		}

		ASSERT_IF_TRUE(ebo_id_ != 0, "An index buffer already exist.");

		GL_CHECK(glGenBuffers(1, &ebo_id_));
		ASSERT_IF_TRUE(ebo_id_ == 0, "glGenBuffers() failed to generate buffer id for index buffer.");
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(mesh.indices[0]), (const void*)mesh.indices.data(), GL_STATIC_DRAW));
		index_size_ = mesh.indices.size();
	}

	void VAO::add_indices(const void* buffer_data, const std::size_t& buffer_size, std::size_t indexSize)
	{
		ASSERT_IF_TRUE(vbo_id_map_.empty(), "No attribute buffer exist to use index bufer for!!!");
		ASSERT_IF_TRUE(ebo_id_ != 0, "An index buffer already exist.");

		GL_CHECK(glGenBuffers(1, &ebo_id_));
		ASSERT_IF_TRUE(ebo_id_ == 0, "glGenBuffers() failed to generate buffer id for index buffer.");
		GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id_));
		GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_size, buffer_data, GL_STATIC_DRAW));
		index_size_ = indexSize;
	}

	std::uint32_t VAO::buffer_id(gl::Attribute attrib_type)
	{
		return vbo_id_map_[attrib_type];
	}

	void VAO::bind() const
	{
		glBindVertexArray(vao_id_);
	}

	void VAO::unbind() const
	{
		glBindVertexArray(0);
	}
}
