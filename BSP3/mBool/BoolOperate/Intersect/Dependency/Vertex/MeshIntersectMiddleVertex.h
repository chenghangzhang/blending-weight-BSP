#pragma once


	class MeshIntersectMiddleVertex : public MeshIntersectVertex
	{
	protected:
	public://调试需要，暂时这么写
		//const MeshEdge& m_TriangleEdge;
		const MeshTriangleHandle m_HostTriangle;
		const MeshHalfEdgeHandle m_IntersetEdge;
		Parameter2d m_ParamInTiangle;
	public:
		MeshIntersectMiddleVertex(const Point3d* p_Point3d, const MeshHalfEdgeHandle& p_IntersetEdge, const MeshTriangleHandle& p_HostTriangle, const Parameter2d& p_ParamInTiangle);
		virtual ~MeshIntersectMiddleVertex() {}

	public:
		const MeshHalfEdgeHandle& GetIntersectEdge() const { return m_IntersetEdge; }
		const MeshTriangleHandle& GetHostTriangle() const { return m_HostTriangle; }

	public:
		bool IsTerminal() const override { return false; }
		bool IsOrigin() const override { return false; }

		bool IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const override;
		const MeshTriangleHandle* GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle = nullptr) const override;

		virtual Parameter2d GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const;

		virtual const MeshVertexHandle* GetVertex() { return nullptr; }

	};


