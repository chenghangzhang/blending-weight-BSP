#pragma once


class MeshBoolOperate {

protected: //输入输出变量
	Mesh& m_OriginMeshA;
	Mesh& m_OriginMeshB;
	Tolerance m_Tolerance;

	
protected: //中间变量
	//remesh路径输出
	RemeshPathEdgeResult m_RemeshEdgeResult;

	//remesh新增点
	RemeshNewVertexRecord m_RemeshNewVertexRecord;
	RemeshNewVertexResult m_RemeshNewVertexResult;


	

protected: //控制变量
	bool m_IsInitialize;

public:
	//调试
	MeshIntersectMeshResult m_IntersectResult;

	//调试
	//标记面片是在内还是在外
	ClassifyRecord m_TriangleRecord;

	BSPConstructType m_TreeType;

public:
	MeshBoolOperate(Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_TreeType, const Tolerance& p_Tolerence = Tolerance());
	~MeshBoolOperate();

protected: //流程函数
	void Initialize();
	void MeshPreProcess();
	void GenerateIntersect();
	void GenerateRemesh();
	void Classify();
	void Combine(Mesh& r_Result, const MeshBoolOperateType p_Type);
	

	void CombineTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal);
private: //各个流程函数的细化功能函数
	//Initialize流程的具体功能函数

	//GenerateIntersect流程的具体功能函数

	//Remesh流程的具体功能函数

protected: //控制函数

	void Clear();

public:
	bool Run(Mesh& r_Result, const MeshBoolOperateType p_Type);

	bool RunTest(Mesh& r_Result, const MeshBoolOperateType p_Type, Mesh& r_MeshA, Mesh& r_MeshB, bool needReverseNormal, string file_head);
};
