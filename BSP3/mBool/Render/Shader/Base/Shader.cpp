#include "mPch.h"
#include "Shader.h"


	Shader::Shader(const char* p_VsTemplate, const char* p_FsTemplate)
	{
		m_Id = 0;

		m_pVs = new VertexShader(p_VsTemplate);
		m_pFs = new FragmentShader(p_FsTemplate);

		Initalize();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Id);

		if (nullptr != m_pVs)
		{
			delete m_pVs;
			m_pVs = nullptr;
		}

		if (nullptr != m_pFs)
		{
			delete m_pFs;
			m_pFs = nullptr;
		}
	}

	void Shader::Initalize()
	{
		unsigned int vsId = m_pVs->GetId();
		unsigned int fsId = m_pFs->GetId();

		m_Id = glCreateProgram();
		glAttachShader(m_Id, vsId);
		glAttachShader(m_Id, fsId);
		glLinkProgram(m_Id);

		int isSuccessed = 0;
		char log[512];

		glGetProgramiv(m_Id, GL_LINK_STATUS, &isSuccessed);
		if (!isSuccessed) {
			glGetProgramInfoLog(m_Id, 512, NULL, log);
		}

		glDeleteShader(vsId);
		glDeleteShader(fsId);
	}

	void Shader::UpdateRenderParas(const RenderParas& p_RenderParas)
	{
		UpdateCameraParas(p_RenderParas);
		UpdateModelParas(p_RenderParas);
		UpdateMaterialParas();


		//SetVec3("lightPosition", p_RenderParas.m_LightPosition);
		//SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	}

	void Shader::SetColor(const glm::vec3& p_Color)
	{
		m_Color = p_Color;
	}

	void Shader::UpdateCameraParas(const RenderParas& p_RenderParas)
	{
		SetMat4("projection", p_RenderParas.m_Projection);
		SetMat4("view", p_RenderParas.m_View);
	}

	void Shader::UpdateModelParas(const RenderParas& p_RenderParas)
	{
		SetMat4("model", p_RenderParas.m_Model);
	}

	void Shader::UpdateMaterialParas()
	{
		SetVec3("objectColor", m_Color);
	}

	void Shader::Use()
	{
		assert(0 != m_Id);
		glUseProgram(m_Id);
	}
