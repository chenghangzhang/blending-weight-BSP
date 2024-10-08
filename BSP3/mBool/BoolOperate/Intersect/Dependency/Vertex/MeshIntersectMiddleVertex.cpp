#include "mPch.h"
#include "MeshIntersectMiddleVertex.h"


	MeshIntersectMiddleVertex::MeshIntersectMiddleVertex(const Point3d* p_Point3d, const MeshHalfEdgeHandle& p_IntersetEdge, const MeshTriangleHandle& p_HostTriangle, const Parameter2d& p_ParamInTiangle) :
		MeshIntersectVertex(p_Point3d),
		m_IntersetEdge(p_IntersetEdge),
		m_HostTriangle(p_HostTriangle),
		m_ParamInTiangle(p_ParamInTiangle)
	{

	}

	bool MeshIntersectMiddleVertex::IsIntersectWithTriangle(const Mesh::FaceHandle& p_Triangle) const
	{
		//size_t hostTriangleCount = m_IntersetEdge.GetHostFacetCount();
		//assert(1 == hostTriangleCount || 2 == hostTriangleCount);

		Mesh::HalfedgeHandle interEdge = m_IntersetEdge.first;

		//输入三角形handle
		Mesh::FaceHandle fh = p_Triangle;
		
		if (!m_IntersetEdge.second->is_boundary(interEdge)) {

			//auto test = m_IntersetEdge.second->face_handle(interEdge);

			//当前半边所属三角形handle
			if (m_IntersetEdge.second->face_handle(interEdge) == fh) {
				return true;			
			}
			
		}
			
		//半边反向
		interEdge = m_IntersetEdge.second->opposite_halfedge_handle(interEdge);

		if (!m_IntersetEdge.second->is_boundary(interEdge)) {

			//auto test = m_IntersetEdge.second->face_handle(interEdge);

			//当前半边所属三角形handle
			if (m_IntersetEdge.second->face_handle(interEdge) == fh) {
				return true;
			}

		}


		return false;
	}

	const MeshTriangleHandle* MeshIntersectMiddleVertex::GetIntersectTriangle(const MeshTriangleHandle* p_pTriangle /*= nullptr*/) const
	{

		Mesh::HalfedgeHandle interEdge = m_IntersetEdge.first;
		Mesh::HalfedgeHandle interEdgeOp = m_IntersetEdge.second->opposite_halfedge_handle(interEdge);

		//assert(!m_IntersetEdge.second->is_boundary(interEdge));
		//assert(!m_IntersetEdge.second->is_boundary(interEdgeOp));

		if (m_IntersetEdge.second->is_boundary(interEdge) || m_IntersetEdge.second->is_boundary(interEdgeOp)) {

			return nullptr;
			//bool test1 = m_IntersetEdge.second->is_boundary(interEdge);
			//bool test2 = m_IntersetEdge.second->is_boundary(interEdgeOp);
			//Mesh::Point p1 = m_IntersetEdge.second->point(m_IntersetEdge.second->from_vertex_handle(interEdge));
			//Mesh::Point p2 = m_IntersetEdge.second->point(m_IntersetEdge.second->to_vertex_handle(interEdge));

			//Mesh::FaceHandle fh0 = m_IntersetEdge.second->face_handle(interEdge);
			//Mesh::FaceHandle fh1 = m_IntersetEdge.second->face_handle(interEdgeOp);

			//Mesh::Point p3 = m_IntersetEdge.second->point(m_IntersetEdge.second->from_vertex_handle(interEdge));
			//Mesh::Point p4 = m_IntersetEdge.second->point(m_IntersetEdge.second->to_vertex_handle(interEdge));
		}

		Mesh::FaceHandle fh0 = m_IntersetEdge.second->face_handle(interEdge);
		Mesh::FaceHandle fh1 = m_IntersetEdge.second->face_handle(interEdgeOp);


		const MeshTriangleHandle* tri0 = new MeshTriangleHandle( m_IntersetEdge.second->face_handle(interEdge), m_IntersetEdge.second);
		const MeshTriangleHandle* tri1 = new MeshTriangleHandle( m_IntersetEdge.second->face_handle(interEdgeOp), m_IntersetEdge.second);

		assert(nullptr != tri1);

		if (nullptr == p_pTriangle) //当参数为 nullptr 时，目标是为了获得任意相交三角形
		{
			return tri0;
		}


		//当参数不为 nullptr 时，目标是为了获得另一个相交三角形

		//输入三角形handle
		Mesh::FaceHandle fhIn = p_pTriangle->first;

		if (fhIn == fh0)
		{
			return tri1;
		}
		if (fhIn == fh1)
		{
			return tri0;
		}

		assert(false); //p_pTriangle 不等于任意一个三角形，为调用错误


		//switch (m_IntersetEdge.GetHostFacetCount())
		//{
		//case 1:
		//{
		//	const MeshTriangle* tri = &(m_IntersetEdge.GetHostFacet(0));
		//	assert(nullptr != tri);

		//	if (nullptr == p_pTriangle) //当参数为 nullptr 时，目标是为了获得任意相交三角形
		//	{
		//		return tri;
		//	}

		//	if (p_pTriangle == tri) //当参数不为 nullptr 时，目标是为了获得另一个相交三角形，但由于个数为 1 所以返回 nullptr
		//	{
		//		return nullptr;
		//	}

		//	assert(false); //p_pTriangle 不等于三角形，为调用错误
		//	break;
		//}
		//case 2:
		//{
		//	const MeshTriangle* tri0 = &(m_IntersetEdge.GetHostFacet(0));
		//	const MeshTriangle* tri1 = &(m_IntersetEdge.GetHostFacet(1));
		//	assert(nullptr != tri0);
		//	assert(nullptr != tri1);

		//	if (nullptr == p_pTriangle) //当参数为 nullptr 时，目标是为了获得任意相交三角形
		//	{
		//		return tri0;
		//	}

		//	//当参数不为 nullptr 时，目标是为了获得另一个相交三角形
		//	if (p_pTriangle == tri0)
		//	{
		//		return tri1;
		//	}
		//	if (p_pTriangle == tri1)
		//	{
		//		return tri0;
		//	}

		//	assert(false); //p_pTriangle 不等于任意一个三角形，为调用错误
		//	break;
		//}
		//default:
		//	assert(false);
		//	break;
		//}

		assert(false);
		return nullptr;

	}
	Parameter2d MeshIntersectMiddleVertex::GetParameter2dAtTheTriangle(const MeshTriangleHandle& p_Triangle) const
	{
		return m_ParamInTiangle;
	}
