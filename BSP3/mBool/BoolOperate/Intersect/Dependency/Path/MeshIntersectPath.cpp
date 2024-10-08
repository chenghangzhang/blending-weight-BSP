#include "mPch.h"
#include "MeshIntersectPath.h"


	void MeshIntersectPath::AddVertex(const MeshIntersectVertex& p_Vertex)
	{
		for (const MeshIntersectVertex* pv : m_IntersectVertices)
		{
			if (pv == &p_Vertex)
			{
				assert(false);
			}
		}

		if (m_IntersectVertices.size() >= 1) {
			PathSegment* pathSeg = new PathSegment(m_IntersectVertices.back(), &p_Vertex);		
			m_PathSegment.push_back(pathSeg);
		}
		
		m_IntersectVertices.push_back(&p_Vertex);

	}

	const MeshIntersectVertex& MeshIntersectPath::operator[](size_t p_Index) const
	{
		return *(m_IntersectVertices[p_Index]);
	}

	bool MeshIntersectPath::IsInPath(const MeshIntersectVertex* p_Vertex)
	{
		for (auto& m : m_IntersectVertices) {
			if (m == p_Vertex) {
				return true;
			}
		
		}
		return false;
	}

	void MeshIntersectPath::DeletePathSegment()
	{
		for (auto* itr : m_PathSegment) {
			delete itr;
			itr = nullptr;		
		}

		m_PathSegment.clear();

	}
