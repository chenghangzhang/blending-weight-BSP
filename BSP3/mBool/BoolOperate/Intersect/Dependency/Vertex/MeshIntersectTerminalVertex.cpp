#include "mPch.h"
#include "MeshIntersectTerminalVertex.h"


	MeshIntersectTerminalVertex::MeshIntersectTerminalVertex(const Point3d* p_Point3d, const MeshHalfEdgeHandle& p_HostEdge, double p_HostEdgParam, const MeshTriangleHandle& p_IntersetTriangle) :
		MeshIntersectVertex(p_Point3d),
		m_HostEdge(p_HostEdge),
		m_ParamOnHostEdge(p_HostEdgParam),
		m_IntersetTriangle(p_IntersetTriangle)
	{

	}

	bool MeshIntersectTerminalVertex::IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const
	{

		return p_Triangle == m_IntersetTriangle.first;
	}

	const MeshTriangleHandle* MeshIntersectTerminalVertex::GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle /*= nullptr*/) const
	{

		if (nullptr == p_pTriangle)
		{
			return &m_IntersetTriangle;
		}

		if (p_pTriangle->first == m_IntersetTriangle.first)
		{
			return nullptr;
		}

		assert(false);
		return nullptr;
	}

	Parameter2d MeshIntersectTerminalVertex::GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const
	{
		//通过线段两端点插值出来
		assert(false);
		return Parameter2d();
	}
