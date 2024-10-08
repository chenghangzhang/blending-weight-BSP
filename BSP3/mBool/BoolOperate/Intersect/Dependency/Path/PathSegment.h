#pragma once



	class PathSegment
	{
	public:
		PathSegment(const MeshIntersectVertex* p_Begin, const MeshIntersectVertex* p_End);
		~PathSegment() {};

	public:
		const MeshIntersectVertex* m_Begin;
		const MeshIntersectVertex* m_End;

		Vector3d m_Normal;
	public:

		Vector3d& GetNormal();

	};

