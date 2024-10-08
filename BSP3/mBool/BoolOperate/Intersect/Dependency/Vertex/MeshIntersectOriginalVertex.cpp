#include "mPch.h"
#include "MeshIntersectOriginalVertex.h"



	MeshIntersectOriginalVertex::MeshIntersectOriginalVertex(const Point3d* p_Point3d, const MeshVertexHandle& p_Vertex) :
		MeshIntersectVertex(p_Point3d),
		m_OriginalVertex(p_Vertex)
	{

	}

	Parameter2d MeshIntersectOriginalVertex::GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const
	{
		//通过原始顶点的以及三角形的信息获取二维坐标
		return Parameter2d();
	}


