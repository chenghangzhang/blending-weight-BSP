#pragma once


	template <class TYPE>
	class EdgesRenderer : public SingleRenderer<TYPE>
	{
	protected:
		float m_EdgeWidth;

	public:
		EdgesRenderer(const TYPE& p_EdgeSet);
		virtual ~EdgesRenderer();

	protected:
		void Render() const override;
		bool BindVertexData() override;
		bool BindElementData() override;

	public:
		bool SetEdgeWidth(float p_Width);
		void SetEdgeColor(const glm::vec3& p_Color);
	};

	template <class TYPE>
	EdgesRenderer<TYPE>::EdgesRenderer(const TYPE& p_EdgeSet) :
		SingleRenderer<TYPE>(p_EdgeSet, new SolidShader()),
		m_EdgeWidth(3.0f)
	{

	}

	template <class TYPE>
	EdgesRenderer<TYPE>::~EdgesRenderer()
	{

	}

	template <class TYPE>
	void EdgesRenderer<TYPE>::Render() const
	{
		glLineWidth(m_EdgeWidth);
		glDrawElements(GL_LINES, this->m_EboSize, GL_UNSIGNED_INT, 0);
	}

	template <class TYPE>
	bool EdgesRenderer<TYPE>::BindVertexData()
	{
		return DataBinder::BindEdgesVertexData<TYPE>(this->m_Data, this->m_Vbo);
	}

	template <class TYPE>
	bool EdgesRenderer<TYPE>::BindElementData()
	{
		return DataBinder::BindEdgesElementData<TYPE>(this->m_EboSize, this->m_Data, this->m_Ebo);
	}

	template <class TYPE>
	bool EdgesRenderer<TYPE>::SetEdgeWidth(float p_Width)
	{
		//if (p_Width < 1.0f || p_Width > 20.0f)
		//{
		//	return false;
		//}

		m_EdgeWidth = p_Width;
		return true;
	}

	template <class TYPE>
	void EdgesRenderer<TYPE>::SetEdgeColor(const glm::vec3& p_Color)
	{
		this->m_pShader->SetColor(p_Color);
	}
