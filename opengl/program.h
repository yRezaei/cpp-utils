#pragma once
#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H

#include "tarja/core.hpp"
#include "gl_enums.h"
#include "../resources.h"

namespace gl
{
	using ShaderPathList = std::initializer_list<fs::path>;
	using AttributeList = std::initializer_list<Attribute>;
	using UniformList = std::initializer_list<Uniform>;

	class Program
	{
	private:
		uint32_t id_;
		std::unordered_map<Uniform, uint8_t> uniform_map_;
		std::unordered_map<Attribute, uint8_t> attribute_map_;

	public:
		Program(ShaderPathList shaders_file_path, AttributeList attribute_list);
		Program(ShaderPathList shaders_file_path, AttributeList attribute_list, UniformList uniform_list);
		~Program();
		const uint32_t& id() const;
		void linkUniform(Uniform uniform);
		void linkUniform(UniformList uniform_list);
		const uint8_t& attreibute_id(Attribute attribute);
		const uint8_t& get_uniform_id(Uniform uniform);
		void bind() ;
		void unbind() ;
		void setUniform(const Uniform& uniform, const int& value) ;
		void setUniform(const Uniform& uniform, const uint32_t& value) ;
		void setUniform(const Uniform& uniform, const float& value) ;
		void setUniform(const Uniform& uniform, const glm::uvec2& vec) ;
		void setUniform(const Uniform& uniform, const glm::uvec3& vec) ;
		void setUniform(const Uniform& uniform, const glm::uvec4& vec) ;
		void setUniform(const Uniform& uniform, const glm::ivec2& vec) ;
		void setUniform(const Uniform& uniform, const glm::ivec3& vec) ;
		void setUniform(const Uniform& uniform, const glm::ivec4& vec) ;
		void setUniform(const Uniform& uniform, const glm::vec2& vec) ;
		void setUniform(const Uniform& uniform, const glm::vec3& vec) ;
		void setUniform(const Uniform& uniform, const glm::vec4& vec) ;
		void setUniform(const Uniform& uniform, const glm::mat3& mat) ;
		void setUniform(const Uniform& uniform, const glm::mat4& mat) ;

	private:
		void init(ShaderPathList shaders_file_path);
	};
}

#endif //End of GLSL_PROGRAM_H
