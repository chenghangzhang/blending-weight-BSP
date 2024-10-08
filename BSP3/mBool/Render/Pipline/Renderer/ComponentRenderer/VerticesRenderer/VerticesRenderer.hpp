#pragma once


	template <class VERTEX_SET>
	class VerticesRenderer : public SingleRenderer<VERTEX_SET>
	{
	protected:
		float m_VertexSize;

	public:
		VerticesRenderer(const VERTEX_SET& p_VertexSet);
		virtual ~VerticesRenderer();

	protected:
		void Render() const override;
		bool BindVertexData() override;
		bool BindElementData() override;

	public:
		float GetVertexSize() const { return m_VertexSize; }

		bool SetVertexSize(float p_VertexSize);
		void SetVertexColor(const glm::vec3& p_Color);
	};

	template <class VERTEX_SET>
	VerticesRenderer<VERTEX_SET>::VerticesRenderer(const VERTEX_SET& p_VertexSet) :
		SingleRenderer<VERTEX_SET>(p_VertexSet, new SolidShader()),
		m_VertexSize(1.0f)
	{

	}

	template <class VERTEX_SET>
	VerticesRenderer<VERTEX_SET>::~VerticesRenderer()
	{

	}

	template <class VERTEX_SET>
	void VerticesRenderer<VERTEX_SET>::Render() const
	{
		glPointSize(m_VertexSize);
		glDrawElements(GL_POINTS, this->m_EboSize, GL_UNSIGNED_INT, 0);
	}

	template <class VERTEX_SET>
	bool VerticesRenderer<VERTEX_SET>::BindVertexData()
	{
		return DataBinder::BindVerticesVertexData<VERTEX_SET>(this->m_Data, this->m_Vbo);
	}

	template <class VERTEX_SET>
	bool VerticesRenderer<VERTEX_SET>::BindElementData()
	{
		return DataBinder::BindVerticesElementData<VERTEX_SET>(this->m_EboSize, this->m_Data, this->m_Ebo);
	}

	template <class VERTEX_SET>
	bool VerticesRenderer<VERTEX_SET>::SetVertexSize(float p_VertexSize)
	{
		if (p_VertexSize < 1.0f || p_VertexSize > 20.0f)
		{
			return false;
		}

		m_VertexSize = p_VertexSize;
		return true;
	}

	template <class VERTEX_SET>
	void VerticesRenderer<VERTEX_SET>::SetVertexColor(const glm::vec3& p_Color)
	{
		SingleRenderer<VERTEX_SET>::m_pShader->SetColor(p_Color);
	}
