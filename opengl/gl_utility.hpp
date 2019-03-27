#pragma once
#ifndef GL_UTILITY_HPP
#define GL_UTILITY_HPP

//#define GLEW_STATIC
#include <GL/glew.h>

#include "tarja/core.hpp"
#include "tarja/mathematics.hpp"
#include "../resources.h"

namespace gl
{
	inline void initialize()
	{
		auto error_id = glewInit();
		ASSERT_IF_TRUE (error_id != GLEW_OK, "'glewInit()' " + std::string(reinterpret_cast<const char*>(glewGetErrorString(error_id))));
	}

	inline void enable(int opengl_enum)
	{
		glEnable(opengl_enum);
	}

	inline void disable(int opengl_enum)
	{
		glDisable(opengl_enum);
	}

	inline void viewport(const int& x, const int& y, const int& width, const int& height)
	{
		glViewport(x, y, width, height);
	}

	inline void viewport(const glm::ivec4& rect)
	{
		glViewport(rect.x, rect.y, rect.z, rect.a);
	}

	inline void clearColor(const float& r, const float& g, const float& b, const float& a)
	{
		glClearColor(r, g, b, a);
	}

	inline void clearColor(const glm::ivec4& color)
	{
		glViewport(color.r, color.g, color.b, color.a);
	}

	inline void clear(int opengl_enum)
	{
		glClear(opengl_enum);
	}

	inline void CheckOpenGLError(const char* stmt,  const char* fname, int line)
	{
		switch (glGetError())
		{
		case GL_NO_ERROR:
			return;

		case GL_INVALID_ENUM:
			std::cout << "OpenGL error #" << GL_INVALID_ENUM << " [GL_INVALID_ENUM] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;

		case GL_INVALID_VALUE:	
			std::cout << "OpenGL error #" << GL_INVALID_VALUE << " [GL_INVALID_VALUE] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "OpenGL error #" << GL_INVALID_OPERATION << " [GL_INVALID_OPERATION] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;

		case GL_STACK_OVERFLOW:
			std::cout << "OpenGL error #" << GL_STACK_OVERFLOW << " [GL_STACK_OVERFLOW] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;

		case GL_STACK_UNDERFLOW:
			std::cout << "OpenGL error #" << GL_STACK_UNDERFLOW << " [GL_STACK_UNDERFLOW] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;

		case GL_OUT_OF_MEMORY:
			std::cout << "OpenGL error #" << GL_OUT_OF_MEMORY << " [GL_OUT_OF_MEMORY] in file '" << fname << "' at line #" << line << " in function " << stmt << std::endl;
			break;
		}
		std::exit(1);
	}
}

#define GL_CHECK(stmt) do { \
	stmt; \
	gl::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
} while (0)

namespace gl
{
	enum ShaderType
	{
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		COMPUTE = GL_COMPUTE_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER
	};

	inline uint16_t create_shader(ShaderType shader_type, const std::string& shader_source)
	{
		GLint shader_compilation_succeed = GL_TRUE;
		uint16_t shader_id;
		switch (shader_type)
		{
		case ShaderType::FRAGMENT:
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::VERTEX:
			shader_id = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::GEOMETRY:
			shader_id = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderType::COMPUTE:
			shader_id = glCreateShader(GL_COMPUTE_SHADER);
			break;
		default:
			shader_id = 0;
		}

		ASSERT_IF_TRUE(shader_id == 0, "glCreateShader() failed. Returned id is \"0\".");

		auto source = static_cast<const GLchar*>(shader_source.c_str());
		GLchar infoLog[512];
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compilation_succeed);
		if (shader_compilation_succeed == GL_FALSE)
		{
			glGetShaderInfoLog(shader_id, 512, nullptr, &infoLog[0]);
			CHECK_IF_FALSE(shader_compilation_succeed, infoLog);
		}

		return shader_id;
	}

	inline uint16_t create_shader(fs::path shader_file_path)
	{
		uint16_t shader_id = 0;
		GLint shader_compile_process_succeed = GL_TRUE;
		auto file_extension = shader_file_path.extension().string();

		if (file_extension == ".frag")
			shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		else if (file_extension == ".vert")
			shader_id = glCreateShader(GL_VERTEX_SHADER);
		else if (file_extension == ".geom")
			shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		else if (file_extension == ".comp")
			shader_id = glCreateShader(GL_COMPUTE_SHADER);

		ASSERT_IF_FALSE(shader_id, "'glCreateShader()' failed to create shader ID!");

		std::ifstream source_stream(shader_file_path.string().c_str());
		ASSERT_IF_FALSE(source_stream.good(), "The shader \"" + shader_file_path.string() + "\" source is empty!");

		std::string shader_source((std::istreambuf_iterator<char>(source_stream)), std::istreambuf_iterator<char>());
		ASSERT_IF_TRUE(shader_source.empty(), "The shader file is empty!");

		auto source = static_cast<const GLchar*>(shader_source.c_str());
		GLchar infoLog[512];
		glShaderSource(shader_id, 1, &source, nullptr);
		glCompileShader(shader_id);

		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compile_process_succeed);
		if (shader_compile_process_succeed == GL_FALSE)
		{
			glGetShaderInfoLog(shader_id, 512, nullptr, &infoLog[0]);
			CHECK_IF_FALSE(shader_compile_process_succeed, infoLog);
		}
		return shader_id;
	}
}

#endif // !GL_UTILITY_HPP

