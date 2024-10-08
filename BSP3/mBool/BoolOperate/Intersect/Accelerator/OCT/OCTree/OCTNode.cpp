#include "mPch.h"
#include "OCTreeHeader.h"

OCTreeNode::OCTreeNode(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum, bool p_Isroot):
	m_FacesA(p_FacesA), m_FacesB(p_FacesB), m_Tolerance(p_Tolerance), m_Depth(p_Depth), m_IsRoot(p_Isroot), m_MaxDepth(p_MaxDepth), m_LeafShresholdNum(p_LeafShresholdNum)
{
	//达到截止深度
	if (p_Depth == p_MaxDepth) {
		m_IsLeaf = true;

		SetPointersAsLeaf();

		return;
	}

	//达到叶节点面片数目条件，生成叶节点
	if (p_FacesA.size() + p_FacesB.size() <= p_LeafShresholdNum) {
		m_IsLeaf = true;

		SetPointersAsLeaf();

		return;
	}

	//某一类面片数目为0，生成叶节点
	if ((0 == p_FacesA.size()) || (0 == p_FacesB.size())) {

		m_IsLeaf = true;

		SetPointersAsLeaf();

		return;
	}

	ConstructSons();

}

void OCTreeNode::SetPointersAsLeaf()
{
	m_Sons[0] = nullptr;
	m_Sons[1] = nullptr;
	m_Sons[2] = nullptr;
	m_Sons[3] = nullptr;

	m_Sons[4] = nullptr;
	m_Sons[5] = nullptr;
	m_Sons[6] = nullptr;
	m_Sons[7] = nullptr;
}

void OCTreeNode::DeConstruct()
{
	for (int i = 0; i < 8;i++) {
		if (m_Sons[i]->IsLeaf()) {
			delete m_Sons[i];
		}
		else {
			m_Sons[i]->DeConstruct();
		}
	}

	m_IsLeaf = true;
}

void OCTreeNode::GetLeafNodes(vector<OCTreeNode*>& r_AllLeafNodes)
{
	if (m_IsLeaf) {
		r_AllLeafNodes.push_back(this);
	}
	else {

		for (int i = 0;i < 8;i++) {

			assert(nullptr != m_Sons[i]);

			m_Sons[i]->GetLeafNodes(r_AllLeafNodes);
		}
		
	}
}

bool OCTreeNode::IsLeaf()
{
	return m_IsLeaf;
}

void OCTreeNode::ConstructSons()
{
	vector<TriangleForTree> allFaces(m_FacesA);
	allFaces.insert(allFaces.end(), m_FacesB.begin(), m_FacesB.end());

	AABB aabb(allFaces);

	m_Center = aabb.GetCenter();

	Plane3d partitionPlanes[3] = { Plane3d(m_Center, Vector3d(0, 0, -1.0)), Plane3d(m_Center, Vector3d(0, 1.0, 0)), Plane3d(m_Center, Vector3d(1.0, 0, 0))};
	
	vector<TriangleForTree> sonElements[8];

	vector<TriangleForTree> allfacesCap[4];
	vector<TriangleForTree> allfacesCapTemp[4];

	allfacesCap[0] = allFaces;


	//第一层划分
	{
		vector<TriangleForTree> rPositive, rNegative;
		GetNegativePositiveLabelWithSplitTriangle(allfacesCap[0], rPositive, rNegative, partitionPlanes[0]);

		allfacesCapTemp[0] = rNegative;
		allfacesCapTemp[1] = rPositive;

		allfacesCap[0] = allfacesCapTemp[0];
		allfacesCap[1] = allfacesCapTemp[1];

	}

	//第二层划分
	{
		//左子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[0], rPositive, rNegative, partitionPlanes[1]);

			allfacesCapTemp[0] = rNegative;
			allfacesCapTemp[1] = rPositive;
		}

		//右子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[1], rPositive, rNegative, partitionPlanes[1]);

			allfacesCapTemp[2] = rNegative;
			allfacesCapTemp[3] = rPositive;
		}

		allfacesCap[0] = allfacesCapTemp[0];
		allfacesCap[1] = allfacesCapTemp[1];
		allfacesCap[2] = allfacesCapTemp[2];
		allfacesCap[3] = allfacesCapTemp[3];

	}

	//第三层划分
	{
		//上左子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[0], rPositive, rNegative, partitionPlanes[2]);

			sonElements[0] = rNegative;
			sonElements[1] = rPositive;
		}

		//上右子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[1], rPositive, rNegative, partitionPlanes[2]);

			sonElements[2] = rNegative;
			sonElements[3] = rPositive;
		}

		//下左子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[2], rPositive, rNegative, partitionPlanes[2]);

			sonElements[4] = rNegative;
			sonElements[5] = rPositive;
		}

		//下右子节点
		{
			vector<TriangleForTree> rPositive, rNegative;
			GetNegativePositiveLabelWithSplitTriangle(allfacesCap[3], rPositive, rNegative, partitionPlanes[2]);

			sonElements[6] = rNegative;
			sonElements[7] = rPositive;
		}
	}


	for (int i = 0; i < 8; i++) {
		vector<TriangleForTree> facesFromA, facesFromB;
		SeparationTrianglesFromDifferentMesh(sonElements[i], facesFromA, facesFromB);

		m_Sons[i] = new OCTreeNode(facesFromA, facesFromB, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum);
	}

	m_IsLeaf = false;

}

bool OCTreeNode::GetIntersectPair(IntersectTriangleCheckList& p_IntersectCheckList)
{

	for (auto& fa : m_FacesA) {
		for (auto& fb : m_FacesB) {

			p_IntersectCheckList.push_back(make_pair(fa.m_HoldFace.idx(), fb.m_HoldFace.idx()));

		}
	}

	if ((m_FacesA.size() > 0) && (m_FacesB.size() > 0))
		return true;
	else
		return false;

}

bool OCTreeNode::IsValidNode()
{
	if ((0 == m_FacesA.size()) || (0 == m_FacesB.size()))
		return true;

	return false;
}

int OCTreeNode::GetValidFaces(unordered_set<FaceId>& facesA, unordered_set<FaceId>& facesB)
{
	if ((0 == m_FacesA.size()) || (0 == m_FacesB.size()))
		return 0;

	unordered_set<FaceId> fA, fB;

	for (auto& fa : m_FacesA) {
		facesA.insert(fa.m_HoldFace.idx());
		fA.insert(fa.m_HoldFace.idx());
	}

	for (auto& fb : m_FacesB) {
		facesB.insert(fb.m_HoldFace.idx());
		fB.insert(fb.m_HoldFace.idx());
	}

	return fA.size() + fB.size();
}

void OCTreeNode::GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes, planeRecords p_CurrentRecords)
{
	if (m_IsLeaf) return;


	if (m_IsRoot) {
		assert(m_Depth == 0);

		p_CurrentRecords.clear();
	}


	Plane3d partitionPlanes[3] = { Plane3d(m_Center, Vector3d(0, 0, -1.0)), Plane3d(m_Center, Vector3d(0, 1.0, 0)), Plane3d(m_Center, Vector3d(1.0, 0, 0)) };

	//绘制当前子节点三个基准平面
	r_Pplanes.push_back(make_pair(partitionPlanes[0], p_CurrentRecords));
	r_Pplanes.push_back(make_pair(partitionPlanes[1], p_CurrentRecords));
	r_Pplanes.push_back(make_pair(partitionPlanes[2], p_CurrentRecords));

	for (int i = 0; i < 8;i++) {

		bool pos3 = i & 1;
		bool pos2 = (i >> 1) & 1;
		bool pos1 = (i >> 2) & 1;

		p_CurrentRecords.push_back(make_pair(partitionPlanes[0], pos1));
		p_CurrentRecords.push_back(make_pair(partitionPlanes[1], pos2));
		p_CurrentRecords.push_back(make_pair(partitionPlanes[2], pos3));

		m_Sons[i]->GetPartitionPlane(r_Pplanes, p_CurrentRecords);

		p_CurrentRecords.pop_back();
		p_CurrentRecords.pop_back();
		p_CurrentRecords.pop_back();

	}

}

void OCTreeNode::GetNegativePositiveLabelWithSplitTriangle(vector<TriangleForTree>& originAllFaces, vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane)
{
	negativeFaces.clear();
	positiveFaces.clear();

	for (auto& face : originAllFaces) {
		TrianglePlaneIsIntersectInfo info;

		Triangle3d tri = face.m_Triangle;

		bool canJudgeFaceA = true;
		if (IsZero(tri.GetNormal().cross(partitionPlane.GetNormal()))) {
			if (IsPositive((tri.GetCenter() - partitionPlane.GetOrigin()).dot(partitionPlane.GetNormal()))) {
				info.isIntersect = false;
				info.direction = true;
			}
			else {
				info.isIntersect = false;
				info.direction = false;
			}

		}
		else {
			canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
		}


		if (!canJudgeFaceA) {
			//canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
			positiveFaces.push_back(face);
			negativeFaces.push_back(face);
		}
		else {
			if (info.isIntersect) {
				for (auto posiTri : info.newPositiveTriangles) {

					positiveFaces.push_back({ posiTri, face.m_HoldFace,face.m_holdMesh });
				}

				for (auto negaTri : info.newNegativeTriangles) {

					negativeFaces.push_back({ negaTri, face.m_HoldFace,face.m_holdMesh });
				}

				//negativeFaces.push_back(face);
				//positiveFaces.push_back(face);
			}
			else {
				if (false == info.direction) {
					negativeFaces.push_back(face);
				}
				else {
					positiveFaces.push_back(face);
				}
			}

		}


	}

}

void OCTreeNode::SeparationTrianglesFromDifferentMesh(vector<TriangleForTree> facesOrigin, vector<TriangleForTree>& facesA, vector<TriangleForTree>& facesB)
{
	assert(!m_FacesA.empty());
	assert(!m_FacesB.empty());

	Mesh* meshA = m_FacesA.front().m_holdMesh;
	Mesh* meshB = m_FacesB.front().m_holdMesh;

	for (auto& face : facesOrigin) {

		if (face.m_holdMesh == meshA) {
			facesA.push_back(face);
		}
		else if (face.m_holdMesh == meshB) {
			facesB.push_back(face);
		}
		else {
			assert(false);
		}
	}

}
