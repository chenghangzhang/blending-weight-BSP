#pragma once


	class MeshIntersectTerminalVertex : public MeshIntersectVertex
	{
	protected:
	public://调试需要，暂时这么写
		//const MeshEdge& m_TriangleEdge;
		const MeshHalfEdgeHandle m_HostEdge;
		const MeshTriangleHandle m_IntersetTriangle;
		double m_ParamOnHostEdge;

	public:
		MeshIntersectTerminalVertex(const Point3d* p_Point3d, const MeshHalfEdgeHandle& p_HostEdge, double p_HostEdgParam, const MeshTriangleHandle& m_IntersetTriangle);
		virtual ~MeshIntersectTerminalVertex() {}

	public:
		const MeshHalfEdgeHandle& GetHostEdge() const { return m_HostEdge; }

	public:
		bool IsTerminal() const override { return true; }
		bool IsOrigin() const override { return false; }

		bool IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const override;
		const MeshTriangleHandle* GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle = nullptr) const override;

		virtual Parameter2d GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const override;

		virtual const MeshVertexHandle* GetVertex() { return nullptr; }
	};

