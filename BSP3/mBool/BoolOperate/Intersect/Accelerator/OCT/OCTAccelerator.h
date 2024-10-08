#pragma once


class OCTAccelerator : public MeshIntersectAccelerator {

public:
	OCTAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance);
	virtual ~OCTAccelerator();

public:
	void Initialize() override;
	void GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList) override;
	void ResetGenerateIntersectCheckList() override;

protected:

	OCTree* m_AccTree;

	vector<OCTreeNode*> m_AllLeafNodes;

	vector<OCTreeNode*>::iterator nowNodes;


	unordered_multimap<FaceId, FaceId> m_ProcessedFacesFacePair;

	unordered_multimap<EdgeId, FaceId> m_ProcessedEdgeAFaceBPair;
	unordered_multimap<EdgeId, FaceId> m_ProcessedEdgeBFaceAPair;

};