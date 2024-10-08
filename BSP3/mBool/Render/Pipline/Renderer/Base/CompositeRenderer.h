#pragma once


	class UnitRenderer;

	class CompositeRenderer : public UnitRenderer
	{
	protected:
		list<UnitRenderer*> m_Composite;

	public:
		CompositeRenderer();
		virtual ~CompositeRenderer();

	public:
		void AddRenderUnit(UnitRenderer* p_RenderUnit);

	public:
		bool Initialize() override;
		void Update() override;
		void Render(const RenderParas& p_RenderParas) const override;
	};



