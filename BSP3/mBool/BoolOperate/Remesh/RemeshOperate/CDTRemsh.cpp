#include "mPch.h"
#include "RemeshOperate.h"
#include "CDTRemsh.h"


CDTRemesh::CDTRemesh(Mesh& p_Mesh, MeshIntersectResult& p_RecordMap, RemeshNewVertexRecord& p_ReturnNewVertex, const Tolerance& p_Tolerence/* = Tolerance()*/):
	RemshOperate(p_Mesh, p_RecordMap, p_ReturnNewVertex,p_Tolerence)
{

}


bool CDTRemesh::ProcessConstrainedEdge(PathSegment* p_PathSeg, TriangleArgs& p_TriArgs)
{
	//维护p_TriArgs.m_CheckingEdges


	if (!HasEdge(p_PathSeg, p_TriArgs)) {

		unordered_set<size_t> intersectEdges = FindIntersectEdge(p_PathSeg, p_TriArgs);

		if (1 == intersectEdges.size()) {
			//交换对边

			TestTriangle(p_TriArgs);

			Mesh::EdgeHandle interEdge =  m_OriginalMesh.edge_handle(*intersectEdges.begin());
			//MeshEdge* newEdge = m_OriginalMesh.Swap(*intersectEdges.front(), true, &deletedTriMesh,&newTriMesh);

			Mesh::HalfedgeHandle h1 = m_OriginalMesh.halfedge_handle(interEdge, 0);
			Mesh::HalfedgeHandle h2 = m_OriginalMesh.halfedge_handle(interEdge, 1);

			Mesh::Point p1 = m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(h1));
			Mesh::Point p2 = m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(h2));

			assert((!m_OriginalMesh.is_boundary(h1)) && (!m_OriginalMesh.is_boundary(h2)));

			{
				Mesh::FaceHandle f1 = m_OriginalMesh.face_handle(h1);			
				Mesh::FaceHandle f2 = m_OriginalMesh.face_handle(h2);

				p_TriArgs.m_NewTriangles.erase(f1.idx());
				p_TriArgs.m_NewTriangles.erase(f2.idx());

				Mesh::FaceEdgeIter fe1 = m_OriginalMesh.fe_begin(f1);
				Mesh::FaceEdgeIter fe2 = m_OriginalMesh.fe_begin(f2);

				p_TriArgs.m_CheckingEdges.erase((*(fe1++)).idx());
				p_TriArgs.m_CheckingEdges.erase((*(fe1++)).idx());
				p_TriArgs.m_CheckingEdges.erase((*(fe1)).idx());

				p_TriArgs.m_CheckingEdges.erase((*(fe2++)).idx());
				p_TriArgs.m_CheckingEdges.erase((*(fe2++)).idx());
				p_TriArgs.m_CheckingEdges.erase((*(fe2)).idx());
			}

			//Mesh::VertexHandle newEdgeBegin = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(h1));
			//Mesh::VertexHandle newEdgeEnd = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(h2));

			m_OriginalMesh.flip(interEdge);
			assert(m_OriginalMesh.is_flip_ok(interEdge));

			{
				Mesh::HalfedgeHandle newH1 = m_OriginalMesh.halfedge_handle(interEdge, 0);
				Mesh::HalfedgeHandle newH2 = m_OriginalMesh.halfedge_handle(interEdge, 1);

				p_TriArgs.m_NewTriangles.insert(m_OriginalMesh.face_handle(newH1).idx());
				p_TriArgs.m_NewTriangles.insert(m_OriginalMesh.face_handle(newH2).idx());

						
				Mesh::FaceEdgeIter fe1 = m_OriginalMesh.fe_begin(m_OriginalMesh.face_handle(newH1));
				Mesh::FaceEdgeIter fe2 = m_OriginalMesh.fe_begin(m_OriginalMesh.face_handle(newH2));

				p_TriArgs.m_CheckingEdges.insert((*(fe1++)).idx());
				p_TriArgs.m_CheckingEdges.insert((*(fe1++)).idx());
				p_TriArgs.m_CheckingEdges.insert((*(fe1)).idx());

				p_TriArgs.m_CheckingEdges.insert((*(fe2++)).idx());
				p_TriArgs.m_CheckingEdges.insert((*(fe2++)).idx());
				p_TriArgs.m_CheckingEdges.insert((*(fe2)).idx());

				TestTriangle(p_TriArgs);
			}

			//维护输出表
			m_PathEdgeRecord[interEdge.idx()] = p_PathSeg->GetNormal();

		}
		else if(1 < intersectEdges.size()){
			//搜索外侧多边形包围盒
			//增长圆获取三角化

			TestTriangle(p_TriArgs);

			size_t newEdge = ProcessMultiIntersectEdges(p_PathSeg, intersectEdges, p_TriArgs);

			TestTriangle(p_TriArgs);

			//维护输出表
			m_PathEdgeRecord[newEdge] = p_PathSeg->GetNormal();
		}
		else {
			
			cout << "No IntersectEdges! Size = " << intersectEdges.size() << endl;
		}
		
		
	}

	return false;
}


bool CDTRemesh::ProcessOnEdge(Point3dTriangle3dRelation& rela, FaceId p_Triangle, MeshIntersectVertex* p_MeshVertex, TriangleArgs& p_Args)
{

	//cout << "all faces" << m_OriginalMesh.n_faces()<< endl;
	assert( (p_Triangle >= 0) &&  (p_Triangle <= m_OriginalMesh.n_faces()));

	TestTriangle(p_Args);

	Mesh::FaceHandle oldFace = m_OriginalMesh.face_handle(p_Triangle);
	auto meshVertexPos = this->m_NewVertexs.find(p_MeshVertex);
	
	Mesh::FaceEdgeCCWIter feccwIter = m_OriginalMesh.fe_ccwbegin(oldFace);
	p_Args.m_NewTriangles.erase(p_Triangle);
	p_Args.m_CheckingEdges.erase((*(feccwIter++)).idx());
	p_Args.m_CheckingEdges.erase((*(feccwIter++)).idx());
	p_Args.m_CheckingEdges.erase((*(feccwIter++)).idx());

	TestTriangle(p_Args);

	Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(oldFace);
	Mesh::VertexHandle mV0 = *(fvccwIter++);
	Mesh::VertexHandle mV1 = *(fvccwIter++);
	Mesh::VertexHandle mV2 = *(fvccwIter);


	//bool fe = m_OriginalMesh.is_valid_handle(oldFace);
	//m_OriginalMesh.delete_face(oldFace, false);

	
	//新增点
	Mesh::VertexHandle mvb;

	if (meshVertexPos == this->m_NewVertexs.end()) {

		mvb = m_OriginalMesh.add_vertex(
			Mesh::Point( 
			(*(p_MeshVertex->m_Point3d))[0],
			(*(p_MeshVertex->m_Point3d))[1],
			(*(p_MeshVertex->m_Point3d))[2]
		)
		);
			
		TestTriangle(p_Args);

		this->m_NewVertexs[p_MeshVertex] = mvb.idx();
		
		cout << "new" << endl;
	}
	else {			
		size_t thisVertex = this->m_NewVertexs[p_MeshVertex];
		mvb = m_OriginalMesh.vertex_handle(thisVertex);
		cout << "origin" << endl;
		TestTriangle(p_Args);

		//????????????
		return false;
	}


	Mesh::FaceHandle newTri1;
	Mesh::FaceHandle newTri2;
	Mesh::FaceHandle newTri3;
	Mesh::FaceHandle newTri4;

	//m_OriginalMesh.delete_face(oldFace, false);

	if (0 == rela.m_OnElmentIndex) {

		Mesh::HalfedgeHandle splitEdge = m_OriginalMesh.find_halfedge(mV0, mV1);
		Mesh::HalfedgeHandle oppoSplitEdge = m_OriginalMesh.opposite_halfedge_handle(splitEdge);




		assert(!m_OriginalMesh.is_boundary(splitEdge));
		//assert(!m_OriginalMesh.is_boundary(oppoSplitEdge));

		Mesh::FaceHandle fh1 = m_OriginalMesh.face_handle(splitEdge);



		assert(fh1 == oldFace);

		//{
		//	cout << "fh2" << endl;
		//	cout << "MeshPtr: " << &m_OriginalMesh<< endl;
		//	auto triMath = TranslateToTriangle({ fh2,&m_OriginalMesh });
		//	cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
		//	cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
		//	cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
		//	cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
		//	cout << endl;
		//}





		m_OriginalMesh.delete_face(fh1, false);




		{
			vector<Mesh::VertexHandle> faceHandle;

			faceHandle.push_back(mV0);
			faceHandle.push_back(mvb);
			faceHandle.push_back(mV2);
			newTri1 = m_OriginalMesh.add_face(faceHandle);

			faceHandle.clear();
			faceHandle.push_back(mvb);
			faceHandle.push_back(mV1);
			faceHandle.push_back(mV2);
			newTri2 = m_OriginalMesh.add_face(faceHandle);
		}

		if(!m_OriginalMesh.is_boundary(oppoSplitEdge)); {

			Mesh::VertexHandle mVNeighbor = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(oppoSplitEdge));
			Mesh::FaceHandle fh2 = m_OriginalMesh.face_handle(oppoSplitEdge);

			if (fh2.is_valid()) {
				m_OriginalMesh.delete_face(fh2, false);

				vector<Mesh::VertexHandle> faceHandle;

				faceHandle.push_back(mV0);
				faceHandle.push_back(mVNeighbor);
				faceHandle.push_back(mvb);
				newTri3 = m_OriginalMesh.add_face(faceHandle);

				faceHandle.clear();
				faceHandle.push_back(mvb);
				faceHandle.push_back(mVNeighbor);
				faceHandle.push_back(mV1);
				newTri4 = m_OriginalMesh.add_face(faceHandle);

				assert(m_OriginalMesh.is_valid_handle(newTri3));
				assert(m_OriginalMesh.is_valid_handle(newTri4));


				//若t3，t4不属于该大三角形的记录，则增加新记录			
				if (p_Args.m_NewTriangles.find(fh2.idx()) != p_Args.m_NewTriangles.end()) {
					p_Args.m_NewTriangles.erase(fh2.idx());
					p_Args.m_NewTriangles.insert(newTri3.idx());
					p_Args.m_NewTriangles.insert(newTri4.idx());
				}
				else {
					auto fh2Record = m_ProcessingTriangles.find(fh2.idx());

					if (fh2Record != m_ProcessingTriangles.end()) {
						MeshTriangleIntersectRecord* record = (*fh2Record).second;

						record->hasChanged = true;
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;

					}
					else {
						auto fh2NewRecord = m_NewTrianglesMap2Record.find(fh2.idx());
						assert(fh2NewRecord != m_NewTrianglesMap2Record.end());

						MeshTriangleIntersectRecord* record = (*fh2NewRecord).second;

						record->m_NewTriangles.erase(fh2.idx());
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;
					}
				}
			}





			

		}



	}
	else if (1 == rela.m_OnElmentIndex) {

		Mesh::HalfedgeHandle splitEdge = m_OriginalMesh.find_halfedge(mV1, mV2);
		Mesh::HalfedgeHandle oppoSplitEdge = m_OriginalMesh.opposite_halfedge_handle(splitEdge);




		assert(!m_OriginalMesh.is_boundary(splitEdge));
		//assert(!m_OriginalMesh.is_boundary(oppoSplitEdge));

		Mesh::FaceHandle fh1 = m_OriginalMesh.face_handle(splitEdge);



		assert(fh1 == oldFace);

		m_OriginalMesh.delete_face(fh1, false);




		{
			vector<Mesh::VertexHandle> faceHandle;

			faceHandle.push_back(mV0);
			faceHandle.push_back(mV1);
			faceHandle.push_back(mvb);
			newTri1 = m_OriginalMesh.add_face(faceHandle);

			faceHandle.clear();
			faceHandle.push_back(mvb);
			faceHandle.push_back(mV2);
			faceHandle.push_back(mV0);
			newTri2 = m_OriginalMesh.add_face(faceHandle);
		}

		if (!m_OriginalMesh.is_boundary(oppoSplitEdge)) {

			Mesh::VertexHandle mVNeighbor = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(oppoSplitEdge));
			Mesh::FaceHandle fh2 = m_OriginalMesh.face_handle(oppoSplitEdge);

			if (fh2.is_valid()) {
				m_OriginalMesh.delete_face(fh2, false);

				vector<Mesh::VertexHandle> faceHandle;

				faceHandle.push_back(mvb);
				faceHandle.push_back(mV1);
				faceHandle.push_back(mVNeighbor);
				newTri3 = m_OriginalMesh.add_face(faceHandle);

				faceHandle.clear();
				faceHandle.push_back(mV2);
				faceHandle.push_back(mvb);
				faceHandle.push_back(mVNeighbor);
				newTri4 = m_OriginalMesh.add_face(faceHandle);

				assert(m_OriginalMesh.is_valid_handle(newTri3));
				assert(m_OriginalMesh.is_valid_handle(newTri4));



				//若t3，t4不属于该大三角形的记录，则增加新记录			
				if (p_Args.m_NewTriangles.find(fh2.idx()) != p_Args.m_NewTriangles.end()) {
					p_Args.m_NewTriangles.erase(fh2.idx());
					p_Args.m_NewTriangles.insert(newTri3.idx());
					p_Args.m_NewTriangles.insert(newTri4.idx());
				}
				else {
					auto fh2Record = m_ProcessingTriangles.find(fh2.idx());

					if (fh2Record != m_ProcessingTriangles.end()) {
						MeshTriangleIntersectRecord* record = (*fh2Record).second;

						record->hasChanged = true;
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;

					}
					else {
						auto fh2NewRecord = m_NewTrianglesMap2Record.find(fh2.idx());
						assert(fh2NewRecord != m_NewTrianglesMap2Record.end());

						MeshTriangleIntersectRecord* record = (*fh2NewRecord).second;

						record->m_NewTriangles.erase(fh2.idx());
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;
					}
				}
			
			}
			


		}


		//vector<Mesh::VertexHandle> faceHandle;

		//faceHandle.push_back(mV0);
		//faceHandle.push_back(mV1);
		//faceHandle.push_back(mvb);
		//newTri1 = m_OriginalMesh.add_face(faceHandle);
		//if (!m_OriginalMesh.is_valid_handle(newTri1)) {
		//	faceHandle.clear();
		//	faceHandle.push_back(mV1);
		//	faceHandle.push_back(mV0);
		//	faceHandle.push_back(mvb);
		//	newTri1 = m_OriginalMesh.add_face(faceHandle);

		//	//cout << " t1 v0 v1 vb" << endl;
		//}
		//else { cout << " t1 v1 v0 vb" << endl; }

		//if (newTri1.idx() == 5052) {
		//	Mesh::HalfedgeHandle h0 = m_OriginalMesh.find_halfedge(mV2, mV0);
		//	Mesh::HalfedgeHandle h1 = m_OriginalMesh.find_halfedge(mV2, mvb);
		//	Mesh::HalfedgeHandle h2 = m_OriginalMesh.find_halfedge(mvb, mV0);

		//	Mesh::HalfedgeHandle h3 = m_OriginalMesh.find_halfedge(mV1, mV0);
		//	Mesh::HalfedgeHandle h4 = m_OriginalMesh.find_halfedge(mV1, mvb);

		//	bool ta = m_OriginalMesh.is_boundary(h0);
		//	bool tb = m_OriginalMesh.is_boundary(m_OriginalMesh.opposite_halfedge_handle(h0));

		//	bool tc = m_OriginalMesh.is_boundary(h1);
		//	bool td = m_OriginalMesh.is_boundary(m_OriginalMesh.opposite_halfedge_handle(h1));

		//	{
		//		Mesh::FaceHandle fh = m_OriginalMesh.face_handle(m_OriginalMesh.opposite_halfedge_handle(h1));
		//		auto triMath = TranslateToTriangle({ fh,&m_OriginalMesh });

		//		cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
		//		cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
		//		cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
		//		cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
		//		cout << endl;
		//	}

		//	bool te = m_OriginalMesh.is_boundary(h2);
		//	bool tf = m_OriginalMesh.is_boundary(m_OriginalMesh.opposite_halfedge_handle(h2));

		//	bool tg = m_OriginalMesh.is_boundary(h3);
		//	bool th = m_OriginalMesh.is_boundary(m_OriginalMesh.opposite_halfedge_handle(h3));

		//	bool ti = m_OriginalMesh.is_boundary(h4);
		//	bool tj = m_OriginalMesh.is_boundary(m_OriginalMesh.opposite_halfedge_handle(h4));

		//	Mesh::HalfedgeHandle he = m_OriginalMesh.find_halfedge(mV2, mV0);
		//}

		//faceHandle.clear();
		//faceHandle.push_back(mV2);
		//faceHandle.push_back(mV0);
		//faceHandle.push_back(mvb);
		//newTri2 = m_OriginalMesh.add_face(faceHandle);



		//if (!m_OriginalMesh.is_valid_handle(newTri2)) {
		//	faceHandle.clear();
		//	faceHandle.push_back(mvb);
		//	faceHandle.push_back(mV0);
		//	faceHandle.push_back(mV2);
		//	newTri2 = m_OriginalMesh.add_face(faceHandle);

		//	//cout << " t2 vb v0 v2" << endl;
		//}
		//else { cout << " t2 vb v2 v0" << endl; }


	}
	else if (2 == rela.m_OnElmentIndex) {

		Mesh::HalfedgeHandle splitEdge = m_OriginalMesh.find_halfedge(mV2, mV0);
		Mesh::HalfedgeHandle oppoSplitEdge = m_OriginalMesh.opposite_halfedge_handle(splitEdge);

		auto fhI = m_OriginalMesh.fh_ccwbegin(oldFace);
		Mesh::HalfedgeHandle h1 = *(fhI++);
		Mesh::HalfedgeHandle h2 = *(fhI++);
		Mesh::HalfedgeHandle h3 = *(fhI++);


		//bool t1 = m_OriginalMesh.is_boundary(h1);
		//bool t2 = m_OriginalMesh.is_boundary(h2);
		//bool t3 = m_OriginalMesh.is_boundary(h3);
		//bool t4 = m_OriginalMesh.is_boundary(oldFace);



		//auto triMath = TranslateToTriangle({ oldFace,&m_OriginalMesh });
		//cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
		//cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
		//cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
		//cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
		//cout << endl;

		Mesh::FaceHandle fh1 = m_OriginalMesh.face_handle(h1);
		//Mesh::FaceHandle fh1 = m_OriginalMesh.face_handle(splitEdge);



		//triMath = TranslateToTriangle({ fh1,&m_OriginalMesh });
		//cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
		//cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
		//cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
		//cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
		//cout << endl;


		assert(!m_OriginalMesh.is_boundary(splitEdge));
		//assert(!m_OriginalMesh.is_boundary(oppoSplitEdge));

		assert(fh1 == oldFace);

		m_OriginalMesh.delete_face(fh1, false);




		{
			vector<Mesh::VertexHandle> faceHandle;

			faceHandle.push_back(mV0);
			faceHandle.push_back(mV1);
			faceHandle.push_back(mvb);
			newTri1 = m_OriginalMesh.add_face(faceHandle);

			faceHandle.clear();
			faceHandle.push_back(mvb);
			faceHandle.push_back(mV1);
			faceHandle.push_back(mV2);
			newTri2 = m_OriginalMesh.add_face(faceHandle);
		}

		if(!m_OriginalMesh.is_boundary(oppoSplitEdge)); {

		
			Mesh::VertexHandle mVNeighbor = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(oppoSplitEdge));
			Mesh::FaceHandle fh2 = m_OriginalMesh.face_handle(oppoSplitEdge);

			if (fh2.is_valid()) {
			
				m_OriginalMesh.delete_face(fh2, false);

				vector<Mesh::VertexHandle> faceHandle;

				faceHandle.push_back(mvb);
				faceHandle.push_back(mV2);
				faceHandle.push_back(mVNeighbor);
				newTri3 = m_OriginalMesh.add_face(faceHandle);

				faceHandle.clear();
				faceHandle.push_back(mV0);
				faceHandle.push_back(mvb);
				faceHandle.push_back(mVNeighbor);
				newTri4 = m_OriginalMesh.add_face(faceHandle);


				assert(m_OriginalMesh.is_valid_handle(newTri3));
				assert(m_OriginalMesh.is_valid_handle(newTri4));



				//若t3，t4不属于该大三角形的记录，则增加新记录			
				if (p_Args.m_NewTriangles.find(fh2.idx()) != p_Args.m_NewTriangles.end()) {
					p_Args.m_NewTriangles.erase(fh2.idx());
					p_Args.m_NewTriangles.insert(newTri3.idx());
					p_Args.m_NewTriangles.insert(newTri4.idx());
				}
				else {
					auto fh2Record = m_ProcessingTriangles.find(fh2.idx());

					if (fh2Record != m_ProcessingTriangles.end()) {
						MeshTriangleIntersectRecord* record = (*fh2Record).second;

						record->hasChanged = true;
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;

					}
					else {
						auto fh2NewRecord = m_NewTrianglesMap2Record.find(fh2.idx());
						assert(fh2NewRecord != m_NewTrianglesMap2Record.end());

						MeshTriangleIntersectRecord* record = (*fh2NewRecord).second;

						record->m_NewTriangles.erase(fh2.idx());
						record->m_NewTriangles.insert(newTri3.idx());
						record->m_NewTriangles.insert(newTri4.idx());

						m_NewTrianglesMap2Record[newTri3.idx()] = record;
						m_NewTrianglesMap2Record[newTri4.idx()] = record;
					}

				}
			}

		}
			//vector<Mesh::VertexHandle> faceHandle;

			//faceHandle.push_back(mV0);
			//faceHandle.push_back(mV1);
			//faceHandle.push_back(mvb);
			////newTri1 = m_OriginalMesh.add_face(faceHandle);
			////if (!m_OriginalMesh.is_valid_handle(newTri1)) {
			////	faceHandle.clear();
			////	faceHandle.push_back(mV1);
			////	faceHandle.push_back(mV0);
			////	faceHandle.push_back(mvb);
			////	newTri1 = m_OriginalMesh.add_face(faceHandle);

			////	//cout << " t1 v1 v0 vb" << endl;
			////}
			////else { cout << " t1 v0 v1 vb" << endl; }

			//faceHandle.clear();
			//faceHandle.push_back(mvb);
			//faceHandle.push_back(mV1);
			//faceHandle.push_back(mV2);
			//newTri2 = m_OriginalMesh.add_face(faceHandle);


			////if (!m_OriginalMesh.is_valid_handle(newTri2)) {
			////	faceHandle.clear();
			////	faceHandle.push_back(mvb);
			////	faceHandle.push_back(mV2);
			////	faceHandle.push_back(mV1);
			////	newTri2 = m_OriginalMesh.add_face(faceHandle);

			////	//cout << " t2 vb v2 v1" << endl;
			////}
			////else { cout << " t2 vb v1 v2" << endl; }
	}
	else {
		assert(false);
	}

	assert(m_OriginalMesh.is_valid_handle(newTri1));
	assert(m_OriginalMesh.is_valid_handle(newTri2));


	p_Args.m_NewTriangles.insert(newTri1.idx());
	p_Args.m_NewTriangles.insert(newTri2.idx());

	//维护unordered_set<MeshEdge*> m_CheckingEdges
	Mesh::FaceEdgeCCWIter triOnefeccwIter = m_OriginalMesh.fe_ccwbegin(newTri1);
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx()); 


	triOnefeccwIter = m_OriginalMesh.fe_ccwbegin(newTri2);
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
	p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());

	TestTriangle(p_Args);

	

	return true;
}

bool CDTRemesh::ProcessIn(Point3dTriangle3dRelation& rela, FaceId p_Triangle, MeshIntersectVertex* p_MeshVertex, TriangleArgs& p_Args)
{
	assert( (p_Triangle >=0) && (p_Triangle <= m_OriginalMesh.n_faces()) );

	TestTriangle(p_Args);

	Mesh::FaceHandle oldFace = m_OriginalMesh.face_handle(p_Triangle);
	auto meshVertexPos = this->m_NewVertexs.find(p_MeshVertex);

	Mesh::FaceEdgeCCWIter feccwIter = m_OriginalMesh.fe_ccwbegin(oldFace);

	p_Args.m_NewTriangles.erase(p_Triangle);
	p_Args.m_CheckingEdges.erase((*(feccwIter++)).idx());
	p_Args.m_CheckingEdges.erase((*(feccwIter++)).idx());
	p_Args.m_CheckingEdges.erase((*(feccwIter)).idx());

	//{	
	//	cout << "======================" << endl;
	//	cout << *(p_MeshVertex->m_Point3d) << endl;
	//	Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(oldFace);

	//	cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;
	//	cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;
	//	cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;
	//	cout << "======================" << endl;
	//}


	TestTriangle(p_Args);

	Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(oldFace);
	Mesh::VertexHandle mV0 = *(fvccwIter++);
	Mesh::VertexHandle mV1 = *(fvccwIter++);
	Mesh::VertexHandle mV2 = *(fvccwIter);

	m_OriginalMesh.delete_face(oldFace, false);
	//m_OriginalMesh.garbage_collection();

	//新增点
	Mesh::VertexHandle mvb;

	if (meshVertexPos == this->m_NewVertexs.end()) {

		mvb = m_OriginalMesh.add_vertex(
			Mesh::Point(
				(*(p_MeshVertex->m_Point3d))[0],
				(*(p_MeshVertex->m_Point3d))[1],
				(*(p_MeshVertex->m_Point3d))[2]
			)
		);

		TestTriangle(p_Args);

		this->m_NewVertexs[p_MeshVertex] = mvb.idx();

	}
	else {
		size_t thisVertex = this->m_NewVertexs[p_MeshVertex];
		mvb = m_OriginalMesh.vertex_handle(thisVertex);

		TestTriangle(p_Args);
	}

	TestTriangle(p_Args);
	

	{
		vector<Mesh::VertexHandle> faceHandle;

		//faceHandle.push_back(mV0);
		//faceHandle.push_back(mvb);
		//faceHandle.push_back(mV1);
		//m_OriginalMesh.add_face(faceHandle);

		//faceHandle.clear();
		//faceHandle.push_back(mvb);
		//faceHandle.push_back(mV0);
		//faceHandle.push_back(mV2);
		//m_OriginalMesh.add_face(faceHandle);

		//faceHandle.clear();
		//faceHandle.push_back(mvb);
		//faceHandle.push_back(mV2);
		//faceHandle.push_back(mV1);
		//m_OriginalMesh.add_face(faceHandle);

		faceHandle.push_back(mV1);
		faceHandle.push_back(mvb);
		faceHandle.push_back(mV0);
		m_OriginalMesh.add_face(faceHandle);

		faceHandle.clear();
		faceHandle.push_back(mvb);
		faceHandle.push_back(mV2);
		faceHandle.push_back(mV0);
		m_OriginalMesh.add_face(faceHandle);

		faceHandle.clear();
		faceHandle.push_back(mvb);
		faceHandle.push_back(mV1);
		faceHandle.push_back(mV2);
		m_OriginalMesh.add_face(faceHandle);
	}

	//m_OriginalMesh.split(oldFace, mvb);

 	TestTriangle(p_Args);

	for (Mesh::VertexFaceIter vf = m_OriginalMesh.vf_begin(mvb);vf != m_OriginalMesh.vf_end(mvb); ++vf) {
		Mesh::FaceHandle newTri = *vf;

		p_Args.m_NewTriangles.insert(newTri.idx());
		
		assert(m_OriginalMesh.is_valid_handle(newTri));

		//维护unordered_set<MeshEdge*> m_CheckingEdges
		Mesh::FaceEdgeCCWIter triOnefeccwIter = m_OriginalMesh.fe_ccwbegin(newTri);
		p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
		p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
		p_Args.m_CheckingEdges.insert((*(triOnefeccwIter++)).idx());
	}


	TestTriangle(p_Args);

	return true;

}

bool CDTRemesh::HasEdge(PathSegment* p_PathSeg, TriangleArgs& p_Args) {
	
	size_t m_CorrespondingBegin =
		this->m_NewVertexs.at(p_PathSeg->m_Begin);
	size_t m_CorrespondingEnd =
		this->m_NewVertexs.at(p_PathSeg->m_End);

	Mesh::VertexHandle vBegin = m_OriginalMesh.vertex_handle(m_CorrespondingBegin);
	Mesh::VertexHandle vEnd = m_OriginalMesh.vertex_handle(m_CorrespondingEnd);

	TestTriangle(p_Args);
	
	Mesh::HalfedgeHandle testHandle = m_OriginalMesh.find_halfedge(vBegin, vEnd);

	if (m_OriginalMesh.is_valid_handle(testHandle)) {

		//维护输出表
		m_PathEdgeRecord[m_OriginalMesh.edge_handle(testHandle).idx()] = p_PathSeg->GetNormal();

		return true;	
	}

	return false;
}

unordered_set<size_t> CDTRemesh::FindIntersectEdge(PathSegment* p_PathSeg, TriangleArgs& p_TriArgs)
{

	//cout << "new FInd==============================" << endl;
	//相交边
	unordered_set<size_t> intersectEdges;

	//路径段起点终点
	VertexId pathBegin = this->m_NewVertexs.at(p_PathSeg->m_Begin);
	VertexId pathEnd = this->m_NewVertexs.at(p_PathSeg->m_End);

	Mesh::VertexHandle vBegin = m_OriginalMesh.vertex_handle(pathBegin);
	Mesh::VertexHandle vEnd = m_OriginalMesh.vertex_handle(pathEnd);

	//cout << "pathBegin" << pathBegin << endl;
	//cout << "pathEnd" << pathEnd << endl;

	//cout << "Path edge: " << endl;
	//cout << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathBegin))[0] << " " << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathBegin))[1] << " " << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathBegin))[2] << endl;
	//cout << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathEnd))[0] << " " << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathEnd))[1] << " " << m_OriginalMesh.point(m_OriginalMesh.vertex_handle(pathEnd))[2] << endl;
	//cout << endl;

	//int i = 0;
	for (auto& tri : p_TriArgs.m_NewTriangles) {
		//i++;
		
		//cout << "TriAngle: " << i << endl;

		Mesh::FaceHandle fh = m_OriginalMesh.face_handle(tri);
		Mesh::FaceHalfedgeCCWIter edgeHandle = m_OriginalMesh.fh_ccwbegin(fh);

		for (int i = 0; i < 3; i++){

			Mesh::HalfedgeHandle edge = *(edgeHandle++);
			
			//if (m_OriginalMesh.edge_handle(edge).idx() == 315) {
			//	Mesh::FaceHandle fh3 = m_OriginalMesh.face_handle(tri);
			//}


			assert(m_OriginalMesh.is_valid_handle(edge));

			if ( (m_OriginalMesh.from_vertex_handle(edge) == vBegin) || (m_OriginalMesh.to_vertex_handle(edge) == vBegin)
				||
				(m_OriginalMesh.from_vertex_handle(edge) == vEnd) || (m_OriginalMesh.to_vertex_handle(edge) == vEnd) )
			{
				continue;
			}
			else {

				Point3d pathB(m_OriginalMesh.point(vBegin)); 
				Point3d pathE(m_OriginalMesh.point(vEnd));

				Point3d edgeB(m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(edge)));
				Point3d	edgeE(m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(edge)));

				Vector3d pBeginPend(pathE - pathB);
				Vector3d pBeginEbegin(edgeB - pathB);
				Vector3d pBeginEend(edgeE - pathB);

				Vector3d eBeginEend(edgeE - edgeB);
				Vector3d EbeginPbegin(pathB - edgeB);
				Vector3d EbeginPend(pathE - edgeB);

				pBeginPend.normalize();
				pBeginEbegin.normalize();
				pBeginEend.normalize();
				eBeginEend.normalize();
				EbeginPbegin.normalize();
				EbeginPend.normalize();


				Vector3d cross1 = pBeginEbegin.cross(pBeginPend);
				Vector3d cross2 = pBeginEend.cross(pBeginPend);
				Vector3d cross3 = EbeginPbegin.cross(eBeginEend);
				Vector3d cross4 = EbeginPend.cross(eBeginEend);

				//cross1.normalize();
				//cross2.normalize();
				//cross3.normalize();
				//cross4.normalize();


				//cout << "==========" << endl;
				//cout << pBeginPend.length() << endl;
				//cout << pBeginEbegin.length() << endl;
				//cout << pBeginEend.length() << endl;
				//cout << eBeginEend.length() << endl;
				//cout << EbeginPbegin.length() << endl;
				//cout << EbeginPend.length() << endl;


				
				//cout << "From Vertex: " << m_OriginalMesh.from_vertex_handle(edge).idx() << endl;
				//cout << "To Vertex: " << m_OriginalMesh.to_vertex_handle(edge).idx() << endl;


				double le = cross1.length();
				cout << le << endl;
				le = cross2.length();
				cout << le << endl;
				le = cross3.length();
				cout << le << endl;
				le = cross4.length();
				cout << le << endl;

				if (IsZero(cross1) || IsZero(cross2) || IsZero(cross3) || IsZero(cross4)) continue;


				bool eTwoSides = !IsPositiveNotStrict(cross1.dot(cross2));
				bool pTwoSides = !IsPositiveNotStrict(cross3.dot(cross4));

				//cout << "t1: " << eTwoSides << endl;
				//cout << "t2: " << pTwoSides << endl;

				if (pTwoSides && eTwoSides) {

					//cout << "idx: " << m_OriginalMesh.edge_handle(edge).idx()<<endl;

					//cout << "From Vertex: " << m_OriginalMesh.from_vertex_handle(edge).idx() << endl;
					//cout << "To Vertex: " << m_OriginalMesh.to_vertex_handle(edge).idx() << endl;

					cout << m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(edge))[0] << " " << m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(edge))[1] << " " << m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(edge))[2] << endl;
					cout << m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(edge))[0] << " " << m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(edge))[1] << " " << m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(edge))[2] << endl;

					intersectEdges.insert( m_OriginalMesh.edge_handle(edge).idx() );
				}

			}




		}
		
		
	}

	//cout << "new FInd==============================End" << endl;
	return intersectEdges;
}

EdgeId CDTRemesh::ProcessMultiIntersectEdges(PathSegment* p_PathSeg, unordered_set<EdgeId>& p_InterEdges, TriangleArgs& p_Args)
{
	assert(1 < p_InterEdges.size());

	//获取求解起始点参数
	EdgeId beginEdge = -1;

	size_t testSize = m_OriginalMesh.n_edges();

	VertexId pathBeginVer = -1, pathEndVer = -1;

	Vector3d planeNormal;

	FindPathParameters(p_PathSeg, beginEdge, pathBeginVer, pathEndVer, planeNormal, p_InterEdges, p_Args);

	TestTriangle(p_Args);

	//初始化候选点集
	unordered_set<VertexId> candidateVertex;
	candidateVertex.insert(pathBeginVer);
	candidateVertex.insert(pathEndVer);

	for (auto& edge : p_InterEdges) {
		Mesh::HalfedgeHandle hh = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(edge), 0);

		cout << "candi: " << m_OriginalMesh.from_vertex_handle(hh).idx() << endl;
		cout << "candi: " << m_OriginalMesh.to_vertex_handle(hh).idx() << endl;

		candidateVertex.insert(m_OriginalMesh.from_vertex_handle(hh).idx());
		candidateVertex.insert(m_OriginalMesh.to_vertex_handle(hh).idx());
	}

	TestTriangle(p_Args);

	//获取上下（左右）路径序列
	vector<VertexId> leftPath, rightPath;

	FindLeftRightPath(beginEdge, pathBeginVer, pathEndVer, planeNormal, p_InterEdges, leftPath, rightPath, candidateVertex);

	TestTriangle(p_Args);

	//清理原始Mesh中三角形
	ClearIntersectTriangles(p_InterEdges,p_Args);

	TestTriangle(p_Args);

	//按照增长圆获取以左右路径为起点的三角形顶点组
	vector<array<VertexId,3>> newTriangles;

	GenerateNewTrianglesIndex(newTriangles, leftPath, { pathBeginVer, pathEndVer });
	GenerateNewTrianglesIndex(newTriangles, rightPath, { pathBeginVer, pathEndVer });

	TestTriangle(p_Args);

	//添加三角形,并返回第一个三角形信息
	FaceId firstTri = AddNewTriangles(newTriangles, planeNormal,p_Args);
	assert(0 <= firstTri);

	TestTriangle(p_Args);

	//找到路径对应的边
	Mesh::HalfedgeHandle rPath = m_OriginalMesh.find_halfedge(m_OriginalMesh.vertex_handle(pathBeginVer), m_OriginalMesh.vertex_handle(pathEndVer));
	assert(m_OriginalMesh.is_valid_handle(rPath));

	TestTriangle(p_Args);

	Mesh::EdgeHandle eh = m_OriginalMesh.edge_handle(rPath);

	TestTriangle(p_Args);

	return eh.idx();
}

void CDTRemesh::FindPathParameters(PathSegment* p_PathSeg, EdgeId& r_BeginEdge, VertexId& r_PathBegin, VertexId& r_PathEnd, Vector3d& r_Normal, unordered_set<EdgeId>& p_InterEdges, TriangleArgs& p_Args)
{

	r_PathBegin = m_NewVertexs[p_PathSeg->m_Begin];
	r_PathEnd = m_NewVertexs[p_PathSeg->m_End];

	Vector3d pathDirection(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathEnd)) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathBegin)));
	pathDirection.normalize();

	assert(IsZero(pathDirection.length() - 1));

	//距离起点最近距离和EdgeID
	double minDistance = DBL_MAX;
	EdgeId nearstEdgeId = -1;

	unordered_set<EdgeId> eraseEdges;

	for (auto& edge : p_InterEdges) {
		Mesh::HalfedgeHandle hh = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(edge), 0);
		Mesh::VertexHandle vh1 = m_OriginalMesh.from_vertex_handle(hh);
		Mesh::VertexHandle vh2 = m_OriginalMesh.to_vertex_handle(hh);
		
		Vector3d vec1(m_OriginalMesh.point(vh1) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathBegin)));
		Vector3d vec2(m_OriginalMesh.point(vh2) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathBegin)));
		
		double dot1 = vec1.dot(pathDirection);
		double dot2 = vec2.dot(pathDirection);

		double crossHeight1 = vec1.cross(pathDirection).length();
		double crossHeight2 = vec2.cross(pathDirection).length();

		if (dot1 > dot2) {
			swap(dot1, dot2);
			swap(crossHeight1, crossHeight2);
		}

		assert(dot1 <= dot2);
			

		double fromBeginDistance = dot1 + (dot2 - dot1) * crossHeight1 / (crossHeight1 + crossHeight2);

		//if (fromBeginDistance < 0) {

		//	Point3d pathB(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathBegin)));
		//	Point3d pathE(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathEnd)));

		//	Point3d edgeB(m_OriginalMesh.point(vh1));
		//	Point3d	edgeE(m_OriginalMesh.point(vh2));

		//	Vector3d pBeginPend(pathE - pathB);
		//	Vector3d pBeginEbegin(edgeB - pathB);
		//	Vector3d pBeginEend(edgeE - pathB);

		//	Vector3d eBeginEend(edgeE - edgeB);
		//	Vector3d EbeginPbegin(pathB - edgeB);
		//	Vector3d EbeginPend(pathE - edgeB);

		//	pBeginPend.normalize();
		//	pBeginEbegin.normalize();
		//	pBeginEend.normalize();
		//	eBeginEend.normalize();
		//	EbeginPbegin.normalize();
		//	EbeginPend.normalize();


		//	Vector3d cross1 = pBeginEbegin.cross(pBeginPend);
		//	Vector3d cross2 = pBeginEend.cross(pBeginPend);
		//	Vector3d cross3 = EbeginPbegin.cross(eBeginEend);
		//	Vector3d cross4 = EbeginPend.cross(eBeginEend);

		//	//cross1.normalize();
		//	//cross2.normalize();
		//	//cross3.normalize();
		//	//cross4.normalize();


		//	//cout << "==========" << endl;
		//	//cout << pBeginPend.length() << endl;
		//	//cout << pBeginEbegin.length() << endl;
		//	//cout << pBeginEend.length() << endl;
		//	//cout << eBeginEend.length() << endl;
		//	//cout << EbeginPbegin.length() << endl;
		//	//cout << EbeginPend.length() << endl;



		//	//cout << "From Vertex: " << m_OriginalMesh.from_vertex_handle(edge).idx() << endl;
		//	//cout << "To Vertex: " << m_OriginalMesh.to_vertex_handle(edge).idx() << endl;


		//	double le = cross1.length();
		//	cout << le << endl;
		//	le = cross2.length();
		//	cout << le << endl;
		//	le = cross3.length();
		//	cout << le << endl;
		//	le = cross4.length();
		//	cout << le << endl;

		//	if (IsZero(cross1) || IsZero(cross2) || IsZero(cross3) || IsZero(cross4)) continue;

		//	double le2 = cross1.dot(cross2);
		//	double le3 = cross3.dot(cross4);

		//	bool eTwoSides = !IsPositiveNotStrict(cross1.dot(cross2));
		//	bool pTwoSides = !IsPositiveNotStrict(cross3.dot(cross4));

		//	//cout << "t1: " << eTwoSides << endl;
		//	//cout << "t2: " << pTwoSides << endl;

		//	if (pTwoSides && eTwoSides) {
		//		
		//	}


		//}

		assert(IsPositive(fromBeginDistance));
		assert(IsPositive((m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathEnd)) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(r_PathBegin))).length() - fromBeginDistance));

		if (fromBeginDistance < minDistance) {
			minDistance = fromBeginDistance;
			nearstEdgeId = edge;
		}
	}

	//for (auto& edge : eraseEdges) {
	//	p_InterEdges.erase(edge);
	//}


	assert(nearstEdgeId >= 0);

	r_BeginEdge = nearstEdgeId;


	Mesh::HalfedgeHandle hh = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(nearstEdgeId), 0);
	Mesh::FaceVertexCCWIter fvccwIt = m_OriginalMesh.fv_ccwbegin(m_OriginalMesh.face_handle(hh));
	Mesh::VertexHandle triV0 = *(fvccwIt++);
	Mesh::VertexHandle triV1 = *(fvccwIt++);
	Mesh::VertexHandle triV2 = *(fvccwIt++);

	Triangle3d triMath(array<Point3d, 3>{
			Point3d(m_OriginalMesh.point(triV0)),
			Point3d(m_OriginalMesh.point(triV1)),
			Point3d(m_OriginalMesh.point(triV2))
	});

	r_Normal = triMath.GetNormal();


	assert(r_BeginEdge >= 0);
	assert(r_PathBegin >= 0);
	assert(r_PathEnd >= 0);

	assert(r_PathBegin <= m_OriginalMesh.n_vertices());
	assert(r_PathEnd <= m_OriginalMesh.n_vertices());
	assert(r_BeginEdge <= m_OriginalMesh.n_edges());


	//cout << "begin: " << r_PathBegin << endl;
	//cout << "end: " << r_PathBegin << endl;



	//Mesh::VertexHandle vh = m_OriginalMesh.vertex_handle(r_PathBegin);

	////搜索第一条与路径相交边，作为后续搜索多边形边界起点
	//unordered_set<VertexId> beginNeighbor;
	//for (Mesh::VertexVertexIter vvit = m_OriginalMesh.vv_begin(vh); vvit != m_OriginalMesh.vv_end(vh); ++vvit) {
	//	beginNeighbor.insert((*vvit).idx());	
	//	cout << "beginNeighbor: " << (*vvit).idx() << endl;
	//}


	//Mesh::VertexHandle vh2 = m_OriginalMesh.vertex_handle(r_PathEnd);
	//unordered_set<VertexId> endNeighbor;
	//for (Mesh::VertexVertexIter vvit = m_OriginalMesh.vv_begin(vh2); vvit != m_OriginalMesh.vv_end(vh2); ++vvit) {
	//	endNeighbor.insert((*vvit).idx());
	//	cout << "endNeighbor: " << (*vvit).idx() << endl;
	//}

	//bool hasFindBegin = false;
	//for (auto& edge : p_InterEdges) {
	//	Mesh::HalfedgeHandle hh = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(edge), 0);
	//	VertexId id1 = m_OriginalMesh.from_vertex_handle(hh).idx();
	//	VertexId id2 = m_OriginalMesh.to_vertex_handle(hh).idx();

	//	cout << id1 << "  " << id2 << endl;

	//	if ((beginNeighbor.find(id1) != beginNeighbor.end()) && (beginNeighbor.find(id2) != beginNeighbor.end())) {

	//		r_BeginEdge = edge;

	//		Mesh::FaceVertexCCWIter fvccwIt = m_OriginalMesh.fv_ccwbegin(m_OriginalMesh.face_handle(hh));
	//		Mesh::VertexHandle triV0 = *(fvccwIt++);
	//		Mesh::VertexHandle triV1 = *(fvccwIt++);
	//		Mesh::VertexHandle triV2 = *(fvccwIt++);

	//		Triangle3d triMath(array<Point3d, 3>{
	//				Point3d(m_OriginalMesh.point(triV0)),
	//				Point3d(m_OriginalMesh.point(triV1)),
	//				Point3d(m_OriginalMesh.point(triV2))
	//		});

	//		r_Normal = triMath.GetNormal();
	//		//r_Normal = m_OriginalMesh.normal(m_OriginalMesh.face_handle(hh));

	//		hasFindBegin = true;
	//		break;
	//	}
	//	
	//}

	////int s = m_OriginalMesh.n_edges();
	//assert(hasFindBegin);




}


void CDTRemesh::FindLeftRightPath(EdgeId p_BeginEdge, VertexId p_PathBegin, VertexId p_PathEnd, Vector3d& p_Normal, unordered_set<EdgeId>& p_InterEdges, vector<VertexId>& leftPath, vector<VertexId>& rightPath, unordered_set<VertexId>& p_CandidateVertex)
{

	 ////////////////
	for (auto& itr : p_CandidateVertex) {
		cout << "p_CandidateVertex: " << itr << endl;
	}
	 ////////////////

	leftPath.clear();
	rightPath.clear();

	leftPath.push_back(p_PathBegin);
	rightPath.push_back(p_PathBegin);

	p_CandidateVertex.erase(p_PathBegin);



	//MeshEdge pathEdge(p_PathBegin, p_PathEnd);		
	Vector3d pathDirection(Point3d(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_PathEnd))) 
		- Point3d(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_PathBegin))));


	//初始化路径起点
	Mesh::HalfedgeHandle h0 = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(p_BeginEdge), 0);

	if (IsLeft(p_Normal, pathDirection, 
		Point3d(m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(h0)) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_PathBegin))))) {


		 //////////////////
		cout << "addLeft: " << m_OriginalMesh.from_vertex_handle(h0).idx() << endl;
		cout << "addRight: " << m_OriginalMesh.to_vertex_handle(h0).idx() << endl;
		 //////////////////

		//左侧
		leftPath.push_back(m_OriginalMesh.from_vertex_handle(h0).idx());
		rightPath.push_back(m_OriginalMesh.to_vertex_handle(h0).idx());

		p_CandidateVertex.erase(m_OriginalMesh.from_vertex_handle(h0).idx());
		p_CandidateVertex.erase(m_OriginalMesh.to_vertex_handle(h0).idx());
	}
	else {

		 //////////////////
		cout << "addLeft: "  << m_OriginalMesh.to_vertex_handle(h0).idx() << endl;
		cout << "addRight: " << m_OriginalMesh.from_vertex_handle(h0).idx() << endl;
		 //////////////////

		//右侧
		leftPath.push_back(m_OriginalMesh.to_vertex_handle(h0).idx());
		rightPath.push_back(m_OriginalMesh.from_vertex_handle(h0).idx());

		p_CandidateVertex.erase(m_OriginalMesh.from_vertex_handle(h0).idx());
		p_CandidateVertex.erase(m_OriginalMesh.to_vertex_handle(h0).idx());
	}



	VertexId nowVertex = p_PathBegin;
	VertexId nextVertex = -1;

	VertexId candidateVertex1 = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(h0)).idx();
	VertexId candidateVertex2 = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(m_OriginalMesh.opposite_halfedge_handle(h0))).idx();
	
	if (candidateVertex1 == nowVertex) {
		nextVertex = candidateVertex2;
		assert(p_CandidateVertex.find(candidateVertex2) != p_CandidateVertex.end());

	}
	else if (candidateVertex2 == nowVertex) {
		nextVertex = candidateVertex1;	
		assert(p_CandidateVertex.find(candidateVertex1) != p_CandidateVertex.end());
	}
	else {
		assert(false);
	}

	assert(nextVertex != p_PathEnd);
	assert(nextVertex >= 0);


	while (nextVertex != p_PathEnd) {

		p_CandidateVertex.erase(nextVertex);

		if (IsLeft(p_Normal, pathDirection, 
			Point3d(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(nextVertex)) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_PathBegin)))
		)) {
			cout << "addLeft: " << nextVertex << endl;
			nowVertex = leftPath.back();
			leftPath.push_back(nextVertex);				
		}
		else {
			cout << "addRight: " << nextVertex << endl;
			nowVertex = rightPath.back();
			rightPath.push_back(nextVertex);

		}

		nextVertex = GetNextVertex(p_CandidateVertex, { leftPath.back(),rightPath.back() });

		if (nextVertex == p_PathEnd) {
			leftPath.push_back(nextVertex);
			rightPath.push_back(nextVertex);
			break;
		}


	}


		
}

void CDTRemesh::ClearIntersectTriangles(unordered_set<EdgeId> p_InterEdges, TriangleArgs& p_Args)
{
	unordered_set<FaceId> clearTriangles;

	for (auto& edge : p_InterEdges) {

		Mesh::HalfedgeHandle h1 = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(edge), 0);
		Mesh::HalfedgeHandle h2 = m_OriginalMesh.halfedge_handle(m_OriginalMesh.edge_handle(edge), 1);

		clearTriangles.insert(m_OriginalMesh.face_handle(h1).idx());
		clearTriangles.insert(m_OriginalMesh.face_handle(h2).idx());
			
	}

	assert(0 < clearTriangles.size());

	for (auto& tri : clearTriangles) {

		p_Args.m_NewTriangles.erase(tri);
		m_OriginalMesh.delete_face(m_OriginalMesh.face_handle(tri));
	}

}

void CDTRemesh::GenerateNewTrianglesIndex(vector<array<VertexId, 3>>& r_NewTriangles, vector<VertexId>& p_BoundList, pair<VertexId, VertexId> p_FirstEdge)
{
	//最小角余弦值
	double minAngle = 2;
	VertexId newTriVertex = -1;

	for (auto& ver : p_BoundList) {
		if ((ver == p_BoundList.front()) || (ver == p_BoundList.back())) {
			continue;
		}
			
		Vector3d direc1 = Vector3d( m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_FirstEdge.first)) - 
			m_OriginalMesh.point(m_OriginalMesh.vertex_handle(ver)));
		Vector3d direc2 = Vector3d(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(p_FirstEdge.second)) -
			m_OriginalMesh.point(m_OriginalMesh.vertex_handle(ver)));



		double angle = (direc1|direc2) /(direc1.length() * direc2.length());

		if (angle < minAngle) {
			minAngle = angle;
			newTriVertex = ver;				
		}

	}

	assert(0 <= newTriVertex);

	//添加大三角形
	r_NewTriangles.push_back({ p_FirstEdge.first,p_FirstEdge.second, newTriVertex });


	auto pos = find(p_BoundList.begin(), p_BoundList.end(), newTriVertex);

	vector<VertexId> sonList1(p_BoundList.begin(), pos + 1);
	vector<VertexId> sonList2(pos, p_BoundList.end());

	//处理list1
	if (sonList1.size() < 2) {
		assert(false);		

	}else if (sonList1.size() == 2) {
		//不添加,且list1不再递归
						
	}
	else if (sonList1.size() > 2) {
		pair<VertexId,VertexId> newBaseEdge(p_FirstEdge.first, newTriVertex);

		GenerateNewTrianglesIndex(r_NewTriangles, sonList1, newBaseEdge);

	}else {
		assert(false);		
	}


	//处理list2
	if (sonList2.size() < 2) {
		assert(false);

	}
	else if (sonList2.size() == 2) {
		//不添加,且list2不再递归

	}
	else if (sonList2.size() > 2) {
		pair<VertexId, VertexId> newBaseEdge(newTriVertex, p_FirstEdge.second);

		GenerateNewTrianglesIndex(r_NewTriangles, sonList2, newBaseEdge);

	}
	else {
		assert(false);
	}


}

bool CDTRemesh::IsLeft(Vector3d& p_NormalReference, Vector3d& p_DirectReference, Vector3d p_Calculate)
{
	auto resVec = p_DirectReference.cross(p_Calculate);
	resVec = resVec.normalize();
	p_NormalReference = p_NormalReference.normalize();

	
	if (IsZero(Vector3d(p_NormalReference + resVec),0.1)) {

		//cout << "Right,Zero, length " << Vector3d(p_NormalReference + resVec).length() << endl;
		//右
		return false;		
	}

	//cout << "Left,NoZero, length " << Vector3d(p_NormalReference + resVec).length() << endl;
	//左
	return true;
}

FaceId CDTRemesh::AddNewTriangles(vector<array<VertexId, 3>>& r_NewTriangles, Vector3d& p_Normal, TriangleArgs& p_Args)
{

	bool hasReturnFirstTriangle = false;

	FaceId r_Tri = -1;

	TestTriangle(p_Args);

	for (auto& tri : r_NewTriangles) {

		//cout << "=======================" << endl;
		TestTriangle(p_Args);

		//Mesh::Point p0 = m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[0]));
		//Mesh::Point p1 = m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[1]));
		//Mesh::Point p2 = m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[2]));

		//Vector3d d1(p2-p0);
		//Vector3d d2(p1-p0);
		//
		////Vector3d d3(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[2])) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[1])));
		////Vector3d d4(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[0])) - m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[1])));

		//Vector3d normalNow = (d1 % d2).normalize();
		//Vector3d normalNow2 = (d3 % d4).normalize();

		//Vector3d center((m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[2])) + m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[0])) + m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[1])))/3);
		//Vector3d d5(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[2])));
		//Vector3d d6(m_OriginalMesh.point(m_OriginalMesh.vertex_handle(tri[0])));
		//d5 = d5 - center;
		//d6 = d6 - center;
		//d5.normalize();
		//d6.normalize();
		//Vector3d normalNow3 = (d5 % d6).normalize();

		//cout << "testCross: " << (normalNow3 | normalNow2) << endl;

		//if ((normalNow | p_Normal) < 0) {
		//	VertexId temp = tri[0];
		//	tri[0] = tri[1];
		//	tri[1] = temp;

		//	cout << "reverse normal" << endl;
		//}

		vector<Mesh::VertexHandle> faceHandle;

		//cout << "addTri" << endl;
		Mesh::FaceHandle nnewT;


		faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[0]));
		faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[1]));
		faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[2]));
		Mesh::FaceHandle newT = m_OriginalMesh.add_face(faceHandle);

		if (m_OriginalMesh.is_valid_handle(newT)) {
			//cout << "valid1: " << ( (normalNow.normalize()) | (p_Normal.normalize()) ) << endl;
			//cout << "valid2: " << ((m_OriginalMesh.normal(newT).normalized()) | (p_Normal.normalize())) << endl;
			//cout << "valid3: " << ((m_OriginalMesh.normal(newT).normalized()) | (normalNow.normalize())) << endl;
			nnewT = newT;
		}
		else {
			VertexId temp = tri[0];
			tri[0] = tri[1];
			tri[1] = temp;
		
			faceHandle.clear();
			faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[0]));
			faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[1]));
			faceHandle.push_back(m_OriginalMesh.vertex_handle(tri[2]));
			Mesh::FaceHandle newT2 = m_OriginalMesh.add_face(faceHandle);

			//cout << "Invalid1: " << ((normalNow.normalize()) | (p_Normal.normalize())) << endl;
			//cout << "Invalid2: " << ((m_OriginalMesh.normal(newT2).normalized()) | (p_Normal.normalize())) << endl;
			//cout << "Invalid3: " << ((m_OriginalMesh.normal(newT2).normalized()) | (normalNow.normalize())) << endl;

			nnewT = newT2;
			assert(m_OriginalMesh.is_valid_handle(newT2));
		}
		

		FaceId newTriangle = nnewT.idx();

		p_Args.m_NewTriangles.insert(newTriangle);

		if (!hasReturnFirstTriangle) {
			r_Tri = newTriangle;
			hasReturnFirstTriangle = true;
		}

		TestTriangle(p_Args);

	}

	TestTriangle(p_Args);

	return r_Tri;
}

VertexId CDTRemesh::GetNextVertex(unordered_set<VertexId>& p_CandidateVertex, pair<VertexId, VertexId> p_AxisSymmetry)
{
	VertexId r_Vertex = -1;

	Mesh::HalfedgeHandle hh = m_OriginalMesh.find_halfedge(m_OriginalMesh.vertex_handle(p_AxisSymmetry.first), m_OriginalMesh.vertex_handle(p_AxisSymmetry.second));
	assert(m_OriginalMesh.is_valid_handle(hh));

	VertexId vertexCandi1 = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(hh)).idx();
	VertexId vertexCandi2 = m_OriginalMesh.to_vertex_handle(m_OriginalMesh.next_halfedge_handle(m_OriginalMesh.opposite_halfedge_handle(hh))).idx();

	auto pos1 = p_CandidateVertex.find(vertexCandi1);
	auto pos2 = p_CandidateVertex.find(vertexCandi2);

	if ((pos1 != p_CandidateVertex.end()) && (pos2 == p_CandidateVertex.end())) {
		r_Vertex = *pos1;
	}
	else if ((pos1 == p_CandidateVertex.end()) && (pos2 != p_CandidateVertex.end())) {
		r_Vertex = *pos2;
	}
	else {
		assert(false);	
	}


	assert(r_Vertex >= 0);

	return r_Vertex;
}

