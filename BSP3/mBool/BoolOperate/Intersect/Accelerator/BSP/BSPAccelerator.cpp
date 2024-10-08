#include "mPch.h"
#include "BSPAccelerator.h"



BSPAccelerator::BSPAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance, BSPConstructType p_Type /*= BSPConstructType::SDM*/):
	MeshIntersectAccelerator(p_MeshA,p_MeshB,p_Tolerance), m_Type(p_Type)
{

}

BSPAccelerator::~BSPAccelerator()
{
	if (nullptr != m_AccTree) delete m_AccTree;
}

void BSPAccelerator::Initialize()
{
	//m_AccTree = new BSPTree(const_cast<Mesh&>(m_MeshA), const_cast<Mesh&>(m_MeshB), const_cast<Tolerance&>(m_Tolerance), BSPConstructType::SDM);
	m_AccTree = new BSPTree(const_cast<Mesh&>(m_MeshA), const_cast<Mesh&>(m_MeshB), 
		const_cast<Tolerance&>(m_Tolerance), BSP_MAX_TREE_DEPTH, BSP_LEAF_SHRESHOLD_NUM, m_Type);

	m_AllLeafNodes.clear();

	m_AccTree->GetAllLeafNode(m_AllLeafNodes);

	nowNodes = m_AllLeafNodes.begin();
}


//void BSPAccelerator::GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList)
//{
//	////TestIntersectCheckList intersectCheckList;
//	//IntersectTriangleCheckList intersectCheckList;
//
//	//p_IntersectCheckList.clear();
//
//	//auto t1 = std::chrono::steady_clock::now();
//
//
//	////for (int i = 0;(i < 5) && (nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {
//	//for (int i = 0;(nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {
//	//	(*nowNodes)->GetIntersectPair(intersectCheckList);
//	//}
//
//	//auto t2 = std::chrono::steady_clock::now();
//	//std::cout << "获取初步数据时间：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;
//
//
//	//vector<pair<FaceId, FaceId>> nowProcessFacePairs;
//	//
//
//	////先对采集面片对去重
//	////int ii = 0;
//	////for (auto& checkPair : intersectCheckList) {
//	////	ii++;
//	////	if (ii % (intersectCheckList.size() / 100) == 0) {
//	////		cout << double(ii) / double(intersectCheckList.size()) << endl;
//	////	}
//	////
//	////	FaceId fa = checkPair.first;
//	////	FaceId fb = checkPair.second;
//	////
//	////	if (m_ProcessedFacesFacePair.find(fa) == m_ProcessedFacesFacePair.end()) {
//	////		m_ProcessedFacesFacePair.insert(make_pair(fa, fb));
//	////		nowProcessFacePairs.push_back(make_pair(fa, fb));
//	////	}
//	////	else {
//	////		pair<unordered_multimap<FaceId, FaceId>::iterator, unordered_multimap<FaceId, FaceId>::iterator> range;
//	////		range = m_ProcessedFacesFacePair.equal_range(fa);
//	////
//	////		bool hasFind = false;
//	////		for (auto it = range.first; it != range.second;it++) {
//
//	////			if (it->second == fb) {
//	////				hasFind = true;
//	////				break;
//	////			}
//	////		}
//
//	////		if (!hasFind) {
//	////			m_ProcessedFacesFacePair.insert(make_pair(fa, fb));
//	////			nowProcessFacePairs.push_back(make_pair(fa, fb));
//	////		}
//	////	}
//	////}
//
//
//	//// lambda for user-defined hash function
//	//auto hash = [](const pair<FaceId, FaceId>& c) {
//	//	return hash_val(c.first, c.second);
//	//};
//
//	//// lambda for user-defined equality criterion
//	//auto eq = [](const pair<FaceId, FaceId>& c1, const pair<FaceId, FaceId>& c2) {
//	//	return c1 == c2;
//	//};
//
//	//// create unordered set with user-defined behavior
//	//std::unordered_set<pair<FaceId, FaceId>, decltype(hash), decltype(eq)> nowProcessFacePairsSet(intersectCheckList.size(), hash, eq);
//
//	////先对采集面片对去重
//	//for (auto& checkPair : intersectCheckList) {
//	//	nowProcessFacePairsSet.insert(checkPair);
//	//}
//
//	//auto t3 = std::chrono::steady_clock::now();
//	//std::cout << "面片去重时间：" << std::chrono::duration<double, std::milli>(t3 - t2).count() << "毫秒" << endl;
//
//	//int ii = 0;
//	//for (auto& checkPair : nowProcessFacePairsSet) {
//	//	
//	//	ii++;
//	//	if (ii % (nowProcessFacePairsSet.size() / 100) == 0) {
//	//		std::cout << double(ii) / double(intersectCheckList.size()) << endl;
//	//	}
//
//	//	Mesh::FaceHandle faHandle = m_MeshA.face_handle(checkPair.first);
//	//	Mesh::FaceHandle fbHandle = m_MeshB.face_handle(checkPair.second);
//	//	Mesh* mA = const_cast<Mesh*>(&m_MeshA);
//	//	Mesh* mB = const_cast<Mesh*>(&m_MeshB);
//
//	//	for (Mesh::FaceEdgeIter fhIter = mA->fe_begin(faHandle); fhIter != mA->fe_end(faHandle);++fhIter) {
//
//	//		if (m_ProcessedEdgeAFaceBPair.find((*fhIter).idx()) == m_ProcessedEdgeAFaceBPair.end()) {
//	//			m_ProcessedEdgeAFaceBPair.insert(make_pair((*fhIter).idx(), fbHandle.idx()));
//
//	//			Mesh::HalfedgeHandle h = mA->halfedge_handle((*fhIter), 0);
//
//	//			MeshHalfEdgeHandle mh(h, mA);
//	//			MeshTriangleHandle mt(fbHandle, mB);
//
//	//			//cout << "MeshHalfEdgeHandle  EdgeId: " << mh.first.idx() << "FaceId:   " << mt.first.idx() << endl;
//	//			//cout << "mA: " << mA << endl;
//	//			//cout << "mB: " << mB << endl;
//	//			p_IntersectCheckList.push_back(make_pair(mh, mt));
//	//		}
//	//		else {
//	//			pair<unordered_multimap<FaceId, FaceId>::iterator, unordered_multimap<FaceId, FaceId>::iterator> range;
//	//			range = m_ProcessedEdgeAFaceBPair.equal_range((*fhIter).idx());
//
//	//			bool hasFind = false;
//	//			for (auto it = range.first; it != range.second;it++) {
//
//	//				if (it->second == fbHandle.idx()) {
//	//					hasFind = true;
//	//					break;
//	//				}
//	//			}
//
//	//			if (!hasFind) {
//	//				m_ProcessedEdgeAFaceBPair.insert(make_pair((*fhIter).idx(), fbHandle.idx()));
//
//	//				Mesh::HalfedgeHandle h = mA->halfedge_handle((*fhIter), 0);
//	//				MeshHalfEdgeHandle mh(h, mA);
//
//	//				MeshTriangleHandle mt(fbHandle, mB);
//
//	//				//cout << "MeshHalfEdgeHandle  EdgeId: " << mh.first.idx() << "FaceId:   " << mt.first.idx() << endl;
//	//				//cout << "mA: " << mA << endl;
//	//				//cout << "mB: " << mB << endl;
//
//	//				p_IntersectCheckList.push_back(make_pair(mh, mt));
//	//			}
//	//		}
//	//	}
//	//	
//
//	//	for (Mesh::FaceEdgeIter fhIter = mB->fe_begin(fbHandle); fhIter != mB->fe_end(fbHandle);++fhIter) {
//
//	//		if (m_ProcessedEdgeBFaceAPair.find((*fhIter).idx()) == m_ProcessedEdgeBFaceAPair.end()) {
//	//			m_ProcessedEdgeBFaceAPair.insert(make_pair((*fhIter).idx(), fbHandle.idx()));
//
//	//			Mesh::HalfedgeHandle h = mB->halfedge_handle((*fhIter), 0);
//	//			MeshHalfEdgeHandle mh(h, mB);
//
//	//			MeshTriangleHandle mt(faHandle, mA);
//	//			p_IntersectCheckList.push_back(make_pair(mh, mt));
//	//		}
//	//		else {
//	//			pair<unordered_multimap<FaceId, FaceId>::iterator, unordered_multimap<FaceId, FaceId>::iterator> range;
//	//			range = m_ProcessedEdgeBFaceAPair.equal_range((*fhIter).idx());
//
//	//			bool hasFind = false;
//	//			for (auto it = range.first; it != range.second;it++) {
//
//	//				if (it->second == faHandle.idx()) {
//	//					hasFind = true;
//	//					break;
//	//				}
//	//			}
//
//	//			if (!hasFind) {
//	//				m_ProcessedEdgeAFaceBPair.insert(make_pair((*fhIter).idx(), faHandle.idx()));
//
//	//				Mesh::HalfedgeHandle h = mB->halfedge_handle((*fhIter), 0);
//	//				MeshHalfEdgeHandle mh(h, mB);
//
//	//				MeshTriangleHandle mt(faHandle, mA);
//	//				p_IntersectCheckList.push_back(make_pair(mh, mt));
//	//			}
//	//		}
//	//	}
//
//
//	//}
//
//
//	//auto t4 = std::chrono::steady_clock::now();
//	//std::cout << "EdgeFacePair去重时间：" << std::chrono::duration<double, std::milli>(t4 - t3).count() << "毫秒" << endl;
//
//	////auto t5 = std::chrono::steady_clock::now();
//	////std::cout << "EdgeBFaceAPair去重时间：" << std::chrono::duration<double, std::milli>(t5 - t4).count() << "毫秒" << endl;
//
//
//	//////save pair
//	////ofstream fileA("cowMA.txt");
//	////ofstream fileB("cowMB.txt");
//
//	////Mesh* mA = const_cast<Mesh*>(&m_MeshA);
//	////Mesh* mB = const_cast<Mesh*>(&m_MeshB);
//
//	////for (auto& checkPair : p_IntersectCheckList) {
//
//	////	const MeshHalfEdgeHandle& edge = checkPair.first;
//	////	const MeshTriangleHandle& triangle = checkPair.second;
//
//	////
//	////	if (edge.second == mA) {
//	////		fileA << edge.first.idx() << endl;
//	////		fileA << triangle.first.idx() << endl;
//	////	}
//	////	else if (edge.second == mB) {
//	////		fileB << edge.first.idx() << endl;
//	////		fileB << triangle.first.idx() << endl;
//	////	}
//	////	else {
//	////		assert(false);
//	////	}
//
//	////}
//
//	////fileA.close();
//	////fileB.close();
//
//	//auto t6 = std::chrono::steady_clock::now();
//	//std::cout << "获取求交对总时间：" << std::chrono::duration<double, std::milli>(t6 - t1).count() << "毫秒" << endl;
//	//std::cout << "更新求交对数目：" << p_IntersectCheckList.size() << endl;
//
//	auto t5 = std::chrono::steady_clock::now();
//	p_IntersectCheckList.clear();
//	//recover checkpair
//	ReadCheckPair::ReadCPair(p_IntersectCheckList, m_MeshA, m_MeshB);
//	std::cout << "更新求交对数目：" << p_IntersectCheckList.size() << endl;
//	     
//
//	auto t6 = std::chrono::steady_clock::now();
//	std::cout << "获取求交对总时间：" << std::chrono::duration<double, std::milli>(t6 - t5).count() << "毫秒" << endl;
//}
	
void BSPAccelerator::GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList)
{
	//TestIntersectCheckList intersectCheckList;
	IntersectTriangleCheckList intersectCheckList;

	p_IntersectCheckList.clear();

	auto t1 = std::chrono::steady_clock::now();


	//for (int i = 0;(i < 5) && (nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {
	for (int i = 0;(nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {
		(*nowNodes)->GetIntersectPair(intersectCheckList);
	}

	auto t2 = std::chrono::steady_clock::now();
	//std::cout << "获取初步数据时间：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;


	// lambda for user-defined hash function
	auto hash = [](const pair<FaceId, FaceId>& c) {
		return hash_val(c.first, c.second);
	};

	// lambda for user-defined equality criterion
	auto eq = [](const pair<FaceId, FaceId>& c1, const pair<FaceId, FaceId>& c2) {
		return c1 == c2;
	};

	// create unordered set with user-defined behavior
	std::unordered_set<pair<FaceId, FaceId>, decltype(hash), decltype(eq)> nowProcessFacePairsSet(intersectCheckList.size(), hash, eq);

	std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeAFaceBPairSet(intersectCheckList.size() * 2, hash, eq);

	std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeBFaceAPairSet(intersectCheckList.size() * 2, hash, eq);

	//先对采集面片对去重
	for (auto& checkPair : intersectCheckList) {
		nowProcessFacePairsSet.insert(checkPair);
	}


	auto t3 = std::chrono::steady_clock::now();
	//std::cout << "面片去重时间：" << std::chrono::duration<double, std::milli>(t3 - t2).count() << "毫秒" << endl;

	Mesh* mA = const_cast<Mesh*>(&m_MeshA);
	Mesh* mB = const_cast<Mesh*>(&m_MeshB);

	for (auto& checkPair : nowProcessFacePairsSet) {

		Mesh::FaceHandle faHandle = m_MeshA.face_handle(checkPair.first);
		Mesh::FaceHandle fbHandle = m_MeshB.face_handle(checkPair.second);


		for (Mesh::FaceEdgeIter fhIter = mA->fe_begin(faHandle); fhIter != mA->fe_end(faHandle);++fhIter) {
			auto result = nowProcessEdgeAFaceBPairSet.insert(make_pair((*fhIter).idx(), fbHandle.idx()));
			
			if (result.second) {
				Mesh::HalfedgeHandle h = mA->halfedge_handle((*fhIter), 0);
				MeshHalfEdgeHandle mh(h, mA);

				MeshTriangleHandle mt(fbHandle, mB);
				p_IntersectCheckList.push_back(make_pair(mh, mt));
			}
		}


		for (Mesh::FaceEdgeIter fhIter = mB->fe_begin(fbHandle); fhIter != mB->fe_end(fbHandle);++fhIter) {
			auto result = nowProcessEdgeBFaceAPairSet.insert(make_pair((*fhIter).idx(), faHandle.idx()));

			if (result.second) {
				Mesh::HalfedgeHandle h = mB->halfedge_handle((*fhIter), 0);
				MeshHalfEdgeHandle mh(h, mB);

				MeshTriangleHandle mt(faHandle, mA);
				p_IntersectCheckList.push_back(make_pair(mh, mt));
			}

		}

	}


	auto t4 = std::chrono::steady_clock::now();
	//std::cout << "EdgeFacePair去重时间：" << std::chrono::duration<double, std::milli>(t4 - t3).count() << "毫秒" << endl;


	auto t6 = std::chrono::steady_clock::now();
	//cout << "获取求交对总时间：" << std::chrono::duration<double, std::milli>(t6 - t1).count() << "毫秒" << endl;
	cout << "更新求交对数目：" << p_IntersectCheckList.size() << endl;

	ofstream of("C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bool operate\\num.txt", ios::app);
	of << "===============" << endl;
	of << p_IntersectCheckList.size() << endl;
	of << "===============" << endl;
	of.close();
}

void BSPAccelerator::ResetGenerateIntersectCheckList()
{
	m_AllLeafNodes.clear();
	m_AccTree->GetAllLeafNode(m_AllLeafNodes);
	nowNodes = m_AllLeafNodes.begin();
}
