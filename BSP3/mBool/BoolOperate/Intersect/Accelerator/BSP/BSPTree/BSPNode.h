#pragma once


class BSPTreeNode {

public:
	BSPTreeNode(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum, bool p_Isroot = false, BSPConstructType p_Type = BSPConstructType::AABB_MIDDLE_SPLIT, double p_LambdaS = 1, double p_LambdaD = 0.5);
	//BSPTreeNode(vector<TriangleBSP> p_FacesA, vector<TriangleBSP> p_FacesB, Plane3d p_PartitionPlane, Tolerance& p_Tolerance, bool p_Isroot = false, BSPConstructType p_Type = BSPConstructType::AABB);
	~BSPTreeNode();

public:
	Plane3d m_PartitionPlane;

	BSPTreeNode* m_PositiveSon;
	BSPTreeNode* m_NegativeSon;

	bool m_IsRoot;
	bool m_IsLeaf;
	int m_Depth;

	int m_MaxDepth;
	int m_LeafShresholdNum;


	BSPConstructType m_Type;

	Tolerance& m_Tolerance;

	vector<TriangleForTree> m_FacesA;
	vector<TriangleForTree> m_FacesB;


	double m_LambdaS = 1;
	double m_LambdaD = 0.5;

protected:
	void DeConstruct();

public:
	void GetLeafNodes(vector<BSPTreeNode*>& r_AllLeafNodes);


	bool IsLeaf();

	bool GetIntersectPair(IntersectTriangleCheckList& p_IntersectCheckList);

	bool IsValidNode();
	int GetValidFaces(unordered_set<FaceId>& facesA, unordered_set<FaceId>& facesB);


	void GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes, planeRecords p_CurrentRecords);

	void ConstructAABBSons();

	void ConstructWSDMSons();
	void ConstructWSDM_PCASons();

	void ConstructSAHSons();
	void ConstructObbMiddleSons();
	void ConstructGravity_SPLITSons();
	void ConstructSDM_OBBSons();
	void ConstructOrigin_SDMSons();
	void ConstructSDM_Only_SizeWeightedSons();

	void ConstructAngleSons();
	void ConstructAngleSonsSerial();
	void ConstructAngle_SDMSons();

	void GetNegativePositiveLabelWithSplitTriangle(vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane);
	void GetNegativePositiveLabelWithOutSplitTriangle(vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane);

	void create_bsp_tree_node1(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum);
	void create_bsp_tree_node2(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum);
	void SeparationTrianglesFromDifferentMesh(vector<TriangleForTree> facesOrigin, vector<TriangleForTree>& facesA, vector<TriangleForTree>& facesB);

	int Size();
	pair<int,int> GetRealSize();
};