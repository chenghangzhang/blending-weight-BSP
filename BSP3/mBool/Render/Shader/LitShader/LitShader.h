#pragma once



	class Shader;

	class LitShader : public Shader
	{
	public:
		LitShader();
		virtual ~LitShader();

	protected:
		virtual void UpdateLightParas(const RenderParas& p_RenderParas);

	public:
		virtual void UpdateRenderParas(const RenderParas& p_RenderParas) override;
	};

