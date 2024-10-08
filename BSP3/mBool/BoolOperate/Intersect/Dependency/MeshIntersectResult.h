#pragma once

	class MeshTriangleIntersectRecord;

	class MeshIntersectResult
	{
		using MeshTriangleIntersectRecordMap = unordered_map<FaceId, MeshTriangleIntersectRecord>;

	protected:


		const Tolerance m_Tolerance;

		MeshTriangleIntersectRecordMap m_TriangleIntersectRecordMap;

		vector<MeshIntersectVertex*> m_IntersectVertices;
		vector<MeshIntersectPath*> m_IntersectPaths;

		//vector<MeshIntersectPath*> m_IntersectPathsForMeshA;
		//vector<MeshIntersectPath*> m_IntersectPathsForMeshB;

	public:
		MeshIntersectResult(const Tolerance& p_Tolerance = Tolerance::GetTolerance());
		virtual ~MeshIntersectResult();

		/*	const MeshTriangleIntersectRecord& GetIntersectRecord(const MeshTriangle& p_Triangle);

			MeshTriangleIntersectRecord* hasRecord(const MeshTriangle* p_Triangle) { return nullptr; }*/
	public:
		//ต๗สิ
		vector<MeshIntersectVertex*>& GetIntersectVertices() {return m_IntersectVertices;}
		vector<MeshIntersectPath*>& GetIntersectPaths() { return m_IntersectPaths; }

	public:

		bool IsEmpty() const;
		void Clear();

		bool AddTerminalVertex(const MeshIntersectTerminalVertex& p_Vertex);
		bool AddMiddleVertex(const MeshTriangleHandle& p_Triangle, const MeshIntersectMiddleVertex& p_Vertex);

		size_t GeneratePaths();

		MeshTriangleIntersectRecord* At(const MeshTriangleHandle*);
		const MeshTriangleIntersectRecord* At(const MeshTriangleHandle*) const;

		MeshTriangleIntersectRecord* At(FaceId);
		const MeshTriangleIntersectRecord* At(FaceId) const;
	public:


		//const MeshTriangleIntersectRecord* At(const MeshTriangle* p_pTriangle) const;
		//size_t mapsize() { return m_TriangleIntersectRecordMap.size(); }
		//const MeshIntersectRecord& operator[](const MeshTriangle* p_pTriangle) const;
	};
