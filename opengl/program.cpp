#include "../rendering/gl_utility.hpp"
#include "program.h"

namespace gl
{
	Program::Program(ShaderPathList shaders_file_path, AttributeList attribute_list)
	{
		init(shaders_file_path);
		for (auto& attrib : attribute_list)
		{
			int location_id = 0;
            GL_CHECK(location_id = glGetAttribLocation(id_, AttributeStrings[attrib].c_str()));
            ASSERT_IF_TRUE(location_id < 0, "The attribute \"" + AttributeStrings[attrib] + "\" couldn't be found in program \"" + std::to_string(id_) + "\".");
			attribute_map_[attrib] = location_id;
		}
	}

	Program::Program(ShaderPathList shaders_file_path, AttributeList attribute_list, UniformList uniform_list)
		: id_(0u)
	{
		init(shaders_file_path);
		for (auto& attrib : attribute_list)
		{
			int location_id = 0;
            GL_CHECK(location_id = glGetAttribLocation(id_, AttributeStrings[attrib].c_str()));
            ASSERT_IF_TRUE(location_id < 0, "The attribute \"" + AttributeStrings[attrib] + "\" couldn't be found in program \"" + std::to_string(id_) + "\".");
			attribute_map_[attrib] = location_id;
		}

		for (auto& item : uniform_list)
		{
			int location_id = 0;
            GL_CHECK(location_id = glGetUniformLocation(id_, UniformStrings[item].c_str()));
            ASSERT_IF_TRUE(location_id < 0, "The uniform \"" + UniformStrings[item] + "\" couldn't be found in program \"" + std::to_string(id_) + "\".");
			uniform_map_[item] = location_id;
		}
	}

	Program::~Program()
	{
		if (id_ != 0)
			glDeleteProgram(id_);
	}

	const uint32_t& Program::id() const
	{
		return id_;
	}

	void Program::linkUniform(Uniform uniform)
	{
		int location_id = 0;
        GL_CHECK(location_id = glGetUniformLocation(id_, UniformStrings[uniform].c_str()));
        ASSERT_IF_TRUE(location_id < 0, "The uniform \"" + UniformStrings[uniform] + "\" couldn't be found in program \"" + std::to_string(id_) + "\".");
		uniform_map_[uniform] = location_id;
	}

	void Program::linkUniform(UniformList uniform_list)
	{
		for (auto& item : uniform_list)
		{
			int location_id = 0;
            GL_CHECK(location_id = glGetUniformLocation(id_, UniformStrings[item].c_str()));
            ASSERT_IF_TRUE(location_id < 0, "The uniform \"" + UniformStrings[item] + "\" couldn't be found in program \"" + std::to_string(id_) + "\".");
			uniform_map_[item] = location_id;
		}
	}

	const uint8_t& Program::attreibute_id(Attribute attribute)
	{
		return attribute_map_[attribute];
	}

	const uint8_t& Program::get_uniform_id(Uniform uniform)
	{
        ASSERT_IF_TRUE(uniform_map_.find(uniform) == uniform_map_.end(), "No uniform exist with name \"" + UniformStrings[uniform] + "\" in GLSL program.");
		return uniform_map_[uniform];
	}

	void Program::bind()
	{
		glUseProgram(this->id_);
	}

	void Program::unbind() 
	{
		glUseProgram(0);
	}

	void Program::setUniform(const Uniform& uniform, const int& value) 
	{
		glUniform1i(uniform_map_[uniform], value);
	}

	void Program::setUniform(const Uniform& uniform, const uint32_t& value) 
	{
		glUniform1ui(uniform_map_[uniform], value);
	}

	void Program::setUniform(const Uniform& uniform, const float& value) 
	{
		glUniform1f(uniform_map_[uniform], value);
	}

	void Program::setUniform(const Uniform& uniform, const glm::uvec2& vec)
	{
		glUniform2uiv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::uvec3& vec) 
	{
		glUniform3uiv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::uvec4& vec) 
	{
		glUniform4uiv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::ivec2& vec) 
	{
		glUniform2iv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::ivec3& vec) 
	{
		glUniform3iv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::ivec4& vec) 
	{
		glUniform4iv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::vec2& vec) 
	{
		glUniform2fv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::vec3& vec) 
	{
		glUniform3fv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::vec4& vec) 
	{
		glUniform4fv(uniform_map_[uniform], 1, glm::value_ptr(vec));
	}

	void Program::setUniform(const Uniform& uniform, const glm::mat3& mat) 
	{
		glUniformMatrix3fv(uniform_map_[uniform], 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Program::setUniform(const Uniform& uniform, const glm::mat4& mat) 
	{
		glUniformMatrix4fv(uniform_map_[uniform], 1, GL_FALSE, glm::value_ptr(mat));
	}
	void Program::init(ShaderPathList shaders_file_path)
	{
		std::vector<uint32_t> shaders_id;
		for (auto& sh : shaders_file_path)
		{
			shaders_id.push_back(create_shader(sh));
		}
		ASSERT_IF_TRUE(shaders_id.empty(), "The given shader list is empty.");

		GLint program_link_process_succeed = GL_TRUE;
		GLchar infoLog[512];

		GL_CHECK(id_ = glCreateProgram());
		ASSERT_IF_TRUE(id_ == 0, "OpenGL failed to generate program id.");

		for (auto& sh_id : shaders_id)
			GL_CHECK(glAttachShader(id_, sh_id));

		GL_CHECK(glLinkProgram(id_));

		glGetProgramiv(id_, GL_LINK_STATUS, &program_link_process_succeed);
		if (program_link_process_succeed == GL_FALSE) {
			glGetProgramInfoLog(id_, 512, nullptr, infoLog);
			ASSERT_IF_TRUE(program_link_process_succeed == GL_FALSE, infoLog);
		}

		for (auto& sh_id : shaders_id)
			GL_CHECK(glDeleteShader(sh_id));

	}
}
