#pragma once



class RemeshPathEdge {

public:
	MeshHalfEdgeHandle* m_Edge;		
	Vector3d m_IntersectTriangleNormal;

public:
	RemeshPathEdge(MeshHalfEdgeHandle* p_Edge,PathSegment* p_PathSeg);

};




