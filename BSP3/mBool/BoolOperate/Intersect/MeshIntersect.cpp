#include "mPch.h"
#include "Accelerator/Accelerator.h"
#include "Dependency/Dependency.h"
#include "MeshIntersect.h"


MeshIntersect::MeshIntersect(MeshIntersectMeshResult& r_Result, Mesh& p_MeshA, Mesh& p_MeshB, BSPConstructType p_Type /*= BSPConstructType::SDM*/, const Tolerance& p_Tolerence /*= Tolerance()*/) :
	m_IntersectResult(r_Result),
	m_OriginMeshA(p_MeshA),
	m_OriginMeshB(p_MeshB),
	m_Tolerance(p_Tolerence),
	m_IsInitialize(false)
{
	//m_pAccelerator(new MeshIntersectFaceToFaceR_TreeAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance))
	//m_pAccelerator(new DirectMeshIntersectAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance))
	//m_pAccelerator(new BSPAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance, p_Type))

	if (p_Type == BSPConstructType::OCTREE) {	
		m_pAccelerator = new OCTAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance);
	}
	else {
		m_pAccelerator = new BSPAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance, p_Type);
	}
		

		assert(true == r_Result.empty());

		{		
			auto& rA = m_IntersectResult[&m_OriginMeshA];
			auto& rB = m_IntersectResult[&m_OriginMeshB];
		}

	}

	MeshIntersect::~MeshIntersect()
	{
		assert(nullptr != m_pAccelerator);
		delete m_pAccelerator;
	}

	void MeshIntersect::Initialize()
	{
		assert(false == m_IsInitialize);

		m_pAccelerator->Initialize();

		m_IsInitialize = true;
	}

	void MeshIntersect::Reset()
	{
		assert(nullptr != m_pAccelerator);
		m_pAccelerator->ResetGenerateIntersectCheckList();

		m_IntersectResult[&m_OriginMeshA].Clear();
		m_IntersectResult[&m_OriginMeshB].Clear();
	}

	void MeshIntersect::GenerateIntersect()
	{

		////auto t1 = std::chrono::steady_clock::now();

		//GenerateIntersectCheckList();

		////auto t2 = std::chrono::steady_clock::now();
		////cout << "1：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;

		//GenerateIntersect(m_IntersectCheckList);
		//
		////auto t3 = std::chrono::steady_clock::now();
		////cout << "2：" << std::chrono::duration<double, std::milli>(t3 - t2).count() << "毫秒" << endl;

		//GenerateIntersectPath();

		////auto t4 = std::chrono::steady_clock::now();
		////cout << "3：" << std::chrono::duration<double, std::milli>(t4 - t3).count() << "毫秒" << endl;


		cout << "CheckNum: "<<m_OriginMeshA.n_edges()*m_OriginMeshB.n_faces() + m_OriginMeshB.n_edges() * m_OriginMeshA.n_faces() << endl;
		
		//ofstream of("C:\\Users\\NUOSEN\\Desktop\\\BSP\\bool-new3-d\\TestModel\\bool operate\\num.txt",ios::app);
		//of << "===============" << endl;
		//of << m_OriginMeshA.n_edges() * m_OriginMeshB.n_faces() + m_OriginMeshB.n_edges() * m_OriginMeshA.n_faces() << endl;
		//of << "===============" << endl;
		//of.close();


		//cout << "CheckListNum: " << double(m_OriginMeshA.n_edges() * m_OriginMeshB.n_faces() + m_OriginMeshB.n_edges() * m_OriginMeshA.n_faces())/1000000.0 << endl;
		
		//int i = 0;
		////并行化
		//do
		//{
		//	cout << "GenerateIntersectCheckList" << endl;

		//	GenerateIntersectCheckList();

		//	GenerateIntersect(m_IntersectCheckList);

		//	
		//	cout << i++ << endl;
		//	

		//} while (false == m_IntersectCheckList.empty());

		GenerateIntersectCheckList();

		GenerateIntersect(m_IntersectCheckList);

		GenerateIntersectPath();
		
		assert(m_IntersectResult.size() == 2);
	}

	void MeshIntersect::GenerateIntersect(const IntersectCheckList& p_IntersectCheckList)
	{
		int i = 0;
		for (const auto& ip : p_IntersectCheckList) // ip = intersect pair
		{

			//auto t1 = std::chrono::steady_clock::now();

			GenerateIntersect(ip.first, ip.second);

			//auto t2 = std::chrono::steady_clock::now();

			i++;
			if(i%100000 == 0) cout << i << endl;

			//if(i%9 == 0) cout << "1：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;
			
		}
		//cout << "Times " << i << endl;


		//m_IntersectResult.IsEmpty();
	}

	void MeshIntersect::GenerateIntersectCheckList()
	{
		m_IntersectCheckList.clear();

		assert(nullptr != m_pAccelerator);
		m_pAccelerator->GenerateNextIntersectCheckList(m_IntersectCheckList);
	}


	void MeshIntersect::GenerateIntersect(const MeshHalfEdgeHandle& p_Edge, const MeshTriangleHandle& p_Triangle)
	{

		Mesh::FaceVertexCCWIter fvit = p_Triangle.second->fv_ccwbegin(p_Triangle.first);
		Mesh::VertexHandle triV0 = *fvit;
		Mesh::VertexHandle triV1 = *(++fvit);
		Mesh::VertexHandle triV2 = *(++fvit);

		//auto p0 = p_Triangle.second->point(triV0);
		//auto p1 = p_Triangle.second->point(triV1);
		//auto p2 = p_Triangle.second->point(triV1);
		//double length = (p2 - p1).length();

		//模型存在边长为0的边
		//assert(p_Edge.second->calc_edge_length(p_Edge.first) > m_Tolerance.Linear());
		if (p_Edge.second->calc_edge_length(p_Edge.first) < m_Tolerance.Linear()) return;

		//模型存在面积为0的三角形
		//assert(p_Triangle.second->calc_face_area(p_Triangle.first) > m_Tolerance.Linear());
		if (p_Triangle.second->calc_face_area(p_Triangle.first) < m_Tolerance.Linear()) return;



		Triangle3d triMath(array<Point3d,3>{ 
			Point3d(p_Triangle.second->point(triV0)), 
			Point3d(p_Triangle.second->point(triV1)), 
			Point3d(p_Triangle.second->point(triV2)) 
		});

		Mesh::VertexHandle edgeV0 = p_Edge.second->from_vertex_handle(p_Edge.first);
		Mesh::VertexHandle edgeV1 = p_Edge.second->to_vertex_handle(p_Edge.first);

		LineSegment3d lineSegMath(
			Point3d(p_Edge.second->point(edgeV0)),
			Point3d(p_Edge.second->point(edgeV1))
			);


		auto intersectResult = IntersectPlaneLineSegment(
			triMath, 
			lineSegMath,
			m_Tolerance
		);

		//处理重合情况
		if (intersectResult.IsNotType(IntersectResult::Type::Regular)) // 暂不考虑重合在面上的线
		{
			return;
		}

		Point3d* intersect = new Point3d(intersectResult.m_Intersect);
		const double& intersectParamOnEdge = intersectResult.m_ParamOnLine;
		const Parameter2d& intersectParamInTriangle = intersectResult.m_ParamOnPlane;

		uint onElementIndex = uint(-1);
		auto relation = DetectCoplanarPoint3dTriangle3dRelation(*intersect, triMath, m_Tolerance);

		if (Point3dTriangle3dRelation::Type::Out == relation.m_Relation)
		{
			delete intersect;
			return;
		}

		if (Point3dTriangle3dRelation::Type::In == relation.m_Relation)
		{
			//active
			//在 p_Edge 所属三角形中，添加相交端点的记录
			MeshIntersectTerminalVertex tv(intersect, p_Edge, intersectParamOnEdge, p_Triangle);

			//cout << "In ===================" << p_Edge.second << "       " << p_Triangle.second << endl;
			//cout << (*intersect)[0] << ", " << (*intersect)[1] << ", " << (*intersect)[2] << endl;

			
			m_IntersectResult[p_Edge.second].AddTerminalVertex(tv);

			//passive
			//在 p_Triangle 中，添加相交中间点的记录
			MeshIntersectMiddleVertex mv(intersect, p_Edge, p_Triangle, intersectParamInTriangle);
			m_IntersectResult[p_Triangle.second].AddMiddleVertex(p_Triangle, mv);

			return;
		}

		if (Point3dTriangle3dRelation::Type::OnEdge == relation.m_Relation)
		{
			cout << "OnE ===================" << endl;
			cout << "p_EdgeMeshPtr: " << p_Edge.second <<"  "<< p_Edge.first.idx() << endl;
			cout << "p_TriangleMeshPtr: " << p_Triangle.second << "  " << p_Triangle.first.idx() << endl;
			cout << (*intersect)[0] << ", " << (*intersect)[1] << ", " << (*intersect)[2] << endl;

			//在 p_Edge 所属三角形中，添加相交端点的记录
			MeshIntersectTerminalVertex tv(intersect, p_Edge, intersectParamOnEdge, p_Triangle);
			m_IntersectResult[p_Edge.second].AddTerminalVertex(tv);

			return;
		}

		if (Point3dTriangle3dRelation::Type::OnVertex == relation.m_Relation)
		{
			//cout << "OnV ===================" << p_Edge.second << "       " << p_Triangle.second << endl;
			//cout << (*intersect)[0] << ", " << (*intersect)[1] << ", " << (*intersect)[2] << endl;

			//在 p_Edge 所属三角形中，添加相交端点的记录
			MeshIntersectTerminalVertex tv(intersect, p_Edge, intersectParamOnEdge, p_Triangle);
			m_IntersectResult[p_Edge.second].AddTerminalVertex(tv);


			//

			return;
		}

		//assert(false);
	}

	void MeshIntersect::GenerateIntersectPath()
	{
		auto& rec1 = m_IntersectResult.at(&m_OriginMeshA);
		rec1.GeneratePaths();

		//cout << "meshA " << &m_OriginMeshA << endl;

		auto& rec2 = m_IntersectResult.at(&m_OriginMeshB);
		rec2.GeneratePaths();
	}

	void MeshIntersect::SetAccelerator(MeshIntersectAccelerator::Type p_AccleratorType)
	{
		if (nullptr != m_pAccelerator)
		{
			delete m_pAccelerator;
			m_pAccelerator = nullptr;
		}

		switch (p_AccleratorType)
		{
		case MeshIntersectAccelerator::Type::MERGE_BSP_TREE:
			assert(false);
			//m_pAccelerator = new MeshTriangleMergeBspTreeAccelerator(m_OriginMeshA, m_OriginMeshB, m_Tolerance);
			break;
		default:
			assert(false);
			break;
		}

		m_pAccelerator->Initialize();
	}

	bool MeshIntersect::Run()
	{
		if (false == m_IsInitialize)
		{
			Initialize();
		}

		assert(true == m_IsInitialize);

		Reset();

		GenerateIntersect();

		return true;
	}

	bool MeshIntersect::RunGenerateIntersectCheckList()
	{
		if (false == m_IsInitialize)
		{
			Initialize();
		}

		assert(true == m_IsInitialize);

		Reset();

		size_t totalIntersectCheckPairCount = 0;

		do
		{
			GenerateIntersectCheckList();
			size_t intersectCheckPairCount = m_IntersectCheckList.size();
			//cout << "当前 GenerateIntersectCheckList 尺寸：" << intersectCheckPairCount << endl;
			totalIntersectCheckPairCount += intersectCheckPairCount;

		} while (false == m_IntersectCheckList.empty());

		//cout << "全部 IntersectCheckPair 数量：" << totalIntersectCheckPairCount << endl;

		return true;
	}

	//MeshIntersectResult* IntersectMesh(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance)
	//{
	//	MeshIntersect meshIntersect(p_MeshA, p_MeshB);
	//	meshIntersect.Run();
	//	//cout << meshIntersect.m_IntersectResult.mapsize() << endl;;

	//	return meshIntersect.m_IntersectResult;
	//}
