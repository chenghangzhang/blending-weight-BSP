#pragma once


	struct Point3dTriangle3dRelation
	{
	public:
		enum class Type : unsigned char
		{
			Out = 0,		//0x00000000
			OnVertex = 3,	//0x00000011
			OnEdge = 5,		//0x00000101
			In = 9			//0x00001001
		};

	public:
		Tolerance m_Tolerance;
		Type m_Relation;
		size_t m_OnElmentIndex;

		pair<double, double> m_UVPara;
	public:
		Point3dTriangle3dRelation();
		~Point3dTriangle3dRelation() {}
	};


	Point3dTriangle3dRelation DetectPoint3dTriangle3dRelation(const Point3d& p_Point, const Triangle3d& p_Triangle, const Tolerance& p_Tolerance = Tolerance::GetTolerance());
	Point3dTriangle3dRelation DetectCoplanarPoint3dTriangle3dRelation(const Point3d& p_Point, const Triangle3d& p_Triangle, const Tolerance& p_Tolerance = Tolerance::GetTolerance());



