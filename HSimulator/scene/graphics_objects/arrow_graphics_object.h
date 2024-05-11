#pragma once 

#include <scene/graphics_objects/graphics_object.h>


namespace HSim
{
	class ArrowGraphicsObject : public GraphicsObject
	{
	public:
		ArrowGraphicsObject();
		ArrowGraphicsObject(const ArrowGraphicsObject* other);
		~ArrowGraphicsObject();

		void clone(std::shared_ptr<GraphicsObject>& ptr) override;
	};
	
} // namespace HSim