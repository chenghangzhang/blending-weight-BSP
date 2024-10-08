#include "mPch.h"

#include "DataTranslate.h"

Triangle_Set DataTranslate::TranslateMesh(Mesh& p_Mesh)
{
	Triangle_Set r_TriangleSet;

	if (!p_Mesh.has_face_normals()) {
		p_Mesh.request_face_normals();
	}


	for (Mesh::FaceIter face = p_Mesh.faces_begin(); face != p_Mesh.faces_end(); ++face) {
		
		Mesh::FaceVertexIter fv_it = p_Mesh.fv_iter(*face);
		
		Mesh::VertexHandle v0 = *(fv_it++);
		Mesh::VertexHandle v1 = *(fv_it++);
		Mesh::VertexHandle v2 = *fv_it;

		Vector3d p0(p_Mesh.point(v0));
		Vector3d p1(p_Mesh.point(v1));
		Vector3d p2(p_Mesh.point(v2));

		Vector3d normal(p_Mesh.normal(*face));

		r_TriangleSet.push_back(make_tuple(p0, p1, p2, normal));
		
	}



	return r_TriangleSet;
}

void DataTranslate::TranslateMeshWithlable(Mesh& p_Mesh, unordered_map<FaceId, bool> p_classify, pair<Triangle_Set, Triangle_Set>& r_labeledTriangles)
{
	if (!p_Mesh.has_face_normals()) {
		p_Mesh.request_face_normals();
	}
	

	Triangle_Set& triangles1 = r_labeledTriangles.first;
	Triangle_Set& triangles2 = r_labeledTriangles.second;

	int i = 1;
	for (auto& record: p_classify) {

		FaceId triId = record.first;
		Mesh::FaceHandle triHandle = p_Mesh.face_handle(triId);

		//if (p_Mesh.status(triHandle).deleted()) {
		//	cout << i++ << endl;
		//	continue;		
		//}

		if (!p_Mesh.is_valid_handle(triHandle)) {
			cout << i++ << endl;
			continue;		
		}

		bool labelS = record.second;

		

		Mesh::FaceVertexIter fv_it = p_Mesh.fv_iter(triHandle);

		Mesh::VertexHandle v0 = *(fv_it++);
		Mesh::VertexHandle v1 = *(fv_it++);
		Mesh::VertexHandle v2 = *fv_it;

		Vector3d p0(p_Mesh.point(v0));
		Vector3d p1(p_Mesh.point(v1));
		Vector3d p2(p_Mesh.point(v2));

		Vector3d normal(p_Mesh.normal(triHandle));

		if (labelS) {
			//cout << "triangles1: " << triHandle.idx() << endl;
			triangles1.push_back(make_tuple(p0, p1, p2, normal));
		}
		else {
			cout << "triangles2: " << triHandle.idx() << endl;
			triangles2.push_back(make_tuple(p0, p1, p2, normal));
		}


	}
	
}

AABB DataTranslate::TranslateMesh2AABB(Mesh& p_Mesh)
{
	vector<Triangle3d> triangleSet;
	p_Mesh.request_face_normals();

	for (Mesh::FaceIter face = p_Mesh.faces_begin(); face != p_Mesh.faces_end(); ++face) {

		Mesh::FaceVertexIter fv_it = p_Mesh.fv_iter(*face);

		Mesh::VertexHandle v0 = *(fv_it++);
		Mesh::VertexHandle v1 = *(fv_it++);
		Mesh::VertexHandle v2 = *fv_it;

		Point3d p0(p_Mesh.point(v0));
		Point3d p1(p_Mesh.point(v1));
		Point3d p2(p_Mesh.point(v2));

		Vector3d normal(p_Mesh.normal(*face));

		triangleSet.push_back(Triangle3d({ p0, p1, p2 }));

	}



	return AABB(triangleSet);
}

Mesh DataTranslate::TranslateAABB2Mesh(AABB& p_AABB)
{
	return p_AABB.Transform2Mesh();
}

Edge_Set DataTranslate::TranslateEdge(Mesh& p_Mesh)
{
	int i = 0;
	Edge_Set r_EdgeSet;
	unordered_set<size_t> parsedEdge;


	for (Mesh::HalfedgeIter hit = p_Mesh.halfedges_begin(); hit != p_Mesh.halfedges_end(); ++hit) {

		if (parsedEdge.find(hit->idx()) != parsedEdge.end()) continue;

		parsedEdge.insert(hit->idx());
		parsedEdge.insert(p_Mesh.opposite_halfedge_handle(*hit).idx());
		i++;
		

		Mesh::VertexHandle v0 = p_Mesh.from_vertex_handle(*hit);
		Mesh::VertexHandle v1 = p_Mesh.to_vertex_handle(*hit);


		Vector3d p0(p_Mesh.point(v0));
		Vector3d p1(p_Mesh.point(v1));


		r_EdgeSet.push_back(p0);
		r_EdgeSet.push_back(p1);
	}
	cout << i << endl;
	return r_EdgeSet;
}

Vertex_Set DataTranslate::TranslateVertex(Mesh& p_Mesh)
{
	Vertex_Set r_VertexSet;

	for (Mesh::VertexIter vi = p_Mesh.vertices_begin(); vi != p_Mesh.vertices_end(); ++vi) {

		r_VertexSet.push_back( Point3d(p_Mesh.point(*vi)));

	}


	return r_VertexSet;
}

pair<Triangle_Set, Edge_Set> DataTranslate::TranslatePartitionPlane(TreeFaceRenderingInfo& partiPlanes, Mesh& r_PartionPlaneMesh)
{
	double scale = 15;

	Tolerance toler = Tolerance::GetTolerance();

	Triangle_Set r_RenderFaces;

	for (auto& planeToRender : partiPlanes) {
		//pair<Plane3d, planeRecords>

		Vector3d normal = planeToRender.first.GetNormal();
		Point3d p0 = planeToRender.first.GetOrigin();

		Point3d p1;
		Point3d p2;
		Point3d p3;
		Point3d p4;

		//cout << normal[0] << "," << normal[1] << "," << normal[2] << "," << endl;
		//cout << p0[0] << "," << p0[1] << "," << p0[2] << "," << endl;

		array<Point3d, 4> points = GetOffSectPoint(p0, normal, scale);
		p1 = points[0];
		p2 = points[1];
		p3 = points[2];
		p4 = points[3];

		//r_RenderFaces.push_back(make_tuple(p2, p3, p4, normal));
		//r_RenderFaces.push_back(make_tuple(p2, p1, p3, normal));

		vector<Triangle3d> renderTriangles;
		renderTriangles.push_back(Triangle3d({ p2, p3, p4 }));
		renderTriangles.push_back(Triangle3d({ p2, p1, p3 }));



		for (auto& record : planeToRender.second) {
			//using planeRecord = pair<Plane3d, bool>;
			              
			vector<Triangle3d> afterPartitionTriangles;

			for (auto& tri : renderTriangles) {

				TrianglePlaneIsIntersectInfo info;

				TrianglePlaneIsIntersect(tri, record.first, info, toler);

				if (info.isIntersect) {
					//intersect

					if (record.second != true) {
						afterPartitionTriangles.insert(afterPartitionTriangles.end(), info.newPositiveTriangles.begin(), info.newPositiveTriangles.end());
					}
					else {
						afterPartitionTriangles.insert(afterPartitionTriangles.end(), info.newNegativeTriangles.begin(), info.newNegativeTriangles.end());					
					}

				}
				else {
					//no intersect

					if (record.second != info.direction) {
						afterPartitionTriangles.push_back(tri);
					}
					else {
						//pass
					}
				}
			}		

			renderTriangles.clear();
			renderTriangles = afterPartitionTriangles;
		}

		for (auto& tri : renderTriangles) {
			r_RenderFaces.push_back({ tri.VertexAt(0),tri.VertexAt(1),tri.VertexAt(2),tri.GetNormal() });		
		}


	}

	Edge_Set r_RenderEdges;

	for (auto& tri : r_RenderFaces) {

		//output edges
		r_RenderEdges.push_back(std::get<0>(tri));
		r_RenderEdges.push_back(std::get<1>(tri));

		r_RenderEdges.push_back(std::get<1>(tri));
		r_RenderEdges.push_back(std::get<2>(tri));

		r_RenderEdges.push_back(std::get<2>(tri));
		r_RenderEdges.push_back(std::get<0>(tri));

		//output triangle Mesh of the plane
		Mesh::VertexHandle mv0 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<0>(tri)));
		Mesh::VertexHandle mv1 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<1>(tri)));
		Mesh::VertexHandle mv2 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<2>(tri)));

		std::vector<Mesh::VertexHandle>  face_vhandles;
		face_vhandles.clear();
		face_vhandles.push_back(mv0);
		face_vhandles.push_back(mv1);
		face_vhandles.push_back(mv2);
		r_PartionPlaneMesh.add_face(face_vhandles);
	}

	return make_pair(r_RenderFaces,r_RenderEdges);
}

pair<Triangle_Set, Edge_Set> DataTranslate::TranslatePartitionPlane(TreeFaceRenderingInfo& partiPlanes, AABB& splittingRestrictAABB, Mesh& r_PartionPlaneMesh)
{
	//沿平面扩展长度
	//AABB最长MaxInnerLength
	//沿着一个方向MaxInnerLength一定能超过aabb盒子范围
	double scale = splittingRestrictAABB.GetMaxInnerLength();

	Tolerance toler = Tolerance::GetTolerance();

	Triangle_Set r_RenderFaces;

	for (auto& planeToRender : partiPlanes) {
		//pair<Plane3d, planeRecords>

		Vector3d normal = planeToRender.first.GetNormal();
		Point3d p0 = planeToRender.first.GetOrigin();

		Point3d p1;
		Point3d p2;
		Point3d p3;
		Point3d p4;

		//cout << normal[0] << "," << normal[1] << "," << normal[2] << "," << endl;
		//cout << p0[0] << "," << p0[1] << "," << p0[2] << "," << endl;

		array<Point3d, 4> points = GetOffSectPoint(p0, normal, scale);
		p1 = points[0];
		p2 = points[1];
		p3 = points[2];
		p4 = points[3];

		//r_RenderFaces.push_back(make_tuple(p2, p3, p4, normal));
		//r_RenderFaces.push_back(make_tuple(p2, p1, p3, normal));

		vector<Triangle3d> renderTriangles;
		renderTriangles.push_back(Triangle3d({ p2, p3, p4 }));
		renderTriangles.push_back(Triangle3d({ p2, p1, p3 }));


		splittingRestrictAABB.AddPlaneRecords(planeToRender.second);

		for (auto& record : planeToRender.second) {
			//using planeRecord = pair<Plane3d, bool>;

			vector<Triangle3d> afterPartitionTriangles;

			for (auto& tri : renderTriangles) {

				TrianglePlaneIsIntersectInfo info;

				TrianglePlaneIsIntersect(tri, record.first, info, toler);

				if (info.isIntersect) {
					//intersect

					if (record.second != true) {
						afterPartitionTriangles.insert(afterPartitionTriangles.end(), info.newPositiveTriangles.begin(), info.newPositiveTriangles.end());
					}
					else {
						afterPartitionTriangles.insert(afterPartitionTriangles.end(), info.newNegativeTriangles.begin(), info.newNegativeTriangles.end());
					}

				}
				else {
					//no intersect

					if (record.second != info.direction) {
						afterPartitionTriangles.push_back(tri);
					}
					else {
						//pass
					}
				}
			}

			renderTriangles.clear();
			renderTriangles = afterPartitionTriangles;
		}

		for (auto& tri : renderTriangles) {
			r_RenderFaces.push_back({ tri.VertexAt(0),tri.VertexAt(1),tri.VertexAt(2),tri.GetNormal() });
		}


	}

	Edge_Set r_RenderEdges;

	for (auto& tri : r_RenderFaces) {

		//output edges
		r_RenderEdges.push_back(std::get<0>(tri));
		r_RenderEdges.push_back(std::get<1>(tri));

		r_RenderEdges.push_back(std::get<1>(tri));
		r_RenderEdges.push_back(std::get<2>(tri));

		r_RenderEdges.push_back(std::get<2>(tri));
		r_RenderEdges.push_back(std::get<0>(tri));

		//output triangle Mesh of the plane
		Mesh::VertexHandle mv0 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<0>(tri)));
		Mesh::VertexHandle mv1 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<1>(tri)));
		Mesh::VertexHandle mv2 = r_PartionPlaneMesh.add_vertex(Mesh::Point(std::get<2>(tri)));

		std::vector<Mesh::VertexHandle>  face_vhandles;
		face_vhandles.clear();
		face_vhandles.push_back(mv0);
		face_vhandles.push_back(mv1);
		face_vhandles.push_back(mv2);
		r_PartionPlaneMesh.add_face(face_vhandles);
	}

	return make_pair(r_RenderFaces, r_RenderEdges);
}



Triangle_Set DataTranslate::TranslateTriangleBSPs(BSPTreeNode* leafNode)
{
	assert(leafNode->IsLeaf());

	Triangle_Set r_Triangles;

	vector<TriangleForTree> fs1 = leafNode->m_FacesA;
	vector<TriangleForTree> fs2 = leafNode->m_FacesB;

	fs1.insert(fs1.end(), fs2.begin(), fs2.end());
	
	for (auto& tri : fs1) {
		r_Triangles.push_back({ tri.m_Triangle.VertexAt(0),tri.m_Triangle.VertexAt(1),tri.m_Triangle.VertexAt(2),tri.m_Triangle.GetNormal() });
	}
	
	

	return r_Triangles;
}

pair<Triangle_Set, Triangle_Set> DataTranslate::TranslateTriangleBSPsFromTwoMeshs(BSPTreeNode* leafNode)
{
	assert(leafNode->IsLeaf());

	Triangle_Set r_Triangles1;
	Triangle_Set r_Triangles2;

	vector<TriangleForTree> fs1 = leafNode->m_FacesA;
	vector<TriangleForTree> fs2 = leafNode->m_FacesB;

	for (auto& tri : fs1) {
		r_Triangles1.push_back({ tri.m_Triangle.VertexAt(0),tri.m_Triangle.VertexAt(1),tri.m_Triangle.VertexAt(2),tri.m_Triangle.GetNormal() });
	}

	for (auto& tri : fs2) {
		r_Triangles2.push_back({ tri.m_Triangle.VertexAt(0),tri.m_Triangle.VertexAt(1),tri.m_Triangle.VertexAt(2),tri.m_Triangle.GetNormal() });
	}

	return { r_Triangles1,r_Triangles2 };
}

pair<Mesh, Mesh> DataTranslate::TranslateNode2SeperatedTriangleMesh(BSPTreeNode* leafNode)
{
	assert(leafNode->IsLeaf());

	vector<TriangleForTree> fs1 = leafNode->m_FacesA;
	vector<TriangleForTree> fs2 = leafNode->m_FacesB;

	Mesh r_MeshA, r_MeshB;

	for (auto tri : fs1) {
		vector<Mesh::VertexHandle> vh;
		vh.push_back(r_MeshA.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(0))));
		vh.push_back(r_MeshA.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(1))));
		vh.push_back(r_MeshA.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(2))));

		r_MeshA.add_face(vh);
	}

	for (auto tri : fs2) {
		vector<Mesh::VertexHandle> vh;
		vh.push_back(r_MeshB.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(0))));
		vh.push_back(r_MeshB.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(1))));
		vh.push_back(r_MeshB.add_vertex(Mesh::Point(tri.m_Triangle.VertexAt(2))));

		r_MeshB.add_face(vh);
	}


	return make_pair(r_MeshA,r_MeshB);
}

Mesh DataTranslate::TranslateFaceID2Mesh(Mesh& m_OriginMesh, unordered_set<FaceId>& facesId, bool needReverseNormal /*= false*/ )
{
	Mesh r_Mesh;

	for (auto& faceId : facesId) {
		Mesh::FaceHandle fh = m_OriginMesh.face_handle(faceId);
		
		Mesh::FaceVertexIter fvIter = m_OriginMesh.fv_begin(fh);

		vector<Mesh::VertexHandle> vh;


		vh.push_back(r_Mesh.add_vertex(m_OriginMesh.point(*fvIter)));
		++fvIter;
		vh.push_back(r_Mesh.add_vertex(m_OriginMesh.point(*fvIter)));
		++fvIter;
		vh.push_back(r_Mesh.add_vertex(m_OriginMesh.point(*fvIter)));

		if (needReverseNormal) {
			swap(vh[0], vh[1]);
		}


		r_Mesh.add_face(vh);
	}

	
	return r_Mesh;
}

Mesh DataTranslate::TranslatePointCloud2Mesh(vector<Point3d> pointCloud, double radii, Mesh& r_Mesh)
{
	r_Mesh.clear();

	if (pointCloud.size() >= 100) {
		
		vector<Point3d>::iterator pBegin = pointCloud.begin();

		vector<Point3d>::iterator pMiddle = pBegin + pointCloud.size() / 2.0;
		
		vector<Point3d> son1(pBegin, pMiddle);
		vector<Point3d> son2(pMiddle, pointCloud.end());

		Mesh son1Mesh, son2Mesh;

		std::thread pointClouds1MeshThread(&DataTranslate::TranslatePointCloud2Mesh, son1, radii, std::ref(son1Mesh));
		std::thread pointClouds2MeshThread(&DataTranslate::TranslatePointCloud2Mesh, son2, radii, std::ref(son2Mesh));


		pointClouds1MeshThread.join();
		pointClouds2MeshThread.join();

		merge_meshes(r_Mesh, son1Mesh);
		merge_meshes(r_Mesh, son2Mesh);
	}
	else {

		for (auto& point : pointCloud) {
			Mesh ballMesh = Ball::GenerateBallMesh(point, radii);

			merge_meshes(r_Mesh, ballMesh);

		}

	}



	return r_Mesh;
}
