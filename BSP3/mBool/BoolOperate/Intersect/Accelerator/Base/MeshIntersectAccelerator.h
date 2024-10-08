#pragma once

using IntersectCheckList = vector<pair<const MeshHalfEdgeHandle, const MeshTriangleHandle>>;
using TestIntersectCheckList = vector<pair<MeshHalfEdgeHandle, MeshTriangleHandle>>;

using IntersectTriangleCheckList = vector<pair<FaceId, FaceId>>;

class MeshIntersectAccelerator
{
public:
	enum class Type : char
	{
		NONE,
		MERGE_BSP_TREE,
		R_TREE,
		OCT_TREE
	};

protected:
	const Mesh& m_MeshA;
	const Mesh& m_MeshB;
	const Tolerance& m_Tolerance;



public:
	MeshIntersectAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance);
	virtual ~MeshIntersectAccelerator();

public:
	virtual void Initialize() = 0;
	virtual void GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList) = 0;
	virtual void ResetGenerateIntersectCheckList() = 0;

};

