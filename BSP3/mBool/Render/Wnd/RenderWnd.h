#pragma once

using namespace std;

struct GLFWwindow;




	class RenderScence;
	class Camera;

	class RenderWnd
	{
	protected:
		GLFWwindow* m_pWnd;

		unsigned int m_WndWidth;
		unsigned int m_WndHeight;

	public: //临时用于旋转model
		double m_RotateY;
		double m_RotateX;

	public:
		const RenderScence* m_pScence;
		Camera m_Camera;

	public:
		RenderWnd(unsigned int p_WndWidth = 800, unsigned int p_WndHeight = 800);
		virtual ~RenderWnd();

	protected:
		void Initialize();

	public:
		//暂时用的函数
		void SetScence(const RenderScence* p_pScence);

		void Run();

	protected:
		static map<GLFWwindow*, RenderWnd*> GlfwToRenderWndMap;
	public:
		static RenderWnd& GetRenderWnd(GLFWwindow* p_Wnd);

	};

