#pragma once


	class DirectMeshIntersectAccelerator : public MeshIntersectAccelerator
	{
	protected:
		Mesh::EdgeIter m_ActiveEdgeNow;
		TriIter m_PassiveTriangleNow;

		Mesh::EdgeIter m_PassiveEdgeNow;
		TriIter m_AcitveTriangleNow;

		//size_t m_ActiveEdgeIndex;
		//size_t m_PassiveTriangleIndex;

		//size_t m_PassiveEdgeIndex;
		//size_t m_AcitveTriangleIndex;

		size_t m_IntersectCheckListSize;

		//unordered_set<size_t> m_ParsedEdgeA;
		//unordered_set<size_t> m_ParsedEdgeB;
	public:
		DirectMeshIntersectAccelerator(const Mesh& p_MeshA, const Mesh& p_MeshB, const Tolerance& p_Tolerance);
		virtual ~DirectMeshIntersectAccelerator();

	public:
		void Initialize() override;
		void GenerateNextIntersectCheckList(IntersectCheckList& p_IntersectCheckList) override;
		void ResetGenerateIntersectCheckList() override;
	};


