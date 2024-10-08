#include "mPch.h"
#include "OCTAccelerator.h"

OCTAccelerator::OCTAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance):
	MeshIntersectAccelerator(p_MeshA, p_MeshB, p_Tolerance)
{

}

OCTAccelerator::~OCTAccelerator()
{
	if (nullptr != m_AccTree) delete m_AccTree;
}

void OCTAccelerator::Initialize()
{
	m_AccTree = new OCTree(const_cast<Mesh&>(m_MeshA), const_cast<Mesh&>(m_MeshB),
		const_cast<Tolerance&>(m_Tolerance), OCT_MAX_TREE_DEPTH, OCT_LEAF_SHRESHOLD_NUM);

	m_AllLeafNodes.clear();

	m_AccTree->GetAllLeafNode(m_AllLeafNodes);

	nowNodes = m_AllLeafNodes.begin();
}

void OCTAccelerator::GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList)
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
	cout << "八叉树更新求交对数目：" << p_IntersectCheckList.size() << endl;
}

void OCTAccelerator::ResetGenerateIntersectCheckList()
{
	m_AllLeafNodes.clear();
	m_AccTree->GetAllLeafNode(m_AllLeafNodes);
	nowNodes = m_AllLeafNodes.begin();
}
