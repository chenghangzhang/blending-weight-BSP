#include "mPch.h"
#include "OCTreeHeader.h"

OCTree::OCTree(Mesh& p_meshA, Mesh& p_meshB, Tolerance& p_Tolerance, int p_MaxDepth, int p_LeafShresholdNum):
	m_Tolerance(p_Tolerance)
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

	m_Header = new OCTreeNode(facesA, facesB, m_Tolerance, 0, p_MaxDepth, p_LeafShresholdNum, true);

}

bool OCTree::Initialize()
{
	return false;
}

void OCTree::GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes)
{
	m_Header->GetPartitionPlane(r_Pplanes, planeRecords());
}

void OCTree::GetAllLeafNode(vector<OCTreeNode*>& r_AllLeafNodes)
{
	m_Header->GetLeafNodes(r_AllLeafNodes);
}
