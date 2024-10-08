#pragma once


class BSPTree {


public:
	BSPTree(Mesh& p_meshA, Mesh& p_meshB, Tolerance& p_Tolerance, int p_MaxDepth, int p_LeafShresholdNum, BSPConstructType p_Type = BSPConstructType::AABB_MIDDLE_SPLIT, double p_LambdaS = 1, double p_LambdaD = 0.5);

protected:
	bool Initialize();

protected:
	vector<TriangleForTree> m_MeshA;
	vector<TriangleForTree> m_MeshB;

	BSPConstructType m_Type;
	Tolerance& m_Tolerance;

	BSPTreeNode* m_Header;

public:
	void GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes);


public:
	void GetAllLeafNode(vector<BSPTreeNode*>& r_AllLeafNodes);
};