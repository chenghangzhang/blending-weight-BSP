#pragma once


	class RenderParas;

	class UnitRenderer
	{
	protected:
		bool m_IsRender;

	public:
		UnitRenderer();
		virtual ~UnitRenderer() = 0 {}

	public:
		virtual bool Initialize() = 0;
		virtual void Update() = 0;

	public:
		void SetIsRender(bool p_IsRender) { m_IsRender = p_IsRender; }

	public:
		virtual void Render(const RenderParas& p_RenderParas) const = 0;
	};
