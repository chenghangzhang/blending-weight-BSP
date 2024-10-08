#pragma once



	class VertexShader;
	class FragmentShader;

	class RenderParas;

	class Shader
	{
	protected:
		// 程序ID
		unsigned int m_Id;

		VertexShader* m_pVs;
		FragmentShader* m_pFs;

	protected:
		glm::vec3 m_Color;

	public:
		// 构造器读取并构建着色器
		Shader(const char* p_VsTemplate, const char* p_FsTemplate);
		virtual ~Shader() = 0;

	protected:
		void Initalize();

		void SetVec3(const string& name, const glm::vec3& value) const
		{
			assert(0 != m_Id);
			glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
		}

		void SetVec3(const string& name, float x, float y, float z) const
		{
			assert(0 != m_Id);
			glUniform3f(glGetUniformLocation(m_Id, name.c_str()), x, y, z);
		}

		void SetVec4(const string& name, const glm::vec4& value) const
		{
			assert(0 != m_Id);
			glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
		}

		void SetMat4(const string& name, glm::mat4 value) const
		{
			assert(0 != m_Id);
			glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
		}

		virtual void UpdateCameraParas(const RenderParas& p_RenderParas);
		virtual void UpdateModelParas(const RenderParas& p_RenderParas);
		virtual void UpdateMaterialParas();

	public:
		// 使用/激活程序
		void Use();

		virtual void UpdateRenderParas(const RenderParas& p_RenderParas);

		void SetColor(const glm::vec3& p_Color);

	private:
		Shader(const Shader&) : m_Color(1.0f, 1.0f, 1.0f)
		{
			assert(false);
			m_Id = 0;
			m_pFs = nullptr;
			m_pVs = nullptr;
		}
	};


