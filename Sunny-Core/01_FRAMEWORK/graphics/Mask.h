#pragma once

#include "../include.h"
#include "../maths/maths.h"
#include "../directx/Texture2D.h"

namespace sunny
{
	namespace graphics
	{
		struct Mask
		{
			directx::Texture* texture;
			maths::mat4 transform;

			Mask(directx::Texture2D* texture, const maths::mat4& transform = maths::mat4::Identity())
			: texture(texture), transform(transform)
			{
				this->transform = maths::mat4::Scale(maths::vec3((float)texture->GetWidth() / (float)texture->GetHeight(), 1.0f, 1.0f));
			}
		};
	}
}