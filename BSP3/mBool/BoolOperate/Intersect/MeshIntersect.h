#pragma once

class MeshIntersectResult;
class MeshIntersectAccelerator;

using MeshIntersectMeshResult = unordered_map<Mesh*, MeshIntersectResult>;

class MeshIntersect
	{
	public:
		MeshIntersect(MeshIntersectMeshResult& r_Result, Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_Type = BSPConstructType::WSDM, const Tolerance& p_Tolerence = Tolerance());
		virtual ~MeshIntersect();

	protected: //输入输出输出变量
		//输入
		Mesh& m_OriginMeshA;
		Mesh& m_OriginMeshB;
		Tolerance m_Tolerance;

		//输出 
		MeshIntersectMeshResult& m_IntersectResult;

	protected: //控制变量
		bool m_IsInitialize;

		using IntersectCheckList = vector<pair<const MeshHalfEdgeHandle, const MeshTriangleHandle>>;

		IntersectCheckList m_IntersectCheckList;
		

	protected: //加速结构
		MeshIntersectAccelerator* m_pAccelerator;

	protected: //流程函数
		void Initialize();
		void Reset();

		void GenerateIntersect();

	protected: //各个流程函数的细化功能函数
		//GenerateIntersect流程的具体功能函数
		void GenerateIntersectCheckList();
		void GenerateIntersect(const IntersectCheckList& p_IntersectCheckList);


		void GenerateIntersect(const MeshHalfEdgeHandle& p_Edge, const MeshTriangleHandle& p_Triangle); //函数为不采用任何加速结构进行求交判断
		void GenerateIntersectPath();

	public:
		void SetAccelerator(MeshIntersectAccelerator::Type p_AccleratorType);
		bool Run();
		bool RunGenerateIntersectCheckList(); //临时调试 GenerateIntersectCheckList 功能
	};

	//MeshIntersectResult* IntersectMesh(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance = Tolerance());

