#include "mPch.h"
#include "Classify.h"


void ClassifyOperate::ProcessMesh(Mesh& p_Mesh)
{
	TriangleSet triangles = InitialMesh(p_Mesh);

	RemeshPathEdgeRecord& pathRecord = m_Result.at(&p_Mesh);

	unordered_map<FaceId, bool>& classifyRecord = m_ClassifyResult[&p_Mesh];
	classifyRecord.clear();

	//TriangleSet nextWaveTriangles;

	m_NextWaveTriangles.clear();

	for (auto& path : pathRecord) {
		
		ProcessEdge(path.first, triangles, pathRecord, classifyRecord, p_Mesh);

	}

	int i = 0;
	while (!m_NextWaveTriangles.empty()) {

		//if (i == 5) break;
		i++;

		if (i % 3 == 0) {
			cout << "Times " << i << endl;
			cout << "nextSize" << m_NextWaveTriangles.size() << endl;
		}

		m_NowWaveTriangles.clear();
		m_NowWaveTriangles = m_NextWaveTriangles;

		m_NextWaveTriangles.clear();

		for (auto& f : m_NowWaveTriangles) {
			LableTriangle(f, classifyRecord, p_Mesh, triangles,pathRecord);
		}


		/*FaceId tri = *triangles.begin();
		LableIsolatedTriangle(tri, triangles,classifyRecord, p_Mesh);*/

	}


	//assert(true == triangles.empty());

	//while (!triangles.empty()) {

	//	FaceId tri = *triangles.begin();
	//	LableIsolatedTriangle(tri, triangles,classifyRecord, p_Mesh);

	//}

	//assert(true == triangles.empty());
}

ClassifyOperate::TriangleSet ClassifyOperate::InitialMesh(const Mesh& p_Mesh)
{
	TriangleSet r_triangles;

	for (Mesh::FaceIter fh = p_Mesh.faces_sbegin(); fh != p_Mesh.faces_end(); ++fh) {
		r_triangles.insert((*fh).idx());	
	}
	

	return r_triangles;
}

void ClassifyOperate::ProcessEdge(EdgeId p_Path, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh)
{
	Mesh::HalfedgeHandle h0 = p_Mesh.halfedge_handle(p_Mesh.edge_handle(p_Path), 0);
	if (p_Mesh.is_valid_handle(p_Mesh.face_handle(h0))) {
		LableTriangleOnTheEdge(p_Mesh.face_handle(h0).idx(), p_Triangles, p_pathRecord, p_ClassifyRecord, p_Mesh);
	}
	

	Mesh::HalfedgeHandle h1 = p_Mesh.opposite_halfedge_handle(h0);
	if (p_Mesh.is_valid_handle(p_Mesh.face_handle(h1))) {
		LableTriangleOnTheEdge(p_Mesh.face_handle(h1).idx(), p_Triangles, p_pathRecord, p_ClassifyRecord, p_Mesh);
	}
	
}

bool ClassifyOperate::LableTriangle(FaceId p_Triangle, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord) {

	if(IsLabeled(p_Triangle, p_ClassifyRecord)) return true;


	//遍历三条边,寻找路径边
	for (Mesh::FaceEdgeIter feI = p_Mesh.fe_begin(p_Mesh.face_handle(p_Triangle)); feI != p_Mesh.fe_end(p_Mesh.face_handle(p_Triangle)); ++feI) {
		RemeshPathEdgeRecord::iterator itr = p_pathRecord.find((*feI).idx());

		if (itr != p_pathRecord.end()) {
			assert(false);
			//在路径上
		}
	}


	unordered_set<FaceId> OneRingTri;

	bool hasFind = false;

	for (Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(p_Triangle)); ff != p_Mesh.ff_end(p_Mesh.face_handle(p_Triangle)); ++ff) {
		
		OneRingTri.insert((*ff).idx());

		if (p_ClassifyRecord.end() != p_ClassifyRecord.find( (*ff).idx() )) {

			//找到标记三角形
			p_ClassifyRecord[p_Triangle] = p_ClassifyRecord[(*ff).idx()];

			p_Triangles.erase(p_Triangle);

			hasFind = true;
		}
		
		
	}

	assert(hasFind);

	for (auto& f : OneRingTri) {
		bool condi1 = (m_NowWaveTriangles.end() == m_NowWaveTriangles.find(f));
		bool condi2 = (p_ClassifyRecord.end() == p_ClassifyRecord.find(f));

		if (condi1 && condi2) {
			m_NextWaveTriangles.insert(f);		
		}
	}


	return false;
}


bool ClassifyOperate::LableTriangleOnTheEdge(FaceId p_Triangle, TriangleSet& p_Triangles, RemeshPathEdgeRecord& p_pathRecord, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh)
{
	
	//已经有记录直接返回
	if (p_ClassifyRecord.end() != p_ClassifyRecord.find(p_Triangle)) return true;

	//遍历三条边,寻找路径边
	for (Mesh::FaceEdgeIter feI = p_Mesh.fe_begin(p_Mesh.face_handle(p_Triangle)); feI != p_Mesh.fe_end(p_Mesh.face_handle(p_Triangle)); ++feI) {
		RemeshPathEdgeRecord::iterator itr = p_pathRecord.find( (*feI).idx());

		if (itr != p_pathRecord.end()) {
			//在路径上

			Mesh::FaceVertexIter fv = p_Mesh.fv_begin(p_Mesh.face_handle(p_Triangle));

			Mesh::VertexHandle v0 = *(fv++);
			Mesh::VertexHandle v1 = *(fv++);
			Mesh::VertexHandle v2 = *(fv++);

			Vector3d judgeVector;

			Mesh::HalfedgeHandle path = p_Mesh.halfedge_handle(p_Mesh.edge_handle((*itr).first),0);
			Mesh::VertexHandle pathBegin = p_Mesh.from_vertex_handle(path);
			Mesh::VertexHandle pathEnd = p_Mesh.to_vertex_handle(path);

			if ((v0 != pathBegin) && (v0 != pathEnd)) {
				judgeVector = Vector3d(p_Mesh.point(v0) - p_Mesh.point(v2));
			
			}
			else if ((v1 != pathBegin) && (v1 != pathEnd)) {
				judgeVector = Vector3d(p_Mesh.point(v1) - p_Mesh.point(v2));
			
			}
			else if ((v2 != pathBegin) && (v2 != pathEnd)) {
				judgeVector = Vector3d(p_Mesh.point(v2) - p_Mesh.point(v1));
			
			}
			else {
				assert(false);
			}

			double dotRes = judgeVector | (itr->second);

			if (dotRes < 0) {
				//内部

				//cout << "label: " << p_Triangle << endl;

				p_ClassifyRecord[p_Triangle] = false;

			}
			else {
				p_ClassifyRecord[p_Triangle] = true;

			}

			//完成处理
			p_Triangles.erase(p_Triangle);

			//级联标记邻接三角形为下一轮待处理
			for (Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(p_Triangle)); ff != p_Mesh.ff_end(p_Mesh.face_handle(p_Triangle)); ++ff) {

				//标记邻接三角形为下一轮待处理
				if (!IsLabeled((*ff).idx(), p_ClassifyRecord)) {
					m_NextWaveTriangles.insert((*ff).idx());
				}

			}

			return true;
		}
	}

	assert(false);
	//该三角形不在路径边上
	//搜索邻接三角形，判断邻接三角形属性，使该三角形与其一致
	//for (Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(p_Triangle)); ff != p_Mesh.ff_end(p_Mesh.face_handle(p_Triangle)); ++ff) {

	//	//if (p_Mesh.status(*ff).deleted()) continue;;

	//	if (p_ClassifyRecord.end() != p_ClassifyRecord.find((*ff).idx())) {

	//		p_ClassifyRecord[p_Triangle] = p_ClassifyRecord[(*ff).idx()];
	//		p_Triangles.erase(p_Triangle);
	//		break;

	//	}
	//}

	//在这之前必须完成标记
	//assert(p_ClassifyRecord.end() != p_ClassifyRecord.find(p_Triangle));

	//for (Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(p_Triangle)); ff != p_Mesh.ff_end(p_Mesh.face_handle(p_Triangle)); ++ff) {
	//		LableTriangle((*ff).idx(), p_Triangles, p_pathRecord, p_ClassifyRecord,p_Mesh);
	//}


	return false;
}

bool ClassifyOperate::SearchLabeledTriangle(vector<FaceId> p_NowLevelTriangles, unordered_map<FaceId, bool>& p_ClassifyRecord, unordered_set<FaceId> p_SearchedTri, Mesh& p_Mesh)
{
	//搜索当前水平集三角形，判断邻接三角形是否标记
	vector<FaceId> p_NextLevelTriangles;

	for (auto& tri : p_NowLevelTriangles) {

		p_SearchedTri.insert(tri);

		if (p_ClassifyRecord.end() != p_ClassifyRecord.find(tri)) {
			//找到标记三角形
			return p_ClassifyRecord[tri];
		}

		vector<FaceId> oneRTri;

		{
			Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(tri));
			oneRTri.push_back((*(ff++)).idx());
			oneRTri.push_back((*(ff++)).idx());
			oneRTri.push_back((*(ff)).idx());
			
		}

		
		for (auto& triR: oneRTri) {

			auto pos = find(p_SearchedTri.begin(), p_SearchedTri.end(), triR);
			auto pos2 = find(p_NextLevelTriangles.begin(), p_NextLevelTriangles.end(), triR);

			if ((pos == p_SearchedTri.end()) && (pos2 == p_NextLevelTriangles.end())) {
				p_NextLevelTriangles.push_back(triR);				
			}
			
		}
	}

	return SearchLabeledTriangle(p_NextLevelTriangles, p_ClassifyRecord, p_SearchedTri, p_Mesh);

	assert(false);
	return false;
}

bool ClassifyOperate::IsLabeled(FaceId p_Triangle, unordered_map<FaceId, bool>& p_ClassifyRecord)
{
	if (p_ClassifyRecord.end() != p_ClassifyRecord.find(p_Triangle)) return true;
	return false;
}

bool ClassifyOperate::LableIsolatedTriangle(FaceId& p_Triangle, TriangleSet& p_Triangles, unordered_map<FaceId, bool>& p_ClassifyRecord, Mesh& p_Mesh)
{
	assert(!IsLabeled(p_Triangle, p_ClassifyRecord));

	unordered_set<FaceId> searchedTri;
	searchedTri.insert(p_Triangle);

	vector<FaceId> oneRTri;

	Mesh::FaceFaceIter ff = p_Mesh.ff_begin(p_Mesh.face_handle(p_Triangle));
	
	//Mesh::FaceHandle f = *(ff++);

	oneRTri.push_back((*(ff++)).idx());
	oneRTri.push_back((*(ff++)).idx());
	oneRTri.push_back((*(ff)).idx());

	bool labelResult = SearchLabeledTriangle(oneRTri, p_ClassifyRecord, searchedTri, p_Mesh);
	
	p_ClassifyRecord[p_Triangle] = labelResult;

	p_Triangles.erase(p_Triangle);

	return true;
}

void ClassifyOperate::run()
{
	for (auto& me : m_Meshs) {

		//cout << "======================"  << endl;
		ProcessMesh(const_cast<Mesh&>(*me));



	}

	//Mesh::Point pr1(4.71116, - 0.187841, 0.35);
	//Mesh::Point pr2(4.65398, 0.115213, 0.35);
	//Mesh::Point pr3(4.59798, 0.287238, 0.35);
	//Mesh::Point pr4(4.549, 0.55, 0.35);

	//pair<Mesh::VertexHandle, Mesh::VertexHandle> handles1, handles2, handles3, handles4;

	//for (auto vhi = m_Meshs[0]->vertices_begin(); vhi != m_Meshs[0]->vertices_end(); ++vhi) {
	//	Mesh::Point p = m_Meshs[0]->point(*vhi);

	//	if (IsZero(Vector3d(p - pr1))) handles1.first = *vhi;
	//	if (IsZero(Vector3d(p - pr2))) handles2.first = *vhi;
	//	if (IsZero(Vector3d(p - pr3))) handles3.first = *vhi;
	//	if (IsZero(Vector3d(p - pr4))) handles4.first = *vhi;

	//}

	//for (auto vhi = m_Meshs[1]->vertices_begin(); vhi != m_Meshs[1]->vertices_end(); ++vhi) {
	//	Mesh::Point p = m_Meshs[1]->point(*vhi);

	//	if (IsZero(Vector3d(p - pr1))) handles1.second = *vhi;
	//	if (IsZero(Vector3d(p - pr2))) handles2.second = *vhi;
	//	if (IsZero(Vector3d(p - pr3))) handles3.second = *vhi;
	//	if (IsZero(Vector3d(p - pr4))) handles4.second = *vhi;

	//}

	//Mesh::HalfedgeHandle he = m_Meshs[1]->find_halfedge(handles1.second, handles4.second);
	//Mesh::HalfedgeHandle he2 = m_Meshs[1]->opposite_halfedge_handle(he);

	//Mesh::FaceHandle f1 = m_Meshs[1]->face_handle(he);
	//Mesh::FaceHandle f2 = m_Meshs[1]->face_handle(he2);

	//auto& label = m_ClassifyResult[m_Meshs[1]];
	//auto re = label[f1.idx()];
	//auto re2 = label[f2.idx()];

 }




