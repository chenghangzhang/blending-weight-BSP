#include "mPch.h"
#include "RenderWnd.h"



	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void ProcessKeyInput(GLFWwindow* p_pWindow, double p_DeltaTime);

	RenderWnd::RenderWnd(unsigned int p_WndWidth, unsigned int p_WndHeight)
	{
		m_WndWidth = p_WndWidth > 0 ? p_WndWidth : 800;
		m_WndHeight = p_WndHeight > 0 ? p_WndHeight : 800;

		m_pWnd = nullptr;
		m_pScence = nullptr;

		m_RotateX = m_RotateY = 0.0;

		Initialize();
	}

	RenderWnd::~RenderWnd()
	{
		GlfwToRenderWndMap.erase(m_pWnd);
	}

	void RenderWnd::Initialize()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_pWnd = glfwCreateWindow(m_WndWidth, m_WndHeight, "LearnOpenGL", NULL, NULL);

		if (nullptr == m_pWnd)
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(m_pWnd);

		glfwSetFramebufferSizeCallback(m_pWnd, FrameBufferSizeCallback);
		glfwSetCursorPosCallback(m_pWnd, MouseMoveCallback);
		glfwSetScrollCallback(m_pWnd, MouseScrollCallback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//	std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		GlfwToRenderWndMap[m_pWnd] = this;
	}

	void RenderWnd::SetScence(const RenderScence* p_pScence)
	{
		m_pScence = p_pScence;
	}
	void RenderWnd::Run()
	{
		if (nullptr == m_pWnd)
		{
			return;
		}

		static double LastTime = -1.0f;

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(m_pWnd))
		{
			double currentTime = glfwGetTime();

			if (0 > LastTime)
			{
				LastTime = currentTime;
				continue;
			}

			double deltaTime = currentTime - LastTime;
			LastTime = currentTime;

			ProcessKeyInput(m_pWnd, deltaTime);

			RenderParas renderParas;
			{
				// pass projection matrix to shader (note that in this case it could change every frame)
				renderParas.m_Projection = glm::perspective(glm::radians(float(m_Camera.GetFov())), (float)m_WndWidth / (float)m_WndHeight, 0.1f, 100.0f);
				// camera/view transformation
				renderParas.m_View = m_Camera.GetViewMatrix();

				mat4& model = renderParas.m_Model;
				{
					model = mat4(0.1f);  model[3][3] = 1.0f;
					model = glm::rotate(model, glm::radians(float(m_RotateX)), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(float(m_RotateY)), glm::vec3(0.0f, 1.0f, 0.0f));
				}

				renderParas.m_LightPosition = vec3(1.2f, 1.0f, 2.0f);
				renderParas.m_LightColor = vec3(1.0f, 1.0f, 1.0f);
			}

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (nullptr != m_pScence)
			{
				m_pScence->Render(renderParas);
			}

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(m_pWnd);
			glfwPollEvents();
		}

		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	}

	std::map<GLFWwindow*, RenderWnd*> RenderWnd::GlfwToRenderWndMap;
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void FrameBufferSizeCallback(GLFWwindow* p_pWnd, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
	RenderWnd& RenderWnd::GetRenderWnd(GLFWwindow* p_Wnd)
	{
		if (GlfwToRenderWndMap.find(p_Wnd) != GlfwToRenderWndMap.end())
		{
			return *(GlfwToRenderWndMap[p_Wnd]);
		}

		assert(false);
		throw;
	}
	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void MouseMoveCallback(GLFWwindow* p_pWnd, double xpos, double ypos)
	{
		static double LastX = 0.0, LastY = 0.0;
		static bool IsFirstMouseMove = true;
		static double RotateSpeed = 0.1;

		if (GLFW_PRESS == glfwGetMouseButton(p_pWnd, GLFW_MOUSE_BUTTON_RIGHT))
		{
			if (IsFirstMouseMove)
			{
				LastX = xpos;
				LastY = ypos;
				IsFirstMouseMove = false;
				return;
			}

			double xoffset = RotateSpeed * (xpos - LastX);
			double yoffset = RotateSpeed * (LastY - ypos); // reversed since y-coordinates go from bottom to top

			auto& wnd = RenderWnd::GetRenderWnd(p_pWnd);
			Camera& camera = wnd.m_Camera;
			camera.Rotate(xoffset, yoffset);
		}

		if (GLFW_PRESS == glfwGetMouseButton(p_pWnd, GLFW_MOUSE_BUTTON_LEFT))
		{
			if (IsFirstMouseMove)
			{
				LastX = xpos;
				LastY = ypos;
				IsFirstMouseMove = false;
				return;
			}

			double xoffset = xpos - LastX;
			double yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top

			auto& wnd = RenderWnd::GetRenderWnd(p_pWnd);

			if (abs(xoffset) > abs(yoffset))
			{
				xoffset *= RotateSpeed * 2;
				wnd.m_RotateY += xoffset;
			}
			else
			{
				yoffset *= RotateSpeed * 2;
				wnd.m_RotateX += yoffset;
			}
		}

		LastX = xpos;
		LastY = ypos;
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void MouseScrollCallback(GLFWwindow* p_pWnd, double xoffset, double yoffset)
	{
		auto& wnd = RenderWnd::GetRenderWnd(p_pWnd);
		Camera& camera = wnd.m_Camera;
		camera.Zoom((float)yoffset);
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void ProcessKeyInput(GLFWwindow* p_pWnd, double p_DeltaTime)
	{
		static double CameraMoveSpeed = 1.0;

		if (glfwGetKey(p_pWnd, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(p_pWnd, true);

		auto& wnd = RenderWnd::GetRenderWnd(p_pWnd);
		Camera& camera = wnd.m_Camera;
		float step = (float)(CameraMoveSpeed * p_DeltaTime);

		if (glfwGetKey(p_pWnd, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(Camera::MoveDirection::Up, step);

		if (glfwGetKey(p_pWnd, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(Camera::MoveDirection::Down, step);

		if (glfwGetKey(p_pWnd, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(Camera::MoveDirection::Left, step);

		if (glfwGetKey(p_pWnd, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(Camera::MoveDirection::Right, step);
	}
