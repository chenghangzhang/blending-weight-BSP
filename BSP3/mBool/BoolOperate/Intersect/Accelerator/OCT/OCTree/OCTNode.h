#pragma once

class OCTreeNode {

public:
	OCTreeNode(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum, bool p_Isroot = false);
	

public:
	Point3d m_Center;

	OCTreeNode* m_Sons[8] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };



	bool m_IsRoot;
	bool m_IsLeaf;

	int m_Depth;

	int m_MaxDepth;
	int m_LeafShresholdNum;

	Tolerance& m_Tolerance;

	vector<TriangleForTree> m_FacesA;
	vector<TriangleForTree> m_FacesB;

protected:
	void SetPointersAsLeaf();
	void DeConstruct();

public:

	void GetLeafNodes(vector<OCTreeNode*>& r_AllLeafNodes);

	bool IsLeaf();

	void ConstructSons();


	bool GetIntersectPair(IntersectTriangleCheckList& p_IntersectCheckList);

	bool IsValidNode();
	int GetValidFaces(unordered_set<FaceId>& facesA, unordered_set<FaceId>& facesB);


	void GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes, planeRecords p_CurrentRecords);

	void GetNegativePositiveLabelWithSplitTriangle(vector<TriangleForTree>& originAllFaces, vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane);
	
	//void GetNegativePositiveLabelWithOutSplitTriangle(vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane);

	void SeparationTrianglesFromDifferentMesh(vector<TriangleForTree> facesOrigin, vector<TriangleForTree>& facesA, vector<TriangleForTree>& facesB);

};