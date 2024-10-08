#include "mPch.h"
#include "BSPTreeHeader.h"

BSPTree::BSPTree(Mesh& p_meshA, Mesh& p_meshB, Tolerance& p_Tolerance, int p_MaxDepth, int p_LeafShresholdNum, BSPConstructType p_Type, double p_LambdaS /*= 1*/, double p_LambdaD /*= 0.5*/):
	m_Tolerance(p_Tolerance),m_Type(p_Type)
{
	vector<TriangleForTree> facesA;
	vector<TriangleForTree> facesB;

	for (Mesh::FaceIter fh = p_meshA.faces_sbegin(); fh != p_meshA.faces_end(); ++fh) {

		facesA.push_back({ TranslateToTriangle(make_pair(*fh, &p_meshA)), *fh, &p_meshA });
	}

	for (Mesh::FaceIter fh = p_meshB.faces_sbegin(); fh != p_meshB.faces_end(); ++fh) {
		facesB.push_back({ TranslateToTriangle(make_pair(*fh, &p_meshB)), *fh, &p_meshB });
	}

	m_MeshA = facesA;
	m_MeshB = facesB;

	m_Header = new BSPTreeNode(facesA, facesB, m_Tolerance, 0, p_MaxDepth, p_LeafShresholdNum, true, p_Type, p_LambdaS, p_LambdaD);


	//Initialize();
}

bool BSPTree::Initialize()
{
	//vector<MeshTriangleHandle> facesA;
	//vector<MeshTriangleHandle> facesB;

	//for (Mesh::FaceIter fh = m_MeshA.faces_sbegin(); fh != m_MeshA.faces_end(); ++fh) {
	//	facesA.push_back(make_pair(*fh, &m_MeshA));
	//}
	//for (Mesh::FaceIter fh = m_MeshB.faces_sbegin(); fh != m_MeshB.faces_end(); ++fh) {
	//	facesB.push_back(make_pair(*fh, &m_MeshB));
	//}

	//m_Header = new BSPTreeNode(facesA, facesB, m_Tolerance,0);


	return true;
}

void BSPTree::GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes)
{

	m_Header->GetPartitionPlane(r_Pplanes, planeRecords() );
}

void BSPTree::GetAllLeafNode(vector<BSPTreeNode*>& r_AllLeafNodes)
{
	m_Header->GetLeafNodes(r_AllLeafNodes);
}
