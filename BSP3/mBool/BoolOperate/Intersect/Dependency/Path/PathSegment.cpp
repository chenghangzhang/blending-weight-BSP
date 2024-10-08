#include "mPch.h"

#include "PathSegment.h"


	PathSegment::PathSegment(const MeshIntersectVertex* p_Begin, const MeshIntersectVertex* p_End):
		m_Begin(p_Begin),m_End(p_End)
	{
		assert(p_Begin != nullptr);
		assert(p_End != nullptr);

		if (p_Begin->IsTerminal() && p_End->IsTerminal()) {
			//Tv->Tv
			assert(true == p_Begin->IsIntersectWithTriangle(p_End->GetIntersectTriangle()->first));

			m_Normal = p_End->GetIntersectTriangle()->second->normal(p_End->GetIntersectTriangle()->first);
		}
		else if (p_Begin->IsTerminal() && (!p_End->IsTerminal())) {
			assert(true == p_End->IsIntersectWithTriangle(p_Begin->GetIntersectTriangle()->first));
			
			m_Normal = p_Begin->GetIntersectTriangle()->second->normal(p_Begin->GetIntersectTriangle()->first);
		}
		else if ((!p_Begin->IsTerminal()) && (!p_End->IsTerminal())) {
			
			const MeshHalfEdgeHandle& interEdge = dynamic_cast<const MeshIntersectMiddleVertex*>(p_Begin)->GetIntersectEdge();

			Mesh::FaceHandle fh0 = interEdge.second->face_handle(interEdge.first);
			Mesh::FaceHandle fh1 = interEdge.second->opposite_face_handle(interEdge.first);

			if (p_End->IsIntersectWithTriangle(fh0))
			{
				m_Normal = interEdge.second->normal(fh0);
			}
			else if (p_End->IsIntersectWithTriangle(fh1)) 
			{
				m_Normal = interEdge.second->normal(fh1);		
			}
	
		}
		else if ((!p_Begin->IsTerminal()) && p_End->IsTerminal()) {
			assert(true == p_Begin->IsIntersectWithTriangle(p_End->GetIntersectTriangle()->first));
			m_Normal = p_End->GetIntersectTriangle()->second->normal(p_End->GetIntersectTriangle()->first);
		}
		else {
			assert(false);		
		}

	}


	//void PathSegment::CalculateNormal()
	//{
	//	//使得相交三角形的外法向投影在该线段法向为正时，取该方向

	//}

	Vector3d& PathSegment::GetNormal()
	{
		
		return m_Normal;
	}



