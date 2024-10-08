#include "mPch.h"
#include "RenderScence.h"



	RenderScence::RenderScence()
	{

	}

	RenderScence::~RenderScence()
	{
		for (auto pu : m_Units)
		{
			if (nullptr != pu)
			{
				delete pu;
			}
		}

		m_Units.clear();
	}

	void RenderScence::AddUnit(UnitRenderer* p_pUnit)
	{
		if (nullptr == p_pUnit)
		{
			assert(false);
			throw;
		}

		p_pUnit->Initialize();
		p_pUnit->Update();

		m_Units.push_back(p_pUnit);
	}

	void RenderScence::Render(const RenderParas& p_RenderParas) const
	{
		for (const auto pu : m_Units)
		{
			pu->Render(p_RenderParas);
		}
	}

