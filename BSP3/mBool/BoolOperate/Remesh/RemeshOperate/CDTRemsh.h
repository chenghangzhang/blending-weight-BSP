#pragma once

class CDTRemesh : public RemshOperate
{

public:

	CDTRemesh(Mesh& p_Mesh, MeshIntersectResult& p_RecordMap, RemeshNewVertexRecord& p_ReturnNewVertex, const Tolerance& p_Tolerence = Tolerance());
	virtual ~CDTRemesh() {}

protected:
	virtual bool ProcessOnEdge(Point3dTriangle3dRelation& rela, FaceId, MeshIntersectVertex*, TriangleArgs&) override;
	virtual bool ProcessIn(Point3dTriangle3dRelation& rela, FaceId p_Triangle, MeshIntersectVertex*, TriangleArgs&) override;
	virtual bool ProcessConstrainedEdge(PathSegment* p_PathSeg, TriangleArgs&) override;

	bool HasEdge(PathSegment* p_PathSeg, TriangleArgs&);
	unordered_set<size_t> FindIntersectEdge(PathSegment* p_PathSeg, TriangleArgs& p_TriArgs);

	size_t ProcessMultiIntersectEdges(PathSegment* p_PathSeg, unordered_set<size_t>& p_InterEdges, TriangleArgs& p_Args);

	void FindPathParameters(PathSegment* p_PathSeg, size_t& r_BeginEdge, size_t& r_PathBegin, size_t& r_PathEnd, Vector3d& r_Normal, unordered_set<size_t>& p_InterEdges, TriangleArgs& p_Args);
	void FindLeftRightPath(size_t p_BeginEdge, size_t p_PathBegin, size_t p_PathEnd, Vector3d& p_Normal, unordered_set<size_t>& p_InterEdges, vector<size_t>& leftPath, vector<size_t>& rightPath, unordered_set<size_t>& p_CandidateVertex);
	
	void ClearIntersectTriangles(unordered_set<EdgeId> p_InterEdges, TriangleArgs& p_Args);		
	void GenerateNewTrianglesIndex(vector<array<VertexId,3>>& r_NewTriangles, vector<VertexId>& p_BoundList, pair<VertexId, VertexId> p_FirstEdge);
		
	bool IsLeft(Vector3d& p_NormalReference, Vector3d& p_DirectReference, Vector3d p_Calculate);
	FaceId AddNewTriangles(vector<array<VertexId, 3>>& r_NewTriangles, Vector3d& p_Normal,TriangleArgs& p_Args);
	VertexId GetNextVertex(unordered_set<VertexId>& p_CandidateVertex, pair<VertexId, VertexId> p_AxisSymmetry);
		
protected:

};



