#include "mPch.h"
#include "MeshIntersectAccelerator.h"


	MeshIntersectAccelerator::MeshIntersectAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance) :
		m_MeshA(p_MeshA),
		m_MeshB(p_MeshB),
		m_Tolerance(p_Tolerance)
	{

	}

	MeshIntersectAccelerator::~MeshIntersectAccelerator()
	{
	}

