#pragma once


	template <class TRIANGLES>
	class TrianglesRenderer : public SingleRenderer<TRIANGLES>
	{
	public:
		TrianglesRenderer(const TRIANGLES& p_Mesh);
		virtual ~TrianglesRenderer() {}

	protected:
		virtual void Render() const override;
		bool BindVertexData() override;
		bool BindElementData() override;

	public:
		void SetTriangleColor(const glm::vec3& p_Color)
		{
			this->m_pShader->SetColor(p_Color);
		}
	};

	template <class TRIANGLES>
	TrianglesRenderer<TRIANGLES>::TrianglesRenderer(const TRIANGLES& p_Mesh) :
		SingleRenderer<TRIANGLES>(p_Mesh, new LitShader())
	{

	}

	template <class TRIANGLES>
	bool TrianglesRenderer<TRIANGLES>::BindVertexData()
	{
		return DataBinder::BindTrianglesVertexData<TRIANGLES>(this->m_Data, this->m_Vbo);
	}

	template <class TRIANGLES>
	bool TrianglesRenderer<TRIANGLES>::BindElementData()
	{
		return DataBinder::BindTrianglesElementData<TRIANGLES>(this->m_EboSize, this->m_Data, this->m_Ebo);
	}

	template <class TRIANGLES>
	void TrianglesRenderer<TRIANGLES>::Render() const
	{
		glDrawElements(GL_TRIANGLES, this->m_EboSize, GL_UNSIGNED_INT, 0);
	}
