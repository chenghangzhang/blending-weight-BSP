#include "mPch.h"
#include "LineSegmentIntersect.h"




	IntersectLinePlaneResult::IntersectLinePlaneResult(IntersectResult::Type p_Type) :
		IntersectResult(p_Type),
		m_Intersect(0),
		m_ParamOnLine(0),
		m_ParamOnPlane(0)
	{

	}

	IntersectLinePlaneResult::IntersectLinePlaneResult(IntersectResult::Type p_Type, const Point3d& p_Intersect, const double& p_ParamOnLine, const Parameter2d& p_ParamOnPlane) :
		IntersectResult(p_Type),
		m_Intersect(p_Intersect),
		m_ParamOnLine(p_ParamOnLine),
		m_ParamOnPlane(p_ParamOnPlane)
	{
		//assert(true == IsIntersect());
	}


	bool IntersectPointLineSegment(LineSegment3d& p_LineSeg, const Point3d& p_Point, const Tolerance& p_Tolerance)
	{
		Point3d& beginP = p_LineSeg.GetBegin();
		Point3d& beginE = p_LineSeg.GetEnd();

		if (IsZero( (beginP - p_Point).length())  || IsZero((beginE - p_Point).length())) return true;

		Vector3d v1 = beginE - beginP;
		Vector3d v2 = p_Point - beginP;
		Vector3d cr = v1.cross(v2);
		
		return IsZero(cr.length());
	}

	bool ColineLineSegment(LineSegment3d& p_LineSeg, LineSegment3d p_LineSeg2, const Tolerance& p_Tolerance)
	{
		return  IntersectPointLineSegment(p_LineSeg, p_LineSeg2.GetBegin()) && IntersectPointLineSegment(p_LineSeg, p_LineSeg2.GetEnd());
	}

	bool ColineLineSegmentS(vector<LineSegment3d>& p_LineSegS, LineSegment3d p_LineSeg2, const Tolerance& p_Tolerance)
	{
		bool isColin = false;
		for (auto& lin : p_LineSegS) {
			isColin = ColineLineSegment(lin, p_LineSeg2);			
		}


		return isColin;
	}

	/// <summary>
	/// /////////////////////
	/// </summary>
	/// <param name="p_Line"></param>
	/// <param name="p_Plane"></param>
	/// <param name="p_Tolerance"></param>
	/// <returns></returns>
	/// 
	IntersectLinePlaneResult IntersectLinePlane(const Line3d& p_Line, const Plane3d& p_Plane, const Tolerance& p_Tolerance/*= Tolerance()*/)
	{
		return IntersectPlaneLine(p_Plane, p_Line, p_Tolerance);
	}

	IntersectLinePlaneResult IntersectPlaneLine(const Plane3d& p_Plane, const Line3d& p_Line, const Tolerance& p_Tolerance/*= Tolerance()*/)
	{
		const Point3d point = p_Line.GetOrigin();
		const Vector3d normal = p_Plane.GetNormal();

		if (true == IsPerpendicular(normal, p_Line.GetDirection(), p_Tolerance.Linear()))//若平行，法向量和直线方向向量点积为0
		{
			if (true == IsOn(point, p_Plane, p_Tolerance)) //若线在面上，线上的任意一点在面上；
			{													   //面上任意一条直线与法向量垂直，所以单独列出
				return IntersectLinePlaneResult(IntersectResult::Type::Overlap);
			}

			return IntersectLinePlaneResult(IntersectResult::Type::None);
		}
		else
		{
			const auto& planeNormal = p_Plane.GetNormal();
			const auto& planeOrigin = p_Plane.GetOrigin();
			const auto& lineOrigin = p_Line.GetOrigin();
			const auto& lineDirection = p_Line.GetDirection();
			
			//检验这个公式是否正确 //it's right
			double p = (planeNormal | (planeOrigin - lineOrigin)) / (planeNormal | lineDirection);
			Point3d intersect = p_Line.CalculateValue(p);

			Vector3d intersectVector = intersect - planeOrigin; //交点向量

			double u = intersectVector | p_Plane.GetDirectionU();
			double v = intersectVector | p_Plane.GetDirectionV();

			return IntersectLinePlaneResult(IntersectResult::Type::Regular, intersect, p, Parameter2d(u, v));
		}

		assert(false);
		return IntersectLinePlaneResult(IntersectResult::Type::None);
	}


	/// <summary>
	/// ///////////////////////////////////
	/// </summary>
	/// <param name="p_Type"></param>
	/// 
	IntersectLineSegmentPlaneResult::IntersectLineSegmentPlaneResult(IntersectResult::Type p_Type) :
		IntersectLinePlaneResult(p_Type, Point3d(0), 0, Parameter2d(0,0)),
		m_Intersect(Point3d(0)),
		m_ParamOnLine(0),
		m_ParamOnPlane(Parameter2d(0))
	{

	}

	IntersectLineSegmentPlaneResult::IntersectLineSegmentPlaneResult(IntersectResult::Type p_Type, const Point3d& p_Intersect, const double& p_ParamOnLineSegment, const Parameter2d& p_ParamOnPlane):
		IntersectLinePlaneResult(p_Type, p_Intersect, p_ParamOnLineSegment, p_ParamOnPlane),
		m_Intersect(p_Intersect),
		m_ParamOnLine(p_ParamOnLineSegment),
		m_ParamOnPlane(p_ParamOnPlane)
	{

	}


	IntersectLineSegmentPlaneResult IntersectLineSegmentPlane(const LineSegment3d& p_LineSegment, const Triangle3d& p_Plane, const Tolerance& p_Tolerance /*= Tolerance()*/)
	{
		return IntersectPlaneLineSegment(p_Plane, p_LineSegment, p_Tolerance);
	}

	IntersectLineSegmentPlaneResult IntersectPlaneLineSegment(const Triangle3d& p_Plane, const LineSegment3d& p_LineSegment, const Tolerance& p_Tolerance /*= Tolerance()*/)
	{

		const Vector3d& planeNormal = p_Plane.GetNormal();

		if (true == IsPerpendicular(planeNormal, p_LineSegment.GetDirection()))//若平行，法向量和直线方向向量点积为0
		{
			if (true == IsOn(p_LineSegment.GetOrigin(), p_Plane, p_Tolerance)) //若线在面上，线上的任意一点在面上；
			{
				return IntersectLineSegmentPlaneResult(IntersectResult::Type::Overlap);
			}

			return IntersectLineSegmentPlaneResult(IntersectResult::Type::None);
		}
		else
		{
			const auto& planeOrigin = p_Plane.GetCenter();
			const auto& lineSegmentOrigin = p_LineSegment.GetOrigin();
			const auto& lineSegmentDirection = p_LineSegment.GetDirection();

			double pteset = (planeNormal | lineSegmentDirection);
			double p = (planeNormal | (planeOrigin - lineSegmentOrigin)) / (planeNormal | lineSegmentDirection);

			if (IsBetween(p, 0.0, p_LineSegment.GetLength(), p_Tolerance.m_Linear))
			{
				Point3d intersect = p_LineSegment.CalculateValue(p);

				Vector3d intersectVector = Vector3d(intersect - planeOrigin); //交点向量

				auto& v0 = p_Plane.VertexAt(0);
				const Vector3d& v1 = p_Plane.GetNormal();

				Vector3d Ux(v0 - planeOrigin);
				Ux.normalize();
				
				Vector3d Uy = Ux.cross(v1);
				Uy.normalize();


				double u = intersectVector | Ux;
				double v = intersectVector | Uy;

				return IntersectLineSegmentPlaneResult(IntersectResult::Type::Regular, intersect, p, Parameter2d(u, v));;
			}

			return IntersectLineSegmentPlaneResult(IntersectResult::Type::None);
		}

		assert(false);
		return IntersectLineSegmentPlaneResult(IntersectResult::Type::None);
	}

	//IntersectLineSegmentPlaneResult IntersectPlaneLineSegment(pair<Mesh&,MeshTriangle&> p_PlaneInfo, pair<Mesh&, MeshEdge&> p_LineSegmentInfo, const Tolerance& p_Tolerance)
	//{
	//	p_PlaneInfo.first.handle(p_PlaneInfo.second);

	//	for(Mesh::FaceVertexIter points = )

	//	return IntersectPlaneLineSegment(const MeshTriangle & p_Plane, const MeshEdge & p_LineSegment, const Tolerance & p_Tolerance);
	//}





