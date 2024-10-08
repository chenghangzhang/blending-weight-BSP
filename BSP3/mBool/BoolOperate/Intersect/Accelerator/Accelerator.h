#pragma once

#define BSP_MAX_TREE_DEPTH 10
#define BSP_LEAF_SHRESHOLD_NUM 25

#define OCT_MAX_TREE_DEPTH 4
#define OCT_LEAF_SHRESHOLD_NUM 25

#include "Base/MeshIntersectAccelerator.h"

#include "Direct/DirectMeshIntersectAccelerator.h"

#include "BSP/BSPTree/BSPTreeHeader.h"
#include "BSP/BSPAccelerator.h"

#include "OCT/OCTree/OCTreeHeader.h"
#include "OCT/OCTAccelerator.h"