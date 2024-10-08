#include "mPch.h"
#include "FragmentShader.h"


	FragmentShader::FragmentShader(const char* p_Template)
	{
		if (nullptr == p_Template)
		{
			assert(false);
			throw;
		}

		// fragment shader
		m_Id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_Id, 1, &p_Template, NULL);
		glCompileShader(m_Id);

		int isSuccessed = 0;
		char log[512];

		// check for shader compile errors
		glGetShaderiv(m_Id, GL_COMPILE_STATUS, &isSuccessed);
		if (!isSuccessed)
		{
			glGetShaderInfoLog(m_Id, 512, NULL, log);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
			assert(false);
			throw;
		}
	}

	FragmentShader::~FragmentShader()
	{

	}
