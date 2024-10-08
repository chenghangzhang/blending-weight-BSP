#pragma once


	class VertexShader
	{
	public:
		unsigned int m_Id;

	public:
		unsigned int GetId() const { return m_Id; }

	public:
		VertexShader(const char* p_Template);
		virtual ~VertexShader();
	};

