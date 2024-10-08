#pragma once



	class RenderParas
	{
	public:
		glm::mat4 m_Model;
		glm::mat4 m_Projection;
		glm::mat4 m_View;

		glm::vec3 m_LightPosition;
		glm::vec3 m_LightColor;

	public:
		RenderParas()
		{
			//			CONSOLE_TRACE(_T("未初始化成员变量"));
		}
		~RenderParas() {}
	};

