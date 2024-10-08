#include "mPch.h"
#include "MeshTriangleIntersectRecord.h"



	MeshTriangleIntersectRecord::MeshTriangleIntersectRecord() :
		m_TerminalVertices(0),
		m_MiddleVertices(0),
		m_IntersectPaths(0),
		m_pVertexIntersectCountRecord(nullptr)
	{

	}

	MeshTriangleIntersectRecord::~MeshTriangleIntersectRecord()
	{
		if (nullptr != m_pVertexIntersectCountRecord)
		{
			delete m_pVertexIntersectCountRecord;
			m_pVertexIntersectCountRecord = nullptr;
		}
	}

	vector<MeshIntersectVertex*>& MeshTriangleIntersectRecord::GetMeshIntersectVertex()
	{

		vector<MeshIntersectVertex*>* mIVs = new vector<MeshIntersectVertex*>();
		for (auto& mTV : m_TerminalVertices) {
			mIVs->push_back(const_cast<MeshIntersectTerminalVertex*>(mTV));
		}



		for (auto& mMV : m_MiddleVertices) {
			mIVs->push_back(const_cast<MeshIntersectMiddleVertex*>(mMV));
		}

		//mIVs->insert(mIVs->end(), m_TerminalVertices.begin(), m_TerminalVertices.end());
		//mIVs->insert(mIVs->end(), m_MiddleVertices.begin(), m_MiddleVertices.end());

		return *mIVs;
		// 
		//vector<MeshIntersectVertex*> aa;
		//return aa;

		// TODO: 在此处插入 return 语句
	}

	bool MeshTriangleIntersectRecord::IsValidPath(const MeshIntersectPath& p_Path)
	{
		if (p_Path.Size() <= 1)
		{
			return false;
		}

		const MeshIntersectVertex& pathBegin = p_Path.Front();
		const MeshIntersectVertex& pathEnd = p_Path.Back();

		//当路径两端都为端点时，是合理路径
		if (pathBegin.IsTerminal() && pathEnd.IsTerminal())
		{
			return true;
		}

		return false;
	}

	bool MeshTriangleIntersectRecord::HasCoincidentTerminalVertex(const MeshIntersectTerminalVertex& p_Vertex, const Tolerance& p_Tolerance)
	{
		for (auto& pTv : m_TerminalVertices)
		{
			if ((*(pTv->m_Point3d)- *(p_Vertex.m_Point3d)).length() < p_Tolerance.m_Squared)
			{
				//相同位置端点
				
				if (pTv->GetIntersectTriangle(nullptr) == p_Vertex.GetIntersectTriangle(nullptr))
				{
					//相交三角形相同

					if ( (&pTv->GetHostEdge()) == (&p_Vertex.GetHostEdge()) ) {
						//所属HostEdge相同
						return true;
					}
				}
			}
		}

		//暂时先不做重复点的处理，等待构造测试样例，待完善

		return false;
	}

	bool MeshTriangleIntersectRecord::HasCoincidentMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance)
	{
		for (auto& pMv : m_MiddleVertices)
		{
			if ((*(pMv->m_Point3d)- *(p_Vertex.m_Point3d)).length() < p_Tolerance.m_Squared)
			{
				//相同位置点

				if (&pMv->GetHostTriangle() == &p_Vertex.GetHostTriangle())
				{
					//相交三角形相同

					if (&pMv->GetIntersectEdge() == &p_Vertex.GetIntersectEdge()) {
						//所属HostEdge相同
						return true;
					}
				}
			}
		}
		

		return false;
	}

	void MeshTriangleIntersectRecord::AddNewTerminalVertex(const MeshIntersectTerminalVertex* p_pVertex)
	{
		assert(nullptr != p_pVertex);
		
		//test have coincidence point
		bool hasSame = false;

		for (auto& tv : m_TerminalVertices) {
			if ((tv->GetHostEdge().first.idx() == p_pVertex->GetHostEdge().first.idx()) &&
				(tv->GetIntersectTriangle()->first.idx() == p_pVertex->GetIntersectTriangle()->first.idx())
				) {
				hasSame = true;
				break;
			}
		
		}

		if(!hasSame) m_TerminalVertices.push_back(p_pVertex);
		
	}

	MeshIntersectVertex* MeshTriangleIntersectRecord::AddMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance)
	{

		//test have coincidence point
		bool hasSame = false;

		for (auto& mv : m_MiddleVertices) {
			assert((mv->GetHostTriangle().first.idx() == p_Vertex.GetHostTriangle().first.idx()));

			const MeshHalfEdgeHandle& mie = mv->GetIntersectEdge();
			Mesh::HalfedgeHandle ie = mie.first;
			Mesh::HalfedgeHandle oppoIe = mie.second->opposite_halfedge_handle(ie);

			if ((ie.idx() == p_Vertex.GetIntersectEdge().first.idx()) || (oppoIe.idx() == p_Vertex.GetIntersectEdge().first.idx())) {
				hasSame = true;
				break;
			}

		}

		if (!hasSame) {
			auto* pNewVertex = new MeshIntersectMiddleVertex(p_Vertex);
			m_MiddleVertices.push_back(pNewVertex);

			return pNewVertex;
		}

		return nullptr;
	}

	vector<MeshIntersectPath*> MeshTriangleIntersectRecord::GeneratePaths()
	{
		m_IntersectPaths.clear();
		assert(nullptr == m_pVertexIntersectCountRecord);

		if (0 == m_TerminalVertices.size())
		{
			//端点开始寻找，只存在中点的三角形会在其他路径中通过端点遍历到
			//特殊情况所有相交点只存在中点//暂不考虑
			return m_IntersectPaths;
		}

		//for (auto& v : m_TerminalVertices) {
		//	cout << "Edge " << v->GetHostEdge().first.idx() << endl;
		//	cout << "Face " << v->GetIntersectTriangle()->first.idx() << endl;
		//}


		//初始化所有顶点相交次数
		m_pVertexIntersectCountRecord = new VertexIntersectCountRecordMap;
		auto& intersectCountRecord = *m_pVertexIntersectCountRecord;

		//先放端点，这样可以保证在 tsl::ordered_map 遍历中先处理端点
		for (auto& pTv : m_TerminalVertices)
		{
			assert(nullptr != pTv);
			assert(intersectCountRecord.find(pTv) == intersectCountRecord.end()); //没有重复点
			intersectCountRecord[pTv] = pTv->GetIntersectCount();
		}

		for (auto& pMv : m_MiddleVertices)
		{
			assert(nullptr != pMv);
			assert(intersectCountRecord.find(pMv) == intersectCountRecord.end()); //没有重复点
			intersectCountRecord[pMv] = pMv->GetIntersectCount();
		}

		while (0 != intersectCountRecord.size())
		{
			auto record = intersectCountRecord.begin(); // record 为迭代器 iterator
			const MeshIntersectVertex& v = *(record.key());
			size_t& intersectCount = record.value(); // record->second 为常量不能用作引用
			assert(0 < intersectCount);


			if (!v.IsTerminal()) {
				intersectCountRecord.erase(&v);
				continue;
			}

			const MeshTriangleHandle* pIntersectTriangle = v.GetIntersectTriangle();
			assert(nullptr != pIntersectTriangle);



			MeshIntersectPath& newPath = *(new MeshIntersectPath); //貌似这个用法不是很规范
			newPath.AddVertex(v);
			intersectCount--; //标记当前路径数量

			GeneratePath(newPath, *pIntersectTriangle);

			//判断当前生成路径是否有效
			if (true == IsValidPath(newPath))
			{
				//有效路径则开始维护 intersectCountRecord 信息，并保存路径
				size_t newPathSize = newPath.Size();
				for (size_t i = 0; i < newPathSize; i++)
				{
					const auto& v = newPath[i];
					const size_t& c = intersectCountRecord[&v];
					if (0 == c)
					{
						intersectCountRecord.erase(&v);
					}
				}

				m_IntersectPaths.push_back(&newPath);
			}
			else
			{
				//无效路径则开始回复 intersectCountRecord 信息，并保存路径
				newPath.DeletePathSegment();

				size_t newPathSize = newPath.Size();
				for (size_t i = 0; i < newPathSize; i++)
				{
					const auto& v = newPath[i];
					size_t& c = intersectCountRecord[&v];

					if (0 == c) // 这仅适用于目前无分叉路径的情况，仅每个点仅能放到 1 条路径中
					{
						intersectCountRecord.erase(&v);
					}

					// c++; //回复在路径递归过程中暂时移除的 count
				}
			}
		}

		return m_IntersectPaths;
	}

	void MeshTriangleIntersectRecord::GeneratePath(MeshIntersectPath& r_Path, const MeshTriangleHandle& p_IntersectTriangle)
	{
		assert(nullptr != m_pVertexIntersectCountRecord);
		auto& intersectCountRecord = *m_pVertexIntersectCountRecord;
		assert(0 < intersectCountRecord.size());


		const MeshTriangleHandle* nextTriangle = nullptr;

		for (auto record = intersectCountRecord.begin(); record != intersectCountRecord.end(); record++) //record 为迭代器 iterator, 但是支持for(:) 遍历
		{
			const MeshIntersectVertex& v = *(record.key());
			size_t& intersectCount = record.value(); // record->second 为常量不能用作引用

			if (0 == intersectCount || &v == &(r_Path.Back())) //不和上个节点判断 或 当前节点在当前路径已经没有相交数
			{
				continue;
			}

			if (true == v.IsIntersectWithTriangle(p_IntersectTriangle.first))
			{
				r_Path.AddVertex(v);
				intersectCount--; //无论是否创建成功都要将 count 减 1

				const auto* pNextIntersectTriangle = v.GetIntersectTriangle(&p_IntersectTriangle);
				if (nullptr != pNextIntersectTriangle) //还可以继续递归添加路径
				{
					return GeneratePath(r_Path, *pNextIntersectTriangle);
				}
				else //到了路径终点, 返回 
				{
					return;
				}
			}
		}


		////搜索中间节点 MiddleVertex
		//for (auto& pMv : m_MiddleVertices)
		//{
		//	if (pMv != &(r_Path.Back()))
		//	{
		//		//与起点不重合
		//		auto& intersectMvTriangles = pMv->GetIntersectEdge().GetHostTriangles();

		//		for (const auto& tri : intersectMvTriangles)
		//		{
		//			if (tri == &p_IntersectTriangle)
		//			{
		//				if (1 == intersectMvTriangles.size())
		//				{
		//					//该路径到达面片边界
		//					return false;
		//				}
		//				else
		//				{

		//					assert(2 == intersectMvTriangles.size());

		//					//搜索下一个三角形
		//					nextTriangle = (&p_IntersectTriangle == intersectMvTriangles[0]) ? intersectMvTriangles[1] : intersectMvTriangles[0];
		//					return GeneratePath(r_Path, *nextTriangle);
		//				}
		//			}
		//		}
		//	}
		//}

		//assert(nullptr == nextTriangle);

		////搜索端点 TerminalVertex
		////没有搜索到 Middle Vertex ,搜索 TerminalVertex
		//for (auto& pTv : m_TerminalVertices)
		//{
		//	if (&(pTv->GetIntersectTriangle()) == &p_IntersectTriangle)
		//	{
		//		r_Path.AddVertex(*pTv);
		//		//路径以TerminalVertex结束
		//		return true;
		//	}
		//}

		//只有一点相交，不形成路径
		//暂不考虑
		//return false;
	}
