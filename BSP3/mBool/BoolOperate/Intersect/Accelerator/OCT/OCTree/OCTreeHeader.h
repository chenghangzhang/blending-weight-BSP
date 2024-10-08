#pragma once

enum class OCTNodeDirection {
	// 
	//       --------------
	//	      0	    -    1
	//	      ----------------
	//           2    -     3
	//          ----------------
	//				  -
	// 				  -
	// 				  -
	// 				  -
	// 				  -
	//       --------------
	//	      4	    -    5
	//	      ----------------
	//           6    -     7
	//          ----------------

	D0,D1,D2,D3,D4,D5,D6,D7
};

using OCTPlaneRecord = pair<Point3d, OCTNodeDirection>;
using OCTPlaneRecords = vector<OCTPlaneRecord>;

using OCTFaceRenderingInfo = vector<Point3d>;


#include "OCTNode.h"
#include "OCTree.h"