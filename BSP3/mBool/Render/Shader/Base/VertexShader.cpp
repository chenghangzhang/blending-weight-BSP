#include "mPch.h"
#include "VertexShader.h"



	VertexShader::VertexShader(const char* p_Template)
	{
		if (nullptr == p_Template)
		{
			assert(false);
			throw;
		}

		m_Id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_Id, 1, &p_Template, NULL);
		glCompileShader(m_Id);

		int isSuccessed = 0;
		char log[512];

		glGetShaderiv(m_Id, GL_COMPILE_STATUS, &isSuccessed);
		if (!isSuccessed)
		{
			glGetShaderInfoLog(m_Id, 512, NULL, log);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
			assert(false);
			throw;
		}
	}

	VertexShader::~VertexShader()
	{

	}
