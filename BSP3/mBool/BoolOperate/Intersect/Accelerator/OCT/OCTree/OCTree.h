#pragma once


class OCTree {


public:
	OCTree(Mesh& p_meshA, Mesh& p_meshB, Tolerance& p_Tolerance, int p_MaxDepth, int p_LeafShresholdNum);

protected:
	bool Initialize();

protected:
	vector<TriangleForTree> m_MeshA;
	vector<TriangleForTree> m_MeshB;

	Tolerance& m_Tolerance;

	OCTreeNode* m_Header;

public:
	void GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes);


public:
	void GetAllLeafNode(vector<OCTreeNode*>& r_AllLeafNodes);
};