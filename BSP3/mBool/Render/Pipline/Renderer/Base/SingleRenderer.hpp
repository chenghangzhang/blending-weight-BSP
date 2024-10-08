#pragma once


	//父类
	class UnitRenderer;
	//成员
	class Shader;
	//class DiscreteBase;
	class RenderParas;
	class DataBind;

	template<class Type>
	class SingleRenderer : public UnitRenderer
	{
	protected:
		Shader* m_pShader;

		const Type& m_Data;

		// 顶点数组对象：Vertex Array Object，VAO
		unsigned int m_Vao;

		//顶点缓冲对象：Vertex Buffer Object，VBO
		unsigned int m_Vbo;

		//索引缓冲对象：Element Buffer Object，EBO 或 Index Buffer Object，IBO
		unsigned int m_Ebo;
		unsigned int m_EboSize;

	public:
		glm::vec3 m_Color; //暂时放在这里，觉得有些不合适

	public:
		SingleRenderer(const Type& p_Data, Shader* p_pShader);
		virtual ~SingleRenderer() = 0;

	protected:
		virtual bool BindVertexData();
		virtual bool BindElementData();

		virtual void Render() const;
	public:
		bool Initialize() override;
		void Update() override;

		void Render(const RenderParas& p_RenderParas) const override;
	};

	template<class Type>
	SingleRenderer<Type>::SingleRenderer(const Type& p_Data, Shader* p_pShader)
		:m_Data(p_Data)
	{
		if (nullptr == p_pShader)
		{
			assert(false);
			throw;
		}

		m_pShader = p_pShader;

		m_Vao = m_Vbo = m_Ebo = 0;

		m_IsRender = true;
	}

	template<class Type>
	SingleRenderer<Type>::~SingleRenderer()
	{
		assert(nullptr != m_pShader);
		delete m_pShader;
	}

	template<class Type>
	bool SingleRenderer<Type>::BindVertexData()
	{
		assert(false);
		return false;

		//	return DataBind::BindVertexData(m_Data, m_Vbo);
	}

	template<class Type>
	bool SingleRenderer<Type>::BindElementData()
	{
		assert(false);
		return false;

		//return DataBind::BindElementData(m_Data, m_Ebo, m_EboSize);
	}

	template<class Type>
	void SingleRenderer<Type>::Render() const
	{
		assert(false);
	}

	template<class Type>
	bool SingleRenderer<Type>::Initialize()
	{
		if (0 != m_Vao || 0 != m_Vbo || 0 != m_Ebo)
		{
			assert(false);
			throw std::invalid_argument("");
		}

		try
		{
			glGenVertexArrays(1, &m_Vao);

			glGenBuffers(1, &m_Vbo);
			glGenBuffers(1, &m_Ebo);
		}
		catch (...)
		{
			assert(false);
			throw std::invalid_argument("");
		}

		assert(0 != m_Vao);
		assert(0 != m_Vbo);
		assert(0 != m_Ebo);

		return true;
	}

	template<class Type>
	void SingleRenderer<Type>::Update()
	{
		assert(0 != m_Vao);
		assert(0 != m_Vbo);
		assert(0 != m_Ebo);

		bool bindResult = false;

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(m_Vao);
		{
			try
			{
				bindResult = BindVertexData(); assert(true == bindResult);
				bindResult = BindElementData(); assert(true == bindResult);
			}
			catch (...)
			{
				assert(false);
				throw std::invalid_argument("");
			}
		}
	}

	template<class Type>
	void SingleRenderer<Type>::Render(const RenderParas& p_RenderParas) const
	{
		assert(nullptr != m_pShader);

		if (false == m_IsRender)
		{
			return;
		}

		m_pShader->Use();

		m_pShader->UpdateRenderParas(p_RenderParas);

		assert(0 != m_Vao);
		assert(0 != m_Vbo);
		assert(0 != m_Ebo);

		glBindVertexArray(m_Vao);

		this->Render();
	}
