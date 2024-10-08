#pragma once


enum class MeshBoolOperateType
{
	Union,
	Intersection,
	Difference
};



using RemeshPathEdgeRecord = unordered_map<EdgeId, Vector3d>;
using RemeshPathEdgeResult = unordered_map<const Mesh*, RemeshPathEdgeRecord>;

using RemeshNewVertexRecord = unordered_map<VertexId, const Point3d*>;
using RemeshNewVertexResult = unordered_map<const Mesh*, RemeshNewVertexRecord>;

//true 表示外部,false 表示内部
using ClassifyRecord = unordered_map<const Mesh*, unordered_map<FaceId, bool>>;


#include "Intersect/Dependency/Dependency.h"
#include "Intersect/Accelerator/Accelerator.h"



#include "Intersect/MeshIntersect.h"

#include "Remesh/Remesh.h"

#include "Classify/Classify.h"

#include "Combine/Combine.h"

#include "PreProcess/PreProcess.h"

#include "MeshBoolOperate.h"
