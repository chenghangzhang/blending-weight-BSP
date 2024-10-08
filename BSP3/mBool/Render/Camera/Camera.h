#pragma once

using glm::vec3;
using glm::mat4;


	class Camera
	{
	public:
		enum class MoveDirection : char
		{
			Forward,
			Backward,
			Up,
			Down,
			Left,
			Right
		};

	protected:
		// camera Attributes
		vec3 m_Position;

		vec3 m_Front;
		vec3 m_Up;
		vec3 m_Right;

		float m_FocalDistance;
		vec3 m_Focus;

		// euler Angles
		double m_Yaw;
		double m_Pitch;
		// camera options
		double m_Fov;

	public:

		// constructor with vectors
		Camera(vec3 p_Position = vec3(0, 0, 3), double p_Yaw = -90, double p_Pitch = 0);

		void Move(MoveDirection p_Direction, float p_Step);
		void Zoom(float p_ZoomOffset);
		void Rotate(double p_YawOffset, double p_PitchOffset);

	public:
		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		mat4 GetViewMatrix() const;
		double GetFov() const { return m_Fov; }

	protected:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void Update();
	};





