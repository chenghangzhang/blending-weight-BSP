#pragma once



	class MeshIntersectVertex
	{

	public: //暂时 public
		const Point3d* m_Point3d;
		const size_t m_IntersectCount = 1;

	public:
		MeshIntersectVertex(const Point3d* p_Point3d) : m_Point3d(p_Point3d) {};
		//virtual ~MeshIntersectVertex() = 0 {}
		virtual ~MeshIntersectVertex() = 0 {}

	public:
		size_t GetIntersectCount() const { return m_IntersectCount; }

	public: //虚函数
		virtual bool IsTerminal() const = 0; //目前只有 2 个子类 Terminal 和 Middle，所以暂时用这种方式，后续如果扩展可以改变方式		
		virtual bool IsOrigin() const = 0;

		//virtual bool IsMiddle() const = 0;

		virtual const MeshVertexHandle* GetVertex() = 0;

		virtual bool IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const = 0;
		virtual const MeshTriangleHandle* GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle = nullptr) const = 0;

		virtual Parameter2d GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const = 0;
	};
