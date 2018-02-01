#ifndef TH_SHADER_STOCK_H
#define TH_SHADER_STOCK_H

#include <Common\THCommon.h>

namespace THEngine
{
	class Shader;

	class ShaderStock : public Object, public Singleton<ShaderStock>
	{
		friend class Singleton<ShaderStock>;
	private:
		Ptr<Shader> skyBoxShader;
		Ptr<Shader> shadowShader;
		Ptr<Shader> meshShader;
		Ptr<Shader> spriteShader;

	private:
		ShaderStock() = default;

	public:
		virtual ~ShaderStock();

		bool Init();

		inline Ptr<Shader> GetSkyBoxShader() const { return skyBoxShader; }
		inline Ptr<Shader> GetShadowShader() const { return shadowShader; }
		inline Ptr<Shader> GetSpriteShader() const { return spriteShader; }
		inline Ptr<Shader> GetMeshShader() const { return meshShader; }
	};
}

#endif