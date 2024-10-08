#include "mPch.h"
#include "GenerateMesh.h"


bool GenerateMesh::CubicMesh(double length, Mesh::Point center, Mesh& mesh)
{
	if (0 != mesh.n_vertices()) return false;

	double xCenter = center[0];
	double yCenter = center[1];
	double zCenter = center[2];
	double halfLength = length / 2;

	// generate vertices
	Mesh::VertexHandle vhandle[8];
	vhandle[0] = mesh.add_vertex(Mesh::Point(xCenter - halfLength, yCenter - halfLength, zCenter + halfLength));
	vhandle[1] = mesh.add_vertex(Mesh::Point(xCenter + halfLength, yCenter - halfLength, zCenter + halfLength));
	vhandle[2] = mesh.add_vertex(Mesh::Point(xCenter + halfLength, yCenter + halfLength, zCenter + halfLength));
	vhandle[3] = mesh.add_vertex(Mesh::Point(xCenter - halfLength, yCenter + halfLength, zCenter + halfLength));
	vhandle[4] = mesh.add_vertex(Mesh::Point(xCenter - halfLength, yCenter - halfLength, zCenter - halfLength));
	vhandle[5] = mesh.add_vertex(Mesh::Point(xCenter + halfLength, yCenter - halfLength, zCenter - halfLength));
	vhandle[6] = mesh.add_vertex(Mesh::Point(xCenter + halfLength, yCenter + halfLength, zCenter - halfLength));
	vhandle[7] = mesh.add_vertex(Mesh::Point(xCenter - halfLength, yCenter + halfLength, zCenter - halfLength));
	// generate (quadrilateral) faces
	std::vector<Mesh::VertexHandle>  face_vhandles;
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[5]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);


	return true;
}
