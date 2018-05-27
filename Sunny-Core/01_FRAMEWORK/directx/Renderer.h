#pragma once

#include "../include.h"
#include "Context.h"
#include "DebugBuffer.h"
#include "GeometryBuffer.h"

namespace sunny
{
	enum RendererBufferType
	{
		RENDERER_BUFFER_NONE     = 0,
		RENDERER_BUFFER_COLOR    = BIT(0),
		RENDERER_BUFFER_DEPTH    = BIT(1),
		RENDERER_BUFFER_DEFERRED = BIT(2),
		RENDERER_BUFFER_SHADOW   = BIT(3),
		RENDERER_BUFFER_DEBUG    = BIT(4)
	};

	enum class RendererBlendFunction
	{
		NONE,
		ZERO,
		ONE,
		SOURCE_ALPHA,
		DESTINATION_ALPHA,
		ONE_MINUS_SOURCE_ALPHA,
	};

	enum class RendererBlendEquation
	{
		NONE,
		ADD,
		SUBTRACT
	};

	namespace directx
	{
		class Renderer
		{
		private:
			static Renderer* s_instance;

			static std::vector<ID3D11BlendState*>        s_blendStates;
			static std::vector<ID3D11DepthStencilState*> s_depthStencilStates;
			
			Context* m_context;

		protected:
			void InitInternal();

			void ClearInternal(unsigned int buffer);
			void PresentInternal();

			void SetDepthTestingInternal(bool enabled, DIMENSION dimension);
			void SetBlendInternal(bool enabled, DIMENSION dimension);

		private:
			void CreateBlendStates();
			void CreateDepthStencilStates();

		public:
			static void Init();

			inline static void Clear(unsigned int buffer) { s_instance->ClearInternal(buffer); }
			inline static void Present() { s_instance->PresentInternal(); }

			inline static void SetDepthTesting(bool enabled, DIMENSION dimension = DIMENSION::D3) { s_instance->SetDepthTestingInternal(enabled, dimension); }
			inline static void SetBlend(bool enabled, DIMENSION dimension = DIMENSION::D3) { s_instance->SetBlendInternal(enabled, dimension); }
			
		public:

			Renderer();

			inline static Renderer* GetRenderer() { return s_instance; }
		};
	}
}