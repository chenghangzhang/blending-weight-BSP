#include "mPch.h"
#include "Camera.h"



	Camera::Camera(vec3 p_Position /*= vec3(0,0,0)*/, double p_Yaw /*= -90*/, double p_Pitch /*= 0*/)
	{
		m_Position = p_Position;

		m_Front = vec3(0, 0, -1);

		m_FocalDistance = 3.0;
		assert(m_FocalDistance > 0);
		m_Focus = vec3(0, 0, 0); //摄像机 焦点 = m_Position + m_FocalDistance * m_Front;

		m_Yaw = p_Yaw;
		m_Pitch = p_Pitch;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;

		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		m_Fov = 45.0;

		Update();
	}


	mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Focus, m_Up);
	}

	void Camera::Move(MoveDirection p_Direction, float p_Step)
	{
		vec3 offset = vec3(0, 0, 0);

		switch (p_Direction)
		{
		case MoveDirection::Forward:
			offset = m_Front * (p_Step);
			break;
		case MoveDirection::Backward:
			offset = m_Front * (-p_Step);
			break;
		case MoveDirection::Up:
			offset = m_Up * (p_Step);
			break;
		case MoveDirection::Down:
			offset = m_Up * (-p_Step);
			break;
		case MoveDirection::Left:
			offset = m_Right * (-p_Step);
			break;
		case MoveDirection::Right:
			offset = m_Right * (p_Step);
			break;
		default:
			assert(false);
			break;
		}

		m_Position += offset;
		m_Focus += offset;
	}

	void Camera::Rotate(double p_YawOffset, double p_PitchOffset)
	{
		m_Yaw += p_YawOffset;
		m_Pitch += p_PitchOffset;

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;

		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		Update();
	}

	void Camera::Zoom(float p_ZoomOffset)
	{
		assert(m_FocalDistance > 0);

		float offset = m_FocalDistance / 10.0f * p_ZoomOffset;
		m_FocalDistance -= offset; // p_ZoomOffset 大于 0 代表放大， 小于 0 代表缩小

		if (m_FocalDistance < 0.001f)
		{
			m_FocalDistance = 0.001f;
		}

		if (m_FocalDistance > 200.0f)
		{
			m_FocalDistance = 200.0f;
		}

		m_Position = m_Focus - m_FocalDistance * m_Front;
	}

	void Camera::Update()
	{
		if (m_Pitch >= 90 || m_Pitch <= -90) {
			assert(false);
		}

		// calculate the new Front vector
		vec3 front;
		front.x = cos(glm::radians((float)(m_Yaw))) * cos(glm::radians((float)(m_Pitch)));
		front.y = sin(glm::radians((float)(m_Pitch)));
		front.z = sin(glm::radians((float)(m_Yaw))) * cos(glm::radians((float)(m_Pitch)));

		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, vec3(0, 1, 0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_Focus = m_Position + (m_FocalDistance * m_Front);
	}
