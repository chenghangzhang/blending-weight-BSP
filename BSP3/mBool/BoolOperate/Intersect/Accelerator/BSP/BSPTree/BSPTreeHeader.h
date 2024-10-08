#pragma once

enum class BSPConstructType
{
	AABB_MIDDLE_SPLIT, 
	WSDM, WSDM_PCA,
	OriginSDM, SDM_ONLY_SIZEWEIGHT, 
	SAH, 
	ObbMiddel, 
	Gravity_SPLIT, 
	SDM_OBB, 
	Angle, 
	SerialAngle, 
	SDM_Angle,
	OCTREE
};


#define TestType BSPConstructType::Gravity_SPLIT



#include "BSPNode.h"
#include "BSPTree.h"

