#pragma once

#include <HSim/common.h>
#include <scene/game_object.h>

namespace HSim
{
	struct RendererFrame
	{
		
	};

	using RenderFrame = RendererFrame;
	using RFrame = RendererFrame;

	struct RendererFrameBuffer
	{
		// std::queue<RendererFrame> queue;
		std::vector<RendererFrame> buffer;
	};
	
	

} // namespace HSim
