#include "mPch.h"
#include "LitShader.h"



	LitShader::LitShader()
		:Shader(VertexWithNormalShaderScript, LitFragmentShaderScript)
	{

	}

	LitShader::~LitShader()
	{

	}

	void LitShader::UpdateLightParas(const RenderParas& p_RenderParas)
	{
		SetVec3("lightPosition", p_RenderParas.m_LightPosition);
		SetVec3("lightColor", p_RenderParas.m_LightColor);
	}

	void LitShader::UpdateRenderParas(const RenderParas& p_RenderParas)
	{
		Shader::UpdateRenderParas(p_RenderParas);

		UpdateLightParas(p_RenderParas);
	}