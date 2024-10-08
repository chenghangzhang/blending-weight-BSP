#pragma once



	class MeshIntersectOriginalVertex : public MeshIntersectVertex
	{

	protected:
		//const MeshEdge& m_TriangleEdge;
		const MeshVertexHandle m_OriginalVertex;

	public:
		MeshIntersectOriginalVertex(const Point3d* p_Point3d, const MeshVertexHandle& p_Vertex);
		virtual ~MeshIntersectOriginalVertex() {}

	public:
		bool IsTerminal() const override { return false; }
		bool IsOrigin() const override { return true; }

		bool IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const override { return false; }
		const MeshTriangleHandle* GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle = nullptr) const override { return nullptr; }

		virtual Parameter2d GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const;

		virtual const MeshVertexHandle* GetVertex() { return &m_OriginalVertex; }
	};



