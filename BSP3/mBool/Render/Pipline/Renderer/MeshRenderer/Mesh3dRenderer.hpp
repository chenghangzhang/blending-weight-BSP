#pragma once


	class CompositeRenderer;

	template <class MESH3D>
	class Mesh3dRenderer : public CompositeRenderer
	{
	protected:
		VerticesRenderer<MESH3D>& m_VertexRenderer;
		EdgesRenderer<MESH3D>& m_EdgeRenderer;
		TrianglesRenderer<MESH3D>& m_TriangleRenderer;

	public:
		Mesh3dRenderer(const MESH3D& p_Mesh);
		virtual ~Mesh3dRenderer();

	public:
		bool SetVertexSize(float p_VertexSize);

		void SetVertexColor(const glm::vec3& p_Color);
		void SetEdgeColor(const glm::vec3& p_Color);
		void SetTriangleColor(const glm::vec3& p_Color);

		void SetVertexIsRender(bool p_IsRender);
		void SetEdgeIsRender(bool p_IsRender);
		void SetTriangleIsRender(bool p_IsRender);
	};

	template <class MESH3D>
	Mesh3dRenderer<MESH3D>::Mesh3dRenderer(const MESH3D& p_Mesh) :
		m_VertexRenderer(*(new VerticesRenderer(p_Mesh))),
		m_EdgeRenderer(*(new EdgesRenderer(p_Mesh))),
		m_TriangleRenderer(*(new TrianglesRenderer(p_Mesh)))
	{
		this->AddRenderUnit(&m_VertexRenderer);
		this->AddRenderUnit(&m_EdgeRenderer);
		this->AddRenderUnit(&m_TriangleRenderer);
	}

	template <class MESH3D>
	Mesh3dRenderer<MESH3D>::~Mesh3dRenderer()
	{

	}

	template <class MESH3D>
	bool Mesh3dRenderer<MESH3D>::SetVertexSize(float p_VertexSize)
	{
		return m_VertexRenderer.SetVertexSize(p_VertexSize);
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetVertexColor(const glm::vec3& p_Color)
	{
		m_VertexRenderer.SetVertexColor(p_Color);
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetVertexIsRender(bool p_IsRender)
	{
		m_VertexRenderer.SetIsRender(p_IsRender);
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetEdgeColor(const glm::vec3& p_Color)
	{
		m_EdgeRenderer.SetEdgeColor(p_Color);
		m_EdgeRenderer.SetEdgeWidth(5);
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetEdgeIsRender(bool p_IsRender)
	{
		m_EdgeRenderer.SetIsRender(p_IsRender);
		
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetTriangleColor(const glm::vec3& p_Color)
	{
		m_TriangleRenderer.SetTriangleColor(p_Color);
	}

	template <class MESH3D>
	void Mesh3dRenderer<MESH3D>::SetTriangleIsRender(bool p_IsRender)
	{
		m_TriangleRenderer.SetIsRender(p_IsRender);
	}
