#pragma once


	class UnitRenderer;
	class RenderParas;

	class RenderScence
	{
	protected:
		list<UnitRenderer*> m_Units;

	public:
		RenderScence();
		virtual ~RenderScence();

	public:
		void AddUnit(UnitRenderer* p_pUnit);

	public:
		virtual void Render(const RenderParas& p_RenderParas) const;
	};

