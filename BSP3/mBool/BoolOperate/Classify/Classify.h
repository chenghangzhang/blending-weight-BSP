#pragma once


class ClassifyOperate 
{
protected:
	using TriangleSet = unordered_set<FaceId>;

public:
	ClassifyOperate(vector<const Mesh*> p_MeshS, RemeshPathEdgeResult& p_Result, ClassifyRecord& p_ClassifyResult,const Tolerance& p_Tolerence = Tolerance()) :
		m_Meshs(p_MeshS),m_Result(p_Result), m_ClassifyResult(p_ClassifyResult), m_Tolerance(m_Tolerance) {}

	virtual ~ClassifyOperate() {};

protected:
	// ‰»Î ‰≥ˆ
	vector<const Mesh*> m_Meshs;
	RemeshPathEdgeResult& m_Result;

	TriangleSet m_NowWaveTriangles;
	TriangleSet m_NextWaveTriangles;

	Tolerance m_Tolerance;

	// ‰≥ˆ
	ClassifyRecord& m_ClassifyResult;
protected:
	void ProcessMesh(Mesh& p_Mesh);
	TriangleSet InitialMesh(const Mesh& p_Mesh);

	void ProcessEdge(EdgeId p_Path, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh);

	bool LableTriangleOnTheEdge(FaceId p_Triangle, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh);
	
	bool LableTriangle(FaceId p_Triangle, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord);

	bool LableIsolatedTriangle(FaceId& p_Triangle, TriangleSet& p_Triangles, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh);
	bool SearchLabeledTriangle(vector<FaceId> p_NowLevelTriangles, unordered_map<FaceId, bool>& p_ClassifyRecord, unordered_set<FaceId> p_SearchedTri, Mesh& p_Mesh);
	bool IsLabeled(FaceId p_Triangle, unordered_map<FaceId, bool>& p_ClassifyRecord);
	
	
public:
	void run();



};




