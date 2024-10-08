#pragma once


	
	class IntersectResult
	{
	public:
		enum class Type : unsigned char
		{
			None = 0,		//00000000
			Overlap = 1,	//00000001
			Regular = 3		//00000011
		};

	protected:
		const Type m_Type;

	public:
		IntersectResult(Type p_Type) :
			m_Type(p_Type) {}

	public:
		bool IsType(const Type& p_Type) const { return p_Type == m_Type; }
		bool IsNotType(const Type& p_Type) const { return p_Type != m_Type; }
		bool IsIntersect() const { return Type::None != m_Type; }
	};

	bool IntersectPointLineSegment(LineSegment3d& p_LineSeg, const Point3d& p_Point, const Tolerance& p_Tolerance = Tolerance());
	bool ColineLineSegment(LineSegment3d& p_LineSeg, LineSegment3d p_LineSeg2, const Tolerance& p_Tolerance = Tolerance());
	bool ColineLineSegmentS(vector<LineSegment3d>& p_LineSegS, LineSegment3d p_LineSeg2, const Tolerance& p_Tolerance = Tolerance());


	class IntersectLinePlaneResult : public IntersectResult
	{
	public:
		const Point3d m_Intersect;
		const double m_ParamOnLine;
		const Parameter2d m_ParamOnPlane;

	public:
		IntersectLinePlaneResult(IntersectResult::Type p_Type);
		IntersectLinePlaneResult(IntersectResult::Type p_Type, const Point3d& p_Intersect, const double& p_ParamOnLine, const Parameter2d& p_ParamOnPlane);
	};
	
	IntersectLinePlaneResult IntersectLinePlane(const Line3d& p_Line, const Plane3d& p_Plane, const Tolerance& p_Tolerance = Tolerance());
	IntersectLinePlaneResult IntersectPlaneLine(const Plane3d& p_Plane, const Line3d& p_Line, const Tolerance& p_Tolerance = Tolerance());
	

	class IntersectLineSegmentPlaneResult : public IntersectLinePlaneResult
	{
	public:
		const Point3d m_Intersect;
		const double m_ParamOnLine;
		const Parameter2d m_ParamOnPlane;

	public:
		IntersectLineSegmentPlaneResult(IntersectResult::Type p_Type);
		IntersectLineSegmentPlaneResult(IntersectResult::Type p_Type, const Point3d& p_Intersect, const double& p_ParamOnLineSegment, const Parameter2d& p_ParamOnPlane);
	};
	
	IntersectLineSegmentPlaneResult IntersectLineSegmentPlane(const LineSegment3d& p_LineSegment, const Triangle3d& p_Plane, const Tolerance& p_Tolerance = Tolerance());
	IntersectLineSegmentPlaneResult IntersectPlaneLineSegment(const Triangle3d& p_Plane, const LineSegment3d& p_LineSegment, const Tolerance& p_Tolerance = Tolerance());

	//IntersectLineSegmentPlaneResult IntersectPlaneLineSegment(pair<Mesh&, MeshTriangle&> p_PlaneInfo, pair<Mesh&, MeshEdge&> p_LineSegmentInfo, const Tolerance& p_Tolerance);


