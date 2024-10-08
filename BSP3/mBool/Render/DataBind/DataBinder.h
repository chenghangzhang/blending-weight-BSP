#pragma once


	class DataBinder
	{
	public:
		template <class VERTEX_SET>
		static bool BindVerticesVertexData(const VERTEX_SET& p_VertexSet, unsigned int p_Vbo);
		template <class VERTEX_SET>
		static bool BindVerticesElementData(unsigned int& r_EboSize, const VERTEX_SET& p_VertexSet, unsigned int p_Ebo);

		template <class EDGE_SET>
		static bool BindEdgesVertexData(const EDGE_SET& p_EdgeSet, unsigned int p_Vbo);
		template <class EDGE_SET>
		static bool BindEdgesElementData(unsigned int& r_EboSize, const EDGE_SET& p_EdgeSet, unsigned int p_Ebo);

		template <class TRIANGLE_SET>
		static bool BindTrianglesVertexData(const TRIANGLE_SET& p_EdgeSet, unsigned int p_Vbo);
		template <class TRIANGLE_SET>
		static bool BindTrianglesElementData(unsigned int& r_EboSize, const TRIANGLE_SET& p_EdgeSet, unsigned int p_Ebo);

	};

	//typedef vector<Point3d> Vertex_Set;
	//typedef vector<pair<Point3d, Point3d>> Edge_Set;
	//typedef vector<tuple<Point3d, Point3d, Point3d>> Triangle_Set;

	template <class VERTEX_SET>
	bool DataBinder::BindVerticesVertexData(const VERTEX_SET& p_VertexSet, unsigned int p_Vbo)
	{
		if (true == p_VertexSet.empty())
		{
			return false;
		}

		vector<double> vertices(p_VertexSet.size() * 3, 0.0);

		size_t j = 0;
		for (size_t i = 0, j = 0, end = p_VertexSet.size(); i < end; i++, j += 3)
		{
			const auto& p = p_VertexSet[i];

			vertices[j] = p[0];
			vertices[j + 1] = p[1];
			vertices[j + 2] = p[2];
		}

		glBindBuffer(GL_ARRAY_BUFFER, p_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &(vertices[0]), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);

		return true;
	}

	template <class VERTEX_SET>
	bool DataBinder::BindVerticesElementData(unsigned int& r_EboSize, const VERTEX_SET& p_VertexSet, unsigned int p_Ebo)
	{
		if (true == p_VertexSet.empty())
		{
			assert(false);
			return false;
		}

		size_t vertexCount = p_VertexSet.size();
		vector<unsigned int> indices(vertexCount, 0);

		unsigned int index = 0;
		std::generate(indices.begin(), indices.end(), [&]() { return index++; });

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

		r_EboSize = (unsigned int)(indices.size());

		return true;
	}

	template <class EDGE_SET>
	bool DataBinder::BindEdgesVertexData(const EDGE_SET& p_EdgeSet, unsigned int p_Vbo)
	{
		return BindVerticesVertexData(p_EdgeSet, p_Vbo);
	}

	template <class EDGE_SET>
	bool DataBinder::BindEdgesElementData(unsigned int& r_EboSize, const EDGE_SET& p_EdgeSet, unsigned int p_Ebo)
	{
		if (true == p_EdgeSet.empty())
		{
			assert(false);
			return false;
		}

		unordered_map<const void*, unsigned int> m_VertexToIndexMap;

		for (size_t i = 0; i < p_EdgeSet.size(); i++)
		{
			const auto& v = p_EdgeSet[i];
			assert(m_VertexToIndexMap.end() == m_VertexToIndexMap.find(&v));
			m_VertexToIndexMap.insert(make_pair(&v, (unsigned int)(i)));
		}

		size_t edgeCount = p_EdgeSet.size()/2;
		vector<unsigned int> indices(edgeCount * 2, 0);

		for (size_t i = 0, j = 0; i < edgeCount; i++, j += 2)
		{
			const auto& e0 = p_EdgeSet[2 * i];
			const auto& e1 = p_EdgeSet[2 * i + 1];

			indices[j] = m_VertexToIndexMap[&(e0)];
			indices[j + 1] = m_VertexToIndexMap[&(e1)];
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

		r_EboSize = (unsigned int)(indices.size());

		return true;
	}

	template <class TRIANGLE_SET>
	bool DataBinder::BindTrianglesVertexData(const TRIANGLE_SET& p_Mesh, unsigned int p_Vbo)
	{
		size_t faceCount = p_Mesh.size();

		if (0 == faceCount)
		{
			assert(false);
			return false;
		}

		vector<double> verticesWithNormal(faceCount * 18, 0.0);

		for (size_t i = 0, j = 0; i < p_Mesh.size(); i++)
		{
			const auto& face = p_Mesh[i];
			const auto& normal = std::get<3>(face);

			Point3d vv[3] = { std::get<0>(face) ,std::get<1>(face) ,std::get<2>(face) };

			for (unsigned int k = 0; k < 3; k++)
			{
				const auto& v = vv[k];

				verticesWithNormal[j++] = v[0];
				verticesWithNormal[j++] = v[1];
				verticesWithNormal[j++] = v[2];

				verticesWithNormal[j++] = normal[0];
				verticesWithNormal[j++] = normal[1];
				verticesWithNormal[j++] = normal[2];
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, p_Vbo);
		glBufferData(GL_ARRAY_BUFFER, verticesWithNormal.size() * sizeof(double), &(verticesWithNormal[0]), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
		glEnableVertexAttribArray(1);

		return true;
	}

	template <class TRIANGLE_SET>
	bool DataBinder::BindTrianglesElementData(unsigned int& r_EboSize, const TRIANGLE_SET& p_Mesh, unsigned int p_Ebo)
	{
		if (0 == p_Mesh.size())
		{
			assert(false);
			return false;
		}

		vector<unsigned int> indices(p_Mesh.size() * 3, 0);

		for (size_t i = 0, j = 0; i < p_Mesh.size(); i++)
		{
			const auto& f = p_Mesh[i];

			for (size_t k = 0; k < 3; k++, j++)
			{
				assert(j <= UINT_MAX);
				indices[j] = (unsigned int)(j);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

		r_EboSize = (unsigned int)(indices.size());

		return true;
	}
