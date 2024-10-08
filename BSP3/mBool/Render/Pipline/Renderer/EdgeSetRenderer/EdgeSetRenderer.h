#pragma once


	class CompositeRenderer;

	class EdgeSetRenderer : public CompositeRenderer
	{
	protected:
		VerticesRenderer<Edge_Set>& m_VertexRenderer;
		EdgesRenderer<Edge_Set>& m_EdgeRenderer;

	public:
		EdgeSetRenderer(const Edge_Set& p_EdgeSet);
		virtual ~EdgeSetRenderer();

	public:
		float GetVertexSize() const;

	public:
		bool SetVertexSize(float p_VertexSize);
		void SetVertexColor(const glm::vec3& p_Color);
		void SetEdgeColor(const glm::vec3& p_Color);
	};
