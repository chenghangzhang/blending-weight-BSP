#pragma once

// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
// ----------------------------------------------------------------------------

typedef OpenMesh::TriMesh_ArrayKernelT<>  Mesh;

typedef Mesh::VertexIter VIter;
typedef Mesh::HalfedgeIter HeIter;
typedef Mesh::FaceIter TriIter;

typedef OpenMesh::Vec3d Vector3d;
typedef OpenMesh::Vec3d Point3d;


typedef pair<Mesh::VertexHandle, Mesh*> MeshVertexHandle;
typedef pair<Mesh::HalfedgeHandle, Mesh*> MeshHalfEdgeHandle;
typedef pair<Mesh::EdgeHandle, Mesh*> MeshEdgeHandle;
typedef pair<Mesh::FaceHandle, Mesh*> MeshTriangleHandle;


typedef pair<Mesh::Vertex*, Mesh*> MeshVertexRef;
typedef pair<Mesh::Halfedge*, Mesh*> MeshEdgeRef;
typedef pair<Mesh::Face*, Mesh*> MeshTriangleRef;

typedef size_t VertexId;
typedef size_t EdgeId;
typedef size_t HalfEdgeId;
typedef size_t FaceId;


typedef vector<Point3d> Vertex_Set;
typedef vector<Point3d> Edge_Set;
//typedef vector<pair<Point3d, Point3d>> Edge_Set;
typedef vector<tuple<Point3d, Point3d,Point3d, Vector3d>> Triangle_Set;






