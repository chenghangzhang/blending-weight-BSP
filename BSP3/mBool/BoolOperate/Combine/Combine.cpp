#include "mPch.h"
#include "Combine.h"


CombineOperate::CombineOperate(Mesh& p_MeshA, Mesh& p_MeshB, const MeshBoolOperateType p_Type, RemeshNewVertexResult& p_NewVertexs, ClassifyRecord& p_TriangleRecord, const Tolerance& p_Tolerence/* = Tolerance()*/):
	m_OriginalMeshA(p_MeshA), m_OriginalMeshB(p_MeshB), m_Type(p_Type), m_IntersectVertexs(p_NewVertexs),m_TriangleRecord(p_TriangleRecord), m_Tolerance(p_Tolerence)
{
	m_classifyTriangleA = m_TriangleRecord[&m_OriginalMeshA];
	m_classifyTriangleB = m_TriangleRecord[&m_OriginalMeshB];

	m_newVertexA = m_IntersectVertexs[&m_OriginalMeshA];
	m_newVertexB = m_IntersectVertexs[&m_OriginalMeshB];
}

CombineOperate::~CombineOperate()
{
}

void CombineOperate::GetMeshFrom2Models(Mesh& r_Mesh1, Mesh& r_Mesh2, bool needReverseNormal)
{
	//assert((m_OutputFacesFromMeshA.size() != 0) && (m_OutputFacesFromMeshB.size() != 0));

	r_Mesh1 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshA, m_OutputFacesFromMeshA);

	if (m_Type == MeshBoolOperateType::Difference) {
		r_Mesh2 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshB, m_OutputFacesFromMeshB, !needReverseNormal);
	}
	else {
		r_Mesh2 = DataTranslate::TranslateFaceID2Mesh(m_OriginalMeshB, m_OutputFacesFromMeshB);
	}
	
}

void CombineOperate::run(Mesh& r_Mesh)
{
	//根据布尔运算类型，确定哪些面片需要保留，并将保留的面片生成新的Mesh


	switch (m_Type) {
		case MeshBoolOperateType::Difference:
			ProcessDifference(r_Mesh);
			break;
		case MeshBoolOperateType::Intersection:
			ProcessIntersection(r_Mesh);
			break;
		case MeshBoolOperateType::Union:
			ProcessUnion(r_Mesh);
			break;
		default:

			break;

	}

}




void CombineOperate::ProcessDifference(Mesh& r_Mesh)
{
	//读取两个Mesh中的分类信息unordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//从而搭建新的网格

	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();

	for (auto& itr : m_classifyTriangleA) {

		assert(itr.first >= 0);

		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}

	}

	for (auto& itr : m_classifyTriangleB) {

		assert(itr.first >= 0);

		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB, true);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}


}

void CombineOperate::ProcessIntersection(Mesh& r_Mesh)
{
	//读取两个Mesh中的分类信息unordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//从而搭建新的网格
	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();


	for (auto& itr : m_classifyTriangleA) {

		//assert(itr.first != nullptr);
		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}

	}

	for (auto& itr : m_classifyTriangleB) {

		//assert(itr.first != nullptr);
		if (false == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}
}

void CombineOperate::ProcessUnion(Mesh& r_Mesh)
{
	//读取两个Mesh中的分类信息unordered_map<TRIANGLE, bool> m_TriangleInOutRecord;
	//从而搭建新的网格
	m_OutputFacesFromMeshA.clear();
	m_OutputFacesFromMeshB.clear();

	for (auto& itr : m_classifyTriangleA) {

		//assert(itr.first != nullptr);

		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshA);

			m_OutputFacesFromMeshA.insert(itr.first);
		}
			
	}

	for (auto& itr : m_classifyTriangleB) {

		//assert(itr.first != nullptr);
		if (true == itr.second) {
			AddTriangle(r_Mesh, itr.first, m_OriginalMeshB);

			m_OutputFacesFromMeshB.insert(itr.first);
		}

	}

}

void CombineOperate::AddTriangle(Mesh& r_Mesh, FaceId p_Triangle, Mesh& p_SourceMesh, bool p_NeedDifferNormal /*= false*/)
{
	//const MeshVertex& mv0 = p_Triangle->VertexAt(0);

	array<FaceId, 3> newTriangle = {-1 , -1, -1};

	Mesh::FaceVertexCCWIter fv = p_SourceMesh.fv_ccwbegin(p_SourceMesh.face_handle(p_Triangle));
	if (!p_NeedDifferNormal) {
		
		for (int i = 0;i < 3;i++) {
			VertexId mv = (*fv++).idx();
			newTriangle[i] = AddVertex(r_Mesh, mv,p_SourceMesh);
		}
		
	}
	else {

		newTriangle[0] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
		newTriangle[2] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
		newTriangle[1] = AddVertex(r_Mesh, (*fv++).idx(), p_SourceMesh);
	}


	//assert(!IsContainerHas(newTriangle, nullptr))
	vector<Mesh::VertexHandle> face{
		r_Mesh.vertex_handle(newTriangle[0]) ,
		r_Mesh.vertex_handle(newTriangle[1]) ,
		r_Mesh.vertex_handle(newTriangle[2])
	};

	r_Mesh.add_face(face);
}

VertexId CombineOperate::AddVertex(Mesh& r_Mesh, VertexId p_Vertex, Mesh& p_SourceMesh)
{
	unordered_map<VertexId, VertexId>* addedVertex = nullptr;
	unordered_map<VertexId, VertexId>* addedVertexAnother = nullptr;

	RemeshNewVertexRecord* newVertexRecord = nullptr;
	RemeshNewVertexRecord* newVertexRecordAnother = nullptr;



	if ((&p_SourceMesh) == (&m_OriginalMeshA)) 
	{
		addedVertex = &m_AddedVertexA;
		addedVertexAnother = &m_AddedVertexB;

		newVertexRecord = &m_newVertexA;
		newVertexRecordAnother = &m_newVertexB;
	}
	else if((&p_SourceMesh) == (&m_OriginalMeshB))
	{
		addedVertex = &m_AddedVertexB;
		addedVertexAnother = &m_AddedVertexA;

		newVertexRecord = &m_newVertexB;
		newVertexRecordAnother = &m_newVertexA;
	}
	else {
		assert(false);
	}
	
	assert(nullptr != addedVertex);
	assert(nullptr != addedVertexAnother);
	assert(nullptr != newVertexRecord);
	assert(nullptr != newVertexRecordAnother);
	
	auto itrAdded = addedVertex->find(p_Vertex);

	//先判断是否该点已经添加到网格中
	if (addedVertex->end() == itrAdded) {
		//没有添加

			RemeshNewVertexRecord::iterator itr = newVertexRecord->find(p_Vertex);

			//判断是否属于是由于求交产生的新的点
			if (newVertexRecord->end() == itr) {

				//不是，直接添加
				Mesh::VertexHandle newVertex = r_Mesh.add_vertex(p_SourceMesh.point(p_SourceMesh.vertex_handle(p_Vertex)));
				(*addedVertex)[p_Vertex] = newVertex.idx();

				return newVertex.idx();

			}
			else {

				//该点属于是由于求交产生的新的点
				const Point3d* thisPoint = newVertexRecord->at(p_Vertex);
				//vector<MeshVertex*> samePositionVertex;

				//查找该点是否已经添加
				auto itrHere = addedVertex->find(p_Vertex);
				if (addedVertex->end() != itrHere) {
					return itrHere->second;
				}

				//该点未添加
				//查找其他位置相同的求交点是否已经添加到网格中
				// 
				//在当前所属mesh参数中查找相同位置点
				for(auto& itr: *newVertexRecord){

					if ((itr.second == thisPoint) && (itr.first != p_Vertex)) {

						auto itrHere = addedVertex->find(itr.first);

						//相同位置点已经添加，直接返回
						if (addedVertex->end() != itrHere) {
							return itrHere->second;
						}

					}

				}

				//在另一Mesh所属参数中查找相同位置点
				for (auto& itr : *newVertexRecordAnother) {

					//只需确认是否位置一致
					if (itr.second == thisPoint) {
						auto itrHere = addedVertexAnother->find(itr.first);

						//相同位置点已经添加，直接返回
						if (addedVertexAnother->end() != itrHere) {
							return itrHere->second;
						}

					}

				}

				//相同位置点没有已经添加的，直接添加
				Mesh::VertexHandle newVertex = r_Mesh.add_vertex(p_SourceMesh.point(p_SourceMesh.vertex_handle(p_Vertex)));
				(*addedVertex)[p_Vertex] = newVertex.idx();

				return newVertex.idx();
			}

	}
	else {
		return itrAdded->second;		
	}

	assert(false);
	return -1;
}



