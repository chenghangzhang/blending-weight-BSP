#include "mPch.h"
#include "Point3dTriangle3dRelation.h"

//double a1, a2, a3, a4, a5, a6, a7, a8, a9,a10,a11,a12;



	Point3dTriangle3dRelation::Point3dTriangle3dRelation() :
		m_Tolerance(),
		m_Relation(Type::Out),
		m_OnElmentIndex(0)
	{
		m_UVPara = make_pair(0.0, 0.0);
	}

	Point3dTriangle3dRelation DetectPoint3dTriangle3dRelation(const Point3d& p_Point, const Triangle3d& p_Triangle, const Tolerance& p_Tolerance /*= DEFALUT_TOLERANCE*/)
	{
		//首先判断点是否与面片共面
		if (false == IsOn(p_Point, p_Triangle, p_Tolerance))
		{
			Point3dTriangle3dRelation r_Realtion;
			r_Realtion.m_Relation = Point3dTriangle3dRelation::Type::Out;
			return r_Realtion;
		}

		return DetectCoplanarPoint3dTriangle3dRelation(p_Point, p_Triangle, p_Tolerance);
	}

	Point3dTriangle3dRelation DetectCoplanarPoint3dTriangle3dRelation(const Point3d& p_Point, const Triangle3d& p_Triangle, const Tolerance& p_Tolerance /*= DEFALUT_TOLERANCE*/)
	{
		//assert(true == IsOn(p_Point, p_Triangle, p_Tolerance));
		if (!IsOn(p_Point, p_Triangle, p_Tolerance)) {
			IsOn(p_Point, p_Triangle, p_Tolerance);
		}

		//暂时这里先不重构退化问题的代码，看是否之前判断杜绝出现退化的情况。

		Point3dTriangle3dRelation r_Relation;
		r_Relation.m_Tolerance = p_Tolerance;

		const Point3d& a = p_Triangle.VertexAt(0);
		const Point3d& b = p_Triangle.VertexAt(1);
		const Point3d& c = p_Triangle.VertexAt(2);

		Vector3d vab(b - a);
		Vector3d vbc(c - b);
		Vector3d vca(a - c);

		Vector3d vap(p_Point - a);
		Vector3d vbp(p_Point - b);
		Vector3d vcp(p_Point - c);
		



		//Vector3d cross1 = vab.cross(vap);
		//Vector3d cross2 = vbc.cross(vbp);
		//Vector3d cross3 = vca.cross(vcp);


		if (IsZero(vap, p_Tolerance.Linear())) {
			//点在a上
			r_Relation.m_OnElmentIndex = 0;
			r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			return r_Relation;		
		}
		if (IsZero(vbp, p_Tolerance.Linear())) {
			//点在b上
			r_Relation.m_OnElmentIndex = 1;
			r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			return r_Relation;
		}
		if (IsZero(vcp, p_Tolerance.Linear())) {
			//点在c上
			r_Relation.m_OnElmentIndex = 2;
			r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			return r_Relation;
		}

		//cout<<"不重合于点"<<endl;


		//a1 = vap.length();
		//a2 = vbp.length();
		//a3 = vcp.length();
		//a4 = cross1.length();
		//a5 = cross2.length();
		//a6 = cross3.length();
		//a7 = cross1.dot(cross2);
		//a8 = cross2.dot(cross3);

		//a9 = vab.length();
		//a10 = vbc.length();
		//a11 = vca.length();

		
		//cout << "==================" << endl;
		//cout << "vap " << vap.length() << endl;
		//cout << "vbp " << vbp.length() << endl;
		//cout << "vcp " << vcp.length() << endl;
		//cout << "abcross1 " << cross1.length() << endl;
		//cout << "bccross1 " << cross2.length() << endl;
		//cout << "cacross1 " << cross3.length() << endl;
		//cout << "crossTest " << cross1.dot(cross2) << endl;
		//cout << "crossTest " << cross2.dot(cross3) << endl;
		//cout << "==================" << endl;
		

		Vector3d vabNormalized = vab.normalized();
		Vector3d vbcNormalized = vbc.normalized();
		Vector3d vcaNormalized = vca.normalized();

		Vector3d vapNormalized = vap.normalized();
		Vector3d vbpNormalized = vbp.normalized();
		Vector3d vcpNormalized = vcp.normalized();

		Vector3d cross1 = vabNormalized.cross(vapNormalized);
		Vector3d cross2 = vbcNormalized.cross(vbpNormalized);
		Vector3d cross3 = vcaNormalized.cross(vcpNormalized);


		//ab,ap共线
		if (IsZero(cross1, p_Tolerance.Linear())) {

			double project = vab.dot(vap);
		
			//cout << "重合于边ab" << endl;
			//cout << "project: " << project << endl;
			//cout << "edgeLengthSquare: " << vab.length() * vab.length() << endl;

			//cout << "betweenTest " << project<<"   "<< vab.length() * vab.length() << endl;
		
			//if (IsZero(project)) {
			//	assert(false);
			//	//点在a上
			//	r_Relation.m_OnElmentIndex = 0;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			//else if ( IsEqual(project, vab.length() * vab.length(), p_Tolerance.Linear()) ) {
			//	assert(false);
			//	//点在b上
			//	r_Relation.m_OnElmentIndex = 1;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			//else if (IsBetween(project, 0, vab.length() * vab.length(), p_Tolerance.Linear())) {
			if (IsBetween(project, 0, vab.length() * vab.length(), p_Tolerance.Linear())) {
				//点在ab上
					
				//cout << "On ab" << endl;
				r_Relation.m_OnElmentIndex = 0;
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
				return r_Relation;
				
			}
			else {
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
				return r_Relation;
			}	
			
		}

		//bc,bp共线
		if (IsZero(cross2, p_Tolerance.Linear())) {

			double project = vbc.dot(vbp);

			//cout << "重合于边bc" << endl;
			//cout << "project: " << project << endl;
			//cout << "edgeLengthSquare: " << vbc.length() * vbc.length() << endl;



			//cout << "betweenTest " << project << "   " << vbc.length() * vbc.length() << endl;
			
			//if (IsZero(project)) {
			//	assert(false);
			//	//点在b上
			//	r_Relation.m_OnElmentIndex = 1;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			////else if (IsEqual(project, vbc.length() * vbc.length(), p_Tolerance.Linear())) {
			//else if (IsBetween(project, 0, vbc.length() * vbc.length(), p_Tolerance.Linear())) {
			//	assert(false);
			//	//点在c上
			//	r_Relation.m_OnElmentIndex = 2;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			//else if (IsBetween(project, 0, vbc.length() * vbc.length(), p_Tolerance.Linear())) {
			if (IsBetween(project, 0, vbc.length() * vbc.length(), p_Tolerance.Linear())) {
				//点在bc上
				
				//cout << "On bc" << endl;
				r_Relation.m_OnElmentIndex = 1;
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
				return r_Relation;

			}
			else {
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
				return r_Relation;
			}

		}


		//ca,cp共线
		if (IsZero(cross3, p_Tolerance.Linear())) {

			double project = vca.dot(vcp);

			//cout << "重合于边ac" << endl;
			//cout << "project: " << project << endl;
			//cout << "edgeLengthSquare: " << vca.length() * vca.length() << endl;


			//cout << "betweenTest " << project << "   " << vca.length() * vca.length() << endl;
			
			//if (IsZero(project)) {
			//	assert(false);
			//	//点在c上
			//	r_Relation.m_OnElmentIndex = 2;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			//else if (IsEqual(project, vca.length() * vca.length(), p_Tolerance.Linear())) {
			//	assert(false);
			//	//点在a上
			//	r_Relation.m_OnElmentIndex = 0;
			//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
			//	return r_Relation;
			//}
			//else if (IsBetween(project, 0, vca.length() * vca.length(), p_Tolerance.Linear())) {
			if (IsBetween(project, 0, vca.length() * vca.length(), p_Tolerance.Linear())) {
				//点在ca上
				
				//cout << "On ca" << endl;
				
				r_Relation.m_OnElmentIndex = 2;
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
				return r_Relation;

			}
			else {
				r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
				return r_Relation;
			}

		}


		//cout << "不重合于边和点" << endl;
		//cout << "==================" << endl;

		//cout << "vab " << vab.length() << endl;
		//cout << "vbc " << vbc.length() << endl;
		//cout << "vca " << vca.length() << endl;
		//cout << "vap " << vap.length() << endl;
		//cout << "vbp " << vbp.length() << endl;
		//cout << "vcp " << vcp.length() << endl;
		//cout << "abcross1 " << cross1.length() << endl;
		//cout << "bccross1 " << cross2.length() << endl;
		//cout << "cacross1 " << cross3.length() << endl;
		//cout << "crossTest " << cross1.dot(cross2) << endl;
		//cout << "crossTest " << cross2.dot(cross3) << endl;
		//cout << "==================" << endl;

		if ((cross1.dot(cross2) > 0) && (cross2.dot(cross3) > 0)) {
			//cout << "IN" << endl;
			r_Relation.m_Relation = Point3dTriangle3dRelation::Type::In;
			return r_Relation;
		}
		else {
			//cout << "OUT" << endl;
			r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
			return r_Relation;
		}


		////假设A、B、C分别为三角面片的三个顶点
		////v0 = B - A, v1 = C - A, v2 = P - A;
		////v2 可以用v0，v1作为基底表示
		//const Point3d& a = p_Triangle.VertexAt(0);
		//const Point3d& b = p_Triangle.VertexAt(1);
		//const Point3d& c = p_Triangle.VertexAt(2);

		//Vector3d v0 = c - a;
		//Vector3d v1 = b - a;
		//Vector3d v2 = p_Point - a;
		//
		//double dot00 = v0 | v0;
		//double dot01 = v0 | v1;
		//double dot02 = v0 | v2;
		//double dot11 = v1 | v1;
		//double dot12 = v1 | v2;
		//
		//double u, v; //v2 = A + u * v0 + v * v1

		//double denom = dot00 * dot11 - dot01 * dot01;

		//double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		//u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		//v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		//bool uIsZero = IsZero(u, p_Tolerance.m_Squared);
		//bool uIsOne = IsZero(u - 1.0, p_Tolerance.m_Squared);

		//bool vIsZero = IsZero(v, p_Tolerance.m_Squared);
		//bool vIsOne = IsZero(v - 1.0, p_Tolerance.m_Squared);

		//Point3dTriangle3dRelation r_Relation;
		//r_Relation.m_Tolerance = p_Tolerance;
		//r_Relation.m_UVPara = make_pair(u, v);

		////cout << "u: " << u << endl;
		////cout << "v: " << v << endl;

		////三角形形成的角外部
		//if (u < -p_Tolerance.m_Squared || v < -p_Tolerance.m_Squared)
		//{
		//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
		//	return r_Relation;
		//}



		////on vertex
		//{
		//	if (uIsZero && vIsOne)//u = 0, v = 1, P on B
		//	{
		//		r_Relation.m_OnElmentIndex = 1;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
		//		return r_Relation;
		//	}

		//	if (uIsOne && vIsZero)//u = 1, v = 0, P on C
		//	{
		//		r_Relation.m_OnElmentIndex = 2;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
		//		return r_Relation;
		//	}

		//	if (uIsZero && vIsZero)//u = 0, v = 0, P on A
		//	{
		//		r_Relation.m_OnElmentIndex = 0;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnVertex;
		//		return r_Relation;
		//	}
		//}

		////on edge
		//{
		//	if (uIsZero && v > -p_Tolerance.m_Squared && u + v < 1+ p_Tolerance.m_Squared) //u = 0, u + v < 1, P on AB
		//	{
		//		r_Relation.m_OnElmentIndex = 0;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
		//		return r_Relation;
		//	}

		//	if (vIsZero && u > -p_Tolerance.m_Squared && u + v < 1+ p_Tolerance.m_Squared) //v = 0, u + v < 1, P on AC
		//	{
		//		r_Relation.m_OnElmentIndex = 2;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
		//		return r_Relation;
		//	}

		//	if (u > -p_Tolerance.m_Squared && v > -p_Tolerance.m_Squared && IsEqual(u + v, 1, p_Tolerance.m_Squared)) //u,v > 0, u + v = 1, P on BC
		//	{
		//		r_Relation.m_OnElmentIndex = 1;
		//		r_Relation.m_Relation = Point3dTriangle3dRelation::Type::OnEdge;
		//		return r_Relation;
		//	}
		//}

		////in facet
		//if (u > -p_Tolerance.m_Squared && v > -p_Tolerance.m_Squared && u + v < 1 + p_Tolerance.m_Squared)
		//{
		//	r_Relation.m_Relation = Point3dTriangle3dRelation::Type::In;
		//	return r_Relation;
		//}

		////assert(false);
		//r_Relation.m_Relation = Point3dTriangle3dRelation::Type::Out;
		//return r_Relation;

	}



	//int EdgeTriangle(const MeshEdge& p_MeshEdge, const MeshTriangle& p_Triangle, const Tolerance& p_Tolerance /*= Tolerance()*/)
	//{
	//	assert(true == IsOn(p_Vertex, (Plane)p_Triangle));
	//	暂时这里先不重构退化问题的代码，看是否之前判断杜绝出现退化的情况。
	//	unsigned int r_OnElementIndex = Math::UNINT_MAX;

	//	const Point* a = &p_Triangle[0];
	//	const Point* b = &p_Triangle[1];
	//	const Point* c = &p_Triangle[2];

	//	const VertexBase* edgeA = &p_MeshEdge[0];
	//	const VertexBase* edgeB = &p_MeshEdge[1];

	//	if (edgeA == a) {
	//		if (edgeB == b) {
	//			Edge是AB
	//			r_OnElementIndex = 0;
	//		}
	//		else if (edgeB == c) {
	//			Edge是Ac
	//			r_OnElementIndex = 2;
	//		}
	//		else {
	//			assert(false);
	//		}

	//	}
	//	else if (edgeA == b) {
	//		if (edgeB == a) {
	//			Edge是AB
	//			r_OnElementIndex = 0;
	//		}
	//		else if (edgeB == c) {
	//			Edge是BC
	//			r_OnElementIndex = 1;
	//		}
	//		else {
	//			assert(false);
	//		}


	//	}
	//	else if (edgeA == c) {
	//		if (edgeB == a) {
	//			Edge是AC
	//			r_OnElementIndex = 2;
	//		}
	//		else if (edgeB == b) {
	//			Edge是BC
	//			r_OnElementIndex = 1;
	//		}
	//		else {
	//			assert(false);
	//		}


	//	}
	//	else {
	//		assert(false);
	//	}

	//	return r_OnElementIndex;
	//}

	//bool EdgeIntersect(const Point& p_Begin, const Point& p_End, const Point& p_Edge2Begin, const Point& p_Edge2End)
	//{
	//	Tolerance toler;

	//	cout << "EdgeIntersect" << endl;
	//	cout << p_Begin.X << " " << p_Begin.Y << " " << p_Begin.Z << endl;
	//	cout << p_End.X << " " << p_End.Y << " " << p_End.Z << endl;
	//	cout << p_Edge2Begin.X << " " << p_Edge2Begin.Y << " " << p_Edge2Begin.Z << endl;
	//	cout << p_Edge2End.X << " " << p_Edge2End.Y << " " << p_Edge2End.Z << endl;

	//	Vector v1 = Vector(p_End - p_Begin);
	//	Vector v2 = Vector(p_Edge2End - p_Begin);
	//	Vector v3 = Vector(p_Edge2Begin - p_Begin);

	//	if (v1.GetLengh() < toler.m_Squared) return false;

	//	Vector crossVect1 = v1.Cross(v2);
	//	Vector crossVect2 = v1.Cross(v3);
	//	bool intersectFirst = ((crossVect1).Dot(crossVect2) < toler.m_Squared);

	//	Vector v4 = Vector(p_Edge2End - p_Edge2Begin);
	//	Vector v5 = Vector(p_Begin - p_Edge2Begin);
	//	Vector v6 = Vector(p_End - p_Edge2Begin);

	//	Vector crossVect3 = v4.Cross(v5);
	//	Vector crossVect4 = v4.Cross(v6);

	//	bool intersectSecond = ((crossVect3).Dot(crossVect4) < toler.m_Squared);

	//	if (crossVect1.GetLengh() <= toler.m_Squared || crossVect2.GetLengh() <= toler.m_Squared ||
	//		crossVect3.GetLengh() <= toler.m_Squared || crossVect4.GetLengh() <= toler.m_Squared) {
	//		cout << "GetLength" << endl;
	//		cout << crossVect1.GetLengh() << endl;
	//		cout << crossVect2.GetLengh() << endl;
	//		cout << crossVect3.GetLengh() << endl;
	//		cout << crossVect4.GetLengh() << endl;
	//		return false;
	//	}

	//	return intersectFirst && intersectSecond;
	//}



