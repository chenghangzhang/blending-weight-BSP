#pragma once


	class MeshIntersectVertex;
	class MeshIntersectPath;

	class MeshTriangleIntersectRecord
	{
		friend class MeshIntersectResult;
		//friend class CcwGraph;

	private:
	public://调试需要，暂时public
		vector<const MeshIntersectTerminalVertex*> m_TerminalVertices;
		vector<const MeshIntersectMiddleVertex*> m_MiddleVertices;
		vector<MeshIntersectPath*> m_IntersectPaths;


		typedef tsl::ordered_map<const MeshIntersectVertex*, size_t> VertexIntersectCountRecordMap;
		VertexIntersectCountRecordMap* m_pVertexIntersectCountRecord;

	public:
		unordered_set<FaceId> m_NewTriangles;
		bool hasChanged = false;
	
	public:
		MeshTriangleIntersectRecord();
		virtual ~MeshTriangleIntersectRecord();

		size_t GetPathCount() { return m_IntersectPaths.size(); }		
		size_t GetIntersectVerticesCount() { return (m_TerminalVertices.size() + m_MiddleVertices.size()); }

	public:
		vector<MeshIntersectPath*>& GetPaths() { return m_IntersectPaths; }
		const vector<MeshIntersectPath*>& GetPaths() const { return m_IntersectPaths; }

		vector<MeshIntersectVertex*>& GetMeshIntersectVertex();

	protected:
		bool IsValidPath(const MeshIntersectPath& p_Path);

		bool HasCoincidentTerminalVertex(const MeshIntersectTerminalVertex& p_Vertex, const Tolerance& p_Tolerance);
		bool HasCoincidentMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance);

	protected:
		void AddNewTerminalVertex(const MeshIntersectTerminalVertex* p_pVertex);
		MeshIntersectVertex* AddMiddleVertex(const MeshIntersectMiddleVertex& p_Vertex, const Tolerance& p_Tolerance);

		vector<MeshIntersectPath*> GeneratePaths();
		void GeneratePath(MeshIntersectPath& r_Path, const MeshTriangleHandle& p_IntersectTriangle);


	};

