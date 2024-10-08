#pragma once

#include <iostream>
#include <unordered_map>
#include "Tessil/ordered_map"
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <cassert>
#include <unordered_set>
#include <algorithm>
#include <Eigen/Dense>
#include <sstream>
#include <thread>  
#include <ctime>
#include <random>

#include <io.h>

#include <windows.h>
#include <psapi.h>
//#include <stdio.h>
//
//int main()
//{
//    PROCESS_MEMORY_COUNTERS pmc;
//    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
//    {
//        printf("当前进程占用内存大小为：%d KB\n", pmc.WorkingSetSize / 1024);
//    }
//    return 0;
//}

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

using namespace std;


#include "MyMesh.h"

#include "MathSurport/Utils.h"

#include "BoolOperate/BoolRely.h"

#include "Render/Render.h"

#include "DataChange/DataTranslate.h"

#include "DataGenerate/GenerateMesh.h"
#include "DataGenerate/ReadCheckPair.h"

