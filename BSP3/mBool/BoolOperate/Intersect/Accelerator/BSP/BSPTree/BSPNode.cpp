#include "mPch.h"
#include "BSPTreeHeader.h"


BSPTreeNode::BSPTreeNode(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum, bool p_Isroot /*= false*/, BSPConstructType p_Type /*= BSPConstructType::AABB*/, double p_LambdaS /*= 1*/, double p_LambdaD /*= 0.5*/) :
	m_FacesA(p_FacesA), m_FacesB(p_FacesB), m_Type(p_Type), m_Tolerance(p_Tolerance), m_Depth(p_Depth), m_IsRoot(p_Isroot), m_MaxDepth(p_MaxDepth), m_LeafShresholdNum(p_LeafShresholdNum), m_LambdaS(p_LambdaS), m_LambdaD(p_LambdaD)
{
	//达到截止深度
	if (p_Depth == p_MaxDepth) {
		m_IsLeaf = true;
		m_PositiveSon = nullptr;
		m_NegativeSon = nullptr;
		return;
	}

	//达到叶节点面片数目条件，生成叶节点
	if (p_FacesA.size() + p_FacesB.size() <= p_LeafShresholdNum) {
		m_IsLeaf = true;
		m_PositiveSon = nullptr;
		m_NegativeSon = nullptr;

		return;
	}

	//某一类面片数目为0，生成叶节点
	if ((0 == p_FacesA.size()) || (0 == p_FacesB.size())) {

		m_IsLeaf = true;
		m_PositiveSon = nullptr;
		m_NegativeSon = nullptr;

		return;
	}


	switch (m_Type) {
	case BSPConstructType::AABB_MIDDLE_SPLIT:
		ConstructAABBSons();
		break;

	case BSPConstructType::WSDM:
		ConstructWSDMSons();
		break;

	case BSPConstructType::WSDM_PCA:
		ConstructWSDM_PCASons();
		break;

	case BSPConstructType::SAH:
		ConstructSAHSons();
		break;

	case BSPConstructType::ObbMiddel:
		ConstructObbMiddleSons();
		break;

	case BSPConstructType::Gravity_SPLIT:
		ConstructGravity_SPLITSons();
		break;

	case BSPConstructType::SDM_OBB:
		ConstructGravity_SPLITSons();
		break;
	case BSPConstructType::Angle:
		ConstructAngleSons();
		break;
	case BSPConstructType::SerialAngle:
		ConstructAngleSonsSerial();
		break;
	case BSPConstructType::SDM_Angle:
		ConstructAngle_SDMSons();
		break;
	case BSPConstructType::OriginSDM:
		ConstructOrigin_SDMSons();
		break;
	case BSPConstructType::SDM_ONLY_SIZEWEIGHT:
		ConstructSDM_Only_SizeWeightedSons();
		break;
	default:
		assert(false);
	}

}

BSPTreeNode::~BSPTreeNode()
{
	DeConstruct();
}

//BSPTreeNode::BSPTreeNode(vector<TriangleBSP> p_FacesA, vector<TriangleBSP> p_FacesB, Plane3d p_PartitionPlane, Tolerance& p_Tolerance, bool p_Isroot /*= false*/, BSPConstructType p_Type /*= BSPConstructType::AABB*/):
//	m_FacesA(p_FacesA), m_FacesB(p_FacesB), m_Type(p_Type), m_PartitionPlane(p_PartitionPlane), m_Tolerance(p_Tolerance), m_IsRoot(p_Isroot)
//{
//
//
//}
void BSPTreeNode::create_bsp_tree_node1(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum) {
    m_PositiveSon=new BSPTreeNode(p_FacesA, p_FacesB, p_Tolerance, p_Depth, p_MaxDepth, p_LeafShresholdNum, false, BSPConstructType::Angle);
}
void BSPTreeNode::create_bsp_tree_node2(vector<TriangleForTree> p_FacesA, vector<TriangleForTree> p_FacesB, Tolerance& p_Tolerance, int p_Depth, int p_MaxDepth, int p_LeafShresholdNum) {
	m_NegativeSon = new BSPTreeNode(p_FacesA, p_FacesB, p_Tolerance, p_Depth, p_MaxDepth, p_LeafShresholdNum, false, BSPConstructType::Angle);
}

void BSPTreeNode::DeConstruct()
{
	if (m_PositiveSon->IsLeaf()) {
		delete m_PositiveSon;
	}
	else {
		m_PositiveSon->DeConstruct();
		delete m_PositiveSon;
	}

	if (m_NegativeSon->IsLeaf()) {
		delete m_NegativeSon;
	}
	else {
		m_NegativeSon->DeConstruct();
		delete m_NegativeSon;
	}

	m_IsLeaf = true;
}

void BSPTreeNode::GetLeafNodes(vector<BSPTreeNode*>& r_AllLeafNodes)
{
	if (m_IsLeaf) {
		r_AllLeafNodes.push_back(this);
	}
	else {

		assert(nullptr != m_PositiveSon);
		assert(nullptr != m_NegativeSon);

		m_PositiveSon->GetLeafNodes(r_AllLeafNodes);
		m_NegativeSon->GetLeafNodes(r_AllLeafNodes);
	}

}

int BSPTreeNode::Size()
{
	return m_FacesA.size() + m_FacesB.size();
}

bool BSPTreeNode::IsLeaf()
{
	return m_IsLeaf;
}

bool BSPTreeNode::GetIntersectPair(IntersectTriangleCheckList& p_IntersectCheckList)
{
	
	for (auto& fa : m_FacesA) {
		for (auto& fb : m_FacesB) {
			p_IntersectCheckList.push_back(make_pair(fa.m_HoldFace.idx(), fb.m_HoldFace.idx()));

		}
	}


	if ((m_FacesA.size() > 0) && (m_FacesB.size() > 0))
		return true;
	else
		return false;

}

bool BSPTreeNode::IsValidNode()
{
	if ((0 == m_FacesA.size()) || (0 == m_FacesB.size()))
		return true;

	return false;
}

int BSPTreeNode::GetValidFaces(unordered_set<FaceId>& facesA, unordered_set<FaceId>& facesB)
{
	if ((0 == m_FacesA.size()) || (0 == m_FacesB.size()))
		return 0;

	unordered_set<FaceId> fA, fB;

	for (auto& fa : m_FacesA) {
		facesA.insert(fa.m_HoldFace.idx());
		fA.insert(fa.m_HoldFace.idx());
	}

	for (auto& fb : m_FacesB) {
		facesB.insert(fb.m_HoldFace.idx());
		fB.insert(fb.m_HoldFace.idx());
	}

	return fA.size() + fB.size();
}

void BSPTreeNode::GetPartitionPlane(TreeFaceRenderingInfo& r_Pplanes, planeRecords p_CurrentRecords)
{
	if (m_IsLeaf) return;


	if (m_IsRoot) {
		assert(m_Depth == 0);

		p_CurrentRecords.clear();
	}


	r_Pplanes.push_back(make_pair(m_PartitionPlane, p_CurrentRecords));



	p_CurrentRecords.push_back(make_pair(m_PartitionPlane, true));
	m_PositiveSon->GetPartitionPlane(r_Pplanes, p_CurrentRecords);

	p_CurrentRecords.pop_back();

	p_CurrentRecords.push_back(make_pair(m_PartitionPlane, false));
	m_NegativeSon->GetPartitionPlane(r_Pplanes, p_CurrentRecords);


}
void BSPTreeNode::ConstructAngleSons()
{
	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;


	//int k = 0;
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;
	double sumXi = 0;
	double sumYi = 0;
	double sumZi = 0;
	double Xi = 0;
	double Yi = 0;
	double Zi = 0;

	double tplaneX = 0;
	double tplaneY = 0;
	double tplaneZ = 0;
	Point3d point;
	double parameterA = 0;
	double parameterB = 0;
	double parameterC = 0;
	double medianX = 0;
	double medianY = 0;
	double medianZ = 0;

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			point = tri.VertexAt(i);
			sumX += point[0];
			sumY += point[1];
			sumZ += point[2];
		}
		//k++;
	}
	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			point = tri.VertexAt(i);
			sumX += point[0];
			sumY += point[1];
			sumZ += point[2];
		}
		//k++;
	}
	medianX = sumX / (3.0 * (fFirst.size() + fSecond.size()) );
	medianY = sumY / (3.0 * (fFirst.size() + fSecond.size()));
	medianZ = sumZ / (3.0 * (fFirst.size() + fSecond.size()));

	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}

	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();

			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();

			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index minEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&minEigvalusPos);

	int minEValus = int(minEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, minEValus), rEvecs(1, minEValus), rEvecs(2, minEValus));

	Plane3d partitionPlane(Point3d(medianX, medianY, medianZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);


	//		m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum);
	//		m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum);
	std::thread positiveThread(&BSPTreeNode::create_bsp_tree_node1, this, ref(facesAPositive), ref(facesBPositive), ref(m_Tolerance), ref(m_Depth) + 1, ref(m_MaxDepth), ref(m_LeafShresholdNum));
	std::thread negativeThread(&BSPTreeNode::create_bsp_tree_node2, this, ref(facesANegative), ref(facesBNegative), ref(m_Tolerance), ref(m_Depth) + 1, ref(m_MaxDepth), ref(m_LeafShresholdNum));
 
	positiveThread.join();
	negativeThread.join();
	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}


void BSPTreeNode::ConstructAngleSonsSerial()
{
	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;


	//int k = 0;
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;
	double sumXi = 0;
	double sumYi = 0;
	double sumZi = 0;
	double Xi = 0;
	double Yi = 0;
	double Zi = 0;

	double tplaneX = 0;
	double tplaneY = 0;
	double tplaneZ = 0;
	Point3d point;
	double parameterA = 0;
	double parameterB = 0;
	double parameterC = 0;
	double medianX = 0;
	double medianY = 0;
	double medianZ = 0;

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			point = tri.VertexAt(i);
			sumX += point[0];
			sumY += point[1];
			sumZ += point[2];
		}
		//k++;
	}
	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			point = tri.VertexAt(i);
			sumX += point[0];
			sumY += point[1];
			sumZ += point[2];
		}
		//k++;
	}
	medianX = sumX / (3.0 * (fFirst.size() + fSecond.size()));
	medianY = sumY / (3.0 * (fFirst.size() + fSecond.size()));
	medianZ = sumZ / (3.0 * (fFirst.size() + fSecond.size()));




	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}

	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();
			
			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}



	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {

			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();

			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index minEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&minEigvalusPos);

	int minEValus = int(minEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, minEValus), rEvecs(1, minEValus), rEvecs(2, minEValus));

	Plane3d partitionPlane(Point3d(medianX, medianY, medianZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);


	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SerialAngle);
	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SerialAngle);
	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructAngle_SDMSons()
{

	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;


	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;


	double standVarXA = 0;
	double standVarYA = 0;
	double standVarZA = 0;

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];
		}
	}

	double xBarA = sumXA / (fFirst.size() * 3);
	double yBarA = sumYA / (fFirst.size() * 3);
	double zBarA = sumZA / (fFirst.size() * 3);

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			standVarXA += (point[0] - xBarA) * (point[0] - xBarA);
			standVarYA += (point[1] - yBarA) * (point[1] - yBarA);
			standVarZA += (point[2] - zBarA) * (point[2] - zBarA);
		}

	}

	standVarXA = sqrt(standVarXA / (fFirst.size() * 3 - 1));
	standVarYA = sqrt(standVarYA / (fFirst.size() * 3 - 1));
	standVarZA = sqrt(standVarZA / (fFirst.size() * 3 - 1));

	double scaleA = sqrt(standVarXA * standVarYA * standVarZA);



	//meshB
	double standVarXB = 0;
	double standVarYB = 0;
	double standVarZB = 0;

	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];
		}
	}

	double xBarB = sumXB / (fSecond.size() * 3);
	double yBarB = sumYB / (fSecond.size() * 3);
	double zBarB = sumZB / (fSecond.size() * 3);

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			standVarXB += (point[0] - xBarB) * (point[0] - xBarB);
			standVarYB += (point[1] - yBarB) * (point[1] - yBarB);
			standVarZB += (point[2] - zBarB) * (point[2] - zBarB);
		}


	}

	standVarXB = sqrt(standVarXB / (fSecond.size() * 3 - 1));
	standVarYB = sqrt(standVarYB / (fSecond.size() * 3 - 1));
	standVarZB = sqrt(standVarZB / (fSecond.size() * 3 - 1));
	double scaleB = sqrt(standVarXB * standVarYB * standVarZB);


	//weight
	double disWeightA = scaleB / (scaleA + scaleB);
	double disWeightB = scaleA / (scaleA + scaleB);

	double numWeightA = (double)fSecond.size() / (double)(fFirst.size() + fSecond.size());
	double numWeightB = (double)fFirst.size() / (double)(fFirst.size() + fSecond.size());

	//ofstream outFile("..//TestModel//thingi10k//res2.txt",ios::app);
	//if (m_Depth == 0) {
	//	if (scaleA / scaleB <= 1) {
	//		outFile << "distribution compare " << scaleB / scaleA << endl;
	//	}
	//	else {
	//		outFile << "distribution compare " << scaleA / scaleB << endl;
	//	}
	//	
	//	if (fFirst.size() >= fSecond.size()) {
	//		outFile << "size compare " << (double)fFirst.size() / (double)fSecond.size() << endl;
	//	}
	//	else {
	//		outFile << "size compare " << (double)fSecond.size() / (double)fFirst.size() << endl;
	//	}

	//}
	//outFile.close();

	//
	double lambdaS = 1, lambdaD = 0.5;

	double weightA = pow(disWeightA, lambdaD) * pow(numWeightA, lambdaS);
	double weightB = pow(disWeightB, lambdaD) * pow(numWeightB, lambdaS);

	//double partialA = -1, partialB = -1, partialC = -1;
	//if (disWeightA > disWeightB)
	//	partialA = disWeightA / disWeightB;
	//else
	//	partialA = disWeightB / disWeightA;

	//if (numWeightA > numWeightB)
	//	partialB = numWeightA / numWeightB;
	//else
	//	partialB = numWeightB / numWeightA;

	//if (weightA > weightB)
	//	partialC = weightA / weightB;
	//else
	//	partialC = weightB / weightA;

	//double weightA = disWeightA * numWeightA;
	//double weightB = disWeightB * numWeightB;




	//double weightA = numWeightA;
	//double weightB = numWeightB;
	//double weightA = disWeightA;
	//double weightB = disWeightB;

	double medianX = sumXA * weightA + sumXB * weightB;
	double medianY = sumYA * weightA + sumYB * weightB;
	double medianZ = sumZA * weightA + sumZB * weightB;


	medianX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	medianY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	medianZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);




	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}

	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();
			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			double le = (point - Point3d(medianX, medianY, medianZ)).length();
			paraMatrix(0, 0) += pow(point[0] - medianX, 2) / le;
			paraMatrix(1, 1) += pow(point[1] - medianY, 2) / le;
			paraMatrix(2, 2) += pow(point[2] - medianZ, 2) / le;

			paraMatrix(0, 1) += (point[0] - medianX) * (point[1] - medianY) / le;
			paraMatrix(1, 0) += (point[0] - medianX) * (point[1] - medianY) / le;

			paraMatrix(0, 2) += (point[0] - medianX) * (point[2] - medianZ) / le;
			paraMatrix(2, 0) += (point[0] - medianX) * (point[2] - medianZ) / le;

			paraMatrix(1, 2) += (point[1] - medianY) * (point[2] - medianZ) / le;
			paraMatrix(2, 1) += (point[1] - medianY) * (point[2] - medianZ) / le;
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index minEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&minEigvalusPos);

	int minEValus = int(minEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, minEValus), rEvecs(1, minEValus), rEvecs(2, minEValus));

	Plane3d partitionPlane(Point3d(medianX, medianY, medianZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);


	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_Angle);
	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_Angle);
	
	
	//std::thread positiveThread(&BSPTreeNode::create_bsp_tree_node1, this, ref(facesAPositive), ref(facesBPositive), ref(m_Tolerance), ref(m_Depth) + 1, ref(m_MaxDepth), ref(m_LeafShresholdNum));
	//std::thread negativeThread(&BSPTreeNode::create_bsp_tree_node2, this, ref(facesANegative), ref(facesBNegative), ref(m_Tolerance), ref(m_Depth) + 1, ref(m_MaxDepth), ref(m_LeafShresholdNum));

	//positiveThread.join();
	//negativeThread.join();


	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructAABBSons()
{
	vector<TriangleForTree> allFaces(m_FacesA);
	allFaces.insert(allFaces.end(), m_FacesB.begin(), m_FacesB.end());

	AABB aabb(allFaces);
	Plane3d partitionPlane = aabb.GetLongestAxisEqualParitionPlane();

	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructWSDMSons()
{

	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;

	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	//double standVarXA = 0;
	//double standVarYA = 0;
	//double standVarZA = 0;

	//for (auto& fa : fFirst) {
	//	Triangle3d tri = fa.m_Triangle;

	//	for (int i = 0;i < 3;i++) {
	//		Point3d point = tri.VertexAt(i);
	//		sumXA += point[0];
	//		sumYA += point[1];
	//		sumZA += point[2];
	//	}
	//}

	//double xBarA = sumXA / (fFirst.size() * 3);
	//double yBarA = sumYA / (fFirst.size() * 3);
	//double zBarA = sumZA / (fFirst.size() * 3);


	//for (auto& fa : fFirst) {
	//	Triangle3d tri = fa.m_Triangle;

	//	for (int i = 0;i < 3;i++) {
	//		Point3d point = tri.VertexAt(i);

	//		standVarXA += (point[0] - xBarA) * (point[0] - xBarA);
	//		standVarYA += (point[1] - yBarA) * (point[1] - yBarA);
	//		standVarZA += (point[2] - zBarA) * (point[2] - zBarA);
	//	}

	//}


	///////////////////////
	double sumXA_Square = 0;
	double sumYA_Square = 0;
	double sumZA_Square = 0;
	double sumXAYA = 0;
	double sumXAZA = 0;
	double sumYAZA = 0;

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];

			sumXA_Square += point[0] * point[0];
			sumYA_Square += point[1] * point[1];
			sumZA_Square += point[2] * point[2];

			sumXAYA += point[0] * point[1];
			sumXAZA += point[0] * point[2];
			sumYAZA += point[1] * point[2];
		}
	}

	double xBarA = sumXA / (fFirst.size() * 3);
	double yBarA = sumYA / (fFirst.size() * 3);
	double zBarA = sumZA / (fFirst.size() * 3);
	
	double standVarXA = sumXA_Square - 2 * sumXA * xBarA + fFirst.size() * 3 * xBarA * xBarA;
	double standVarYA = sumYA_Square - 2 * sumYA * yBarA + fFirst.size() * 3 * yBarA * yBarA;
	double standVarZA = sumZA_Square - 2 * sumZA * zBarA + fFirst.size() * 3 * zBarA * zBarA;

	///////////////////////
	standVarXA = sqrt(standVarXA / (fFirst.size() * 3 - 1));
	standVarYA = sqrt(standVarYA / (fFirst.size() * 3 - 1));
	standVarZA = sqrt(standVarZA / (fFirst.size() * 3 - 1));

	double scaleA = sqrt(standVarXA * standVarYA * standVarZA);



	//meshB
	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;

	//double standVarXB = 0;
	//double standVarYB = 0;
	//double standVarZB = 0;

	//for (auto& fb : fSecond) {
	//	Triangle3d tri = fb.m_Triangle;

	//	for (int i = 0;i < 3;i++) {
	//		Point3d point = tri.VertexAt(i);

	//		sumXB += point[0];
	//		sumYB += point[1];
	//		sumZB += point[2];
	//	}
	//}

	//double xBarB = sumXB / (fSecond.size() * 3);
	//double yBarB = sumYB / (fSecond.size() * 3);
	//double zBarB = sumZB / (fSecond.size() * 3);

	//for (auto& fb : fSecond) {

	//	Triangle3d tri = fb.m_Triangle;

	//	for (int i = 0;i < 3;i++) {
	//		Point3d point = tri.VertexAt(i);

	//		standVarXB += (point[0] - xBarB) * (point[0] - xBarB);
	//		standVarYB += (point[1] - yBarB) * (point[1] - yBarB);
	//		standVarZB += (point[2] - zBarB) * (point[2] - zBarB);
	//	}


	//}


	///////////////////////
	double sumXB_Square = 0;
	double sumYB_Square = 0;
	double sumZB_Square = 0;
	double sumXBYB = 0;
	double sumXBZB = 0;
	double sumYBZB = 0;

	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];

			sumXB_Square += point[0] * point[0];
			sumYB_Square += point[1] * point[1];
			sumZB_Square += point[2] * point[2];

			sumXBYB += point[0] * point[1];
			sumXBZB += point[0] * point[2];
			sumYBZB += point[1] * point[2];
		}
	}

	double xBarB = sumXB / (fSecond.size() * 3);
	double yBarB = sumYB / (fSecond.size() * 3);
	double zBarB = sumZB / (fSecond.size() * 3);

	double standVarXB = sumXB_Square - 2 * sumXB * xBarB + fSecond.size() * 3 * xBarB * xBarB;
	double standVarYB = sumYB_Square - 2 * sumYB * yBarB + fSecond.size() * 3 * yBarB * yBarB;
	double standVarZB = sumZB_Square - 2 * sumZB * zBarB + fSecond.size() * 3 * zBarB * zBarB;

	///////////////////////
	standVarXB = sqrt(standVarXB / (fSecond.size() * 3 - 1));
	standVarYB = sqrt(standVarYB / (fSecond.size() * 3 - 1));
	standVarZB = sqrt(standVarZB / (fSecond.size() * 3 - 1));

	double scaleB = sqrt(standVarXB * standVarYB * standVarZB);


	//weight
	double disWeightA = scaleB / (scaleA + scaleB);
	double disWeightB = scaleA / (scaleA + scaleB);

	double numWeightA = (double)fSecond.size() / (double)(fFirst.size() + fSecond.size());
	double numWeightB = (double)fFirst.size() / (double)(fFirst.size() + fSecond.size());

	//ofstream outFile("..//TestModel//thingi10k//res2.txt",ios::app);
	//if (m_Depth == 0) {
	//	if (scaleA / scaleB <= 1) {
	//		outFile << "distribution compare " << scaleB / scaleA << endl;
	//	}
	//	else {
	//		outFile << "distribution compare " << scaleA / scaleB << endl;
	//	}
	//	
	//	if (fFirst.size() >= fSecond.size()) {
	//		outFile << "size compare " << (double)fFirst.size() / (double)fSecond.size() << endl;
	//	}
	//	else {
	//		outFile << "size compare " << (double)fSecond.size() / (double)fFirst.size() << endl;
	//	}

	//}
	//outFile.close();

	double lambdaS = m_LambdaS, lambdaD = m_LambdaD;
	//double lambdaS = 1, lambdaD = 0.5;

	double weightA = pow(disWeightA, lambdaD) * pow(numWeightA, lambdaS);
	double weightB = pow(disWeightB, lambdaD) * pow(numWeightB, lambdaS);

	//double partialA = -1, partialB = -1, partialC = -1;
	//if (disWeightA > disWeightB)
	//	partialA = disWeightA / disWeightB;
	//else
	//	partialA = disWeightB / disWeightA;

	//if (numWeightA > numWeightB)
	//	partialB = numWeightA / numWeightB;
	//else
	//	partialB = numWeightB / numWeightA;

	//if (weightA > weightB)
	//	partialC = weightA / weightB;
	//else
	//	partialC = weightB / weightA;

	//double weightA = disWeightA * numWeightA;
	//double weightB = disWeightB * numWeightB;




	//double weightA = numWeightA;
	//double weightB = numWeightB;
	//double weightA = disWeightA;
	//double weightB = disWeightB;

	double centerX = sumXA * weightA + sumXB * weightB;
	double centerY = sumYA * weightA + sumYB * weightB;
	double centerZ = sumZA * weightA + sumZB * weightB;


	centerX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);


	Eigen::Matrix3d totalCovMatrix = Eigen::Matrix3d::Zero();
	{
		Eigen::Matrix3d covMatrixA = Eigen::Matrix3d::Zero(), covMatrixB = Eigen::Matrix3d::Zero();

		covMatrixA(0, 0) += sumXA_Square - 2 * sumXA * centerX + fFirst.size() * 3 * centerX * centerX;
		covMatrixA(1, 1) += sumYA_Square - 2 * sumYA * centerY + fFirst.size() * 3 * centerY * centerY;
		covMatrixA(2, 2) += sumZA_Square - 2 * sumZA * centerZ + fFirst.size() * 3 * centerZ * centerZ;

		covMatrixA(0, 1) += sumXAYA - sumXA * centerY - sumYA * centerX + fFirst.size() * 3 * centerX * centerY;
		covMatrixA(1, 0) += covMatrixA(0, 1);

		covMatrixA(0, 2) += sumXAZA - sumXA * centerZ - sumZA * centerX + fFirst.size() * 3 * centerX * centerZ;
		covMatrixA(2, 0) += covMatrixA(0, 2);

		covMatrixA(1, 2) += sumYAZA - sumYA * centerZ - sumZA * centerY + fFirst.size() * 3 * centerY * centerZ;
		covMatrixA(2, 1) += covMatrixA(1, 2);




		covMatrixB(0, 0) += sumXB_Square - 2 * sumXB * centerX + fSecond.size() * 3 * centerX * centerX;
		covMatrixB(1, 1) += sumYB_Square - 2 * sumYB * centerY + fSecond.size() * 3 * centerY * centerY;
		covMatrixB(2, 2) += sumZB_Square - 2 * sumZB * centerZ + fSecond.size() * 3 * centerZ * centerZ;

		covMatrixB(0, 1) += sumXBYB - sumXB * centerY - sumYB * centerX + fSecond.size() * 3 * centerX * centerY;
		covMatrixB(1, 0) += covMatrixB(0, 1);

		covMatrixB(0, 2) += sumXBZB - sumXB * centerZ - sumZB * centerX + fSecond.size() * 3 * centerX * centerZ;
		covMatrixB(2, 0) += covMatrixB(0, 2);

		covMatrixB(1, 2) += sumYBZB - sumYB * centerZ - sumZB * centerY + fSecond.size() * 3 * centerY * centerZ;
		covMatrixB(2, 1) += covMatrixB(1, 2);


		totalCovMatrix = covMatrixA * weightA + covMatrixB * weightB;
	}



	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(totalCovMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index maxEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

	int maxEValus = int(maxEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));


	//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
	//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

	Plane3d partitionPlane(Point3d(centerX, centerY, centerZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::WSDM);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::WSDM);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;

}

void BSPTreeNode::ConstructWSDM_PCASons()
{

	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;

	//auto testTri = fFirst[0];
	//testTri.m_Triangle = Triangle3d({ Point3d{1 ,2 ,3}, Point3d{9,9,9}, Point3d{3,4,7} });
	//fFirst.clear();
	//fFirst.push_back(testTri);

	//parse CA
	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	double sumXA_Square = 0;
	double sumYA_Square = 0;
	double sumZA_Square = 0;
	double sumXAYA = 0;
	double sumXAZA = 0;
	double sumYAZA = 0;


	Eigen::MatrixXd cAPointsMatrix = Eigen::MatrixXd::Zero(fFirst.size() * 3, 3);

	int countA = 0;
	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			cAPointsMatrix(countA, 0) = point[0];
			cAPointsMatrix(countA, 1) = point[1];
			cAPointsMatrix(countA, 2) = point[2];
			countA++;

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];

			sumXA_Square += point[0] * point[0];
			sumYA_Square += point[1] * point[1];
			sumZA_Square += point[2] * point[2];

			sumXAYA += point[0] * point[1];
			sumXAZA += point[0] * point[2];
			sumYAZA += point[1] * point[2];
		}
	}

	double xBarA = sumXA / (fFirst.size() * 3);
	double yBarA = sumYA / (fFirst.size() * 3);
	double zBarA = sumZA / (fFirst.size() * 3);

	double standVarXA = 0;
	double standVarYA = 0;
	double standVarZA = 0;

	{
		//covariance matrix CA
		Eigen::Matrix3d covMatrix = Eigen::Matrix3d::Zero();

		covMatrix(0, 0) += sumXA_Square - 2 * sumXA * xBarA + fFirst.size() * 3 * xBarA * xBarA;
		covMatrix(1, 1) += sumYA_Square - 2 * sumYA * yBarA + fFirst.size() * 3 * yBarA * yBarA;
		covMatrix(2, 2) += sumZA_Square - 2 * sumZA * zBarA + fFirst.size() * 3 * zBarA * zBarA;

		covMatrix(0, 1) += sumXAYA - sumXA * yBarA - sumYA * xBarA + fFirst.size() * 3 * xBarA * yBarA;
		covMatrix(1, 0) += covMatrix(0, 1);

		covMatrix(0, 2) += sumXAZA - sumXA * zBarA - sumZA * xBarA + fFirst.size() * 3 * xBarA * zBarA;
		covMatrix(2, 0) += covMatrix(0, 2);

		covMatrix(1, 2) += sumYAZA - sumYA * zBarA - sumZA * yBarA + fFirst.size() * 3 * yBarA * zBarA;
		covMatrix(2, 1) += covMatrix(1, 2);

		covMatrix /= (fFirst.size() * 3);

		

		Eigen::EigenSolver<Eigen::MatrixXd> eSolver(covMatrix);
		Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
		//Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

		//main direction
		Eigen::MatrixXd rEvecs = cEvecs.real();
		//Eigen::MatrixXd rEvals = cEvals.real();

		assert(IsZero(1 - rEvecs.col(0).norm()));
		assert(IsZero(1 - rEvecs.col(1).norm()));
		assert(IsZero(1 - rEvecs.col(2).norm()));

		cAPointsMatrix.col(0) -= xBarA * Eigen::VectorXd::Ones(cAPointsMatrix.col(0).size());
		cAPointsMatrix.col(1) -= yBarA * Eigen::VectorXd::Ones(cAPointsMatrix.col(0).size());
		cAPointsMatrix.col(2) -= zBarA * Eigen::VectorXd::Ones(cAPointsMatrix.col(0).size());

		Eigen::MatrixXd newCAPointsMatrix = cAPointsMatrix * rEvecs;

		double detaXSquareSum = newCAPointsMatrix.col(0).array().square().sum();
		double detaYSquareSum = newCAPointsMatrix.col(1).array().square().sum();
		double detaZSquareSum = newCAPointsMatrix.col(2).array().square().sum();

		standVarXA = sqrt(detaXSquareSum / (fFirst.size() * 3 - 1));
		standVarYA = sqrt(detaYSquareSum / (fFirst.size() * 3 - 1));
		standVarZA = sqrt(detaZSquareSum / (fFirst.size() * 3 - 1));
	}
		
	double scaleA = sqrt(standVarXA * standVarYA * standVarZA);


	//parse CB
	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;

	double sumXB_Square = 0;
	double sumYB_Square = 0;
	double sumZB_Square = 0;
	double sumXBYB = 0;
	double sumXBZB = 0;
	double sumYBZB = 0;

	Eigen::MatrixXd cBPointsMatrix = Eigen::MatrixXd::Zero(fSecond.size() * 3, 3);

	int countB = 0;
	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			cBPointsMatrix(countB, 0) = point[0];
			cBPointsMatrix(countB, 1) = point[1];
			cBPointsMatrix(countB, 2) = point[2];
			countB++;

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];

			sumXB_Square += point[0] * point[0];
			sumYB_Square += point[1] * point[1];
			sumZB_Square += point[2] * point[2];
			sumXBYB += point[0] * point[1];
			sumXBZB += point[0] * point[2];
			sumYBZB += point[1] * point[2];
		}
	}

	double xBarB = sumXB / (fSecond.size() * 3);
	double yBarB = sumYB / (fSecond.size() * 3);
	double zBarB = sumZB / (fSecond.size() * 3);


	double standVarXB = 0;
	double standVarYB = 0;
	double standVarZB = 0;


	{
		//covariance matrix CA
		Eigen::Matrix3d covMatrix = Eigen::Matrix3d::Zero();

		covMatrix(0, 0) += sumXB_Square - 2 * sumXB * xBarB + fSecond.size() * 3 * xBarB * xBarB;
		covMatrix(1, 1) += sumYB_Square - 2 * sumYB * yBarB + fSecond.size() * 3 * yBarB * yBarB;
		covMatrix(2, 2) += sumZB_Square - 2 * sumZB * zBarB + fSecond.size() * 3 * zBarB * zBarB;

		covMatrix(0, 1) += sumXBYB - sumXB * yBarB - sumYB * xBarB + fSecond.size() * 3 * xBarB * yBarB;
		covMatrix(1, 0) += covMatrix(0, 1);

		covMatrix(0, 2) += sumXBZB - sumXB * zBarB - sumZB * xBarB + fSecond.size() * 3 * xBarB * zBarB;
		covMatrix(2, 0) += covMatrix(0, 2);

		covMatrix(1, 2) += sumYBZB - sumYB * zBarB - sumZB * yBarB + fSecond.size() * 3 * yBarB * zBarB;
		covMatrix(2, 1) += covMatrix(1, 2);

		covMatrix /= (fSecond.size() * 3);


		Eigen::EigenSolver<Eigen::MatrixXd> eSolver(covMatrix);
		Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
		//Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

		//main direction
		Eigen::MatrixXd rEvecs = cEvecs.real();
		//Eigen::MatrixXd rEvals = cEvals.real();

		assert(IsZero(1 - rEvecs.col(0).norm()));
		assert(IsZero(1 - rEvecs.col(1).norm()));
		assert(IsZero(1 - rEvecs.col(2).norm()));

		cBPointsMatrix.col(0) -= xBarB * Eigen::VectorXd::Ones(cBPointsMatrix.col(0).size());
		cBPointsMatrix.col(1) -= yBarB * Eigen::VectorXd::Ones(cBPointsMatrix.col(0).size());
		cBPointsMatrix.col(2) -= zBarB * Eigen::VectorXd::Ones(cBPointsMatrix.col(0).size());

		Eigen::MatrixXd newCBPointsMatrix = cBPointsMatrix * rEvecs;

		double detaXSquareSum = newCBPointsMatrix.col(0).array().square().sum();
		double detaYSquareSum = newCBPointsMatrix.col(1).array().square().sum();
		double detaZSquareSum = newCBPointsMatrix.col(2).array().square().sum();

		standVarXB = sqrt(detaXSquareSum / (fSecond.size() * 3 - 1));
		standVarYB = sqrt(detaYSquareSum / (fSecond.size() * 3 - 1));
		standVarZB = sqrt(detaZSquareSum / (fSecond.size() * 3 - 1));
	}

	double scaleB = sqrt(standVarXB * standVarYB * standVarZB);





	//weight
	double disWeightA = scaleB / (scaleA + scaleB);
	double disWeightB = scaleA / (scaleA + scaleB);

	double numWeightA = (double)fSecond.size() / (double)(fFirst.size() + fSecond.size());
	double numWeightB = (double)fFirst.size() / (double)(fFirst.size() + fSecond.size());



	double lambdaS = m_LambdaS, lambdaD = m_LambdaD;
	//double lambdaS = 1, lambdaD = 0.5;

	double weightA = pow(disWeightA, lambdaD) * pow(numWeightA, lambdaS);
	double weightB = pow(disWeightB, lambdaD) * pow(numWeightB, lambdaS);


	double centerX = sumXA * weightA + sumXB * weightB;
	double centerY = sumYA * weightA + sumYB * weightB;
	double centerZ = sumZA * weightA + sumZB * weightB;


	centerX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);



	Eigen::Matrix3d totalCovMatrix = Eigen::Matrix3d::Zero();
	{
		Eigen::Matrix3d covMatrixA = Eigen::Matrix3d::Zero(), covMatrixB = Eigen::Matrix3d::Zero();

		covMatrixA(0, 0) += sumXA_Square - 2 * sumXA * centerX + fFirst.size() * 3 * centerX * centerX;
		covMatrixA(1, 1) += sumYA_Square - 2 * sumYA * centerY + fFirst.size() * 3 * centerY * centerY;
		covMatrixA(2, 2) += sumZA_Square - 2 * sumZA * centerZ + fFirst.size() * 3 * centerZ * centerZ;

		covMatrixA(0, 1) += sumXAYA - sumXA * centerY - sumYA * centerX + fFirst.size() * 3 * centerX * centerY;
		covMatrixA(1, 0) += covMatrixA(0, 1);

		covMatrixA(0, 2) += sumXAZA - sumXA * centerZ - sumZA * centerX + fFirst.size() * 3 * centerX * centerZ;
		covMatrixA(2, 0) += covMatrixA(0, 2);

		covMatrixA(1, 2) += sumYAZA - sumYA * centerZ - sumZA * centerY + fFirst.size() * 3 * centerY * centerZ;
		covMatrixA(2, 1) += covMatrixA(1, 2);




		covMatrixB(0, 0) += sumXB_Square - 2 * sumXB * centerX + fSecond.size() * 3 * centerX * centerX;
		covMatrixB(1, 1) += sumYB_Square - 2 * sumYB * centerY + fSecond.size() * 3 * centerY * centerY;
		covMatrixB(2, 2) += sumZB_Square - 2 * sumZB * centerZ + fSecond.size() * 3 * centerZ * centerZ;

		covMatrixB(0, 1) += sumXBYB - sumXB * centerY - sumYB * centerX + fSecond.size() * 3 * centerX * centerY;
		covMatrixB(1, 0) += covMatrixB(0, 1);

		covMatrixB(0, 2) += sumXBZB - sumXB * centerZ - sumZB * centerX + fSecond.size() * 3 * centerX * centerZ;
		covMatrixB(2, 0) += covMatrixB(0, 2);

		covMatrixB(1, 2) += sumYBZB - sumYB * centerZ - sumZB * centerY + fSecond.size() * 3 * centerY * centerZ;
		covMatrixB(2, 1) += covMatrixB(1, 2);


		totalCovMatrix = covMatrixA * weightA + covMatrixB * weightB;
	}



	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(totalCovMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index maxEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

	int maxEValus = int(maxEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));


	//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
	//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

	Plane3d partitionPlane(Point3d(centerX, centerY, centerZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, m_Type);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, m_Type);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructSAHSons()
{
	const size_t smallNodeThreshold = 256;


	if (m_FacesA.size() + m_FacesB.size() > smallNodeThreshold) {

		//large node middle split

		vector<TriangleForTree> allFaces(m_FacesA);
		allFaces.insert(allFaces.end(), m_FacesB.begin(), m_FacesB.end());

		AABB aabb(allFaces);
		Plane3d partitionPlane = aabb.GetLongestAxisEqualParitionPlane();

		vector<TriangleForTree> rPositive, rNegative;
		GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


		vector<TriangleForTree> facesAPositive, facesANegative;
		vector<TriangleForTree> facesBPositive, facesBNegative;

		SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
		SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

		//建立positive子树
		m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SAH);

		m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SAH);

		m_IsLeaf = false;

		m_PartitionPlane = partitionPlane;

	}
	else {

		
		//small node SAH
		//not split triangles
		vector<TriangleForTree> allFaces(m_FacesA);
		allFaces.insert(allFaces.end(), m_FacesB.begin(), m_FacesB.end());

		AABB aabb(allFaces);

		double currentSAHCost = double(m_Depth) + m_FacesA.size() + m_FacesB.size();

		//Plane3d partitionPlane = aabb.GetLongestAxisEqualParitionPlane();

		double minSAHCost = currentSAHCost;


		bool hasBetterSAH = false;
		Plane3d sahPlane;
		vector<TriangleForTree> rSahPositive, rSahNegative;


		for (auto& triCandidate : allFaces) {

			vector<TriangleForTree> rPositive, rNegative;

			bool sd = triCandidate.m_Triangle.isValid();
			double l1 = (triCandidate.m_Triangle.m_Vertices[0] - triCandidate.m_Triangle.m_Vertices[1]).length();
			double l2 = (triCandidate.m_Triangle.m_Vertices[1] - triCandidate.m_Triangle.m_Vertices[2]).length();
			double l3 = (triCandidate.m_Triangle.m_Vertices[2] - triCandidate.m_Triangle.m_Vertices[0]).length();

			Plane3d sahCandidatePlane = triCandidate.m_Triangle;

			GetNegativePositiveLabelWithOutSplitTriangle(rPositive, rNegative, sahCandidatePlane);

			if (rPositive.empty() || rNegative.empty()) continue;

			AABB positiveAABB(rPositive), negativeAABB(rNegative);


			double sAHCost = double(m_Depth) + rPositive.size() * positiveAABB.GetSurfaceArea() / aabb.GetSurfaceArea()
				+ rNegative.size() * negativeAABB.GetSurfaceArea() / aabb.GetSurfaceArea();

			if (sAHCost < minSAHCost) {
				minSAHCost = sAHCost;
				sahPlane = sahCandidatePlane;
				rSahPositive = rPositive;
				rSahNegative = rNegative;
				hasBetterSAH = true;
			}


		}

		if (hasBetterSAH) {

			vector<TriangleForTree> facesAPositive, facesANegative;
			vector<TriangleForTree> facesBPositive, facesBNegative;

			SeparationTrianglesFromDifferentMesh(rSahPositive, facesAPositive, facesBPositive);
			SeparationTrianglesFromDifferentMesh(rSahNegative, facesANegative, facesBNegative);

			//建立positive子树
			m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SAH);

			m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SAH);

			m_IsLeaf = false;

			m_PartitionPlane = sahPlane;

		}
		else {
			//all sah cost increase
			//not construct depth+1 son node

			m_IsLeaf = true;
			m_PositiveSon = nullptr;
			m_NegativeSon = nullptr;

			return;
		}

	}



}

void BSPTreeNode::ConstructObbMiddleSons()
{
	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;

	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];
		}
	}

	//meshB
	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;

	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];
		}
	}

	double xBarB = sumXB / (fSecond.size() * 3);
	double yBarB = sumYB / (fSecond.size() * 3);
	double zBarB = sumZB / (fSecond.size() * 3);

	double xBar = (sumXA + sumXB) / (fFirst.size() * 3 + fSecond.size() * 3);
	double yBar = (sumYA + sumYB) / (fFirst.size() * 3 + fSecond.size() * 3);
	double zBar = (sumZA + sumZB) / (fFirst.size() * 3 + fSecond.size() * 3);

	//初始化协方差矩阵
	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}

	//计算协方差矩阵中来自MeshA的分量
	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += pow(point[0] - xBar, 2);
			paraMatrix(1, 1) += pow(point[1] - yBar, 2);
			paraMatrix(2, 2) += pow(point[2] - zBar, 2);

			paraMatrix(0, 1) += (point[0] - xBar) * (point[1] - yBar);
			paraMatrix(1, 0) += (point[0] - xBar) * (point[1] - yBar);

			paraMatrix(0, 2) += (point[0] - xBar) * (point[2] - zBar);
			paraMatrix(2, 0) += (point[0] - xBar) * (point[2] - zBar);

			paraMatrix(1, 2) += (point[1] - yBar) * (point[2] - zBar);
			paraMatrix(2, 1) += (point[1] - yBar) * (point[2] - zBar);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += pow(point[0] - xBar, 2);
			paraMatrix(1, 1) += pow(point[1] - yBar, 2);
			paraMatrix(2, 2) += pow(point[2] - zBar, 2);

			paraMatrix(0, 1) += (point[0] - xBar) * (point[1] - yBar);
			paraMatrix(1, 0) += (point[0] - xBar) * (point[1] - yBar);

			paraMatrix(0, 2) += (point[0] - xBar) * (point[2] - zBar);
			paraMatrix(2, 0) += (point[0] - xBar) * (point[2] - zBar);

			paraMatrix(1, 2) += (point[1] - yBar) * (point[2] - zBar);
			paraMatrix(2, 1) += (point[1] - yBar) * (point[2] - zBar);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index maxEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

	int maxEValus = int(maxEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	


	//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
	//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

	Vector3d eigenVec1(rEvecs(0, 0), rEvecs(1, 0), rEvecs(2, 0));
	Vector3d eigenVec2(rEvecs(0, 1), rEvecs(1, 1), rEvecs(2, 1));
	Vector3d eigenVec3(rEvecs(0, 2), rEvecs(1, 2), rEvecs(2, 2));

	//计算投影在三个主方向上的中点
	

	Point3d initialPoint = fFirst.front().m_Triangle.VertexAt(0);

	double minProjectX = eigenVec1.dot(initialPoint);
	double maxProjectX = eigenVec1.dot(initialPoint);

	double minProjectY = eigenVec2.dot(initialPoint);
	double maxProjectY = eigenVec2.dot(initialPoint);

	double minProjectZ = eigenVec3.dot(initialPoint);
	double maxProjectZ = eigenVec3.dot(initialPoint);


	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			minProjectX = min(eigenVec1.dot(point), minProjectX);
			maxProjectX = max(eigenVec1.dot(point), maxProjectX);

			minProjectY = min(eigenVec2.dot(point), minProjectY);
			maxProjectY = max(eigenVec2.dot(point), maxProjectY);

			minProjectZ = min(eigenVec3.dot(point), minProjectZ);
			maxProjectZ = max(eigenVec3.dot(point), maxProjectZ);

		}
	}

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			minProjectX = min(eigenVec1.dot(point), minProjectX);
			maxProjectX = max(eigenVec1.dot(point), maxProjectX);

			minProjectY = min(eigenVec2.dot(point), minProjectY);
			maxProjectY = max(eigenVec2.dot(point), maxProjectY);

			minProjectZ = min(eigenVec3.dot(point), minProjectZ);
			maxProjectZ = max(eigenVec3.dot(point), maxProjectZ);
		}
	}

	Point3d obbCenter( (minProjectX + maxProjectX)/2 * eigenVec1 + (minProjectY + maxProjectY) / 2 * eigenVec2 + (minProjectZ + maxProjectZ) / 2 * eigenVec3);

	Vector3d planeNormal;
	//Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));

	if ((maxProjectX - minProjectX) > (maxProjectY - minProjectY)) {
		if ((maxProjectX - minProjectX) > (maxProjectZ - minProjectZ)) {
			//max x
			planeNormal = Vector3d(rEvecs(0, 0), rEvecs(1, 0), rEvecs(2, 0));
		}
		else {
			//max z
			planeNormal = Vector3d(rEvecs(0, 2), rEvecs(1, 2), rEvecs(2, 2));
		}
	
	}
	else {

		if ((maxProjectY - minProjectY) > (maxProjectZ - minProjectZ)) {
			//max y
			planeNormal = Vector3d(rEvecs(0, 1), rEvecs(1, 1), rEvecs(2, 1));
		}
		else {
			//max z
			planeNormal = Vector3d(rEvecs(0, 0), rEvecs(1, 0), rEvecs(2, 0));
		}
	
	
	}

	Plane3d partitionPlane(obbCenter, planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//提前终止条件
	if (double(rPositive.size() + rNegative.size()) >= 2 * double(m_FacesA.size() + m_FacesB.size())) {
		m_PositiveSon = nullptr;
		m_NegativeSon = nullptr;
		m_IsLeaf = true;

		return;
	}

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::ObbMiddel);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::ObbMiddel);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructGravity_SPLITSons()
{
	vector<TriangleForTree> allFaces(m_FacesA);
	allFaces.insert(allFaces.end(), m_FacesB.begin(), m_FacesB.end());

	AABB aabb(allFaces);
	Vector3d directionLongest = aabb.GetLongestAxis();

	double areaSum = 0;
	Point3d gravityPoint(0, 0, 0);

	for (auto& face : allFaces) {
		Vector3d v1 = face.m_Triangle.VertexAt(1) - face.m_Triangle.VertexAt(0);
		Vector3d v2 = face.m_Triangle.VertexAt(2) - face.m_Triangle.VertexAt(0);

		double triArea = v1.cross(v2).length() / 2;
		areaSum += triArea;

		gravityPoint += (face.m_Triangle.VertexAt(0) + face.m_Triangle.VertexAt(1) + face.m_Triangle.VertexAt(2)) * triArea;


	}

	gravityPoint /= 3;
	gravityPoint /= areaSum;

	Plane3d partitionPlane(gravityPoint, directionLongest);

	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::Gravity_SPLIT);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::Gravity_SPLIT);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructSDM_OBBSons()
{
	if (m_Depth <= 8) {
		auto fFirst = m_FacesA;
		auto fSecond = m_FacesB;

		//ofstream fileA("PointInfoA.txt");
		//for (auto& fa : fFirst) {
		//	Triangle3d tri = fa.m_Triangle;

		//	for (int i = 0;i < 3;i++) {
		//		Point3d point = tri.VertexAt(i);

		//		fileA << "(" << point[0] << "," << point[1] << "," << point[2] << ")" << endl;
		//	}

		//}

		//ofstream fileB("PointInfoB.txt");
		//for (auto& fb : fSecond) {
		//	Triangle3d tri = fb.m_Triangle;

		//	for (int i = 0;i < 3;i++) {
		//		Point3d point = tri.VertexAt(i);

		//		fileB << "(" << point[0] << "," << point[1] << "," << point[2] << ")" << endl;
		//	}

		//}

		double sumXA = 0;
		double sumYA = 0;
		double sumZA = 0;

		double sumXB = 0;
		double sumYB = 0;
		double sumZB = 0;


		double standVarXA = 0;
		double standVarYA = 0;
		double standVarZA = 0;

		for (auto& fa : fFirst) {
			Triangle3d tri = fa.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				sumXA += point[0];
				sumYA += point[1];
				sumZA += point[2];
			}
		}

		double xBarA = sumXA / (fFirst.size() * 3);
		double yBarA = sumYA / (fFirst.size() * 3);
		double zBarA = sumZA / (fFirst.size() * 3);

		for (auto& fa : fFirst) {
			Triangle3d tri = fa.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				standVarXA += (point[0] - xBarA) * (point[0] - xBarA);
				standVarYA += (point[1] - yBarA) * (point[1] - yBarA);
				standVarZA += (point[2] - zBarA) * (point[2] - zBarA);
			}

		}

		standVarXA = sqrt(standVarXA / (fFirst.size() * 3 - 1));
		standVarYA = sqrt(standVarYA / (fFirst.size() * 3 - 1));
		standVarZA = sqrt(standVarZA / (fFirst.size() * 3 - 1));

		double scaleA = sqrt(standVarXA * standVarYA * standVarZA);



		//meshB
		double standVarXB = 0;
		double standVarYB = 0;
		double standVarZB = 0;

		for (auto& fb : fSecond) {
			Triangle3d tri = fb.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				sumXB += point[0];
				sumYB += point[1];
				sumZB += point[2];
			}
		}

		double xBarB = sumXB / (fSecond.size() * 3);
		double yBarB = sumYB / (fSecond.size() * 3);
		double zBarB = sumZB / (fSecond.size() * 3);

		for (auto& fb : fSecond) {

			Triangle3d tri = fb.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				standVarXB += (point[0] - xBarB) * (point[0] - xBarB);
				standVarYB += (point[1] - yBarB) * (point[1] - yBarB);
				standVarZB += (point[2] - zBarB) * (point[2] - zBarB);
			}


		}

		standVarXB = sqrt(standVarXB / (fSecond.size() * 3 - 1));
		standVarYB = sqrt(standVarYB / (fSecond.size() * 3 - 1));
		standVarZB = sqrt(standVarZB / (fSecond.size() * 3 - 1));
		double scaleB = sqrt(standVarXB * standVarYB * standVarZB);


		//weight
		double disWeightA = scaleB / (scaleA + scaleB);
		double disWeightB = scaleA / (scaleA + scaleB);

		double numWeightA = (double)fSecond.size() / (double)(fFirst.size() + fSecond.size());
		double numWeightB = (double)fFirst.size() / (double)(fFirst.size() + fSecond.size());



		//
		double lambdaS = 1, lambdaD = 0.5;

		double weightA = pow(disWeightA, lambdaD) * pow(numWeightA, lambdaS);
		double weightB = pow(disWeightB, lambdaD) * pow(numWeightB, lambdaS);

		double partialA = -1, partialB = -1, partialC = -1;
		if (disWeightA > disWeightB)
			partialA = disWeightA / disWeightB;
		else
			partialA = disWeightB / disWeightA;

		if (numWeightA > numWeightB)
			partialB = numWeightA / numWeightB;
		else
			partialB = numWeightB / numWeightA;

		if (weightA > weightB)
			partialC = weightA / weightB;
		else
			partialC = weightB / weightA;

		//double weightA = disWeightA * numWeightA;
		//double weightB = disWeightB * numWeightB;




		//double weightA = numWeightA;
		//double weightB = numWeightB;
		//double weightA = disWeightA;
		//double weightB = disWeightB;

		double centerX = sumXA * weightA + sumXB * weightB;
		double centerY = sumYA * weightA + sumYB * weightB;
		double centerZ = sumZA * weightA + sumZB * weightB;


		centerX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
		centerY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
		centerZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);



		Eigen::Matrix3d paraMatrix;
		for (int i = 0;i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				paraMatrix(i, j) = 0;
			}
		}


		//cout << "==========" << endl;
		//cout << paraMatrix<< endl;

		for (auto& fa : fFirst) {

			Triangle3d tri = fa.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				paraMatrix(0, 0) += weightA * pow(point[0] - centerX, 2);
				paraMatrix(1, 1) += weightA * pow(point[1] - centerY, 2);
				paraMatrix(2, 2) += weightA * pow(point[2] - centerZ, 2);

				paraMatrix(0, 1) += weightA * (point[0] - centerX) * (point[1] - centerY);
				paraMatrix(1, 0) += weightA * (point[0] - centerX) * (point[1] - centerY);

				paraMatrix(0, 2) += weightA * (point[0] - centerX) * (point[2] - centerZ);
				paraMatrix(2, 0) += weightA * (point[0] - centerX) * (point[2] - centerZ);

				paraMatrix(1, 2) += weightA * (point[1] - centerY) * (point[2] - centerZ);
				paraMatrix(2, 1) += weightA * (point[1] - centerY) * (point[2] - centerZ);
			}


		}

		//cout << "==========" << endl;
		//cout << paraMatrix << endl;

		for (auto& fb : fSecond) {

			Triangle3d tri = fb.m_Triangle;

			for (int i = 0;i < 3;i++) {
				Point3d point = tri.VertexAt(i);

				paraMatrix(0, 0) += weightB * pow(point[0] - centerX, 2);
				paraMatrix(1, 1) += weightB * pow(point[1] - centerY, 2);
				paraMatrix(2, 2) += weightB * pow(point[2] - centerZ, 2);

				paraMatrix(0, 1) += weightB * (point[0] - centerX) * (point[1] - centerY);
				paraMatrix(1, 0) += weightB * (point[0] - centerX) * (point[1] - centerY);

				paraMatrix(0, 2) += weightB * (point[0] - centerX) * (point[2] - centerZ);
				paraMatrix(2, 0) += weightB * (point[0] - centerX) * (point[2] - centerZ);

				paraMatrix(1, 2) += weightB * (point[1] - centerY) * (point[2] - centerZ);
				paraMatrix(2, 1) += weightB * (point[1] - centerY) * (point[2] - centerZ);
			}


		}

		//cout << "==========" << endl;
		//cout << paraMatrix << endl;


		Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
		Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
		Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

		Eigen::MatrixXd rEvecs = cEvecs.real();
		Eigen::MatrixXd rEvals = cEvals.real();

		Eigen::MatrixXf::Index maxEigvalusPos;
		rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

		int maxEValus = int(maxEigvalusPos);

		//cout << "==========" << endl;
		//cout << paraMatrix << endl;

		Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));


		//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
		//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

		Plane3d partitionPlane(Point3d(centerX, centerY, centerZ), planeNormal);


		vector<TriangleForTree> rPositive, rNegative;
		GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


		vector<TriangleForTree> facesAPositive, facesANegative;
		vector<TriangleForTree> facesBPositive, facesBNegative;

		SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
		SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

		//建立positive子树
		m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_OBB);

		m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_OBB);

		m_IsLeaf = false;

		m_PartitionPlane = partitionPlane;
	}
	else {
	
	
	}

}

void BSPTreeNode::ConstructOrigin_SDMSons()
{
	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;

	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;


	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];
		}
	}



	//meshB

	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];
		}
	}


	double weightA = 1.0;
	double weightB = 1.0;

	double centerX = sumXA * weightA + sumXB * weightB;
	double centerY = sumYA * weightA + sumYB * weightB;
	double centerZ = sumZA * weightA + sumZB * weightB;

	centerX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);



	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}


	//cout << "==========" << endl;
	//cout << paraMatrix<< endl;

	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += weightA * pow(point[0] - centerX, 2);
			paraMatrix(1, 1) += weightA * pow(point[1] - centerY, 2);
			paraMatrix(2, 2) += weightA * pow(point[2] - centerZ, 2);

			paraMatrix(0, 1) += weightA * (point[0] - centerX) * (point[1] - centerY);
			paraMatrix(1, 0) += weightA * (point[0] - centerX) * (point[1] - centerY);

			paraMatrix(0, 2) += weightA * (point[0] - centerX) * (point[2] - centerZ);
			paraMatrix(2, 0) += weightA * (point[0] - centerX) * (point[2] - centerZ);

			paraMatrix(1, 2) += weightA * (point[1] - centerY) * (point[2] - centerZ);
			paraMatrix(2, 1) += weightA * (point[1] - centerY) * (point[2] - centerZ);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += weightB * pow(point[0] - centerX, 2);
			paraMatrix(1, 1) += weightB * pow(point[1] - centerY, 2);
			paraMatrix(2, 2) += weightB * pow(point[2] - centerZ, 2);

			paraMatrix(0, 1) += weightB * (point[0] - centerX) * (point[1] - centerY);
			paraMatrix(1, 0) += weightB * (point[0] - centerX) * (point[1] - centerY);

			paraMatrix(0, 2) += weightB * (point[0] - centerX) * (point[2] - centerZ);
			paraMatrix(2, 0) += weightB * (point[0] - centerX) * (point[2] - centerZ);

			paraMatrix(1, 2) += weightB * (point[1] - centerY) * (point[2] - centerZ);
			paraMatrix(2, 1) += weightB * (point[1] - centerY) * (point[2] - centerZ);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index maxEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

	int maxEValus = int(maxEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));


	//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
	//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

	Plane3d partitionPlane(Point3d(centerX, centerY, centerZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::OriginSDM);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::OriginSDM);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::ConstructSDM_Only_SizeWeightedSons()
{

	auto fFirst = m_FacesA;
	auto fSecond = m_FacesB;

	double sumXA = 0;
	double sumYA = 0;
	double sumZA = 0;

	double sumXB = 0;
	double sumYB = 0;
	double sumZB = 0;


	for (auto& fa : fFirst) {
		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXA += point[0];
			sumYA += point[1];
			sumZA += point[2];
		}
	}

	double xBarA = sumXA / (fFirst.size() * 3);
	double yBarA = sumYA / (fFirst.size() * 3);
	double zBarA = sumZA / (fFirst.size() * 3);



	//meshB

	for (auto& fb : fSecond) {
		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			sumXB += point[0];
			sumYB += point[1];
			sumZB += point[2];
		}
	}

	double xBarB = sumXB / (fSecond.size() * 3);
	double yBarB = sumYB / (fSecond.size() * 3);
	double zBarB = sumZB / (fSecond.size() * 3);



	//weight
	double numWeightA = (double)fSecond.size() / (double)(fFirst.size() + fSecond.size());
	double numWeightB = (double)fFirst.size() / (double)(fFirst.size() + fSecond.size());

	//
	double lambdaS = 1, lambdaD = 0.5;

	double weightA = pow(numWeightA, lambdaS);
	double weightB = pow(numWeightB, lambdaS);



	double centerX = sumXA * weightA + sumXB * weightB;
	double centerY = sumYA * weightA + sumYB * weightB;
	double centerZ = sumZA * weightA + sumZB * weightB;


	centerX /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerY /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);
	centerZ /= (weightA * fFirst.size() * 3 + weightB * fSecond.size() * 3);



	Eigen::Matrix3d paraMatrix;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			paraMatrix(i, j) = 0;
		}
	}


	//cout << "==========" << endl;
	//cout << paraMatrix<< endl;

	for (auto& fa : fFirst) {

		Triangle3d tri = fa.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += weightA * pow(point[0] - centerX, 2);
			paraMatrix(1, 1) += weightA * pow(point[1] - centerY, 2);
			paraMatrix(2, 2) += weightA * pow(point[2] - centerZ, 2);

			paraMatrix(0, 1) += weightA * (point[0] - centerX) * (point[1] - centerY);
			paraMatrix(1, 0) += weightA * (point[0] - centerX) * (point[1] - centerY);

			paraMatrix(0, 2) += weightA * (point[0] - centerX) * (point[2] - centerZ);
			paraMatrix(2, 0) += weightA * (point[0] - centerX) * (point[2] - centerZ);

			paraMatrix(1, 2) += weightA * (point[1] - centerY) * (point[2] - centerZ);
			paraMatrix(2, 1) += weightA * (point[1] - centerY) * (point[2] - centerZ);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	for (auto& fb : fSecond) {

		Triangle3d tri = fb.m_Triangle;

		for (int i = 0;i < 3;i++) {
			Point3d point = tri.VertexAt(i);

			paraMatrix(0, 0) += weightB * pow(point[0] - centerX, 2);
			paraMatrix(1, 1) += weightB * pow(point[1] - centerY, 2);
			paraMatrix(2, 2) += weightB * pow(point[2] - centerZ, 2);

			paraMatrix(0, 1) += weightB * (point[0] - centerX) * (point[1] - centerY);
			paraMatrix(1, 0) += weightB * (point[0] - centerX) * (point[1] - centerY);

			paraMatrix(0, 2) += weightB * (point[0] - centerX) * (point[2] - centerZ);
			paraMatrix(2, 0) += weightB * (point[0] - centerX) * (point[2] - centerZ);

			paraMatrix(1, 2) += weightB * (point[1] - centerY) * (point[2] - centerZ);
			paraMatrix(2, 1) += weightB * (point[1] - centerY) * (point[2] - centerZ);
		}


	}

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;


	Eigen::EigenSolver<Eigen::MatrixXd> eSolver(paraMatrix);
	Eigen::MatrixXcd cEvecs = eSolver.eigenvectors();
	Eigen::MatrixXcd cEvals = eSolver.eigenvalues();

	Eigen::MatrixXd rEvecs = cEvecs.real();
	Eigen::MatrixXd rEvals = cEvals.real();

	Eigen::MatrixXf::Index maxEigvalusPos;
	rEvals.rowwise().sum().maxCoeff(&maxEigvalusPos);

	int maxEValus = int(maxEigvalusPos);

	//cout << "==========" << endl;
	//cout << paraMatrix << endl;

	Vector3d planeNormal(rEvecs(0, maxEValus), rEvecs(1, maxEValus), rEvecs(2, maxEValus));


	//cout << "normal: " << rEvecs(0, maxEValus) << ", " << rEvecs(1, maxEValus) << ", " << rEvecs(2, maxEValus) << endl;
	//cout << "center: " << centerX << ", " << centerY << ", " << centerZ << endl;

	Plane3d partitionPlane(Point3d(centerX, centerY, centerZ), planeNormal);


	vector<TriangleForTree> rPositive, rNegative;
	GetNegativePositiveLabelWithSplitTriangle(rPositive, rNegative, partitionPlane);


	vector<TriangleForTree> facesAPositive, facesANegative;
	vector<TriangleForTree> facesBPositive, facesBNegative;

	SeparationTrianglesFromDifferentMesh(rPositive, facesAPositive, facesBPositive);
	SeparationTrianglesFromDifferentMesh(rNegative, facesANegative, facesBNegative);

	//建立positive子树
	m_PositiveSon = new BSPTreeNode(facesAPositive, facesBPositive, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_ONLY_SIZEWEIGHT);

	m_NegativeSon = new BSPTreeNode(facesANegative, facesBNegative, m_Tolerance, m_Depth + 1, m_MaxDepth, m_LeafShresholdNum, false, BSPConstructType::SDM_ONLY_SIZEWEIGHT);

	m_IsLeaf = false;

	m_PartitionPlane = partitionPlane;
}

void BSPTreeNode::GetNegativePositiveLabelWithSplitTriangle(vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane)
{
	negativeFaces.clear();
	positiveFaces.clear();

	for (auto& face : m_FacesA) {
		TrianglePlaneIsIntersectInfo info;

		Triangle3d tri = face.m_Triangle;

		bool canJudgeFaceA = true;
		if (IsZero(tri.GetNormal().cross(partitionPlane.GetNormal()))) {
			if (IsPositive((tri.GetCenter() - partitionPlane.GetOrigin()).dot(partitionPlane.GetNormal()))) {
				info.isIntersect = false;
				info.direction = true;
			}
			else {
				info.isIntersect = false;
				info.direction = false;
			}

		}
		else {
			canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
		}


		if (!canJudgeFaceA) {
			//canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
			canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
			positiveFaces.push_back(face);
			negativeFaces.push_back(face);
		}
		else {
			if (info.isIntersect) {
				for (auto posiTri : info.newPositiveTriangles) {

					positiveFaces.push_back({ posiTri, face.m_HoldFace,face.m_holdMesh });
				}

				for (auto negaTri : info.newNegativeTriangles) {

					negativeFaces.push_back({ negaTri, face.m_HoldFace,face.m_holdMesh });
				}

				//negativeFaces.push_back(face);
				//positiveFaces.push_back(face);
			}
			else {
				if (false == info.direction) {
					negativeFaces.push_back(face);
				}
				else {
					positiveFaces.push_back(face);
				}
			}
		
		}


	}

	//AABB aabb1(positiveFaces);
	//AABB aabb2(negativeFaces);

	for (auto& face : m_FacesB) {

		TrianglePlaneIsIntersectInfo info;

		Triangle3d tri = face.m_Triangle;

		bool canJudgeFaceB = true;
		canJudgeFaceB = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);

		if (!canJudgeFaceB) {
			positiveFaces.push_back(face);
			negativeFaces.push_back(face);
		}	
		else {
			if (info.isIntersect) {
				for (auto posiTri : info.newPositiveTriangles) {

					positiveFaces.push_back({ posiTri, face.m_HoldFace,face.m_holdMesh });
				}

				for (auto negaTri : info.newNegativeTriangles) {

					negativeFaces.push_back({ negaTri, face.m_HoldFace,face.m_holdMesh });
				}

			}
			else
			{

				if (false == info.direction) {
					negativeFaces.push_back(face);
				}
				else {
					positiveFaces.push_back(face);
				}
			}
		}


	}
}

void BSPTreeNode::GetNegativePositiveLabelWithOutSplitTriangle(vector<TriangleForTree>& negativeFaces, vector<TriangleForTree>& positiveFaces, Plane3d partitionPlane)
{
	negativeFaces.clear();
	positiveFaces.clear();

	for (auto& face : m_FacesA) {
		TrianglePlaneIsIntersectInfo info;

		Triangle3d tri = face.m_Triangle;

		bool canJudgeFaceA = true;
		if (IsZero(tri.GetNormal().cross(partitionPlane.GetNormal()))) {
			if (IsPositive((tri.GetCenter() - partitionPlane.GetOrigin()).dot(partitionPlane.GetNormal()))) {
				info.isIntersect = false;
				info.direction = true;
			}
			else {
				info.isIntersect = false;
				info.direction = false;
			}

		}
		else {
			canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);
		}


		if (!canJudgeFaceA) {
			//canJudgeFaceA = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);

			positiveFaces.push_back(face);
			negativeFaces.push_back(face);
		}
		else {
			if (info.isIntersect) {

				positiveFaces.push_back(face);
				negativeFaces.push_back(face);

				//negativeFaces.push_back(face);
				//positiveFaces.push_back(face);
			}
			else {
				if (false == info.direction) {
					negativeFaces.push_back(face);
				}
				else {
					positiveFaces.push_back(face);
				}
			}

		}


	}

	//AABB aabb1(positiveFaces);
	//AABB aabb2(negativeFaces);

	for (auto& face : m_FacesB) {

		TrianglePlaneIsIntersectInfo info;

		Triangle3d tri = face.m_Triangle;

		bool canJudgeFaceB = true;
		canJudgeFaceB = TrianglePlaneIsIntersect(tri, partitionPlane, info, m_Tolerance);

		if (!canJudgeFaceB) {

			positiveFaces.push_back(face);
			negativeFaces.push_back(face);
		}
		else {
			if (info.isIntersect) {

				positiveFaces.push_back(face);
				negativeFaces.push_back(face);

			}
			else
			{

				if (false == info.direction) {
					negativeFaces.push_back(face);
				}
				else {
					positiveFaces.push_back(face);
				}
			}
		}


	}
}


void BSPTreeNode::SeparationTrianglesFromDifferentMesh(vector<TriangleForTree> facesOrigin, vector<TriangleForTree>& facesA, vector<TriangleForTree>& facesB)
{
	assert(!m_FacesA.empty());
	assert(!m_FacesB.empty());

	Mesh* meshA = m_FacesA.front().m_holdMesh;
	Mesh* meshB = m_FacesB.front().m_holdMesh;

	for (auto& face : facesOrigin) {

		if (face.m_holdMesh == meshA) {
			facesA.push_back(face);
		}
		else if (face.m_holdMesh == meshB) {
			facesB.push_back(face);
		}
		else {
			assert(false);
		}
	}


}

pair<int, int> BSPTreeNode::GetRealSize()
{
	unordered_set<FaceId> fa, fb;

	for (auto& f : m_FacesA) {
		fa.insert(f.m_HoldFace.idx());
	}

	for (auto& f : m_FacesB) {
		fb.insert(f.m_HoldFace.idx());
	}

	return pair<int, int>(fa.size(),fb.size());
}
