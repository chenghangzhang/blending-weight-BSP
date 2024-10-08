#pragma once


class BSPAccelerator 
	:public MeshIntersectAccelerator
{

public:
	BSPAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance, BSPConstructType p_Type = BSPConstructType::WSDM);
	virtual ~BSPAccelerator();

public:
	void Initialize() override;
	void GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList) override;
	void ResetGenerateIntersectCheckList() override;

protected:

	BSPConstructType m_Type;

	BSPTree* m_AccTree;

	vector<BSPTreeNode*> m_AllLeafNodes;

	vector<BSPTreeNode*>::iterator nowNodes;

	
	unordered_multimap<FaceId, FaceId> m_ProcessedFacesFacePair;

	unordered_multimap<EdgeId, FaceId> m_ProcessedEdgeAFaceBPair;
	unordered_multimap<EdgeId, FaceId> m_ProcessedEdgeBFaceAPair;
};