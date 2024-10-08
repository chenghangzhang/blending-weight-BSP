#include "mPch.h"
#include "../Accelerator.h"
#include "DirectMeshIntersectAccelerator.h"


DirectMeshIntersectAccelerator::DirectMeshIntersectAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance) :
		MeshIntersectAccelerator(p_MeshA, p_MeshB, p_Tolerance)
	{
		m_ActiveEdgeNow = p_MeshA.edges_begin();
		m_PassiveTriangleNow = p_MeshB.faces_begin();

		m_PassiveEdgeNow = p_MeshB.edges_begin();
		m_AcitveTriangleNow = p_MeshA.faces_begin();
		
		
		m_IntersectCheckListSize = 64;

		//在 Reset 函数中初始化
	}

DirectMeshIntersectAccelerator::~DirectMeshIntersectAccelerator()
	{

	}

	void DirectMeshIntersectAccelerator::Initialize()
	{

	}

	void DirectMeshIntersectAccelerator::GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList)
	{
		assert(true == p_IntersectCheckList.empty());

		size_t s = 0;

		//Active边 与 Passive三角网格 求交
		for (m_ActiveEdgeNow; m_ActiveEdgeNow != m_MeshA.edges_end() && s < m_IntersectCheckListSize; ++m_ActiveEdgeNow)
		{

			for (m_PassiveTriangleNow; m_PassiveTriangleNow != m_MeshB.faces_end() && s < m_IntersectCheckListSize; ++m_PassiveTriangleNow)
			{
				const MeshHalfEdgeHandle edge = MeshHalfEdgeHandle(m_MeshA.halfedge_handle(*m_ActiveEdgeNow,0) ,const_cast<Mesh*>(&m_MeshA));
				const MeshTriangleHandle triangle = MeshTriangleHandle(*m_PassiveTriangleNow,const_cast<Mesh*>(&m_MeshB));
				

				p_IntersectCheckList.push_back(make_pair(edge, triangle));
				s++;
			}

			if (s >= m_IntersectCheckListSize)
			{
				break;
			}

			m_PassiveTriangleNow = m_MeshB.faces_begin();
		}

		//Passive边 与 Active三角网格 求交
		for (m_PassiveEdgeNow; m_PassiveEdgeNow != m_MeshB.edges_end() && s < m_IntersectCheckListSize; ++m_PassiveEdgeNow)
		{
			//if (m_ParsedEdgeB.find(m_PassiveEdgeNow->idx()) != m_ParsedEdgeB.end()) continue;
			//if (m_ParsedEdgeB.find(m_MeshB.opposite_halfedge_handle(*m_PassiveEdgeNow).idx()) != m_ParsedEdgeB.end()) continue;

			//m_ParsedEdgeB.insert(m_PassiveEdgeNow->idx());
			//m_ParsedEdgeB.insert(m_MeshB.opposite_halfedge_handle(*m_PassiveEdgeNow).idx());


			for (m_AcitveTriangleNow; m_AcitveTriangleNow != m_MeshA.faces_end() && s < m_IntersectCheckListSize; ++m_AcitveTriangleNow)
			{
				const MeshHalfEdgeHandle edge = MeshHalfEdgeHandle(m_MeshB.halfedge_handle(*m_PassiveEdgeNow, 0) , const_cast<Mesh*>(&m_MeshB));
				const MeshTriangleHandle triangle = MeshTriangleHandle(*m_AcitveTriangleNow, const_cast<Mesh*>(&m_MeshA));

				p_IntersectCheckList.push_back(make_pair(edge, triangle));
				s++;
			}
			
			if (s >= m_IntersectCheckListSize)
			{
				break;
			}

			m_AcitveTriangleNow = m_MeshA.faces_begin();
		}
	}

	void DirectMeshIntersectAccelerator::ResetGenerateIntersectCheckList()
	{
		m_ActiveEdgeNow = m_MeshA.edges_begin();
		m_PassiveTriangleNow = m_MeshB.faces_begin();

		m_PassiveEdgeNow = m_MeshB.edges_begin();
		m_AcitveTriangleNow = m_MeshA.faces_begin();

		m_IntersectCheckListSize = 1000000;


	}

