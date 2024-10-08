#include "mPch.h"
#include "TestBool.h"

namespace TestFunction {

	void TestBool()
	{
		//Mesh meshA, meshB;

		//string m1 = "..//TestModel//batch bool//all models//modelsD//57//bunnyA.obj";
		//string m2 = "..//TestModel//batch bool//all models//modelsD//57//bunnyB.obj";

		//if (!OpenMesh::IO::read_mesh(meshA, m1)) {
		//	std::cerr << "Cannot read mesh" << std::endl;
		//	return;
		//}

		//if (!OpenMesh::IO::read_mesh(meshB, m2)) {
		//	std::cerr << "Cannot read mesh" << std::endl;
		//	return;
		//}


		//ofstream of1("C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bool operate\\meanTime.txt");

		//for (int i = 0; i < 4;i++) {
		//	BSPConstructType bt;
		//	string s;
		//	switch (i)
		//	{
		//	case 0:
		//		bt = BSPConstructType::SDM;
		//		s = "SDM";
		//		break;
		//	case 1:
		//		bt = BSPConstructType::SAH;
		//		s = "SAH";
		//		break;
		//	case 2:
		//		bt = BSPConstructType::ObbMiddel;
		//		s = "OBB";
		//		break;
		//	case 3:
		//		bt = BSPConstructType::Gravity_SPLIT;
		//		s = "Gravity";
		//		break;
		//	default:
		//		break;
		//	}



		//	vector<double> ts;
		//	for (int i = 0; i < 10;i++) {


		//		if (!OpenMesh::IO::read_mesh(meshA, m1)) {

		//			std::cerr << "Cannot read mesh" << std::endl;

		//			return;

		//		}
		//		if (!OpenMesh::IO::read_mesh(meshB, m2)) {

		//			std::cerr << "Cannot read mesh" << std::endl;

		//			return;

		//		}


		//		auto t0 = std::chrono::steady_clock::now();

		//		//进行布尔运算
		//		Mesh result;
		//		MeshBoolOperate mBo(meshA, meshB, bt);

		//		mBo.Run(result, MeshBoolOperateType::Difference);
		//		//Mesh r_MeshA, r_MeshB;
		//		//mBo.RunTest(result, MeshBoolOperateType::Difference, r_MeshA, r_MeshB);

		//		auto t1 = std::chrono::steady_clock::now();
		//		//cout << "boolean time：" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "毫秒" << endl;
		//		ts.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
		//	}

		//	double sum = 0;
		//	for (auto t : ts) {
		//		sum += t;
		//	}

		//	of1 << s << "mean time: " << sum / ts.size() << endl;


		//}

		//of1.close();




		//ofstream of("..\\TestModel\\bool operate\\time.txt");

		//for (int i = 0; i < 5;i++) {
		//	BSPConstructType bt;
		//	string s;

		//	Mesh tMeshA, tMeshB;

		//	switch (i)
		//	{
		//	case 0:
		//		bt = BSPConstructType::SDM;
		//		s = "SDM";
		//		break;
		//	case 1:
		//		bt = BSPConstructType::SAH;
		//		s = "SAH";
		//		break;
		//	case 2:
		//		bt = BSPConstructType::ObbMiddel;
		//		s = "ObbMiddel";
		//		break;
		//	case 3:
		//		bt = BSPConstructType::Gravity_SPLIT;
		//		s = "Gravity_SPLIT";
		//		break;
		//	case 4:
		//		bt = BSPConstructType::OCTREE;
		//		s = "OCT";
		//		break;
		//	default:
		//		break;
		//	}
		//

		//	if (!OpenMesh::IO::read_mesh(tMeshA, m1)) {

		//		std::cerr << "Cannot read mesh" << std::endl;

		//		return;

		//	}
		//	if (!OpenMesh::IO::read_mesh(tMeshB, m2)) {

		//		std::cerr << "Cannot read mesh" << std::endl;

		//		return;

		//	}


		//	auto t0 = std::chrono::steady_clock::now();

		//	//进行布尔运算
		//	Mesh result;
		//	MeshBoolOperate mBo(tMeshA, tMeshB, bt);

		//	mBo.Run(result, MeshBoolOperateType::Difference);
		//	//Mesh r_MeshA, r_MeshB;
		//	//mBo.RunTest(result, MeshBoolOperateType::Difference, r_MeshA, r_MeshB);

		//	auto t1 = std::chrono::steady_clock::now();
		//	of << "boolean time：" << s <<" == " << std::chrono::duration<double, std::milli>(t1 - t0).count() << "毫秒" << endl;

		//}

		//of.close();


		ofstream boolTime("..\\TestModel\\bool operate\\time.txt");

		for (int i = 0; i < 5;i++) {
			BSPConstructType bt;
			string BSP_TREE_TYPE;
			switch (i)
			{
			case 0:
				bt = BSPConstructType::WSDM;
				break;
			case 1:
				bt = BSPConstructType::SAH;
				break;
			case 2:
				bt = BSPConstructType::ObbMiddel;
				break;
			case 3:
				bt = BSPConstructType::Gravity_SPLIT;
				break;
			case 4:
				bt = BSPConstructType::OCTREE;
			default:
				break;
			}

			Mesh meshA, meshB;

			string m1 = "..//TestModel//bool operate//12//bunnyA.obj";
			string m2 = "..//TestModel//bool operate//12//bunnyB.obj";

			if (!OpenMesh::IO::read_mesh(meshA, m1)) {
				std::cerr << "Cannot read mesh" << std::endl;
				return;
			}

			if (!OpenMesh::IO::read_mesh(meshB, m2)) {
				std::cerr << "Cannot read mesh" << std::endl;
				return;
			}

			auto t0 = std::chrono::steady_clock::now();

			//进行布尔运算
			Mesh result;
			MeshBoolOperate mBo(meshA, meshB, bt);
			//MeshBoolOperate mBo(meshA, meshB, BSPConstructType::SAH);
			//MeshBoolOperate mBo(meshA, meshB, BSPConstructType::ObbMiddel);
			//MeshBoolOperate mBo(meshA, meshB, BSPConstructType::Gravity_SPLIT);
			//MeshBoolOperate mBo(meshA, meshB, BSPConstructType::OCTREE);

			//mBo.Run(result, MeshBoolOperateType::Difference);
			Mesh r_MeshB, r_MeshA;
			bool needReverseNormal = false;
			mBo.RunTest(result, MeshBoolOperateType::Difference, r_MeshA, r_MeshB, needReverseNormal, "SingleTest");

			auto t1 = std::chrono::steady_clock::now();
			cout << "boolean time：" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "毫秒" << endl;

			boolTime << std::chrono::duration<double, std::milli>(t1 - t0).count() << endl;

			//write result
			{
				if (!OpenMesh::IO::write_mesh(result, "..//TestModel//bool operate//Test.obj")) {

					std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

					return;
				}

				if (!OpenMesh::IO::write_mesh(r_MeshA, "..//TestModel//bool operate//rMeshATest.obj")) {

					std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

					return;

				}

				if (!OpenMesh::IO::write_mesh(r_MeshB, "..//TestModel//bool operate//rMeshBTest.obj")) {

					std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

					return;

				}

				meshA.garbage_collection();
				meshB.garbage_collection();

				if (!OpenMesh::IO::write_mesh(meshA, "..//TestModel//bool operate//MeshA.obj")) {

					std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

					return;

				}

				if (!OpenMesh::IO::write_mesh(meshB, "..//TestModel//bool operate//MeshB.obj")) {

					std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

					return;

				}
			}
		}

		boolTime.close();

		//if (!OpenMesh::IO::write_mesh(result, "..//TestModel//batch bool//all models//Test.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;
		//}

		//if (!OpenMesh::IO::write_mesh(r_MeshA, "..//TestModel//batch bool//all models//rMeshATest.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		//if (!OpenMesh::IO::write_mesh(r_MeshB, "..//TestModel//batch bool//all models//rMeshBTest.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		//meshA.garbage_collection();
		//meshB.garbage_collection();

		//if (!OpenMesh::IO::write_mesh(meshA, "..//TestModel//batch bool//all models//MeshA.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		//if (!OpenMesh::IO::write_mesh(meshB, "..//TestModel//batch bool//all models//MeshB.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		////结果显示

		//bool showIntersect = false;
		//bool showClassify = false;
		//bool showResult = true;
		//bool showA = false;
		//bool showB = false;

		//RenderWnd wnd;
		//RenderScence scence;

		//if (showIntersect) {
		//	auto& paths = mBo.m_IntersectResult.at(&meshB).GetIntersectPaths();
		//	auto& vers = mBo.m_IntersectResult.at(&meshB).GetIntersectVertices();
 
		//	//显示交点
		//	Vertex_Set intersectVertex;

		//	for (MeshIntersectVertex* p_ver : vers)
		//	{
		//		intersectVertex.push_back(*p_ver->m_Point3d);
		//	}
		//	{
		//		if (intersectVertex.size() > 0)
		//		{
		//			auto* ra = new VerticesRenderer<Vertex_Set>(intersectVertex);
		//			ra->SetVertexSize(35.0);
		//			ra->SetVertexColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//			scence.AddUnit(ra);
		//		}
		//	}



		//	//{
		//	//	Vertex_Set intersectVertex;

		//	//	intersectVertex.push_back(Point3d(-0.1, 0.5, 0.2));

		//	//	{
		//	//		if (intersectVertex.size() > 0)
		//	//		{
		//	//			auto* ra = new VerticesRenderer<Vertex_Set>(intersectVertex);
		//	//			ra->SetVertexSize(15.0);
		//	//			ra->SetVertexColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//	//			scence.AddUnit(ra);
		//	//		}
		//	//	}
		//	//
		//	//
		//	//}

		//	//显示相交路径
		//	for (MeshIntersectPath* p_Path : paths)
		//	{
		//		Edge_Set intersectPaths;

		//		size_t pathSize = p_Path->Size();

		//		for (size_t j = 0; j < pathSize; j++)
		//		{
		//			if (j > 0) {
		//				intersectPaths.push_back(*(*p_Path)[j - 1].m_Point3d);
		//				intersectPaths.push_back(*(*p_Path)[j].m_Point3d);
		//			}

		//		}

		//		{
		//			if (intersectPaths.size() > 0)
		//			{
		//				auto* ra = new EdgesRenderer<Edge_Set>(intersectPaths);
		//				ra->SetEdgeWidth(30.0);
		//				ra->SetEdgeColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//				scence.AddUnit(ra);
		//			}
		//		}

		//	}
		//
		//
		//}



		////显示分类结果
		//if (showClassify) {
		//	Mesh& meshShow = meshA;

		//	ClassifyRecord& labelResult = mBo.m_TriangleRecord;
		//	unordered_map<FaceId, bool>& labelRecord = labelResult[&meshShow];

		//	pair<Triangle_Set, Triangle_Set> labeledTris;
		//	DataTranslate::TranslateMeshWithlable(meshShow, labelRecord, labeledTris);

		//	result.garbage_collection();
		//	result.request_face_normals();

		//	Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshShow);
		//	Edge_Set thisEdge = DataTranslate::TranslateEdge(meshShow);
		//	Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshShow);


		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(labeledTris.first);
		//		ra->SetTriangleColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}

		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(labeledTris.second);
		//		ra->SetTriangleColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}
		//
		//}



		////显示result
		//if (showResult)
		//{
		//	result.garbage_collection();

		//	Triangle_Set thisMesh = DataTranslate::TranslateMesh(result);
		//	Edge_Set thisEdge = DataTranslate::TranslateEdge(result);
		//	Vertex_Set thisVertex = DataTranslate::TranslateVertex(result);


		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//		ra->SetTriangleColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}
 
 
		//	//{
		//	//	auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//	//	ra->SetVertexSize(15.0);
		//	//	ra->SetVertexColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//	//	scence.AddUnit(ra);
		//	//}

		//	{
		//		auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

		//		//ra->SetVertexSize(15.0);
		//		ra->SetEdgeColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//		scence.AddUnit(ra);
		//	}
		//}


		////显示meshA
		//if (showA)
		//{
		//	Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshA);
		//	Edge_Set thisEdge = DataTranslate::TranslateEdge(meshA);
		//	Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshA);

		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//		ra->SetTriangleColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}

		//	//{
		//	//	auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//	//	ra->SetVertexSize(15.0);
		//	//	ra->SetVertexColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//	//	scence.AddUnit(ra);
		//	//}

		//	{
		//		auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

		//		ra->SetEdgeWidth(30);
		//		ra->SetEdgeColor(glm::vec3(0.0f, 0.0f, 0.0f));
		//		scence.AddUnit(ra);
		//	}
		//}

		////显示meshB
		//if (showB)
		//{
		//	Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshB);
		//	Edge_Set thisEdge = DataTranslate::TranslateEdge(meshB);
		//	Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshB);

		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//		ra->SetTriangleColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}

		//	//{
		//	//	auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//	//	ra->SetVertexSize(15.0);
		//	//	ra->SetVertexColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//	//	scence.AddUnit(ra);
		//	//}

		//	//{
		//	//	auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);
		//	//	ra->SetEdgeColor(glm::vec3(0.0f, 0.0f, 0.0f));
		//	//	scence.AddUnit(ra);
		//	//}
		//}


		//wnd.SetScence(&scence);
		//wnd.Run();

		
	}

	void BatchTestBool()
	{

		//BSP Tree test
		for (int i = 0; i < 5;i++) {
			BSPConstructType bt;
			string BSP_TREE_TYPE;
			switch (i)
			{
			case 0:
				bt = BSPConstructType::WSDM;
				BSP_TREE_TYPE = "SDM";
				break;
			case 1:
				bt = BSPConstructType::SAH;
				BSP_TREE_TYPE = "SAH";
				break;
			case 2:
				bt = BSPConstructType::ObbMiddel;
				BSP_TREE_TYPE = "OBB";
				break;
			case 3:
				bt = BSPConstructType::Gravity_SPLIT;
				BSP_TREE_TYPE = "Gravity";
				break;
			case 4:
				bt = BSPConstructType::OCTREE;
				BSP_TREE_TYPE = "Octree";
			default:
				break;
			}




		Mesh meshA, meshB;

		//vector<string> filedata{ "modelsA//", "modelsB//", "modelsC//",  "modelsD//" };
		vector<string> filedata{ "modelsD//" };

		for (auto& fHead : filedata) {
				
			int jmax;
			int jmin;
			if (fHead == "modelsA//") {
				jmax = 16;
				jmin = 1;
			}
			else if (fHead == "modelsB//") {
				jmax = 24;
				jmin = 1;
			
			}
			else if (fHead == "modelsC//") {
				jmax = 5;
				jmin = 1;
			}
			else if (fHead == "modelsD//") {
				jmax = 41;
				jmin = 1;
			}
			else {
				assert(false);
			}


			for (int j = jmin; j < jmax;j++) {
				
				string baseAddress = "..//TestModel//batch bool//all models//" + fHead + to_string(j);

				vector<string>  files;
				getFileNames(baseAddress, files);

				vector<string> files2;
				for (auto& f : files) {
					string endstr = f.substr(f.length() - 3, 3);

					if ((endstr == "obj") || (endstr == "stl") || (endstr == "STL")) {
						files2.push_back(f);
					}

				}

				assert(files2.size() == 2);
				files = files2;

				if (!OpenMesh::IO::read_mesh(meshA, files[0])) {

					std::cerr << "Cannot read mesh" + files[0] << std::endl;

					return;

				}
				if (!OpenMesh::IO::read_mesh(meshB, files[1])) {

					std::cerr << "Cannot read mesh" + files[1] << std::endl;

					return;

				}



				auto t0 = std::chrono::steady_clock::now();

				//进行布尔运算
				Mesh result;
				MeshBoolOperate mBo(meshA, meshB, bt);

				//mBo.Run(result, MeshBoolOperateType::Difference);
				Mesh r_MeshA, r_MeshB;
				mBo.RunTest(result, MeshBoolOperateType::Difference, r_MeshA, r_MeshB, false, BSP_TREE_TYPE);

				auto t1 = std::chrono::steady_clock::now();
				cout << "boolean time：" << std::chrono::duration<double, std::milli>(t1 - t0).count() << "毫秒" << endl;

				ofstream of5("..//TestModel//batch bool//" + BSP_TREE_TYPE +"TotalTime.txt", ios::app);
				of5 << std::chrono::duration<double, std::milli>(t1 - t0).count() << endl;
				of5.close();
			}

				
		}
			
			


			//vector<double> ts;
			//for (int i = 0; i < 10;i++) {



			//	ts.push_back(std::chrono::duration<double, std::milli>(t1 - t0).count());
			//}




		}

	}

	void TestWrite()
	{
		Mesh meshA, meshB;

		//if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//bunny.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		//MeshNormalize(meshA,0.2);

		GenerateMesh::CubicMesh(0.9, Mesh::Point(0, 0, 0), meshB);
		MeshTransform(meshB, Vector3d(5, 0, 0));
		MeshTransform(meshB, Vector3d(-0.001, 0.1, -0.1));
		 
		//if (!OpenMesh::IO::write_mesh(meshA, "..//TestModel//bunnyA.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;

		//}

		if (!OpenMesh::IO::write_mesh(meshB, "..//TestModel//cubicB.obj")) {

			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

			return;

		}

		//MeshTransform(meshA, Vector3d(0.5, 0, 0));
		//if (!OpenMesh::IO::write_mesh(meshA, "..//TestModel//bunny2.obj")) {

		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//	return;
		//}
	}

	void TestReadObjWriteStl()
	{
		Mesh meshA,meshB;

		if (!OpenMesh::IO::read_mesh(meshA, "C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bunny\\16\\bunnyA.STL")) {

			std::cerr << "Cannot write mesh to file 'bunny.obj'" << std::endl;

			return;

		}
		if (!OpenMesh::IO::read_mesh(meshB, "C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bunny\\16\\bunnyB.STL")) {

			std::cerr << "Cannot write mesh to file 'bunny.obj'" << std::endl;

			return;

		}



		if (!OpenMesh::IO::write_mesh(meshA, "C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bunny\\16\\bunnyA.obj")) {

			std::cerr << "Cannot write mesh to file 'bunny.stl'" << std::endl;

			return;

		}
		if (!OpenMesh::IO::write_mesh(meshB, "C:\\Users\\NUOSEN\\Desktop\\work\\BSP\\bool-new3-d\\TestModel\\bunny\\16\\bunnyB.obj")) {

			std::cerr << "Cannot write mesh to file 'bunny.stl'" << std::endl;

			return;

		}
	}
	
	void TestOpenMesh()
	{
		Mesh meshA, meshB;

		if (!OpenMesh::IO::read_mesh(meshA, "C:\\Users\\NUOSEN\\Desktop\\BSP\\bool-new3-d\\TestModel\\bunny\\6\\subtree1.obj"))
		{

			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

			return;

		}

		// write mesh to output.obj


		RenderWnd wnd;
		RenderScence scence;

		//显示meshA
		if (1)
		{
			Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshA);
			Edge_Set thisEdge = DataTranslate::TranslateEdge(meshA);
			Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshA);

			{
				auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
				ra->SetTriangleColor(glm::vec3(1.0f, 0.0f, 0.0f));
				scence.AddUnit(ra);
			}

			{
				auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
				ra->SetVertexSize(15.0);
				ra->SetVertexColor(glm::vec3(0.0f, 1.0f, 1.0f));
				scence.AddUnit(ra);
			}

			{
				auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

				//ra->SetVertexSize(15.0);
				ra->SetEdgeColor(glm::vec3(0.0f, 1.0f, 0.0f));
				scence.AddUnit(ra);
			}
		}

		////显示meshB
		//if (1)
		//{
		//	Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshB);
		//	Edge_Set thisEdge = DataTranslate::TranslateEdge(meshB);
		//	Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshB);

		//	{
		//		auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//		ra->SetTriangleColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//		scence.AddUnit(ra);
		//	}

		//	{
		//		auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//		ra->SetVertexSize(15.0);
		//		ra->SetVertexColor(glm::vec3(0.0f, 1.0f, 1.0f));
		//		scence.AddUnit(ra);
		//	}

		//	{
		//		auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

		//		//ra->SetVertexSize(15.0);
		//		ra->SetEdgeColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//		scence.AddUnit(ra);
		//	}
		//}


		wnd.SetScence(&scence);
		wnd.Run();


		//Mesh tMesh;

		//Mesh::VertexHandle vhandle[8];

		//vhandle[0] = tMesh.add_vertex(Mesh::Point(0, 0, 2));

		//vhandle[1] = tMesh.add_vertex(Mesh::Point(2, 0, 2));

		//vhandle[2] = tMesh.add_vertex(Mesh::Point(2, 2, 2));

		//vhandle[3] = tMesh.add_vertex(Mesh::Point(0, 2, 2));

		//vhandle[4] = tMesh.add_vertex(Mesh::Point(0, 0, 0));

		//vhandle[5] = tMesh.add_vertex(Mesh::Point(2, 0, 0));

		//vhandle[6] = tMesh.add_vertex(Mesh::Point(2, 2, 0));

		//vhandle[7] = tMesh.add_vertex(Mesh::Point(0, 2, 0));

		//// generate (quadrilateral) faces

		//std::vector<Mesh::VertexHandle>  face_vhandles;

		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[0]);

		//face_vhandles.push_back(vhandle[1]);

		//face_vhandles.push_back(vhandle[2]);

		//face_vhandles.push_back(vhandle[3]);

		//tMesh.add_face(face_vhandles);



		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[7]);

		//face_vhandles.push_back(vhandle[6]);

		//face_vhandles.push_back(vhandle[5]);

		//face_vhandles.push_back(vhandle[4]);

		//tMesh.add_face(face_vhandles);



		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[1]);

		//face_vhandles.push_back(vhandle[0]);

		//face_vhandles.push_back(vhandle[4]);

		//face_vhandles.push_back(vhandle[5]);

		//tMesh.add_face(face_vhandles);



		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[2]);

		//face_vhandles.push_back(vhandle[1]);

		//face_vhandles.push_back(vhandle[5]);

		//face_vhandles.push_back(vhandle[6]);

		//tMesh.add_face(face_vhandles);



		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[3]);

		//face_vhandles.push_back(vhandle[2]);

		//face_vhandles.push_back(vhandle[6]);

		//face_vhandles.push_back(vhandle[7]);

		//tMesh.add_face(face_vhandles);


		//face_vhandles.clear();

		//face_vhandles.push_back(vhandle[0]);

		//face_vhandles.push_back(vhandle[3]);

		//face_vhandles.push_back(vhandle[7]);

		//face_vhandles.push_back(vhandle[4]);

		//tMesh.add_face(face_vhandles);

		//tMesh.request_edge_status();
		//tMesh.request_vertex_status();
		//tMesh.request_face_status();



		/*
		//cout <<"==================" << endl;

		//{
		//	Mesh::FaceHandle fh = tMesh.face_handle(0);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	cout << "Face0 p1: " << p1 << endl;
		//	cout << "Face0 p2: " << p2 << endl;
		//	cout << "Face0 p3: " << p3 << endl;
		//}

		//{
		//	Mesh::FaceHandle fh = tMesh.face_handle(1);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	cout << "Face1 p1: " << p1 << endl;
		//	cout << "Face1 p2: " << p2 << endl;
		//	cout << "Face1 p3: " << p3 << endl;
		//}

		//{
		//	Mesh::FaceHandle fh = tMesh.face_handle(2);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	cout << "Face2 p1: " << p1 << endl;
		//	cout << "Face2 p2: " << p2 << endl;
		//	cout << "Face2 p3: " << p3 << endl;
		//}


		//{
		//	cout << "=====================" << endl;

		//	Mesh::FaceHandle fh = *tMesh.faces_begin();

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	Mesh::Point pC = (p1 + p2 + p3) / 3;

		//	Mesh::VertexHandle va = tMesh.add_vertex(pC);

		//	tMesh.delete_face(fh, false);


		//	face_vhandles.clear();
		//	face_vhandles.push_back(v0);
		//	face_vhandles.push_back(v1);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace1 = tMesh.add_face(face_vhandles);

		//	face_vhandles.clear();
		//	face_vhandles.push_back(v2);
		//	face_vhandles.push_back(v0);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace2 = tMesh.add_face(face_vhandles);

		//	face_vhandles.clear();
		//	face_vhandles.push_back(v1);
		//	face_vhandles.push_back(v2);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace3 = tMesh.add_face(face_vhandles);

		//	int j = 0;
		//	if (tMesh.is_valid_handle(newFace1)) j++;
		//	if (tMesh.is_valid_handle(newFace2)) j++;
		//	if (tMesh.is_valid_handle(newFace3)) j++;

		//	cout << newFace1.idx() << endl;
		//	cout << newFace2.idx() << endl;
		//	cout << newFace3.idx() << endl;

		//	cout << j << endl;

		//	{
		//		cout << "new face point 1" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace1);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}

		//	{
		//		cout << "new face point 2" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace2);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}

		//	{
		//		cout << "new face point 3" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace3);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}
		//}


		//{
		//	cout << "=====================" << endl;

		//	Mesh::FaceHandle fh = tMesh.face_handle(2);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	Mesh::Point pC = (p1 + p2 + p3) / 3;

		//	Mesh::VertexHandle va = tMesh.add_vertex(pC);

		//	tMesh.delete_face(fh, false);


		//	face_vhandles.clear();
		//	face_vhandles.push_back(v0);
		//	face_vhandles.push_back(v1);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace1 = tMesh.add_face(face_vhandles);

		//	face_vhandles.clear();
		//	face_vhandles.push_back(v2);
		//	face_vhandles.push_back(v0);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace2 = tMesh.add_face(face_vhandles);

		//	face_vhandles.clear();
		//	face_vhandles.push_back(v1);
		//	face_vhandles.push_back(v2);
		//	face_vhandles.push_back(va);
		//	Mesh::FaceHandle newFace3 = tMesh.add_face(face_vhandles);

		//	int j = 0;
		//	if (tMesh.is_valid_handle(newFace1)) j++;
		//	if (tMesh.is_valid_handle(newFace2)) j++;
		//	if (tMesh.is_valid_handle(newFace3)) j++;

		//	cout << newFace1.idx() << endl;
		//	cout << newFace2.idx() << endl;
		//	cout << newFace3.idx() << endl;

		//	cout << j << endl;

		//	{
		//		cout << "new face point 1" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace1);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}

		//	{
		//		cout << "new face point 2" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace2);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}

		//	{
		//		cout << "new face point 3" << endl;
		//		Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(newFace3);

		//		Mesh::VertexHandle v0 = *(fv++);
		//		Mesh::VertexHandle v1 = *(fv++);
		//		Mesh::VertexHandle v2 = *(fv++);

		//		Mesh::Point p1 = tMesh.point(v0);
		//		Mesh::Point p2 = tMesh.point(v1);
		//		Mesh::Point p3 = tMesh.point(v2);

		//		cout << "Face0 p1: " << p1 << endl;
		//		cout << "Face0 p2: " << p2 << endl;
		//		cout << "Face0 p3: " << p3 << endl;
		//	}
		//}


		//cout << "===============" << endl;

		//for (int i = 0;i < tMesh.n_faces();i++) {
		//
		//	Mesh::FaceHandle fh = tMesh.face_handle(i);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	cout << "Face" << i << " p1: " << p1 << endl;
		//	cout << "Face" << i << " p2: " << p2 << endl;
		//	cout << "Face" << i << " p3: " << p3 << endl;

		//	cout  << endl;
		//}

		//tMesh.garbage_collection();

		//cout << "****************" << endl;
		//for (int i = 0;i < tMesh.n_faces();i++) {

		//	Mesh::FaceHandle fh = tMesh.face_handle(i);

		//	Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//	Mesh::VertexHandle v0 = *(fv++);
		//	Mesh::VertexHandle v1 = *(fv++);
		//	Mesh::VertexHandle v2 = *(fv++);

		//	Mesh::Point p1 = tMesh.point(v0);
		//	Mesh::Point p2 = tMesh.point(v1);
		//	Mesh::Point p3 = tMesh.point(v2);

		//	cout << "Face" << i << " p1: " << p1 << endl;
		//	cout << "Face" << i << " p2: " << p2 << endl;
		//	cout << "Face" << i << " p3: " << p3 << endl;

		//	cout << endl;
		//}

		//cout << "===============" << endl;
		*/

		//Mesh::FaceHandle fh = *tMesh.faces_begin();

		//Mesh::FaceVertexCCWIter fv = tMesh.fv_ccwbegin(fh);

		//Mesh::VertexHandle v0 = *(fv++);
		//Mesh::VertexHandle v1 = *(fv++);

		//Mesh::Point p1 = tMesh.point(v0);
		//Mesh::Point p2 = tMesh.point(v1);

		//Mesh::Point pC = (p1 + p2) / 2;

		//for (Mesh::FaceIter fi = tMesh.faces_begin(); fi != tMesh.faces_end(); ++fi) {
		//	cout << (*fi).idx() << endl;
		//}

		//Mesh::EdgeHandle eh = tMesh.edge_handle(tMesh.find_halfedge(v0, v1));
		//Mesh::VertexHandle va = tMesh.add_vertex(pC);
		// 
		//tMesh.split_edge(eh, va);

		//for (Mesh::FaceIter fi = tMesh.faces_begin(); fi != tMesh.faces_end(); ++fi) {
		//	cout << (*fi).idx() << endl;
		//}

		//Mesh::EdgeHandle eh = *mesh.edges_begin();
		//Mesh::HalfedgeHandle he1 = mesh.halfedge_handle(eh, 0);
		//Mesh::HalfedgeHandle he2 = mesh.halfedge_handle(eh, 1);

		//cout << mesh.point(mesh.from_vertex_handle(he1)) << endl;
		//cout << mesh.point(mesh.to_vertex_handle(he1)) << endl;

		//cout << mesh.point(mesh.from_vertex_handle(he2)) << endl;
		//cout << mesh.point(mesh.to_vertex_handle(he2)) << endl;

		//mesh.flip(eh);

		//cout << mesh.point(mesh.from_vertex_handle(he1)) << endl;
		//cout << mesh.point(mesh.to_vertex_handle(he1)) << endl;

		//cout << mesh.point(mesh.from_vertex_handle(he2)) << endl;
		//cout << mesh.point(mesh.to_vertex_handle(he2)) << endl;




		//// write mesh to output.obj

		//try

		//{

		//	if (!OpenMesh::IO::write_mesh(tMesh, "output.off"))

		//	{

		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;

		//		return;

		//	}

		//}

		//catch (std::exception& x)

		//{

		//	std::cerr << x.what() << std::endl;

		//	return;

		//}



		return;
	}

	void TestMath()
	{
		Vector3d v(0.7, 0.7, 0.7);
		
		auto v2 = v.normalized();
		cout << v.length() << endl;
		cout << v2.length() << endl;

		v.normalize();
		cout << v.length() << endl;

		
	}

	void TestBspTreeSDMParameters()
	{
		

		ofstream of("..//TreePara.txt", ios::trunc);
		vector<double> treeParas{ 2,1,0.5 };

		for (auto& p1 : treeParas) {
			for (auto& p2 : treeParas) {

				of << "lambdaS: " << p1 << ", lambdaD: " << p2 << endl;

				//for (int i = 7; i < 17;i++) {
				//	if (i == 14)
				//		continue;
				//	if (i == 15)
				//		continue;
				//  Mesh meshA, meshB;
				//	string testResult_Num = to_string(i);

				//	string fileType = "obj";
				//	string fileTitle = "bunny//";

				//	string outputPartitionModelAddress = "..//TestModel//" + fileTitle + testResult_Num + "//TestTreeResult//";


				//	if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyA." + fileType)) {
				//		//if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//bool operate//" + testResult_Num + "//bunnyA." + fileType)) {
				//		std::cerr << "Cannot read mesh" << std::endl;

				//		return;

				//	}
				//	if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyB." + fileType)) {
				//		//if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//bool operate//" + testResult_Num + "//bunnyB." + fileType)) {
				//		std::cerr << "Cannot read mesh" << std::endl;

				//		return;

				//	}


				for (int i = 1; i < 51; i++) {

					cout << "thingi10k:============== " << i << endl;

					Mesh meshA, meshB;

					string baseAddress = "..//TestModel//thingi10k//" + to_string(i);

					string outputPartitionModelAddress = "..//TestModel//thingi10k//";

					vector<string>  files;
					getFileNames(baseAddress, files);

					if (!OpenMesh::IO::read_mesh(meshA, files[0])) {

						std::cerr << "Cannot read mesh" << std::endl;

						return;

					}
					if (!OpenMesh::IO::read_mesh(meshB, files[1])) {

						std::cerr << "Cannot read mesh" << std::endl;

						return;

					}


					Tolerance toler;

					{
						meshA.request_vertex_status();
						meshA.request_edge_status();
						meshA.request_face_status();

						meshB.request_vertex_status();
						meshB.request_edge_status();
						meshB.request_face_status();

						meshA.request_face_normals();
						meshB.request_face_normals();

						meshA.update_face_normals();
						meshB.update_face_normals();
					}

					PreProcess pr({ &meshA,&meshB }, toler);
					pr.run();
			
					array<double, 5> checkPairInfo;

					auto SDMUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkPairInfo, BSPConstructType::WSDM, p1, p2);
					cout << "========" << endl;
					cout << "lambdaS: " << p1 << ", lambdaD: " << p2 << ", SDMUTFR: " << SDMUTFR.first << endl;
					cout << "========" << endl;

					of << SDMUTFR.first << endl;
	
				}

			}

		}
		of.close();

	}

	void TestBspTree()
	{
		//BatchTestClassicModels();
		//BatchTestThingi10K();

		SingleTestClassicModelsForBSP();
		//SingleTestThingi10K();

		//

		//{
		//	auto* ra = new TrianglesRenderer<Triangle_Set>(planeTris);
		//	ra->SetTriangleColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//	scence.AddUnit(ra);
		//}

		//{
		//	auto* ra = new EdgesRenderer<Edge_Set>(planeEdges);

		//	ra->SetEdgeWidth(10);
		//	ra->SetEdgeColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//	scence.AddUnit(ra);
		//}

		//if (!OpenMesh::IO::write_mesh(planeMesh, outputAddress)) {
		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//	return;
		//}








		//if (false) {
		//	//显示meshA
		//	if (true)
		//	{

		//		Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshA);
		//		Edge_Set thisEdge = DataTranslate::TranslateEdge(meshA);
		//		Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshA);

		//		{
		//			auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//			ra->SetTriangleColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//			scence.AddUnit(ra);
		//		}


		//		{
		//			auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//			ra->SetVertexSize(15.0);
		//			ra->SetVertexColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//			scence.AddUnit(ra);
		//		}

		//		{
		//			auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

		//			//ra->SetVertexSize(15.0);
		//			ra->SetEdgeColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//			scence.AddUnit(ra);
		//		}
		//	}

		//	//显示meshB
		//	if (true)
		//	{

		//		Triangle_Set thisMesh = DataTranslate::TranslateMesh(meshB);
		//		Edge_Set thisEdge = DataTranslate::TranslateEdge(meshB);
		//		Vertex_Set thisVertex = DataTranslate::TranslateVertex(meshB);

		//		{
		//			auto* ra = new TrianglesRenderer<Triangle_Set>(thisMesh);
		//			ra->SetTriangleColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//			scence.AddUnit(ra);
		//		}


		//		{
		//			auto* ra = new VerticesRenderer<Vertex_Set>(thisVertex);
		//			ra->SetVertexSize(15.0);
		//			ra->SetVertexColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//			scence.AddUnit(ra);
		//		}

		//		{
		//			auto* ra = new EdgesRenderer<Edge_Set>(thisEdge);

		//			//ra->SetVertexSize(15.0);
		//			ra->SetEdgeColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//			scence.AddUnit(ra);
		//		}
		//	}
		//}




	}

	void TestOctTree()
	{
		Mesh meshA, meshB;

		string testResult_Num = "25";
		string fileType = "obj";
		string fileTitle = "bunny//";

		string outputPartitionPlaneAddress1 = "..//TestModel//" + fileTitle + testResult_Num + "//OCTPartitionPlane1.obj";
		string outputPartitionPlaneAddress2 = "..//TestModel//" + fileTitle + testResult_Num + "//OCTPartitionPlane2.obj";
		string outputPartitionPlaneAddress3 = "..//TestModel//" + fileTitle + testResult_Num + "//OCTPartitionPlane3.obj";
		string outputPartitionPlaneAddress4 = "..//TestModel//" + fileTitle + testResult_Num + "//OCTPartitionPlane4.obj";


		string outputPartitionModelAddress = "..//TestModel//" + fileTitle + testResult_Num + "//TestTreeResult//";


		if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyA." + fileType)) {
			//if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//bool operate//" + testResult_Num + "//bunnyA." + fileType)) {
			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}
		if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyB." + fileType)) {
			//if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//bool operate//" + testResult_Num + "//bunnyB." + fileType)) {
			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}



		Tolerance toler;

		{
			meshA.request_vertex_status();
			meshA.request_edge_status();
			meshA.request_face_status();

			meshB.request_vertex_status();
			meshB.request_edge_status();
			meshB.request_face_status();

			meshA.request_face_normals();
			meshB.request_face_normals();

			meshA.update_face_normals();
			meshB.update_face_normals();
		}


		PreProcess pr({ &meshA,&meshB }, toler);
		pr.run();

		array<double, 5> checkpairInfo;

		//auto SDMUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SDM);
		//auto SAHUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SAH);
		//auto ObbMiddel = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::ObbMiddel);
		//auto Gravity = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::Gravity_SPLIT);
		auto Oct = TestOctTimeConsum(meshA, meshB, toler, 4, 25, outputPartitionModelAddress, true, true , checkpairInfo);

		cout << "========" << endl;
		//cout << "SDMUTFR:     " << SDMUTFR.first << endl;
		//cout << "SAHUTFR:     " << SAHUTFR.first << endl;
		//cout << "ObbMiddelUTFR:  " << ObbMiddel.first << endl;
		//cout << "GravityUTFR: " << Gravity.first << endl;
		cout << "OCTUTFR:     " << Oct.first << endl;
		cout << "OCTITRA:     " << Oct.second << endl;
		cout << "========" << endl;


		////显示分割平面	 
		//{
		//	BSPTree* faceTree = new BSPTree(meshA, meshB, toler, 10, 25, BSPConstructType::SDM);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);


		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.2);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	//tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress2)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}

		////显示分割平面	 
		//{
		//	OCTree* faceTree = new OCTree(meshA, meshB, toler, 4, 25);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);

		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.1);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress1)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}


	}

	void SingleTestClassicModelsForBSP()
	{

		Mesh meshA, meshB;

		string testResult_Num = "12";
		string fileType = "obj";
		string fileTitle = "bool operate//";

		string outputPartitionPlaneAddress1 = "..//TestModel//" + fileTitle + testResult_Num + "//PartitionPlane1.obj";
		string outputPartitionPlaneAddress2 = "..//TestModel//" + fileTitle + testResult_Num + "//PartitionPlane2.obj";
		string outputPartitionPlaneAddress3 = "..//TestModel//" + fileTitle + testResult_Num + "//PartitionPlane3.obj";
		string outputPartitionPlaneAddress4 = "..//TestModel//" + fileTitle + testResult_Num + "//PartitionPlane4.obj";


		string outputPartitionModelAddress = "..//TestModel//" + fileTitle + testResult_Num + "//TestTreeResult//";


		if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyA." + fileType)) {
		//if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//bool operate//" + testResult_Num + "//bunnyA." + fileType)) {
			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}
		if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//" + fileTitle + testResult_Num + "//bunnyB." + fileType)) {
		//if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//bool operate//" + testResult_Num + "//bunnyB." + fileType)) {
			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}

		//AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//Mesh MboxA = boxA.Transform2Mesh();
		//Mesh MboxB = boxB.Transform2Mesh();

		//if (!OpenMesh::IO::write_mesh(MboxA, "..//TestModel//" + fileTitle + testResult_Num + "//AABB1.obj")) {
		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//	return;
		//}

		//if (!OpenMesh::IO::write_mesh(MboxB, "..//TestModel//" + fileTitle + testResult_Num + "//AABB2.obj")) {
		//	std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//	return;
		//}

		Tolerance toler;

		{
			meshA.request_vertex_status();
			meshA.request_edge_status();
			meshA.request_face_status();

			meshB.request_vertex_status();
			meshB.request_edge_status();
			meshB.request_face_status();

			meshA.request_face_normals();
			meshB.request_face_normals();

			meshA.update_face_normals();
			meshB.update_face_normals();
		}


		PreProcess pr({ &meshA,&meshB }, toler);
		pr.run();

		//ofstream outFileCheckPairs("..//TestModel//" + fileTitle + "//CheckPairs.txt");
		//ofstream outFileCheckIntersectionTime("..//TestModel//" + fileTitle + "//CheckIntersectionTime.txt");

		//for (int i = 3; i < 16;i++) {


		//	array<double, 5> checkpairInfoWSDM, checkpairInfoSAH, checkpairInfoObbMiddel, checkpairInfoGravity, checkpairInfoOCT;

		//	auto WSDMNTRR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, true, checkpairInfoWSDM, BSPConstructType::WSDM);
		//	auto SAHNTRR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, true, checkpairInfoSAH, BSPConstructType::SAH);
		//	auto ObbMiddelNTRR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, true, checkpairInfoObbMiddel, BSPConstructType::ObbMiddel);
		//	auto GravityNTRR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, true, checkpairInfoGravity, BSPConstructType::Gravity_SPLIT);
		//	
		//	if (i % 3 == 0) {
		//		auto OCTResNTRR = TestOctTimeConsum(meshA, meshB, toler, i/3, 25, outputPartitionModelAddress, false, true, checkpairInfoOCT);			
		//	}

		//	outFileCheckPairs << checkpairInfoWSDM[0] << ", " << checkpairInfoSAH[0] << ", " << checkpairInfoObbMiddel[0] << ", " << checkpairInfoGravity[0] << ", " << checkpairInfoOCT[0] << endl;
		//	outFileCheckIntersectionTime << checkpairInfoWSDM[3] << ", " << checkpairInfoSAH[3] << ", " << checkpairInfoObbMiddel[3] << ", " << checkpairInfoGravity[3] << ", " << checkpairInfoOCT[3] << endl;			
		//}

		//outFileCheckPairs.close();
		//outFileCheckIntersectionTime.close();

		//TestBspNodeDistributionInfo(meshA, meshB, toler, 15, 5, outputPartitionModelAddress, true, BSPConstructType::SDM);
		//TestBspNodeDistributionInfo(meshA, meshB, toler, 15, 5, outputPartitionModelAddress, true, BSPConstructType::ObbMiddel);

		//for (int i = 3;i >= 1;i--) {
			//TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SDM);
			//TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::ObbMiddel);
			//TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SAH);
			//TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::Gravity_SPLIT);
		//}

		//TestBspTimeConsum(meshA, meshB, toler, 1, 25, outputPartitionModelAddress, false, true, BSPConstructType::SDM);
		//TestBspTimeConsum(meshA, meshB, toler, 2, 25, outputPartitionModelAddress, false, true, BSPConstructType::SDM);
		//TestBspTimeConsum(meshA, meshB, toler, 3, 25, outputPartitionModelAddress, false, true, BSPConstructType::SDM);
		
		//TestBspTimeConsum(meshA, meshB, toler, 3, 25, outputPartitionModelAddress, false, true, BSPConstructType::SAH);
		//TestBspTimeConsum(meshA, meshB, toler, 3, 25, outputPartitionModelAddress, false, true, BSPConstructType::ObbMiddel);
		//TestBspTimeConsum(meshA, meshB, toler, 3, 25, outputPartitionModelAddress, false, true, BSPConstructType::Gravity_SPLIT);
		
		//TestBspNodeDistributionInfo(meshA, meshB, toler, 15, 5, outputPartitionModelAddress, true, BSPConstructType::SDM);
		//TestBspNodeDistributionInfo(meshA, meshB, toler, 15, 5, outputPartitionModelAddress, true, BSPConstructType::ObbMiddel);
		//TestBspNodeDistributionInfo(meshA, meshB, toler, 15, 5, outputPartitionModelAddress, true, BSPConstructType::Gravity_SPLIT);

		//pair<int, int> checkpairInfo;

		//auto WSDM_PCA_UTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfo,BSPConstructType::WSDM_PCA);
		//auto SDMUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfo, BSPConstructType::WSDM);

		//for (int i = 5; i < 18;i++) {
		//	auto WSDM_PCA_UTFR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, false, BSPConstructType::WSDM_PCA);
		//	auto SDMUTFR = TestBspTimeConsum(meshA, meshB, toler, i, 25, outputPartitionModelAddress, false, false, BSPConstructType::WSDM);


		//}
		//auto SAHUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::SAH);
		//auto ObbMiddel = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::ObbMiddel);
		//auto Gravity = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::Gravity_SPLIT);
		
		//auto AngleSerial = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::SerialAngle);
		////auto Angle = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::Angle);
		////auto AngleSDM = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, BSPConstructType::SDM_Angle);

		//

		//ofstream outFileNTRR("..//TestModel//metrics result//NtrrRes.txt");
		//ofstream outFileCheckPairs("..//TestModel//metrics result//CheckpairsRes.txt");
		////ofstream outFileMeanCheckPairs("..//TestModel//metrics result//MeanCheckpairsRes.txt");


		//AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//boxA.Add(boxB);

		//Point3d modelsCenter = boxA.GetCenter();


		//for (int i = 0; i < 6;i++) {			
		//	//rotate along x axis
		//	Mesh rotate_MeshA = MeshRotate(meshA, modelsCenter, { 1,0,0 }, i * 30);
		//	Mesh rotate_MeshB = MeshRotate(meshB, modelsCenter, { 1,0,0 }, i * 30);

		//	for (int j = 0; j < 6;j++) {
		//		//rotate along y axis
		//		rotate_MeshA = MeshRotate(rotate_MeshA, modelsCenter, { 0,1,0 }, j * 30);
		//		rotate_MeshB = MeshRotate(rotate_MeshB, modelsCenter, { 0,1,0 }, j * 30);

		//		for (int k = 0; k < 6;k++) {
		//			cout << "rotate Z: " << k << endl;

		//			//rotate along z axis
		//			rotate_MeshA = MeshRotate(rotate_MeshA, modelsCenter, { 0,0,1 }, k * 30);
		//			rotate_MeshB = MeshRotate(rotate_MeshB, modelsCenter, { 0,0,1 }, k * 30);


		//			array<double, 5> checkpairInfoWSDM_PCA, checkpairInfoWSDM;

		//			auto WSDM_PCA_UTFR = TestBspTimeConsum(rotate_MeshA, rotate_MeshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoWSDM_PCA, BSPConstructType::WSDM_PCA);
		//			auto WSDMUTFR = TestBspTimeConsum(rotate_MeshA, rotate_MeshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoWSDM, BSPConstructType::WSDM);


		//			outFileNTRR << WSDM_PCA_UTFR.first << ", " << WSDMUTFR.first << ",";
		//			outFileCheckPairs << checkpairInfoWSDM_PCA[0] << ", " << checkpairInfoWSDM[0] << ",";
		//		}
		//		outFileNTRR << endl;
		//		outFileCheckPairs << endl;
		//	}

		//	outFileNTRR << "========================================" << endl;
		//	outFileCheckPairs << "========================================" << endl;
		//}

		//outFileNTRR.close();
		//outFileCheckPairs.close();





		//outFileMeanCheckPairs.close();

		//cout << "========" << endl;

		//cout << "WSDM_PCAUTFR:     " << WSDM_PCA_UTFR.first << endl;
		//cout << "WSDMUTFR:     " << SDMUTFR.first << endl;
		//cout << "SAHUTFR:     " << SAHUTFR.first << endl;
		//cout << "ObbMiddelUTFR:  " << ObbMiddel.first << endl;
		//cout << "GravityUTFR: " << Gravity.first << endl;
		
		
		////cout << "AngleSerialUTFR: " << AngleSerial.first << endl;
		////cout << "AngleUTFR: " << Angle.first << endl;
		////cout << "AngleSDMUTFR: " << AngleSDM.first << endl;

		//cout << "SDMITRA:     " << SDMUTFR.second << endl;
		////cout << "SAHITRA:     " << SAHUTFR.second << endl;
		////cout << "ObbMiddelITRA:  " << ObbMiddel.second << endl;
		////cout << "GravityITRA: " << Gravity.second << endl;


			array<double, 5> checkpairInfoWSDM, checkpairInfoSAH, checkpairInfoObbMiddel, checkpairInfoGravity, checkpairInfoOCT;

			auto WSDMNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, checkpairInfoWSDM, BSPConstructType::WSDM);
			auto OCTResNTRR = TestOctTimeConsum(meshA, meshB, toler, 4, 25, outputPartitionModelAddress, true, true, checkpairInfoOCT);

			
			auto SAHNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, checkpairInfoSAH, BSPConstructType::SAH);
			auto ObbMiddelNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, checkpairInfoObbMiddel, BSPConstructType::ObbMiddel);
			auto GravityNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, checkpairInfoGravity, BSPConstructType::Gravity_SPLIT);
	

			cout << "WSDMUTFR:     " << WSDMNTRR.first << endl;
			cout << "SAHUTFR:     " << SAHNTRR.first << endl;
			cout << "ObbMiddelUTFR:  " << ObbMiddelNTRR.first << endl;
			cout << "GravityUTFR: " << GravityNTRR.first << endl;
			cout << "OCTUTFR: " << OCTResNTRR.first << endl;

			cout << "SDMITRA:     " << WSDMNTRR.second << endl;
			cout << "SAHITRA:     " << SAHNTRR.second << endl;
			cout << "ObbMiddelITRA:  " << ObbMiddelNTRR.second << endl;
			cout << "GravityITRA: " << GravityNTRR.second << endl;
			cout << "OCTITRA: " << OCTResNTRR.second << endl;

		std::cout << "========" << endl;	

		////显示分割平面	 
		//{
		//	BSPTree* faceTree = new BSPTree(meshA, meshB, toler, 9, 25, BSPConstructType::WSDM);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);


		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.2);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress1)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}
 
		////显示分割平面	 
		//{
		//	BSPTree* faceTree = new BSPTree(meshA, meshB, toler, 3, 25, BSPConstructType::WSDM_PCA);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);


		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.2);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	//tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress2)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}

		////显示分割平面	 
		//{
		//	BSPTree* faceTree = new BSPTree(meshA, meshB, toler, 10, 25, BSPConstructType::ObbMiddel);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);


		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.2);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	//tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress2)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}

		////显示分割平面	 
		//{
		//	BSPTree* faceTree = new BSPTree(meshA, meshB, toler, 10, 25, BSPConstructType::SAH);
		//	TreeFaceRenderingInfo partiPlanes;
		//	faceTree->GetPartitionPlane(partiPlanes);


		//	AABB boxA(DataTranslate::TranslateMesh2AABB(meshA));
		//	AABB boxB(DataTranslate::TranslateMesh2AABB(meshB));

		//	boxA.Add(boxB);
		//	boxA.Scale(1.2);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	//tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress3)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}



		//{
		//	BSPTree* faceTree1 = new BSPTree(meshA, meshB, toler, 1, 25, BSPConstructType::ObbMiddel);
		//	//TestBspTimeConsum(meshA, meshB, toler, 3, 25, outputPartitionModelAddress, false, BSPConstructType::Gravity_SPLIT);

		//	bspFaceRenderingInfo partiPlanes;
		//	faceTree1->GetPartitionPlane(partiPlanes);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress1)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}


			////显示子树划分出的三角网格
			//vector<BSPTreeNode*> allLeafNodes;
			//faceTree1->GetAllLeafNode(allLeafNodes);

			//cout << meshA.n_faces() << endl;
			//cout << meshB.n_faces() << endl;

			//int ii = 0;
			//for (BSPTreeNode* noded : allLeafNodes) {
			//	ii++;
			//	Mesh tempMeshA, tempMeshB;
			//	tie(tempMeshA, tempMeshB) = DataTranslate::TranslateNode2SeperatedTriangleMesh(noded);

			//	cout << tempMeshA.n_faces() << endl;
			//	cout << tempMeshB.n_faces() << endl;

			//	if (!OpenMesh::IO::write_mesh(tempMeshA, "..//TestModel//bunny//" + testResult_Num + "//" + to_string(ii) + "subMeshA.obj")) {
			//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			//		return;
			//	}
			//	if (!OpenMesh::IO::write_mesh(tempMeshB, "..//TestModel//bunny//" + testResult_Num + "//" + to_string(ii) + "subMeshB.obj")) {
			//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			//		return;
			//	}

			//}


		//	delete faceTree1;
		//}


	}

	void SingleTestThingi10K()
	{

		int num = 20;
		Mesh meshA, meshB;

		string baseAddress = "..//TestModel//thingi10k//" + to_string(num);

		vector<string>  files;
		getFileNames(baseAddress, files);

		if (!OpenMesh::IO::read_mesh(meshA, files[0])) {

			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}
		if (!OpenMesh::IO::read_mesh(meshB, files[1])) {

			std::cerr << "Cannot read mesh" << std::endl;

			return;

		}




		Tolerance toler;

		{
			meshA.request_vertex_status();
			meshA.request_edge_status();
			meshA.request_face_status();

			meshB.request_vertex_status();
			meshB.request_edge_status();
			meshB.request_face_status();

			meshA.request_face_normals();
			meshB.request_face_normals();

			meshA.update_face_normals();
			meshB.update_face_normals();
		}


		PreProcess pr({ &meshA,&meshB }, toler);
		pr.run();

		string outputPartitionModelAddress = "";

		array<double, 5> checkpairInfo;

		auto SDMUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfo,BSPConstructType::WSDM);
		auto SAHUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfo, BSPConstructType::SAH);
		auto ObbMiddel = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfo, BSPConstructType::ObbMiddel);
		auto Gravity = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfo, BSPConstructType::Gravity_SPLIT);

		cout << "========" << endl;
		cout << "SDMUTFR:     " << SDMUTFR.first << endl;
		cout << "SAHUTFR:     " << SAHUTFR.first << endl;
		cout << "ObbMiddelUTFR:  " << ObbMiddel.first << endl;
		cout << "GravityUTFR: " << Gravity.first << endl;
		cout << "========" << endl;

		//BSPTree* faceTree1 = new BSPTree(meshA, meshB, toler, 1, 5, BSPConstructType::SDM);
		//BSPTree* faceTree2 = new BSPTree(meshA, meshB, toler, 1, 5, BSPConstructType::ObbMiddel);

		////显示分割平面	 
		//{
		//	bspFaceRenderingInfo partiPlanes;
		//	faceTree1->GetPartitionPlane(partiPlanes);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress1)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}

		////显示分割后网格
		//{
		//	//显示子树划分出的三角网格
		//	vector<BSPTreeNode*> allLeafNodes;
		//	faceTree1->GetAllLeafNode(allLeafNodes);
		//	
		//	int ii = 0;
		//	for (BSPTreeNode* noded : allLeafNodes) {
		//		ii++;
		//		Mesh tempMeshA, tempMeshB;
		//		tie(tempMeshA,tempMeshB) = DataTranslate::TranslateNode2SeperatedTriangleMesh(noded);

		//		if (!OpenMesh::IO::write_mesh(tempMeshA, "..//TestModel//bunny//" + testResult_Num + "//" + to_string(ii) +"subMeshA.obj")) {
		//			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//			return;
		//		}
		//		if (!OpenMesh::IO::write_mesh(tempMeshB, "..//TestModel//bunny//" + testResult_Num + "//" + to_string(ii) + "subMeshB.obj")) {
		//			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//			return;
		//		}

		//	}

		//}


		//{
		//	bspFaceRenderingInfo partiPlanes;
		//	faceTree2->GetPartitionPlane(partiPlanes);

		//	Mesh planeMesh;
		//	Triangle_Set planeTris;
		//	Edge_Set planeEdges;
		//	tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, planeMesh);

		//	if (!OpenMesh::IO::write_mesh(planeMesh, outputPartitionPlaneAddress2)) {
		//		std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
		//		return;
		//	}
		//}

	}

	void BatchTestClassicModels()
	{
		Mesh meshA, meshB;

		ofstream outFileNTRR("..//TestModel//metrics result//NtrrRes.txt");
		ofstream outFileCheckPairs("..//TestModel//metrics result//CheckpairsRes.txt");
		ofstream outFileMeanCheckPairs("..//TestModel//metrics result//MeanCheckpairsRes.txt");

		bool needWriteResult = true;

		for (int i = 7;i < 37;i++) {
			if ((14 == i) || (24 == i)) continue;

			string testResult_Num = to_string(i);
			string outputPartitionPlaneAddress = "..//TestModel//bunny//" + testResult_Num + "//PartitionPlane.obj";
			string outputPartitionModelAddress = "..//TestModel//bunny//" + testResult_Num + "//TestTreeResult//";

			if (!OpenMesh::IO::read_mesh(meshA, "..//TestModel//bunny//" + to_string(i) + "//bunnyA.obj")) {

				std::cerr << "Cannot read mesh" << std::endl;

				return;
				
			}
			if (!OpenMesh::IO::read_mesh(meshB, "..//TestModel//bunny//" + to_string(i) + "//bunnyB.obj")) {

				std::cerr << "Cannot read mesh" << std::endl;

				return;

			}

			Tolerance toler;

			{
				meshA.request_vertex_status();
				meshA.request_edge_status();
				meshA.request_face_status();

				meshB.request_vertex_status();
				meshB.request_edge_status();
				meshB.request_face_status();

				meshA.request_face_normals();
				meshB.request_face_normals();

				meshA.update_face_normals();
				meshB.update_face_normals();
			}


			PreProcess pr({ &meshA,&meshB }, toler);
			pr.run();

			cout << "classic:========= " << i << endl;
			//auto SDMITRA = TestBspNodeDistributionInfo(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, BSPConstructType::SDM);

			array<double, 5> checkpairInfoWSDM_PCA, checkpairInfoWSDM, checkpairInfoSAH, checkpairInfoObbMiddel, checkpairInfoGravity;

			auto WSDM_PCA_UTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoWSDM_PCA,BSPConstructType::WSDM_PCA);
			auto WSDMUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoWSDM, BSPConstructType::WSDM);
			auto SAHUTFR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoSAH, BSPConstructType::SAH);
			auto ObbMiddel = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoObbMiddel, BSPConstructType::ObbMiddel);
			auto Gravity = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoGravity, BSPConstructType::Gravity_SPLIT);
			
			//auto AngleSerial = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SerialAngle);
			//auto Angle = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::Angle);
			//auto AngleSDM = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, true, true, BSPConstructType::SDM_Angle);

			cout << "========" << endl;
			cout << "WSDM_PCAUTFR:     " << WSDM_PCA_UTFR.first << endl;
			cout << "WSDMUTFR:     " << WSDMUTFR.first << endl;
			cout << "SAHUTFR:     " << SAHUTFR.first << endl;
			cout << "ObbMiddelUTFR:  " << ObbMiddel.first << endl;
			cout << "GravityUTFR: " << Gravity.first << endl;
			
			//cout << "AngleSerialUTFR: " << AngleSerial.first << endl;
			//cout << "AngleUTFR: " << Angle.first << endl;
			//cout << "AngleSDMUTFR: " << AngleSDM.first << endl;
			cout << "========" << endl;

			outFileNTRR << WSDM_PCA_UTFR.first << ", "<< WSDMUTFR.first << ", " << SAHUTFR.first << ", " << ObbMiddel.first << ", " << Gravity.first << endl;
			outFileCheckPairs << checkpairInfoWSDM_PCA[0] << ", " << checkpairInfoWSDM[0]  << ", " << checkpairInfoSAH[0] << ", " << checkpairInfoObbMiddel[0] << ", " << checkpairInfoGravity[0] << endl;
			outFileMeanCheckPairs << checkpairInfoWSDM_PCA[1] << ", " << checkpairInfoWSDM[1] << ", " << checkpairInfoSAH[1] << ", " << checkpairInfoObbMiddel[1] << ", " << checkpairInfoGravity[1] << endl;

		}

		outFileMeanCheckPairs.close();
		outFileCheckPairs.close();
		outFileNTRR.close();

	}

	void BatchTestThingi10K()
	{
		string folder = "thingi10k";
		//string folder = "rotate models";

		string outputPartitionPlaneAddress = "..//TestModel//" + folder + "//PartitionPlane.obj";
		string outputPartitionModelAddress = "..//TestModel//" + folder + "//TestTreeResult//";

		ofstream outFile1("..//TestModel//" + folder + "//NTRR.txt");
		ofstream outFile2("..//TestModel//" + folder + "//ITRA.txt");
		ofstream outFileCheckPairs("..//TestModel//" + folder + "//CheckPairs.txt");
		ofstream outFileMeanCheckPairs("..//TestModel//" + folder + "//MeanCheckPairs.txt");
		ofstream outFileMemoryCost("..//TestModel//" + folder + "//MemoryCost.txt");

		for (int i = 1;i < 101;i++) {

			cout << folder + ":============== " << i << endl;
			Mesh meshA, meshB;

			string baseAddress = "..//TestModel//" + folder + "//" + to_string(i);
			if (i < 51) {
				vector<string>  files;
				getFileNames(baseAddress, files);

				if (!OpenMesh::IO::read_mesh(meshA, files[0])) {

					std::cerr << "Cannot read mesh" << std::endl;

					return;

				}
				if (!OpenMesh::IO::read_mesh(meshB, files[1])) {

					std::cerr << "Cannot read mesh" << std::endl;

					return;

				}
			}
			else {
				if (!OpenMesh::IO::read_mesh(meshA, baseAddress+ "//bunnyA.obj")) {

					std::cerr << "Cannot read mesh" << std::endl;

					return;

				}
				if (!OpenMesh::IO::read_mesh(meshB, baseAddress + "//bunnyB.obj")) {

					std::cerr << "Cannot read mesh" << std::endl;

					return;

				}
			
			}


			Tolerance toler;

			{
				meshA.request_vertex_status();
				meshA.request_edge_status();
				meshA.request_face_status();

				meshB.request_vertex_status();
				meshB.request_edge_status();
				meshB.request_face_status();

				meshA.request_face_normals();
				meshB.request_face_normals();

				meshA.update_face_normals();
				meshB.update_face_normals();
			}


			PreProcess pr({ &meshA,&meshB }, toler);
			pr.run();

			array<double, 5> checkpairInfoWSDM_PCA, checkpairInfoSAH, checkpairInfoObbMiddel, checkpairInfoGravity, checkpairInfoOCT;
			
			array<double, 5> checkpairInfoWSDM;

			//auto WSDM_PCA_NTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, false, checkpairInfoWSDM_PCA, BSPConstructType::WSDM_PCA);
			auto WSDMNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfoWSDM, BSPConstructType::WSDM);
			auto SAHNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfoSAH, BSPConstructType::SAH);
			auto ObbMiddelNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfoObbMiddel, BSPConstructType::ObbMiddel);
			auto GravityNTRR = TestBspTimeConsum(meshA, meshB, toler, 10, 25, outputPartitionModelAddress, false, true, checkpairInfoGravity, BSPConstructType::Gravity_SPLIT);
			auto OCTResNTRR = TestOctTimeConsum(meshA, meshB, toler, 4, 25, outputPartitionModelAddress, false, true, checkpairInfoOCT);
			
			outFile1 << WSDMNTRR.first << endl;
			outFile2 << WSDMNTRR.second << SAHNTRR.second << ObbMiddelNTRR.second << GravityNTRR.second << OCTResNTRR.second << endl;

			outFileCheckPairs << checkpairInfoWSDM[0] << endl;
			outFileMeanCheckPairs << checkpairInfoWSDM[1] << endl;
			outFileMemoryCost << checkpairInfoWSDM[2] << endl;



			//outFile1 << WSDM_PCA_NTRR.first << ", " << SAHNTRR.first << ", " << ObbMiddelNTRR.first << ", " << GravityNTRR.first << ", " << OCTResNTRR.first << endl;
			//outFile2 << WSDM_PCA_NTRR.second << ", " << SAHNTRR.second << ", " << ObbMiddelNTRR.second << ", " << GravityNTRR.second << ", " << OCTResNTRR.second << endl;

			//outFileCheckPairs << checkpairInfoWSDM_PCA[0] << ", " << checkpairInfoSAH[0] << ", " << checkpairInfoObbMiddel[0] << ", " << checkpairInfoGravity[0] << ", " << checkpairInfoOCT[0] << endl;
			//outFileMeanCheckPairs << checkpairInfoWSDM_PCA[1] << ", " << checkpairInfoSAH[1] << ", " << checkpairInfoObbMiddel[1] << ", " << checkpairInfoGravity[1] << ", " << checkpairInfoOCT[1] << endl;
			//outFileMemoryCost << checkpairInfoWSDM_PCA[2] << ", " << checkpairInfoSAH[2] << ", " << checkpairInfoObbMiddel[2] << ", " << checkpairInfoGravity[2] << ", " << checkpairInfoOCT[2] << endl;




			//outFile1 << WSDM_PCA_NTRR.first << ", " << WSDMNTRR.first << ", " << SAHNTRR.first << ", " << ObbMiddelNTRR.first << ", " << GravityNTRR.first << ", " << OCTResNTRR.first << endl;
			//outFile2 << WSDM_PCA_NTRR.second << ", " << WSDMNTRR.second << ", " << SAHNTRR.second << ", " << ObbMiddelNTRR.second << ", " << GravityNTRR.second << ", " << OCTResNTRR.second << endl;

			//outFileCheckPairs << checkpairInfoWSDM_PCA[0] << ", " << checkpairInfoWSDM[0] << ", " << checkpairInfoSAH[0] << ", " << checkpairInfoObbMiddel[0] << ", " << checkpairInfoGravity[0] << ", " << checkpairInfoOCT[0] << endl;
			//outFileMeanCheckPairs << checkpairInfoWSDM_PCA[1] << ", " << checkpairInfoWSDM[1] << ", " << checkpairInfoSAH[1] << ", " << checkpairInfoObbMiddel[1] << ", " << checkpairInfoGravity[1] << ", " << checkpairInfoOCT[1] << endl;
			//outFileMemoryCost << checkpairInfoWSDM_PCA[2] << ", " << checkpairInfoWSDM[2] << ", " << checkpairInfoSAH[2] << ", " << checkpairInfoObbMiddel[2] << ", " << checkpairInfoGravity[2] << ", " << checkpairInfoOCT[2] << endl;



			//outFile2 << SDMITRA.second << endl;

	/*		outFile3 << SAHITRA.first << endl;
			outFile4 << SAHITRA.second << endl;

			outFile5 << ObbMiddelITRA.first << endl;
			outFile6 << ObbMiddelITRA.second << endl;

			outFile7 << GravityITRA.first << endl;
			outFile8 << GravityITRA.second << endl;

			outFile9 << OCTRes.first << endl;
			outFile10 << OCTRes.second << endl;*/

			//outFile << "========" << endl;
			//outFile << "model num:   " << i << endl;

			//outFile << "SDMNTRR:     " << SDMITRA.first << endl;
			//outFile << "SAHNTRR:     " << SAHITRA.first << endl;
			//outFile << "ObbMiddelNTRR:  " << ObbMiddelITRA.first << endl;
			//outFile << "GravityNTRR: " << GravityITRA.first << endl;

			//outFile << "SDMITRA:     " << SDMITRA.second << endl;
			//outFile << "SAHITRA:     " << SAHITRA.second << endl;
			//outFile << "ObbMiddelITRA:  " << ObbMiddelITRA.second << endl;
			//outFile << "GravityITRA: " << GravityITRA.second << endl;

			//outFile << "========" << endl;
		}

		outFile1.close();
		outFile2.close();
		outFileCheckPairs.close();
		outFileMeanCheckPairs.close();
		outFileMemoryCost.close();

	}

	void TestGenerateBall()
	{
		Ball b({ 0,0,0 }, 1);


		if (!OpenMesh::IO::write_mesh(b._ballMesh, "../ball.obj")) {
			std::cerr << "Cannot write mesh to file 'ball.obj'" << std::endl;
		}

	}

	void TestPointCloud()
	{
		//三维点云测试

		
		auto points1 = GetAGroupPointUniformDistribution({ 300,0,0 }, { 350,500,350}, { 0,0,0 }, 1000);
		//auto points2 = GetAGroupPointUniformDistribution({ 0,0,0 }, { 350,500,350 }, { 0,0,0 }, 1000);
		auto points2 = GetAGroupPointUniformDistribution({ 0,0,0 }, { 100,100,100 }, { 0,0,0 }, 100);

		Mesh pointClouds1Mesh;
		Mesh pointClouds2Mesh;

		std::thread pointClouds1MeshThread(&DataTranslate::TranslatePointCloud2Mesh, points1, 10, std::ref(pointClouds1Mesh));
		std::thread pointClouds2MeshThread(&DataTranslate::TranslatePointCloud2Mesh, points2, 10, std::ref(pointClouds2Mesh));

		pointClouds1MeshThread.join();
		pointClouds2MeshThread.join();

		if (!OpenMesh::IO::write_mesh(pointClouds1Mesh, "../2d/pC1.obj")) {
			std::cerr << "Cannot write mesh to file 'ball.obj'" << std::endl;
		}

		if (!OpenMesh::IO::write_mesh(pointClouds2Mesh, "../2d/pC2.obj")) {
			std::cerr << "Cannot write mesh to file 'ball.obj'" << std::endl;
		}

		//显示分割平面	 
		{
			Tolerance toler; 

			BSPTree* faceTree = new BSPTree(pointClouds1Mesh, pointClouds2Mesh, toler, 1, 25, BSPConstructType::WSDM);
			
			TreeFaceRenderingInfo partiPlanes;
			faceTree->GetPartitionPlane(partiPlanes);

			AABB boxA(DataTranslate::TranslateMesh2AABB(pointClouds1Mesh));
			AABB boxB(DataTranslate::TranslateMesh2AABB(pointClouds2Mesh));

			boxA.Add(boxB);
			boxA.Scale(1.2);

			Mesh planeMesh;
			Triangle_Set planeTris;
			Edge_Set planeEdges;
			tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

			if (!OpenMesh::IO::write_mesh(planeMesh, "../2d/SPlaneSDMWithWeight.obj")) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return;
			}
		}

		//显示分割平面	 
		{
			Tolerance toler;

			BSPTree* faceTree = new BSPTree(pointClouds1Mesh, pointClouds2Mesh, toler, 1, 25, BSPConstructType::OriginSDM);

			TreeFaceRenderingInfo partiPlanes;
			faceTree->GetPartitionPlane(partiPlanes);

			AABB boxA(DataTranslate::TranslateMesh2AABB(pointClouds1Mesh));
			AABB boxB(DataTranslate::TranslateMesh2AABB(pointClouds2Mesh));

			boxA.Add(boxB);
			boxA.Scale(1.2);

			Mesh planeMesh;
			Triangle_Set planeTris;
			Edge_Set planeEdges;
			tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

			if (!OpenMesh::IO::write_mesh(planeMesh, "../2d/SPlaneOriginSDM.obj")) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return;
			}
		}

		//显示分割平面	 
		{
			Tolerance toler;

			BSPTree* faceTree = new BSPTree(pointClouds1Mesh, pointClouds2Mesh, toler, 1, 25, BSPConstructType::SDM_ONLY_SIZEWEIGHT);

			TreeFaceRenderingInfo partiPlanes;
			faceTree->GetPartitionPlane(partiPlanes);

			AABB boxA(DataTranslate::TranslateMesh2AABB(pointClouds1Mesh));
			AABB boxB(DataTranslate::TranslateMesh2AABB(pointClouds2Mesh));

			boxA.Add(boxB);
			boxA.Scale(1.2);

			Mesh planeMesh;
			Triangle_Set planeTris;
			Edge_Set planeEdges;
			tie(planeTris, planeEdges) = DataTranslate::TranslatePartitionPlane(partiPlanes, boxA, planeMesh);

			if (!OpenMesh::IO::write_mesh(planeMesh, "../2d/SPlaneSDMOnlySizeWeight.obj")) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return;
			}
		}

	}

	pair<double, double> TestBspTimeConsum(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, bool outPutITRA, array<double,5>& r_CheckPairInfo, BSPConstructType bspType /*= BSPConstructType::AABB*/, double p_LambdaS /*= 1*/, double p_LambdaD /*= 0.5*/)
	{
		double r_UTFR = -1;
		double r_ITRA = -1;

		string baseAddress = p_BaseAddress + to_string(maxDepth) + "//";
		cout << "===============" << endl;
		cout << "maxDepth " << maxDepth << endl;
		cout << "leafShreshold " << leafShreshold << endl;

		string typeString;
		switch (bspType)
		{
		case BSPConstructType::AABB_MIDDLE_SPLIT:
			cout << "BSPConstructType " << "AABB"  << endl;
			typeString = "AABB";
			break;
		case BSPConstructType::WSDM_PCA:
			cout << "BSPConstructType " << "WSDM_PCA" << endl;
			typeString = "WSDM_PCA";
			break;
		case BSPConstructType::WSDM:
			cout << "BSPConstructType " << "WSDM" << endl;
			typeString = "SDM";
			break;
		case BSPConstructType::SAH:
			cout << "BSPConstructType " << "SAH" << endl;
			typeString = "SAH";
			break;
		case BSPConstructType::ObbMiddel:
			cout << "BSPConstructType " << "ObbMiddel" << endl;
			typeString = "ObbMiddel";
			break;
		case BSPConstructType::Gravity_SPLIT:
			cout << "BSPConstructType " << "Gravity_SPLIT" << endl;
			typeString = "Gravity_SPLIT";
			break;
		case BSPConstructType::Angle:
			cout << "BSPConstructType " << "Angle" << endl;
			typeString = "Angle";
			break;
		case BSPConstructType::SerialAngle:
			cout << "BSPConstructType " << "SerialAngle" << endl;
			typeString = "SerialAngle";
			break;
		case BSPConstructType::SDM_Angle :
			cout << "BSPConstructType " << "SDM_Angle" << endl;
			typeString = "SDM_Angle";
			break;
		default:
			break;
		}

		ofstream outTime1("..//TestModel//time//" + typeString + "ConstructTime.txt", ios::app);
		ofstream outTime2("..//TestModel//time//" + typeString + "QueryTime.txt", ios::app);
		ofstream outTime3("..//TestModel//time//" + typeString + "IntersectingTime.txt", ios::app);
		//ofstream outTime4("..//TestModel//time//" + typeString + "IntersectingTime.txt", ios::app);

		auto tBuildTreeBegin = std::chrono::steady_clock::now();

		BSPTree* faceTree = new BSPTree(meshA, meshB, toler, maxDepth, leafShreshold, bspType, p_LambdaS, p_LambdaD);

		auto tBuildTreeEnd = std::chrono::steady_clock::now();
		cout << "Buliding Tree cost： " << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() << "ms" << endl;

		//记录时间
		outTime1 << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() << endl;


		//显示子树划分出的三角网格
		vector<BSPTreeNode*> allLeafNodes;
		faceTree->GetAllLeafNode(allLeafNodes);



		Mesh resultValidMeshA, resultValidMeshB;

		Mesh realIntersectFacesMeshA, realIntersectFacesMeshB;
		Mesh fakeIntersectFacesMeshA, fakeIntersectFacesMeshB;
		Mesh resultInValidMeshA, resultInValidMeshB;
		{
			IntersectTriangleCheckList intersectCheckList;

			// lambda for user-defined hash function
			auto hash = [](const pair<FaceId, FaceId>& c) {
				return hash_val(c.first, c.second);
			};

			// lambda for user-defined equality criterion
			auto eq = [](const pair<FaceId, FaceId>& c1, const pair<FaceId, FaceId>& c2) {
				return c1 == c2;
			};

			// create unordered set with user-defined behavior
			std::unordered_set<pair<FaceId, FaceId>, decltype(hash), decltype(eq)> nowProcessFacePairsSet(intersectCheckList.size(), hash, eq);

			//std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeAFaceBPairSet(intersectCheckList.size() * 2, hash, eq);

			//std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeBFaceAPairSet(intersectCheckList.size() * 2, hash, eq);


			auto tQueryBegin = std::chrono::steady_clock::now();

			//for (int i = 0;(i < 5) && (nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {

			//cout << "nodes count: " << allLeafNodes.size() << endl;

			int validLeafNodeCount = 0;

			for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {
				if ((*nowNodes)->GetIntersectPair(intersectCheckList)) {
					validLeafNodeCount++;					
				}
			}

			//auto t2 = std::chrono::steady_clock::now();
			//std::cout << "获取初步数据时间：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;

			//先对采集面片对去重
			for (auto& checkPair : intersectCheckList) {
				nowProcessFacePairsSet.insert(checkPair);
			}

			auto tQueryEnd = std::chrono::steady_clock::now();
			std::cout << "Query Time：" << std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << "ms" << endl;

			//记录时间
			outTime2 << std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << endl;

			//
			//outTime3 << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() + std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << endl;
			

			
			double memoryCost = 0;
			//ofstream outMemory("..//TestModel//time//" + typeString + "Memory.txt", ios::app);
			{
				PROCESS_MEMORY_COUNTERS pmc;
				if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
				{
					//cout<< "当前进程占用内存大小为：" << double(pmc.WorkingSetSize) / 1024.0 /1024.0 /1024.0<< "GB" << endl;
					//outMemory << double(pmc.WorkingSetSize) / 1024.0 / 1024.0 / 1024.0 << endl;

					memoryCost = double(pmc.WorkingSetSize) / 1024.0 / 1024.0 / 1024.0;
				}
				
			}
			//outMemory.close();




			auto tIntersectionBegin = std::chrono::steady_clock::now();

			int validCount = 0;
			unordered_set<FaceId> realIntersectFacesA, fakeIntersectFacesA;
			unordered_set<FaceId> realIntersectFacesB, fakeIntersectFacesB;

			if (outPutITRA) {
				for (auto& verifyPair : nowProcessFacePairsSet) {

					Mesh::FaceVertexCCWIter fvit = meshA.fv_ccwbegin(meshA.face_handle(verifyPair.first));
					Mesh::VertexHandle triV0 = *fvit;
					Mesh::VertexHandle triV1 = *(++fvit);
					Mesh::VertexHandle triV2 = *(++fvit);

					Triangle3d triMathA(array<Point3d, 3>{
						Point3d(meshA.point(triV0)),
							Point3d(meshA.point(triV1)),
							Point3d(meshA.point(triV2))
					});

					Mesh::FaceVertexCCWIter fvitB = meshB.fv_ccwbegin(meshB.face_handle(verifyPair.second));
					Mesh::VertexHandle triV0B = *fvitB;
					Mesh::VertexHandle triV1B = *(++fvitB);
					Mesh::VertexHandle triV2B = *(++fvitB);

					Triangle3d triMathB(array<Point3d, 3>{
						Point3d(meshB.point(triV0B)),
							Point3d(meshB.point(triV1B)),
							Point3d(meshB.point(triV2B))
					});


					if (TriangleTriangleIsIntersect(triMathA, triMathB)) {
						validCount++;
						realIntersectFacesA.insert(verifyPair.first);
						realIntersectFacesB.insert(verifyPair.second);
					}
					else {
						fakeIntersectFacesA.insert(verifyPair.first);
						fakeIntersectFacesB.insert(verifyPair.second);

					}
				}

			}

			//intersecting time
			auto tIntersectionEnd = std::chrono::steady_clock::now();
			
			outTime3 << std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count() << endl;

			outTime1.close();
			outTime2.close();
			outTime3.close();

			for (auto& fa : realIntersectFacesA) {
				fakeIntersectFacesA.erase(fa);
			}

			for (auto& fa : realIntersectFacesB) {
				fakeIntersectFacesB.erase(fa);
			}

			unordered_set<FaceId> valid_FacesA, valid_FacesB;
			unordered_set<FaceId> inValid_FacesA, inValid_FacesB;

			int validFacePerLeafSum = 0;
			for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {

				validFacePerLeafSum += (*nowNodes)->GetValidFaces(valid_FacesA, valid_FacesB);

			}

			r_UTFR = (1 - double(valid_FacesA.size() + valid_FacesB.size()) / double(meshA.n_faces() + meshB.n_faces())) * 100;
			cout << "UTFR :" << r_UTFR << endl;
			//cout << "ITRA" << double(validCount)/double(nowProcessFacePairsSet.size()) << endl;
			//r_ITRA = double(validCount) / double(nowProcessFacePairsSet.size());
			r_ITRA = double(realIntersectFacesA.size() + realIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size())*100.0;
			cout << "ITRA :" << r_ITRA  << endl;

			cout << "Check Pairs :" << nowProcessFacePairsSet.size() << endl;
			cout << "Check Pairs per leaf node :" << double(nowProcessFacePairsSet.size()) / double(validLeafNodeCount) << endl;
			
			//check pairs
			r_CheckPairInfo[0] = nowProcessFacePairsSet.size();
			r_CheckPairInfo[1] = double(nowProcessFacePairsSet.size()) / double(validLeafNodeCount);

			cout << "Total Tree Time: " << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() + std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << endl;
			cout << "Memory Cost" << memoryCost << "GB" << endl;

			//cout << "Intersecting time cost:" << std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count() << "ms" << endl;

			cout << "Valid Faces per leaf: " << double(validFacePerLeafSum) / double(validLeafNodeCount) << endl;
			cout << "Valid Faces per leaf x leaf nums: " << validFacePerLeafSum << endl;

			//total tree time
			r_CheckPairInfo[2] = std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() + std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count();

			//intersection time
			r_CheckPairInfo[3] = std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count();
			r_CheckPairInfo[4] = validFacePerLeafSum;


			//cout << "ITRA2:" << double(validCount) / double(nowProcessFacePairsSet.size()) << endl;
			//cout << "fakeITRA3:" << double(fakeIntersectFacesA.size() + fakeIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size()) << endl;
			//cout << "fakefaces:" << fakeIntersectFacesA.size() + fakeIntersectFacesB.size() << endl;
			//cout << "report faces:" << valid_FacesA.size() + valid_FacesB.size() << endl;
			//cout << "report real faces:" << realIntersectFacesA.size() + realIntersectFacesB.size() << endl;
			//cout << "efficiency :" << double(nowProcessFacePairsSet.size())/ double(meshA.n_faces() * meshB.n_faces()) << endl;


			if (outPutResultMesh) {

				realIntersectFacesMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, realIntersectFacesA);
				realIntersectFacesMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, realIntersectFacesB);

				fakeIntersectFacesMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, fakeIntersectFacesA);
				fakeIntersectFacesMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, fakeIntersectFacesB);


				for (Mesh::FaceIter fI = meshA.faces_begin(); fI != meshA.faces_end(); ++fI) {
					FaceId faceId = (*fI).idx();

					if (valid_FacesA.find(faceId) == valid_FacesA.end())
						inValid_FacesA.insert(faceId);
				}

				for (Mesh::FaceIter fI = meshB.faces_begin(); fI != meshB.faces_end(); ++fI) {
					FaceId faceId = (*fI).idx();

					if (valid_FacesB.find(faceId) == valid_FacesB.end())
						inValid_FacesB.insert(faceId);
				}
			
				resultInValidMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, inValid_FacesA);
				resultInValidMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, inValid_FacesB);
			
			}
		}

		if (outPutResultMesh) {
			string endAddressA1;
			string endAddressB1;

			string endAddressA2;
			string endAddressB2;

			string endAddressA3;
			string endAddressB3;
			switch (bspType)
			{
			case BSPConstructType::AABB_MIDDLE_SPLIT:
				endAddressA1 = "AABBResultMeshA1Red.obj";
				endAddressB1 = "AABBResultMeshB1Red.obj";

				endAddressA2 = "AABBResultMeshA2Pink.obj";
				endAddressB2 = "AABBResultMeshB2Pink.obj";

				endAddressA3 = "AABBResultMeshA3Blue.obj";
				endAddressB3 = "AABBResultMeshB3Yellow.obj";

				break;
			case BSPConstructType::WSDM:
				endAddressA1 = "WSDMResultMeshA1Red.obj";
				endAddressB1 = "WSDMResultMeshB1Red.obj";

				endAddressA2 = "WSDMResultMeshA2Pink.obj";
				endAddressB2 = "WSDMResultMeshB2Pink.obj";

				endAddressA3 = "WSDMResultMeshA3Blue.obj";
				endAddressB3 = "WSDMResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::WSDM_PCA:
				endAddressA1 = "WSDM_PCAResultMeshA1Red.obj";
				endAddressB1 = "WSDM_PCAResultMeshB1Red.obj";

				endAddressA2 = "WSDM_PCAResultMeshA2Pink.obj";
				endAddressB2 = "WSDM_PCAResultMeshB2Pink.obj";

				endAddressA3 = "WSDM_PCAResultMeshA3Blue.obj";
				endAddressB3 = "WSDM_PCAResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::SAH:
				endAddressA1 = "SAHResultMeshA1Red.obj";
				endAddressB1 = "SAHResultMeshB1Red.obj";

				endAddressA2 = "SAHResultMeshA2Pink.obj";
				endAddressB2 = "SAHResultMeshB2Pink.obj";

				endAddressA3 = "SAHResultMeshA3Blue.obj";
				endAddressB3 = "SAHResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::ObbMiddel:
				endAddressA1 = "ObbMiddelResultMeshA1Red.obj";
				endAddressB1 = "ObbMiddelResultMeshB1Red.obj";

				endAddressA2 = "ObbMiddelResultMeshA2Pink.obj";
				endAddressB2 = "ObbMiddelResultMeshB2Pink.obj";

				endAddressA3 = "ObbMiddelResultMeshA3Blue.obj";
				endAddressB3 = "ObbMiddelResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::Gravity_SPLIT:
				endAddressA1 = "Gravity_SPLITResultMeshA1Red.obj";
				endAddressB1 = "Gravity_SPLITResultMeshB1Red.obj";

				endAddressA2 = "Gravity_SPLITResultMeshA2Pink.obj";
				endAddressB2 = "Gravity_SPLITResultMeshB2Pink.obj";

				endAddressA3 = "Gravity_SPLITResultMeshA3Blue.obj";
				endAddressB3 = "Gravity_SPLITResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::Angle:
				endAddressA1 = "AngleResultMeshA1Red.obj";
				endAddressB1 = "AngleResultMeshB1Red.obj";

				endAddressA2 = "AngleResultMeshA2Pink.obj";
				endAddressB2 = "AngleResultMeshB2Pink.obj";

				endAddressA3 = "AngleResultMeshA3Blue.obj";
				endAddressB3 = "AngleResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::SerialAngle:
				endAddressA1 = "SerialAngleResultMeshA1Red.obj";
				endAddressB1 = "SerialAngleResultMeshB1Red.obj";

				endAddressA2 = "SerialAngleResultMeshA2Pink.obj";
				endAddressB2 = "SerialAngleResultMeshB2Pink.obj";

				endAddressA3 = "SerialAngleResultMeshA3Blue.obj";
				endAddressB3 = "SerialAngleResultMeshB3Yellow.obj";
				break;
			case BSPConstructType::SDM_Angle:
				endAddressA1 = "SDM_AngleResultMeshA1Red.obj";
				endAddressB1 = "SDM_AngleResultMeshB1Red.obj";

				endAddressA2 = "SDM_AngleResultMeshA2Pink.obj";
				endAddressB2 = "SDM_AngleResultMeshB2Pink.obj";

				endAddressA3 = "SDM_AngleResultMeshA3Blue.obj";
				endAddressB3 = "SDM_AngleResultMeshB3Yellow.obj";
				break;
			default:
				break;
			}

			if (!OpenMesh::IO::write_mesh(realIntersectFacesMeshA, baseAddress + endAddressA1)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(realIntersectFacesMeshB, baseAddress + endAddressB1)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(fakeIntersectFacesMeshA, baseAddress + endAddressA2)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(fakeIntersectFacesMeshB, baseAddress + endAddressB2)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(resultInValidMeshA, baseAddress + endAddressA3)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(resultInValidMeshB, baseAddress + endAddressB3)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}
		}


		delete faceTree;

		return make_pair(r_UTFR, r_ITRA);
	}

	pair<double, double> TestOctTimeConsum(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, bool outPutITRA, array<double, 5>& r_CheckPairInfo)
	{
		double r_UTFR = -1;
		double r_ITRA = -1;

		string baseAddress = p_BaseAddress + to_string(maxDepth) + "//";
		cout << "===============" << endl;
		cout << "maxDepth " << maxDepth << endl;
		cout << "leafShreshold " << leafShreshold << endl;
		cout << "OCTree " << endl;

		ofstream outTime1("..//TestModel//time//OctreeTimeConstruct.txt", ios::app);
		ofstream outTime2("..//TestModel//time//OctreeTimeQuery.txt", ios::app);
		ofstream outTime3("..//TestModel//time//OctreeTimeTotal.txt", ios::app);

		auto tBuildTreeBegin = std::chrono::steady_clock::now();

		OCTree* faceTree = new OCTree(meshA, meshB, toler, maxDepth, leafShreshold);

		auto tBuildTreeEnd = std::chrono::steady_clock::now();
		cout << "建树耗时： " << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() << "毫秒" << endl;
		
		//记录时间
		outTime1 << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() << endl;

		//显示子树划分出的三角网格
		vector<OCTreeNode*> allLeafNodes;
		faceTree->GetAllLeafNode(allLeafNodes);



		Mesh resultValidMeshA, resultValidMeshB;

		Mesh realIntersectFacesMeshA, realIntersectFacesMeshB;
		Mesh fakeIntersectFacesMeshA, fakeIntersectFacesMeshB;
		Mesh resultInValidMeshA, resultInValidMeshB;
		{
			IntersectTriangleCheckList intersectCheckList;

			// lambda for user-defined hash function
			auto hash = [](const pair<FaceId, FaceId>& c) {
				return hash_val(c.first, c.second);
			};

			// lambda for user-defined equality criterion
			auto eq = [](const pair<FaceId, FaceId>& c1, const pair<FaceId, FaceId>& c2) {
				return c1 == c2;
			};

			// create unordered set with user-defined behavior
			std::unordered_set<pair<FaceId, FaceId>, decltype(hash), decltype(eq)> nowProcessFacePairsSet(intersectCheckList.size(), hash, eq);

			//std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeAFaceBPairSet(intersectCheckList.size() * 2, hash, eq);

			//std::unordered_set<pair<EdgeId, FaceId>, decltype(hash), decltype(eq)> nowProcessEdgeBFaceAPairSet(intersectCheckList.size() * 2, hash, eq);


			auto tQueryBegin = std::chrono::steady_clock::now();

			//for (int i = 0;(i < 5) && (nowNodes != m_AllLeafNodes.end()); i++, nowNodes++) {

			//cout << "nodes count: " << allLeafNodes.size() << endl;

			int validLeafNodeCount = 0;
			for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {
				if ((*nowNodes)->GetIntersectPair(intersectCheckList)) {				
					validLeafNodeCount++;
				}
			}

			//auto t2 = std::chrono::steady_clock::now();
			//std::cout << "获取初步数据时间：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;

			//先对采集面片对去重
			for (auto& checkPair : intersectCheckList) {
				nowProcessFacePairsSet.insert(checkPair);
			}

			auto tQueryEnd = std::chrono::steady_clock::now();
			std::cout << "Query Time：" << std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << "ms" << endl;

			//记录时间
			outTime2 << std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << endl;

			//记录时间
			//outTime3 << std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() + std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count()  << endl;



			double memoryCost = 0;
			//ofstream outMemory("..//TestModel//time//OctMemory.txt", ios::app);
			{
				PROCESS_MEMORY_COUNTERS pmc;
				if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
				{
					//cout << "Current Progress Memory Consumption：" << double(pmc.WorkingSetSize) / 1024.0 / 1024.0 / 1024.0 << "GB" << endl;
					//outMemory << double(pmc.WorkingSetSize) / 1024.0 / 1024.0 / 1024.0 << endl;
					memoryCost = double(pmc.WorkingSetSize) / 1024.0 / 1024.0 / 1024.0;
				}

			}
			//outMemory.close();


			auto tIntersectionBegin = std::chrono::steady_clock::now();

			int validCount = 0;
			unordered_set<FaceId> realIntersectFacesA, fakeIntersectFacesA;
			unordered_set<FaceId> realIntersectFacesB, fakeIntersectFacesB;

			if (outPutITRA) {
				for (auto& verifyPair : nowProcessFacePairsSet) {

					Mesh::FaceVertexCCWIter fvit = meshA.fv_ccwbegin(meshA.face_handle(verifyPair.first));
					Mesh::VertexHandle triV0 = *fvit;
					Mesh::VertexHandle triV1 = *(++fvit);
					Mesh::VertexHandle triV2 = *(++fvit);

					Triangle3d triMathA(array<Point3d, 3>{
						Point3d(meshA.point(triV0)),
							Point3d(meshA.point(triV1)),
							Point3d(meshA.point(triV2))
					});

					Mesh::FaceVertexCCWIter fvitB = meshB.fv_ccwbegin(meshB.face_handle(verifyPair.second));
					Mesh::VertexHandle triV0B = *fvitB;
					Mesh::VertexHandle triV1B = *(++fvitB);
					Mesh::VertexHandle triV2B = *(++fvitB);

					Triangle3d triMathB(array<Point3d, 3>{
						Point3d(meshB.point(triV0B)),
							Point3d(meshB.point(triV1B)),
							Point3d(meshB.point(triV2B))
					});


					if (TriangleTriangleIsIntersect(triMathA, triMathB)) {
						validCount++;
						realIntersectFacesA.insert(verifyPair.first);
						realIntersectFacesB.insert(verifyPair.second);
					}
					else {
						fakeIntersectFacesA.insert(verifyPair.first);
						fakeIntersectFacesB.insert(verifyPair.second);

					}
				}

			}

			//intersecting time
			auto tIntersectionEnd = std::chrono::steady_clock::now();

			outTime3 << std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count() << endl;

			outTime1.close();
			outTime2.close();
			outTime3.close();


			for (auto& fa : realIntersectFacesA) {
				fakeIntersectFacesA.erase(fa);
			}

			for (auto& fa : realIntersectFacesB) {
				fakeIntersectFacesB.erase(fa);
			}

			unordered_set<FaceId> valid_FacesA, valid_FacesB;
			unordered_set<FaceId> inValid_FacesA, inValid_FacesB;

			int validFacePerLeafSum = 0;
			for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {
				validFacePerLeafSum += (*nowNodes)->GetValidFaces(valid_FacesA, valid_FacesB);
			}

			r_UTFR = (1 - double(valid_FacesA.size() + valid_FacesB.size()) / double(meshA.n_faces() + meshB.n_faces())) * 100;
			cout << "UTFR :" << r_UTFR << endl;
			//cout << "ITRA" << double(validCount)/double(nowProcessFacePairsSet.size()) << endl;
			//r_ITRA = double(validCount) / double(nowProcessFacePairsSet.size());
			r_ITRA = double(realIntersectFacesA.size() + realIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size()) * 100.0;
			cout << "ITRA :" << r_ITRA << endl;

			cout << "Check Pairs :" << nowProcessFacePairsSet.size() << endl;
			cout << "Check Pairs per leaf node :" << double(nowProcessFacePairsSet.size()) / double(validLeafNodeCount) << endl;

			//check pairs
			r_CheckPairInfo[0] = nowProcessFacePairsSet.size();
			r_CheckPairInfo[1] = double(nowProcessFacePairsSet.size()) / double(validLeafNodeCount);

			cout << "Total Tree Time: " << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() + std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count() << endl;
			cout << "Memory Cost" << memoryCost << "GB" << endl;

			//cout << "Intersecting time cost:" << std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count() << "ms" << endl;


			cout << "Valid Faces per leaf: " << double(validFacePerLeafSum) / double(validLeafNodeCount) << endl;
			cout << "Valid Faces per leaf x leaf nums: " << validFacePerLeafSum << endl;


			//total tree time
			r_CheckPairInfo[2] = std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() + std::chrono::duration<double, std::milli>(tQueryEnd - tQueryBegin).count();

			//intersection time
			r_CheckPairInfo[3] = std::chrono::duration<double, std::milli>(tIntersectionEnd - tIntersectionBegin).count();
			r_CheckPairInfo[4] = validFacePerLeafSum;


			//cout << "ITRA2:" << double(validCount) / double(nowProcessFacePairsSet.size()) << endl;
			//cout << "fakeITRA3:" << double(fakeIntersectFacesA.size() + fakeIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size()) << endl;
			//cout << "fakefaces:" << fakeIntersectFacesA.size() + fakeIntersectFacesB.size() << endl;
			//cout << "report faces:" << valid_FacesA.size() + valid_FacesB.size() << endl;
			//cout << "report real faces:" << realIntersectFacesA.size() + realIntersectFacesB.size() << endl;
			//cout << "efficiency :" << double(nowProcessFacePairsSet.size())/ double(meshA.n_faces() * meshB.n_faces()) << endl;


			if (outPutResultMesh) {

				realIntersectFacesMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, realIntersectFacesA);
				realIntersectFacesMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, realIntersectFacesB);

				fakeIntersectFacesMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, fakeIntersectFacesA);
				fakeIntersectFacesMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, fakeIntersectFacesB);


				for (Mesh::FaceIter fI = meshA.faces_begin(); fI != meshA.faces_end(); ++fI) {
					FaceId faceId = (*fI).idx();

					if (valid_FacesA.find(faceId) == valid_FacesA.end())
						inValid_FacesA.insert(faceId);
				}

				for (Mesh::FaceIter fI = meshB.faces_begin(); fI != meshB.faces_end(); ++fI) {
					FaceId faceId = (*fI).idx();

					if (valid_FacesB.find(faceId) == valid_FacesB.end())
						inValid_FacesB.insert(faceId);
				}

				resultInValidMeshA = DataTranslate::TranslateFaceID2Mesh(meshA, inValid_FacesA);
				resultInValidMeshB = DataTranslate::TranslateFaceID2Mesh(meshB, inValid_FacesB);

			}
		}

		if (outPutResultMesh) {

			string endAddressA1 = "OCTResultMeshA1Red.obj";
			string endAddressB1 = "OCTResultMeshB1Red.obj";

			string endAddressA2 = "OCTResultMeshA2Pink.obj";
			string endAddressB2 = "OCTResultMeshB2Pink.obj";

			string endAddressA3 = "OCTResultMeshA3Blue.obj";
			string endAddressB3 = "OCTResultMeshB3Yellow.obj";


			if (!OpenMesh::IO::write_mesh(realIntersectFacesMeshA, baseAddress + endAddressA1)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(realIntersectFacesMeshB, baseAddress + endAddressB1)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(fakeIntersectFacesMeshA, baseAddress + endAddressA2)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(fakeIntersectFacesMeshB, baseAddress + endAddressB2)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(resultInValidMeshA, baseAddress + endAddressA3)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}

			if (!OpenMesh::IO::write_mesh(resultInValidMeshB, baseAddress + endAddressB3)) {
				std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
				return make_pair(r_UTFR, r_ITRA);
			}
		}


		delete faceTree;

		return make_pair(r_UTFR, r_ITRA);
	}

	pair<double,double> TestBspNodeDistributionInfo(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, BSPConstructType bspType)
	{
		double r_UTFR = -1;
		double r_ITRA = -1;

		string baseAddress = p_BaseAddress + to_string(maxDepth) + "//";
		cout << "maxDepth " << maxDepth << endl;
		cout << "leafShreshold " << leafShreshold << endl;

		switch (bspType)
		{
		case BSPConstructType::AABB_MIDDLE_SPLIT:
			cout << "BSPConstructType " << "AABB" << endl;
			break;
		case BSPConstructType::WSDM:
			cout << "BSPConstructType " << "SDM" << endl;
			break;
		case BSPConstructType::SAH:
			cout << "BSPConstructType " << "SAH" << endl;
			break;
		case BSPConstructType::ObbMiddel:
			cout << "BSPConstructType " << "ObbMiddel" << endl;
			break;
		case BSPConstructType::Gravity_SPLIT:
			cout << "BSPConstructType " << "Gravity_SPLIT" << endl;
			break;
		case BSPConstructType::SDM_OBB:
			cout << "BSPConstructType " << "SDM_OBB" << endl;
			break;
		default:
			break;
		}


		auto tBuildTreeBegin = std::chrono::steady_clock::now();

		BSPTree* faceTree = new BSPTree(meshA, meshB, toler, maxDepth, leafShreshold, bspType);

		auto tBuildTreeEnd = std::chrono::steady_clock::now();
		cout << "建树耗时： " << std::chrono::duration<double, std::milli>(tBuildTreeEnd - tBuildTreeBegin).count() << "毫秒" << endl;


		//显示子树划分出的三角网格
		vector<BSPTreeNode*> allLeafNodes;
		faceTree->GetAllLeafNode(allLeafNodes);



		Mesh resultValidMeshA, resultValidMeshB;

		Mesh realIntersectFacesMeshA, realIntersectFacesMeshB;
		Mesh fakeIntersectFacesMeshA, fakeIntersectFacesMeshB;
		Mesh resultInValidMeshA, resultInValidMeshB;
		{
			IntersectTriangleCheckList intersectCheckList;

			// lambda for user-defined hash function
			auto hash = [](const pair<FaceId, FaceId>& c) {
				return hash_val(c.first, c.second);
			};

			// lambda for user-defined equality criterion
			auto eq = [](const pair<FaceId, FaceId>& c1, const pair<FaceId, FaceId>& c2) {
				return c1 == c2;
			};

			// create unordered set with user-defined behavior
			std::unordered_set<pair<FaceId, FaceId>, decltype(hash), decltype(eq)> nowProcessFacePairsSet(intersectCheckList.size(), hash, eq);

			auto t1 = std::chrono::steady_clock::now();


			unordered_map<int, int> depthCount;
			for (int i = 0; i <= maxDepth; i++) {
				depthCount[i] = 0;
			}

			int validNodeCount = 0;

			//for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {

			//	int& j = depthCount[(*nowNodes)->m_Depth];
			//	j++;

			//	cout << "Depth: " << (*nowNodes)->m_Depth << "nodes faces: " << (*nowNodes)->m_FacesA.size() + (*nowNodes)->m_FacesB.size() << "::: faces A size: " << (*nowNodes)->m_FacesA.size() << "::: faces B size: " << (*nowNodes)->m_FacesB.size() << endl;
			//	if ((*nowNodes)->IsValidNode()) validNodeCount++;

			//	if (((*nowNodes)->m_Depth >= 15) && ((*nowNodes)->Size() >= 2000)) {
			//		cout << "Real SizeA: " << (*nowNodes)->GetRealSize().first << "Real SizeB: " << (*nowNodes)->GetRealSize().second << endl;
			//	}

			//	(*nowNodes)->GetIntersectPair(intersectCheckList);
			//}

			//cout << "节点数为：" << allLeafNodes.size() <<" 有效节点数为：" << validNodeCount << " 占比："<<double(validNodeCount)/double(allLeafNodes.size())*100 <<"%" << endl;
			//for (int i = 0; i <= maxDepth; i++) {
			//	cout << "深度为" << i << "的节点占比：" << double(depthCount[i]) / double(allLeafNodes.size()) * 100 << "%" << endl;
			//}



			//auto t2 = std::chrono::steady_clock::now();
			//std::cout << "获取初步数据时间：" << std::chrono::duration<double, std::milli>(t2 - t1).count() << "毫秒" << endl;

			//先对采集面片对去重
			//for (auto& checkPair : intersectCheckList) {
			//	nowProcessFacePairsSet.insert(checkPair);
			//}

			auto t3 = std::chrono::steady_clock::now();
			std::cout << "查询时间：" << std::chrono::duration<double, std::milli>(t3 - t1).count() << "毫秒" << endl;


			int validCount = 0;
			unordered_set<FaceId> realIntersectFacesA, fakeIntersectFacesA;
			unordered_set<FaceId> realIntersectFacesB, fakeIntersectFacesB;

			for (auto& verifyPair : nowProcessFacePairsSet) {

				Mesh::FaceVertexCCWIter fvit = meshA.fv_ccwbegin(meshA.face_handle(verifyPair.first));
				Mesh::VertexHandle triV0 = *fvit;
				Mesh::VertexHandle triV1 = *(++fvit);
				Mesh::VertexHandle triV2 = *(++fvit);

				Triangle3d triMathA(array<Point3d, 3>{
					Point3d(meshA.point(triV0)),
						Point3d(meshA.point(triV1)),
						Point3d(meshA.point(triV2))
				});

				Mesh::FaceVertexCCWIter fvitB = meshB.fv_ccwbegin(meshB.face_handle(verifyPair.second));
				Mesh::VertexHandle triV0B = *fvitB;
				Mesh::VertexHandle triV1B = *(++fvitB);
				Mesh::VertexHandle triV2B = *(++fvitB);

				Triangle3d triMathB(array<Point3d, 3>{
					Point3d(meshB.point(triV0B)),
						Point3d(meshB.point(triV1B)),
						Point3d(meshB.point(triV2B))
				});


				if (TriangleTriangleIsIntersect(triMathA, triMathB)) {
					validCount++;
					realIntersectFacesA.insert(verifyPair.first);
					realIntersectFacesB.insert(verifyPair.second);
				}
				else {
					fakeIntersectFacesA.insert(verifyPair.first);
					fakeIntersectFacesB.insert(verifyPair.second);

				}
			}

			for (auto& fa : realIntersectFacesA) {
				fakeIntersectFacesA.erase(fa);
			}

			for (auto& fa : realIntersectFacesB) {
				fakeIntersectFacesB.erase(fa);
			}

			unordered_set<FaceId> valid_FacesA, valid_FacesB;
			unordered_set<FaceId> inValid_FacesA, inValid_FacesB;

			for (auto nowNodes = allLeafNodes.begin(); nowNodes != allLeafNodes.end(); nowNodes++) {
				(*nowNodes)->GetValidFaces(valid_FacesA, valid_FacesB);
			}

			cout << "UTFR :" << 1 - double(valid_FacesA.size() + valid_FacesB.size()) / double(meshA.n_faces() + meshB.n_faces()) << endl;
			r_UTFR = 1 - double(valid_FacesA.size() + valid_FacesB.size()) / double(meshA.n_faces() + meshB.n_faces());
			//cout << "ITRA" << double(validCount)/double(nowProcessFacePairsSet.size()) << endl;
			//r_ITRA = double(validCount) / double(nowProcessFacePairsSet.size());
			//r_ITRA = double(realIntersectFacesA.size() + realIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size());
			//auto s1 = realIntersectFacesA.size() + realIntersectFacesB.size();
			//auto s2 = valid_FacesA.size() + valid_FacesB.size();
			//cout << "ITRA :" << r_ITRA << endl;
			//cout << "1/ITRA :" << 1.0/r_ITRA << endl;

			//cout << "ITRA2:" << double(validCount) / double(nowProcessFacePairsSet.size()) << endl;
			//cout << "fakeITRA3:" << double(fakeIntersectFacesA.size() + fakeIntersectFacesB.size()) / double(valid_FacesA.size() + valid_FacesB.size()) << endl;
			//cout << "fakefaces:" << fakeIntersectFacesA.size() + fakeIntersectFacesB.size() << endl;
			//cout << "report faces:" << valid_FacesA.size() + valid_FacesB.size() << endl;
			//cout << "report real faces:" << realIntersectFacesA.size() + realIntersectFacesB.size() << endl;

			//auto t3 = std::chrono::steady_clock::now();
			//std::cout << "查询时间：" << std::chrono::duration<double, std::milli>(t3 - t1).count() << "毫秒" << endl;
		}

		delete faceTree;

		return make_pair(r_UTFR, r_ITRA);
	}



}

