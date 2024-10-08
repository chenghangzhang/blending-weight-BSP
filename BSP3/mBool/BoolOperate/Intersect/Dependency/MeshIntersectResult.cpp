#include "mPch.h"
#include "MeshIntersectResult.h"


	MeshIntersectResult::MeshIntersectResult(const Tolerance& p_Tolerance) :
		m_Tolerance(p_Tolerance)
	{

	}

	MeshIntersectResult::~MeshIntersectResult()
	{
		Clear();
	}

	/*const MeshTriangleIntersectRecord& MeshIntersectResult::GetIntersectRecord(const MeshTriangle& pTriangle)
	{
		auto& pRecord = m_TriangleIntersectRecordMap[&pTriangle];

		return pRecord;
	}*/


	bool MeshIntersectResult::IsEmpty() const
	{
		if (m_IntersectVertices.size() > 0 || m_IntersectPaths.size() > 0)
		{
			return false;
		}

		if (0 == m_TriangleIntersectRecordMap.size())
		{
			return true;
		}

		assert(false);
		return false;
	}

	void MeshIntersectResult::Clear()
	{

		for (auto* piv : m_IntersectVertices)
		{
			assert(piv != nullptr);
			delete piv;
		}
		m_IntersectVertices.clear();

		for (auto* pip : m_IntersectPaths)
		{
			assert(pip != nullptr);
			delete pip;
		}
		m_IntersectPaths.clear();

		m_TriangleIntersectRecordMap.clear();
	}

	bool MeshIntersectResult::AddTerminalVertex(const MeshIntersectTerminalVertex& p_Vertex)
	{
		const MeshHalfEdgeHandle& hostEdge = p_Vertex.GetHostEdge();

		if (!hostEdge.second->is_boundary(hostEdge.first)) {
			
			MeshTriangleIntersectRecord& record0 = m_TriangleIntersectRecordMap[
				hostEdge.second->face_handle(hostEdge.first).idx()
			];

			//cout << "firstFace: " << hostEdge.second->face_handle(hostEdge.first).idx() << endl;

			MeshIntersectTerminalVertex* pNewVertex = new MeshIntersectTerminalVertex(p_Vertex);

			record0.AddNewTerminalVertex(pNewVertex);

			Mesh::HalfedgeHandle oppoHHandle = hostEdge.second->opposite_halfedge_handle(hostEdge.first);
			if (!hostEdge.second->is_boundary(oppoHHandle))
			{
				auto& record1 = m_TriangleIntersectRecordMap[
					hostEdge.second->face_handle(oppoHHandle).idx()
				];
				//cout << "secondFace: " << hostEdge.second->face_handle(oppoHHandle).idx() << endl;
				assert(false == record1.HasCoincidentTerminalVertex(p_Vertex, m_Tolerance));

				record1.AddNewTerminalVertex(pNewVertex);
			}

			m_IntersectVertices.push_back(pNewVertex);
		}


		return false;
	}

	bool MeshIntersectResult::AddMiddleVertex(const MeshTriangleHandle& p_Triangle, const MeshIntersectMiddleVertex& p_Vertex)
	{
		auto& record = m_TriangleIntersectRecordMap[
			p_Triangle.first.idx()
		];

		auto* pNewVertex = record.AddMiddleVertex(p_Vertex, m_Tolerance);

		if (nullptr != pNewVertex)
		{
			m_IntersectVertices.push_back(pNewVertex);

			return true;
		}

		return false;
	}

	size_t MeshIntersectResult::GeneratePaths()
	{

		//cout << "+++++++++++++++++++++++++++++++++++++++" << endl;

		assert(0 == m_IntersectPaths.size());

		for (auto& m : m_TriangleIntersectRecordMap)
		{
			//cout << "=================" << endl;
			//auto& triangle = m.first;
			auto& record = m.second;
			
			//{
			//	cout << "MV" << endl;
			//	auto& mv = record.m_MiddleVertices;
			//	for (auto& imv : mv) {
			//		cout << (*imv->m_Point3d)[0] << ", " << (*imv->m_Point3d)[1] << ", " << (*imv->m_Point3d)[2] << endl;
			//	}
			//}
			//
			//{
			//	cout << "TV" << endl;
			//	auto& mv = record.m_TerminalVertices;
			//	for (auto& imv : mv) {
			//		cout << (*imv->m_Point3d)[0] << ", " << (*imv->m_Point3d)[1] << ", " << (*imv->m_Point3d)[2] << endl;
			//	}
			//}

			vector<MeshIntersectPath*> newPaths = record.GeneratePaths();

			m_IntersectPaths.insert(m_IntersectPaths.end(), newPaths.begin(), newPaths.end());
		}
		 
		//int i = 0;
		//for (auto& path : m_IntersectPaths) {
		//	cout << "path: " << i << endl;
		//	i++;

		//	for (auto& interVertex : path->m_IntersectVertices) {

		//		cout << (*interVertex->m_Point3d)[0] <<", "<< (*interVertex->m_Point3d)[1] << ", " << (*interVertex->m_Point3d)[2] << endl;
		//		if (interVertex->IsTerminal()) {
		//			cout << "TV "<< dynamic_cast<const MeshIntersectTerminalVertex*>(interVertex)->m_HostEdge.second << endl;
		//			cout << "TV " << dynamic_cast<const MeshIntersectTerminalVertex*>(interVertex)->m_IntersetTriangle.second << endl;
		//			cout << "TV, HostEdge id £º"<<dynamic_cast<const MeshIntersectTerminalVertex*>(interVertex)->m_HostEdge.first.idx() << endl;
		//			cout << "TV, IntersetTriangle id £º" << dynamic_cast<const MeshIntersectTerminalVertex*>(interVertex)->m_IntersetTriangle.first.idx() << endl;
		//		}
		//		else {
		//			assert(!interVertex->IsOrigin());

		//			cout << "MV " << dynamic_cast<const MeshIntersectMiddleVertex*>(interVertex)->m_HostTriangle.second << endl;
		//			cout << "MV " << dynamic_cast<const MeshIntersectMiddleVertex*>(interVertex)->m_IntersetEdge.second << endl;

		//			cout << "MV, HostFace id £º" << dynamic_cast<const MeshIntersectMiddleVertex*>(interVertex)->m_HostTriangle.first.idx() << endl;
		//			cout << "MV, InterEdge id £º" << dynamic_cast<const MeshIntersectMiddleVertex*>(interVertex)->m_IntersetEdge.first.idx() << endl;
		//		
		//		}
		//	}

		//}


		return m_IntersectPaths.size();
	}

	MeshTriangleIntersectRecord* MeshIntersectResult::At(const MeshTriangleHandle* p_Triangle)
	{
		if (m_TriangleIntersectRecordMap.find(p_Triangle->first.idx()) == m_TriangleIntersectRecordMap.end()) {
			//assert(false);		
			return nullptr;
		}

		return &m_TriangleIntersectRecordMap[p_Triangle->first.idx()];
	}

	const MeshTriangleIntersectRecord* MeshIntersectResult::At(const MeshTriangleHandle* p_Triangle) const
	{
		if (m_TriangleIntersectRecordMap.find(p_Triangle->first.idx()) == m_TriangleIntersectRecordMap.end()) {
			//assert(false);
			return nullptr;
		}

		return &m_TriangleIntersectRecordMap.at(p_Triangle->first.idx());
	}

	MeshTriangleIntersectRecord* MeshIntersectResult::At(FaceId p_Tri)
	{
		if (m_TriangleIntersectRecordMap.find(p_Tri) == m_TriangleIntersectRecordMap.end()) {
			//assert(false);
			return nullptr;
		}

		return &m_TriangleIntersectRecordMap[p_Tri];
	}

	const MeshTriangleIntersectRecord* MeshIntersectResult::At(FaceId p_Tri) const
	{
		if (m_TriangleIntersectRecordMap.find(p_Tri) == m_TriangleIntersectRecordMap.end()) {
			//assert(false);
			return nullptr;
		}

		return &m_TriangleIntersectRecordMap.at(p_Tri);
	}

	//const MeshTriangleIntersectRecord* MeshIntersectResult::At(const MeshTriangle* p_pTriangle) const
	//{
	//	//size_t test= 0;
	//	//for (auto itr: m_Map) {
	//	//	test++;
	//	//
	//	//}
	//	//cout << test << endl;
	//	auto itr = m_TriangleIntersectRecordMap.find(p_pTriangle);

	//	if (itr != m_TriangleIntersectRecordMap.end()) {
	//		return &(m_TriangleIntersectRecordMap.at(p_pTriangle));
	//	}
	//	else {
	//		return nullptr;
	//	}

	//}


