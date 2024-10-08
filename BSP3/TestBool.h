#pragma once


namespace TestFunction {

	void TestBool();

	void BatchTestBool();


	void TestWrite();

	void TestReadObjWriteStl();

	void TestOpenMesh();

	void TestMath();

	void TestBspTreeSDMParameters();

	void TestBspTree();

	void TestOctTree();

	void SingleTestClassicModelsForBSP();
	void SingleTestThingi10K();
	void BatchTestClassicModels();
	void BatchTestThingi10K();

	void TestGenerateBall();

	void TestPointCloud();

	pair<double, double> TestBspTimeConsum(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, bool outPutITRA, array<double,5>& r_CheckPairInfo, BSPConstructType bspType = BSPConstructType::AABB_MIDDLE_SPLIT, double p_LambdaS = 1, double p_LambdaD = 0.5);

	pair<double, double> TestOctTimeConsum(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, bool outPutITRA, array<double, 5>& r_CheckPairInfo);

	pair<double, double> TestBspNodeDistributionInfo(Mesh& meshA, Mesh& meshB, Tolerance& toler, int maxDepth, int leafShreshold, string p_BaseAddress, bool outPutResultMesh, BSPConstructType bspType = BSPConstructType::AABB_MIDDLE_SPLIT);


}