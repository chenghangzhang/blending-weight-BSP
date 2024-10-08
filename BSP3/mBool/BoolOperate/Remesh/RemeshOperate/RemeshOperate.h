#pragma once


class RemshOperate
{

public:
	RemshOperate(Mesh& p_Mesh, MeshIntersectResult& p_RecordMap, RemeshNewVertexRecord& p_ReturnNewVertex, const Tolerance& p_Tolerence = Tolerance());
	virtual ~RemshOperate() {};
	
protected:
	//输入输出

	//输入
	Mesh& m_OriginalMesh;
	MeshIntersectResult& m_RecordMap;

	RemeshNewVertexRecord& m_ReturnNewVertex;
	Tolerance m_Tolerance;

	//输出
	RemeshPathEdgeRecord m_PathEdgeRecord;

	//中间变量
	//SimplifyMesh2d* m_SimplifyMesh2d;
protected:
	//Tolerance m_Toler;
	bool m_IsInitialize;
	unordered_map<FaceId, MeshTriangleIntersectRecord*> m_ProcessingTriangles;
	unordered_map<FaceId, MeshTriangleIntersectRecord*> m_WaitingProcessingTriangles;

	unordered_map<const MeshIntersectVertex*, size_t> m_NewVertexs;
	
	unordered_map<FaceId, MeshTriangleIntersectRecord*> m_NewTrianglesMap2Record;
	unordered_multiset<EdgeId,FaceId> m_ProcessedTvs;

public:
	struct  TriangleArgs {
		unordered_set<EdgeId> m_CheckingEdges;
		unordered_set<FaceId> m_NewTriangles;

		vector<LineSegment3d> m_Edges;
	};

protected:		
	//MeshTriangle* m_ProcessingTriangle;
	//MeshTriangleIntersectRecord* m_TriangleRecord;

	//vector<Point2d*> m_Point2DCloud;
	//vector<Point3d*> m_Point3DCloud;
	//RemshConstraint m_RemeshConstraint;
	//RemshConstraint2D m_2DConstraint;
	//unordered_map<MeshVertex*,MeshIntersectVertex*> 

public:
	RemeshPathEdgeRecord run();


protected: //流程函数
	void Initialize();
	void InitializeSimplifyMesh();

	void TestTriangle(TriangleArgs&);
	void TestEdge(vector<MeshHalfEdgeHandle*>& p_Args);

	//void GetAxial();
	//void GetConstraint();
	//void CoordinateTo2D();	 
	//void GenerateRemesh();
	//void CoordinateTo3D();
	//void OutputResult(Mesh* r_CacheMesh);

protected:	
		
	virtual bool GenerateRemesh(size_t p_Triangle,const MeshTriangleIntersectRecord* p_Record);
	//virtual bool GenerateRemesh(SimplifyMesh2d& p_Mesh2d) = 0;

	virtual bool ProcessOnEdge(Point3dTriangle3dRelation& rela, size_t, MeshIntersectVertex*, TriangleArgs&) = 0;
	virtual bool ProcessIn(Point3dTriangle3dRelation& rela, size_t, MeshIntersectVertex*, TriangleArgs&) = 0;

	virtual bool ProcessConstrainedEdge(PathSegment* p_PathSeg,TriangleArgs&) = 0;

	bool CheckingCDT(TriangleArgs& p_TriArgs);
	//void GetConstraint(const MeshTriangleIntersectRecord* p_Record);
	//vector<Point2d*>& CoordinateTo2D(Vector3d& p_XAxial, Vector3d& p_YAxial, const MeshTriangle& p_Triangle);
	//Point2d Get2DCoordinate(Vector3d& p_XAxial, Vector3d& p_YAxial, const MeshVertex& p_Vertex);
	//Point2d Get2DCoordinate(Vector3d& p_XAxial, Vector3d& p_YAxial, const MeshIntersectVertex* p_IntersectVertex);
	//vector<Point3d*>& Get3DCoordinate(Vector3d& p_XAxial, Vector3d& p_YAxial, const vector<Point2d*>& p_Point2DSet);	
	//void GetAxial(const MeshTriangle& p_Triangle);
	

};



