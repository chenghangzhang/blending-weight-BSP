#include "mPch.h"
#include "EdgeSetRenderer.h"



	EdgeSetRenderer::EdgeSetRenderer(const Edge_Set& p_EdgeSet) :
		m_VertexRenderer(*(new VerticesRenderer<Edge_Set>(p_EdgeSet))),
		m_EdgeRenderer(*(new EdgesRenderer<Edge_Set>(p_EdgeSet)))
	{
		this->AddRenderUnit(&m_VertexRenderer);
		this->AddRenderUnit(&m_EdgeRenderer);
	}

	EdgeSetRenderer::~EdgeSetRenderer()
	{

	}

	float EdgeSetRenderer::GetVertexSize() const
	{
		return m_VertexRenderer.GetVertexSize();
	}

	bool EdgeSetRenderer::SetVertexSize(float p_VertexSize)
	{
		return m_VertexRenderer.SetVertexSize(p_VertexSize);
	}

	void EdgeSetRenderer::SetVertexColor(const glm::vec3& p_Color)
	{
		m_VertexRenderer.SetVertexColor(p_Color);
	}

	void EdgeSetRenderer::SetEdgeColor(const glm::vec3& p_Color)
	{
		m_EdgeRenderer.SetEdgeColor(p_Color);
	}

