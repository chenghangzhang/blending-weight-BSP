#include "mPch.h"
#include "RemeshPathEdge.h"



RemeshPathEdge::RemeshPathEdge(MeshHalfEdgeHandle* p_Edge, PathSegment* p_PathSeg):
	m_Edge(p_Edge),m_IntersectTriangleNormal(p_PathSeg->GetNormal())
{


}




