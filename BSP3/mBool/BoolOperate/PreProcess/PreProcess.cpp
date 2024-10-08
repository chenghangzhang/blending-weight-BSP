#include "mPch.h"
#include "PreProcess.h"

void PreProcess::run()
{

	for (auto& me : m_PreProcessMeshes) {

		Mesh& originMesh = *me;
		//处理边长为0情况（重复点）

		bool isFinished = false;
		while (!isFinished) {

			bool hasFind = false;
			for (Mesh::EdgeIter ei = originMesh.edges_sbegin(); ei != originMesh.edges_end(); ++ei) {
				if (originMesh.calc_edge_length(*ei) <= m_Toler.m_Linear) {

					Mesh::HalfedgeHandle he = originMesh.halfedge_handle(*ei, 0);
					originMesh.collapse(he);
					
					hasFind = true;

					break;
				}

			}

			if(!hasFind) isFinished = true;
			
		}

		originMesh.garbage_collection();


		//isFinished = false;
		//while (!isFinished) {

		//	bool hasFind = false;
		//	for (Mesh::FaceIter fi = originMesh.faces_sbegin(); fi != originMesh.faces_end(); ++fi) {
		//		if (originMesh.calc_face_area(*fi) <= m_Toler.m_Linear) {

		//			
		//			originMesh.delete_face(*fi);

		//			hasFind = true;

		//			break;
		//		}

		//	}

		//	if (!hasFind) isFinished = true;

		//}

		for (Mesh::FaceIter fi = originMesh.faces_sbegin(); fi != originMesh.faces_end(); ++fi) {
			if (originMesh.calc_face_area(*fi) <= m_Toler.m_Linear) {

				originMesh.delete_face(*fi);				

			}

		}

		originMesh.garbage_collection();
	}




}
