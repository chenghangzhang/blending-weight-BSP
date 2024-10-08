#pragma once


	class FragmentShader
	{
	public:
		unsigned int m_Id;

	public:
		unsigned int GetId() const { return m_Id; }

	public:
		FragmentShader(const char* p_Template);
		virtual ~FragmentShader();
	};
