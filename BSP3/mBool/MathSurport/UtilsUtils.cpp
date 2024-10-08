#include "mPch.h"
#include "UtilsUtils.h"



	bool IsPerpendicular(const Vector3d& p_VectorA, const Vector3d& p_VectorB, double p_Tolerance /*= GetTolerance().Linear()*/)
	{
		//double te = fabs(p_VectorA.dot(p_VectorB));

		if (p_Tolerance > fabs(p_VectorA.dot(p_VectorB))) //向量点积为0即垂直
		{
			return true;
		}

		return false;
	}

	bool IsParallel(const Vector3d& p_VectorA, const Vector3d& p_VectorB, double p_Tolerance /*= GetTolerance().Linear()*/) {
		assert(!IsZero(p_VectorA));
		assert(!IsZero(p_VectorB));

		Vector3d ans = p_VectorA.cross(p_VectorB);

		return IsZero(ans);
	}

	bool IsZero(double p_Value, double p_Tolerance /*= GetTolerance().Linear()*/)
	{
		assert(p_Tolerance > 0);
		return fabs(p_Value) <= p_Tolerance;
	}


	bool IsZero(const Vector3d p_Vec, double p_Tolerance /*= GetTolerance().Linear()*/)
	{
		return IsZero(p_Vec.length(), p_Tolerance);
	}

	bool IsEqual(double p_Value1, double p_Value2, double p_Tolerance)
	{
		return IsZero(p_Value1 - p_Value2, p_Tolerance);
	}

	bool IsBetween(double p_Value, double p_Lower, double p_Upper, double p_Tolerance)
	{
		assert(p_Upper != p_Lower);
		assert(p_Lower < p_Upper);
		assert(p_Tolerance > 0);

		if (p_Value < (p_Lower - p_Tolerance))
		{
			return false;
		}

		if (p_Value > (p_Upper + p_Tolerance))
		{
			return false;
		}

		return true;

		//if (p_Value > (p_Lower + p_Tolerance))
		//{
		//	return true;
		//}

		//if (p_Value < (p_Upper - p_Tolerance))
		//{
		//	return true;
		//}

		//return false;
	}

	bool IsNegative(double p_Value, double p_Tolerance)
	{
		if (p_Value + p_Tolerance < 0) return true;

		return false;
	}

	bool IsPositive(double p_Value, double p_Tolerance)
	{
		if (p_Value - p_Tolerance > 0) return true;

		return false;
	}

	bool IsPositiveNotStrict(double p_Value, double p_Tolerance)
	{
		if (p_Value + p_Tolerance > 0) return true;

		return false;
	}

	void MeshNormalize(Mesh& p_Mesh, double scale /*= 1*/ )
	{
		double xc = 0;
		double yc = 0;
		double zc = 0;

		double xmin, xmax, ymin, ymax, zmin, zmax;
		xmin = p_Mesh.point(*p_Mesh.vertices_begin())[0];
		xmax = p_Mesh.point(*p_Mesh.vertices_begin())[0];

		ymin = p_Mesh.point(*p_Mesh.vertices_begin())[1];
		ymax = p_Mesh.point(*p_Mesh.vertices_begin())[1];

		zmin = p_Mesh.point(*p_Mesh.vertices_begin())[2];
		zmax = p_Mesh.point(*p_Mesh.vertices_begin())[2];

		for (Mesh::VertexIter vi = p_Mesh.vertices_begin(); vi != p_Mesh.vertices_end(); ++vi) {
			xc += p_Mesh.point(*vi)[0];
			yc += p_Mesh.point(*vi)[1];
			zc += p_Mesh.point(*vi)[2];

			if (xmin > p_Mesh.point(*vi)[0]) xmin = p_Mesh.point(*vi)[0];
			if (ymin > p_Mesh.point(*vi)[1]) ymin = p_Mesh.point(*vi)[1];
			if (zmin > p_Mesh.point(*vi)[2]) zmin = p_Mesh.point(*vi)[2];

			if (xmax < p_Mesh.point(*vi)[0]) xmax = p_Mesh.point(*vi)[0];
			if (ymax < p_Mesh.point(*vi)[1]) ymax = p_Mesh.point(*vi)[1];
			if (zmax < p_Mesh.point(*vi)[2]) zmax = p_Mesh.point(*vi)[2];
		}

		xc /= p_Mesh.n_vertices();
		yc /= p_Mesh.n_vertices();
		zc /= p_Mesh.n_vertices();

		for (Mesh::VertexIter vi = p_Mesh.vertices_begin(); vi != p_Mesh.vertices_end(); ++vi) {
			p_Mesh.point(*vi)[0] -= xc;
			p_Mesh.point(*vi)[1] -= yc;
			p_Mesh.point(*vi)[2] -= zc;
		}

		double xScale = (xmax - xmin) * 0.5 * scale;
		double yScale = (ymax - ymin) * 0.5 * scale;
		double zScale = (zmax - zmin) * 0.5 * scale;



		for (Mesh::VertexIter vi = p_Mesh.vertices_begin(); vi != p_Mesh.vertices_end(); ++vi) {
			p_Mesh.point(*vi)[0] /= xScale;
			p_Mesh.point(*vi)[1] /= xScale;
			p_Mesh.point(*vi)[2] /= xScale;
		}
	}

	void MeshTransform(Mesh& p_Mesh, Vector3d p_Direction)
	{
		for (Mesh::VertexIter vi = p_Mesh.vertices_begin(); vi != p_Mesh.vertices_end(); ++vi) {
			p_Mesh.point(*vi)[0] += p_Direction[0];
			p_Mesh.point(*vi)[1] += p_Direction[1];
			p_Mesh.point(*vi)[2] += p_Direction[2];
		}
	}

	Mesh MeshRotate(Mesh p_Mesh, Point3d p_center, Vector3d rotateAxis, float angle)
	{
			// 将角度转换为弧度
			float radians = angle * M_PI / 180.0f;
			float cosA = cos(radians);
			float sinA = sin(radians);

			// 归一化旋转轴
			Vector3d normalizedAxis = rotateAxis.normalized();

			// 旋转矩阵
			float ux = normalizedAxis[0];
			float uy = normalizedAxis[1];
			float uz = normalizedAxis[2];

			float rotationMatrix[3][3] = {
				{cosA + ux * ux * (1 - cosA), ux * uy * (1 - cosA) - uz * sinA, ux * uz * (1 - cosA) + uy * sinA},
				{uy * ux * (1 - cosA) + uz * sinA, cosA + uy * uy * (1 - cosA), uy * uz * (1 - cosA) - ux * sinA},
				{uz * ux * (1 - cosA) - uy * sinA, uz * uy * (1 - cosA) + ux * sinA, cosA + uz * uz * (1 - cosA)}
			};

			// 遍历每个顶点，应用旋转
			for (auto v_it = p_Mesh.vertices_begin(); v_it != p_Mesh.vertices_end(); ++v_it) {
				Mesh::Point pos = p_Mesh.point(*v_it);
				// 将顶点坐标移至旋转中心		

				pos -= p_center;

				// 应用旋转
				float x_new = rotationMatrix[0][0] * pos[0] + rotationMatrix[0][1] * pos[1] + rotationMatrix[0][2] * pos[2];
				float y_new = rotationMatrix[1][0] * pos[0] + rotationMatrix[1][1] * pos[1] + rotationMatrix[1][2] * pos[2];
				float z_new = rotationMatrix[2][0] * pos[0] + rotationMatrix[2][1] * pos[1] + rotationMatrix[2][2] * pos[2];

				// 更新顶点坐标
				pos = OpenMesh::Vec3d(x_new, y_new, z_new) + p_center;
				p_Mesh.set_point(*v_it, pos);
			}

			return p_Mesh;
	}


	array<Point3d, 4> GetOffSectPoint(Point3d& p0, Vector3d& normal, double scale)
	{
		Vector3d defaultVector(0, 0, 1);
		if (IsZero(normal.cross(defaultVector).length())) {
			//normal 和 默认中间向量共线
			defaultVector = Vector3d(0, 1, 0);
		}

		Vector3d directionX = normal.cross(defaultVector);
		directionX.normalize();

		Vector3d directionY = normal.cross(directionX);
		directionY.normalize();

		Point3d p1;
		Point3d p2;
		Point3d p3;
		Point3d p4;

		p1 = scale * directionX + scale * directionY + p0;
		p2 = -scale * directionX + scale * directionY + p0;
		p3 = scale * directionX + -scale * directionY + p0;
		p4 = -scale * directionX + -scale * directionY + p0;

		return { p1,p2,p3,p4 };
	}

	void getFileNames(string path, vector<string>& files)
	{

		//文件句柄
		intptr_t hFile = 0;
		//文件信息
		struct _finddata_t fileinfo;
		string p;
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
		{
			do
			{
				//如果是目录,递归查找
				//如果不是,把文件绝对路径存入vector中
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFileNames(p.assign(path).append("\\").append(fileinfo.name), files);
				}
				else
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

	}

	bool testValidTriangle(vector<Point3d> ps)
	{
		assert(ps.size() == 3);

		Point3d p0 = ps[0];
		Point3d p1 = ps[1];
		Point3d p2 = ps[2];

		if (IsZero(p0 - p1) || IsZero(p1 - p2) || IsZero(p2 - p0))
			return false;

		return true;
	}

	bool TrianglePlaneIsIntersect(Triangle3d& p_Tri, const Plane3d& p_Plane, TrianglePlaneIsIntersectInfo& info, Tolerance p_Tolerance)
	{
		Point3d v0 = p_Tri.VertexAt(0);
		Point3d v1 = p_Tri.VertexAt(1);
		Point3d v2 = p_Tri.VertexAt(2);


		Vector3d planeNormal = p_Plane.GetNormal();
		Point3d planeOrigin = p_Plane.GetOrigin();



		bool isonFlag1 = IsOn(v0, p_Plane, p_Tolerance);
		bool isonFlag2 = IsOn(v1, p_Plane, p_Tolerance);
		bool isonFlag3 = IsOn(v2, p_Plane, p_Tolerance);

		double ans1 = (v0 - planeOrigin).normalize().dot(planeNormal);
		double ans2 = (v1 - planeOrigin).normalize().dot(planeNormal);
		double ans3 = (v2 - planeOrigin).normalize().dot(planeNormal);

		bool ispositiveFlag1 = IsPositive((v0 - planeOrigin).normalize().dot(planeNormal));
		bool ispositiveFlag2 = IsPositive((v1 - planeOrigin).normalize().dot(planeNormal));
		bool ispositiveFlag3 = IsPositive((v2 - planeOrigin).normalize().dot(planeNormal));

		
		if (  ((!ispositiveFlag1) || isonFlag1) && ((!ispositiveFlag2) || isonFlag2) && ((!ispositiveFlag3) || isonFlag3)) {
			//三个顶点在平面上也会归属负侧

			//三个顶点在平面负侧
			info.isIntersect = false;
			info.direction = false;
		}
		else if ( ((ispositiveFlag1) || isonFlag1) && ((ispositiveFlag2) || isonFlag2) && ((ispositiveFlag3) || isonFlag3)) {
			//三个顶点在平面正侧
			info.isIntersect = false;
			info.direction = true;
		}
		else 
		{
			//三个顶点在平面两侧
			info.isIntersect = true;
			info.direction = false;	

			EdgePlaneIsIntersectInfo info1, info2, info3;

			bool isInter1 = EdgePlaneIsIntersect(make_pair(v0, v1), p_Plane, info1, Tolerance::GetToleranceHighResolution());


			bool isInter2 = EdgePlaneIsIntersect(make_pair(v1, v2), p_Plane, info2, Tolerance::GetToleranceHighResolution());


			bool isInter3 = EdgePlaneIsIntersect(make_pair(v2, v0), p_Plane, info3, Tolerance::GetToleranceHighResolution());

			//调整参数确保逻辑合理性
			if (EdgePlaneIsIntersectInfo::Type::OnTheBegin == info1.interType) {			
				if (info3.interType != EdgePlaneIsIntersectInfo::Type::OnTheEnd) {
					info3.interType = EdgePlaneIsIntersectInfo::Type::OnTheEnd;
					info3.isIntersect = true;
					info3.intersectPoint = info1.intersectPoint;
				}
			}

			if (EdgePlaneIsIntersectInfo::Type::OnTheEnd == info1.interType) {
				if (info2.interType != EdgePlaneIsIntersectInfo::Type::OnTheBegin) {
					info2.interType = EdgePlaneIsIntersectInfo::Type::OnTheBegin;
					info2.isIntersect = true;
					info2.intersectPoint = info1.intersectPoint;
				}
			}

			if (EdgePlaneIsIntersectInfo::Type::OnTheBegin == info2.interType) {
				if (info1.interType != EdgePlaneIsIntersectInfo::Type::OnTheEnd) {
					info1.interType = EdgePlaneIsIntersectInfo::Type::OnTheEnd;
					info1.isIntersect = true;
					info1.intersectPoint = info2.intersectPoint;
				}
			}

			if (EdgePlaneIsIntersectInfo::Type::OnTheEnd == info2.interType) {
				if (info3.interType != EdgePlaneIsIntersectInfo::Type::OnTheBegin) {
					info3.interType = EdgePlaneIsIntersectInfo::Type::OnTheBegin;
					info3.isIntersect = true;
					info3.intersectPoint = info2.intersectPoint;
				}
			}

			if (EdgePlaneIsIntersectInfo::Type::OnTheBegin == info3.interType) {
				if (info2.interType != EdgePlaneIsIntersectInfo::Type::OnTheEnd) {
					info2.interType = EdgePlaneIsIntersectInfo::Type::OnTheEnd;
					info2.isIntersect = true;
					info2.intersectPoint = info3.intersectPoint;
				}
			}

			if (EdgePlaneIsIntersectInfo::Type::OnTheEnd == info3.interType) {
				if (info1.interType != EdgePlaneIsIntersectInfo::Type::OnTheBegin) {
					info1.interType = EdgePlaneIsIntersectInfo::Type::OnTheBegin;
					info1.isIntersect = true;
					info1.intersectPoint = info3.intersectPoint;
				}
			}


			if ((!isInter1) || (!isInter2) || (!isInter3))
				return false;

			//bool isInter1 = EdgePlaneIsIntersect(make_pair(v0, v1), p_Plane, info1, p_Tolerance);
			//	

			//bool isInter2 = EdgePlaneIsIntersect(make_pair(v1, v2), p_Plane, info2, p_Tolerance);
			//	

			//bool isInter3 = EdgePlaneIsIntersect(make_pair(v2, v0), p_Plane, info3, p_Tolerance);
			//	
			//if ((!isInter1) && (!isInter2) && (!isInter3))
			//	return false;

			//交点在顶点上
			if (EdgePlaneIsIntersectInfo::Type::OnTheBegin == info1.interType) {
				//assert(EdgePlaneIsIntersectInfo::Type::OnTheEnd == info3.interType);

				if (!testValidTriangle({ v0 , v1, info2.intersectPoint }))
					return false;
				if (!testValidTriangle({ v0 , info2.intersectPoint , v2 }))
					return false;

				Triangle3d t1({ v0 , v1, info2.intersectPoint });

				Triangle3d t2({ v0 , info2.intersectPoint , v2 });

				if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
					info.newPositiveTriangles.push_back(t1);

					info.newNegativeTriangles.push_back(t2);
				}
				else {
					info.newPositiveTriangles.push_back(t2);

					info.newNegativeTriangles.push_back(t1);

				}

				return true;
			}
			else if (EdgePlaneIsIntersectInfo::Type::OnTheEnd == info1.interType) {

				//assert(EdgePlaneIsIntersectInfo::Type::OnTheBegin == info2.interType);

				if (!testValidTriangle({ v0 , v1, info3.intersectPoint }))
					return false;
				if (!testValidTriangle({ v2 , info3.intersectPoint , v1 }))
					return false;

				Triangle3d t1({ v0 , v1, info3.intersectPoint });

				Triangle3d t2({ v2 , info3.intersectPoint , v1 });

				if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
					info.newPositiveTriangles.push_back(t1);

					info.newNegativeTriangles.push_back(t2);
				}
				else {
					info.newPositiveTriangles.push_back(t2);

					info.newNegativeTriangles.push_back(t1);

				}

				return true;
			}
			else if (EdgePlaneIsIntersectInfo::Type::OnTheEnd == info2.interType) {
				//assert(EdgePlaneIsIntersectInfo::Type::OnTheBegin == info3.interType);

				if (!testValidTriangle({ v0 , info1.intersectPoint, v2 }))
					return false;
				if (!testValidTriangle({ v2 , info1.intersectPoint , v1 }))
					return false;

				Triangle3d t1({ v0 , info1.intersectPoint, v2 });

				Triangle3d t2({ v2 , info1.intersectPoint , v1 });

				if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
					info.newPositiveTriangles.push_back(t1);

					info.newNegativeTriangles.push_back(t2);
				}
				else {
					info.newPositiveTriangles.push_back(t2);

					info.newNegativeTriangles.push_back(t1);

				}

				return true;
			}


			//交点不在端点上
			if (info1.isIntersect) {
				//assert(EdgePlaneIsIntersectInfo::Type::Regular == info1.interType);

				if (info2.isIntersect) {
					//assert(EdgePlaneIsIntersectInfo::Type::Regular == info2.interType);
					//assert(!info3.isIntersect);
					
					if (!testValidTriangle({ v0 , info1.intersectPoint, v2 }))
						return false;
					if (!testValidTriangle({ v2 , info1.intersectPoint, info2.intersectPoint }))
						return false;
					if (!testValidTriangle({ info1.intersectPoint, v1, info2.intersectPoint }))
						return false;

					Triangle3d t1({ v0 , info1.intersectPoint, v2 });
					Triangle3d t2({ v2 , info1.intersectPoint, info2.intersectPoint });

					Triangle3d t3({ info1.intersectPoint, v1, info2.intersectPoint });

					if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
						info.newPositiveTriangles.push_back(t1);
						info.newPositiveTriangles.push_back(t2);

						info.newNegativeTriangles.push_back(t3);
					}
					else {
						info.newNegativeTriangles.push_back(t1);
						info.newNegativeTriangles.push_back(t2);

						info.newPositiveTriangles.push_back(t3);
					}

					return true;
				}
				else if (info3.isIntersect) {
					//assert(EdgePlaneIsIntersectInfo::Type::Regular == info3.interType);
					//assert(!info2.isIntersect);

					if (!testValidTriangle({ v0 , info1.intersectPoint, info3.intersectPoint }))
						return false;
					if (!testValidTriangle({ info1.intersectPoint, v2 ,  info3.intersectPoint }))
						return false;
					if (!testValidTriangle({ info1.intersectPoint, v1, v2 }))
						return false;

					Triangle3d t1({ v0 , info1.intersectPoint, info3.intersectPoint });

					Triangle3d t2({ info1.intersectPoint, v2 ,  info3.intersectPoint });
					Triangle3d t3({ info1.intersectPoint, v1, v2 });

					if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
						info.newPositiveTriangles.push_back(t1);

						info.newNegativeTriangles.push_back(t2);
						info.newNegativeTriangles.push_back(t3);
					}
					else {
						info.newNegativeTriangles.push_back(t1);

						info.newPositiveTriangles.push_back(t2);
						info.newPositiveTriangles.push_back(t3);
					}

					return true;
				}
				else {
					return false;
				}
									
				
			}
			else if (info2.isIntersect) {

				//assert(EdgePlaneIsIntersectInfo::Type::Regular == info2.interType);
				if (info3.isIntersect) {
					//assert(EdgePlaneIsIntersectInfo::Type::Regular == info3.interType);
					//assert(!info1.isIntersect);

					if (!testValidTriangle({ v0 , v1, info3.intersectPoint }))
						return false;
					if (!testValidTriangle({ info3.intersectPoint , v1, info2.intersectPoint }))
						return false;
					if (!testValidTriangle({ v2, info3.intersectPoint, info2.intersectPoint }))
						return false;

					Triangle3d t1({ v0 , v1, info3.intersectPoint });
					Triangle3d t2({ info3.intersectPoint , v1, info2.intersectPoint });

					Triangle3d t3({ v2, info3.intersectPoint, info2.intersectPoint });

					if (IsPositive((t1.GetCenter() - planeOrigin).dot(planeNormal))) {
						info.newPositiveTriangles.push_back(t1);
						info.newPositiveTriangles.push_back(t2);

						info.newNegativeTriangles.push_back(t3);
					}
					else {
						info.newNegativeTriangles.push_back(t1);
						info.newNegativeTriangles.push_back(t2);

						info.newPositiveTriangles.push_back(t3);
					}

					return true;
				}
				else {

					//assert(false);
					return false;
				}

			}
			else {
				//assert(false);
				return false;
			}


		}
		
		return true;
	}

	bool EdgePlaneIsIntersect(pair<Point3d, Point3d> p_Edge, const Plane3d& p_Plane, EdgePlaneIsIntersectInfo& info, Tolerance p_Tolerance)
	{	
		if ((p_Edge.first - p_Edge.second).length() <= p_Tolerance.Linear()) {
			
			return false;
		}
		

		const Vector3d& planeNormal = p_Plane.GetNormal();
		LineSegment3d edge(p_Edge.first, p_Edge.second);

		if (true == IsPerpendicular(planeNormal, edge.GetDirection(), p_Tolerance.Linear()))//若平行，法向量和直线方向向量点积为0
		{
			if (true == IsOn(p_Edge.first, p_Plane, p_Tolerance)) //若线在面上，线上的任意一点在面上；
			{
				info.isIntersect = false;
				info.interType = EdgePlaneIsIntersectInfo::Type::OverLap;

				return true;
			}

			info.isIntersect = false;
			info.interType = EdgePlaneIsIntersectInfo::Type::None;

			return true;
		}
		else
		{
			const auto& planeOrigin = p_Plane.GetOrigin();
			const auto& lineSegmentOrigin = p_Edge.first;
			const auto& lineSegmentDirection = edge.GetDirection();


			//double pteset = (planeNormal | lineSegmentDirection);
			double p = (planeNormal | (planeOrigin - lineSegmentOrigin)) / (planeNormal | lineSegmentDirection);

			if (IsBetween(p, 0.0, edge.GetLength(), p_Tolerance.m_Linear * edge.GetLength()))
			{
				double le = edge.GetLength();
				if (IsEqual(p, 0.0, p_Tolerance.m_Linear * edge.GetLength())) {
					info.isIntersect = true;
					info.intersectPoint = p_Edge.first;
					info.interType = EdgePlaneIsIntersectInfo::Type::OnTheBegin;

					return true;
				
				}
				else if (IsEqual(p, edge.GetLength(), p_Tolerance.m_Linear * edge.GetLength())) {
					info.isIntersect = true;
					info.intersectPoint = p_Edge.second;
					info.interType = EdgePlaneIsIntersectInfo::Type::OnTheEnd;

					return true;
				
				}

				Point3d intersect = edge.CalculateValue(p);

				info.isIntersect = true;
				info.intersectPoint = intersect;
				info.interType = EdgePlaneIsIntersectInfo::Type::Regular;

				return true;
			}

			info.isIntersect = false;
			info.interType = EdgePlaneIsIntersectInfo::Type::None;
			return true;
		}

		assert((p_Edge.first - p_Edge.second).length() > p_Tolerance.Linear());
		assert(false);
		return false;
	}

	bool TriangleTriangleIsIntersect(Triangle3d& p_Tri1, Triangle3d& p_Tri2, Tolerance p_Tolerance)
	{
		assert(p_Tri1.isValid());
		assert(p_Tri2.isValid());


		for (int i = 0;i < 3;i++) {
			LineSegment3d lineseg = p_Tri1.GetLinesegment(i);
			
			if (EdgeTriangleIsIntersect(lineseg, p_Tri2))
				return true;

		}


		for (int i = 0;i < 3;i++) {
			LineSegment3d lineseg = p_Tri2.GetLinesegment(i);

			if (EdgeTriangleIsIntersect(lineseg, p_Tri1))
				return true;

		}

		return false;
	}

	bool EdgeTriangleIsIntersect(LineSegment3d& p_Line1, Triangle3d& p_Tri2, Tolerance p_Tolerance)
	{

		auto intersectResult = IntersectPlaneLineSegment(
			p_Tri2,
			p_Line1,
			p_Tolerance
		);

		//不相交
		if (intersectResult.IsNotType(IntersectResult::Type::Regular))
		{
			return false;
		}

		Point3d intersect = intersectResult.m_Intersect;
		const double& intersectParamOnEdge = intersectResult.m_ParamOnLine;
		const Parameter2d& intersectParamInTriangle = intersectResult.m_ParamOnPlane;

		uint onElementIndex = uint(-1);
		auto relation = DetectCoplanarPoint3dTriangle3dRelation(intersect, p_Tri2, p_Tolerance);

		if (Point3dTriangle3dRelation::Type::Out == relation.m_Relation)
		{
			return false;
		}

		if (Point3dTriangle3dRelation::Type::In == relation.m_Relation)
		{
			return true;
		}

		if (Point3dTriangle3dRelation::Type::OnEdge == relation.m_Relation)
		{
			return true;
		}

		if (Point3dTriangle3dRelation::Type::OnVertex == relation.m_Relation)
		{
			return true;
		}

		assert(false);
		return false;
	}


	bool IsOn(const Point3d& p_Point, const Plane3d& p_Plane, const Tolerance& p_Tolerance)
	{
		//若点在面上，点与面上的点形成的向量与法向量点积为0
		Vector3d vector = p_Point - p_Plane.GetOrigin();

		if (true == IsZero(vector))
		{
			return true;
		}

		double test = vector.normalize().dot(p_Plane.GetNormal());
		if (true == IsPerpendicular(vector.normalize(), p_Plane.GetNormal()))
		{
			return true;
		}


		return false;
	}

	bool IsOn(const Point3d& p_Point, const Line3d& p_Line)
	{
		Vector3d vector = p_Point - p_Line.GetOrigin();

		if (true == IsZero(vector))
		{
			return true;
		}

		if (true == IsParallel(vector, p_Line.GetDirection()))
		{
			return true;
		}

		return false;
	}

