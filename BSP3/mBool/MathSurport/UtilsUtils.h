#pragma once

//extern bool IsOnTriangle(const Point3d& p_Point, const Triangle3d& p_Plane, const Tolerance& p_Tolerance);
extern bool IsOn(const Point3d& p_Point, const Line3d& p_Line);
extern bool IsOn(const Point3d& p_Point, const Plane3d& p_Plane, const Tolerance& p_Tolerance);

extern	bool IsPerpendicular(const Vector3d& p_VectorA, const Vector3d& p_VectorB, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsParallel(const Vector3d& p_VectorA, const Vector3d& p_VectorB, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsZero(double p_Value, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsZero(const Vector3d p_Vec, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsEqual(double p_Value1, double p_Value2, double p_Tolerance);
extern	bool IsBetween(double p_Value, double p_Lower, double p_Upper, double p_Tolerance);

extern	bool IsNegative(double p_Value, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsPositive(double p_Value, double p_Tolerance = Tolerance::GetTolerance().Linear());
extern	bool IsPositiveNotStrict(double p_Value, double p_Tolerance = Tolerance::GetTolerance().Linear());

extern	void MeshNormalize(Mesh& p_Mesh, double scale = 1);
extern	void MeshTransform(Mesh& p_Mesh, Vector3d p_Direction);

extern  Mesh MeshRotate(Mesh p_Mesh, Point3d p_center, Vector3d rotateAxis, float angle);

extern array<Point3d,4> GetOffSectPoint(Point3d& pOrigin, Vector3d& pNormal, double scale);

void getFileNames(string path, vector<string>& files);

bool testValidTriangle(vector<Point3d> ps);

struct TrianglePlaneIsIntersectInfo
{
	enum class Type
	{
		None,
		Overlap,
		Regular
	};

	//是否相交
	bool isIntersect;

	//如果不相交，三角形在平面的哪一侧
	bool direction;

	//如果相交,返回基于交线分割后的三角形
	vector<Triangle3d> newPositiveTriangles;
	vector<Triangle3d> newNegativeTriangles;
};

struct EdgePlaneIsIntersectInfo
{
	enum class Type
	{
		None,
		OverLap,
		OnTheBegin,
		OnTheEnd,
		Regular
	};

	//是否相交
	bool isIntersect;

	//如果相交,返回交点
	Point3d intersectPoint;

	Type interType;
};

bool TriangleTriangleIsIntersect(Triangle3d& p_Tri1, Triangle3d& p_Tri2, Tolerance p_Tolerance = Tolerance::GetTolerance());
bool EdgeTriangleIsIntersect(LineSegment3d& p_Line1, Triangle3d& p_Tri2, Tolerance p_Tolerance = Tolerance::GetTolerance());



extern	bool TrianglePlaneIsIntersect(Triangle3d& p_Tri, const Plane3d& p_Plane, TrianglePlaneIsIntersectInfo& info, Tolerance p_Tolerance = Tolerance::GetTolerance());
extern	bool EdgePlaneIsIntersect(pair<Point3d,Point3d> p_Edge, const Plane3d& p_Plane, EdgePlaneIsIntersectInfo& info, Tolerance p_Tolerance = Tolerance::GetTolerance());





