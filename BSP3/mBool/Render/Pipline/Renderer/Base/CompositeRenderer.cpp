#include "mPch.h"
#include "CompositeRenderer.h"



	CompositeRenderer::CompositeRenderer()
	{

	}

	CompositeRenderer::~CompositeRenderer()
	{
		for (auto ru : m_Composite)
		{
			delete ru;
		}

		m_Composite.clear();
	}

	void CompositeRenderer::AddRenderUnit(UnitRenderer* p_RenderUnit)
	{
		assert(nullptr != p_RenderUnit);
		m_Composite.push_back(p_RenderUnit);
	}

	bool CompositeRenderer::Initialize()
	{
		for (auto ru : m_Composite)
		{
			assert(nullptr != ru);

			if (false == ru->Initialize())
			{
				assert(false);
				return false;
			}
		}

		return true;
	}

	void CompositeRenderer::Update()
	{
		for (auto ru : m_Composite)
		{
			assert(nullptr != ru);
			ru->Update();
		}
	}

	void CompositeRenderer::Render(const RenderParas& p_RenderParas) const
	{
		if (false == m_IsRender)
		{
			return;
		}

		for (auto ru : m_Composite)
		{
			assert(nullptr != ru);
			ru->Render(p_RenderParas);
		}
	}
