#pragma once

#include "../../sunny.h"
#include "Material.h"

namespace sunny
{
	namespace graphics
	{
		// https://namu.wiki/w/%EB%AC%BC%EB%A6%AC%20%EA%B8%B0%EB%B0%98%20%EB%A0%8C%EB%8D%94%EB%A7%81
		
		// 물리 기반 렌더링
		class PBRMaterial : public Material
		{
		private:
			static directx::Texture2D* s_PreintegratedFG;

		public:
			PBRMaterial(directx::Shader* shader);
			~PBRMaterial();

			//void SetEnviromentMap(directx::TextureCube* texture);
			
			// PBR Statics
			void SetAlbedo(const maths::vec4& color);         // 알베도 
			void SetSpecular(const maths::vec3& color);       // 정반사
			void SetGloss(float value);
			void UsingNormalMap(bool value);

			// PBR Maps
			void SetAlbedoMap  (directx::Texture2D* texture);
			void SetSpecularMap(directx::Texture2D* texture);
			void SetNormalMap  (directx::Texture2D* texture);
			void SetGlossMap   (directx::Texture2D* texture);
		
			directx::Texture* GetAlbedoMap();
			directx::Texture* GetSpecularMap();
			directx::Texture* GetNormalMap();
			directx::Texture* GetGlossMap();
		};
	}
}