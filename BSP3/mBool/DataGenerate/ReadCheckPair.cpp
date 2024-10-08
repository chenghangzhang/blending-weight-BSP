#include "mPch.h"
#include "ReadCheckPair.h"

void ReadCheckPair::ReadCPair(IntersectCheckList& r_List, const Mesh& p_Ma, const Mesh& p_Mb)
{
	r_List.clear();

	Mesh* meshA = const_cast<Mesh*>(&p_Ma);
	Mesh* meshB = const_cast<Mesh*>(&p_Mb);

	//ifstream inFileA("MA.txt"), inFileB("MB.txt");
	ifstream inFileA("cowMA.txt"), inFileB("cowMB.txt");

	string s;
	stringstream stream;
	size_t eID;
	size_t fID;
	stream.clear();


	vector<size_t> fileARecord;

	while (inFileA >> s) {

		{
			stream << s;
			stream >> eID;
			stream.clear();
		}
		{
			inFileA >> s;
			stream << s;
			stream >> fID;
			stream.clear();
		}

		fileARecord.push_back(eID);
		fileARecord.push_back(fID);
	}

	vector<size_t> fileBRecord;

	while (inFileB >> s) {

		{
			stream << s;
			stream >> eID;
			stream.clear();
		}
		{
			inFileB >> s;
			stream << s;
			stream >> fID;
			stream.clear();
		}

		fileBRecord.push_back(eID);
		fileBRecord.push_back(fID);
	}

	for (int i = 0; i < fileARecord.size(); i += 2) {
		MeshHalfEdgeHandle me;
		MeshTriangleHandle mt;


		Mesh::HalfedgeHandle heh = meshA->halfedge_handle(fileARecord[i]);
		me = make_pair(heh, meshA);


		Mesh::FaceHandle fh = meshB->face_handle(fileARecord[i+1]);
		mt = make_pair(fh, meshB);

		r_List.push_back({ me,mt });
	
	}

	for (int i = 0; i < fileBRecord.size(); i += 2) {
		MeshHalfEdgeHandle me;
		MeshTriangleHandle mt;


		Mesh::HalfedgeHandle heh = meshB->halfedge_handle(fileBRecord[i]);

		me = make_pair(heh, meshB);

		Mesh::FaceHandle fh = meshA->face_handle(fileBRecord[i+1]);

		mt = make_pair(fh, meshA);

		r_List.push_back({ me,mt });


	}

	//while (inFileA >> s) {
	//	MeshHalfEdgeHandle me;
	//	MeshTriangleHandle mt;

	//	{
	//		stream << s;
	//		stream >> efID;
	//		stream.clear();

	//		Mesh::HalfedgeHandle heh = meshA->halfedge_handle(efID);

	//		me = make_pair(heh, meshA);
	//	}

	//	inFileA >> s;
	//	{
	//		stream << s;
	//		stream >> efID;
	//		stream.clear();

	//		Mesh::FaceHandle fh = meshB->face_handle(efID);

	//		mt = make_pair(fh, meshB);
	//	}

	//	r_List.push_back({ me,mt });

	//}



	//while (inFileB >> s) {
	//	MeshHalfEdgeHandle me;
	//	MeshTriangleHandle mt;

	//	{
	//		stream << s;
	//		stream >> efID;
	//		stream.clear();

	//		Mesh::HalfedgeHandle heh = meshB->halfedge_handle(efID);

	//		me = make_pair(heh, meshB);
	//	}

	//	inFileB >> s;
	//	{
	//		stream << s;
	//		stream >> efID;
	//		stream.clear();

	//		Mesh::FaceHandle fh = meshA->face_handle(efID);

	//		mt = make_pair(fh, meshA);
	//	}

	//	r_List.push_back({ me,mt });

	//}

	inFileA.close();
	inFileB.close();
}
