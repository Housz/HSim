#pragma once

namespace HSim
{
	struct RenderableState
	{
		bool renderingDataNeedUpdate = true;
		bool aabbNeedUpdate = true;
		// ...
	};

	class Renderable
	{
	public:
		RenderableState state;

		
	};

} // namespace HSim
