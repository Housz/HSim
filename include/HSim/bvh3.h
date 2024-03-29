#pragma once

#include <HSim/aabb3.h>

namespace HSim
{
	struct BVH3Node
	{
		BVH3Node() {}

		BVH3Node(std::shared_ptr<BVH3Node> LChild_, std::shared_ptr<BVH3Node> RChild_)
			: LChild(LChild_), RChild(RChild_) {}

		std::shared_ptr<BVH3Node> LChild = nullptr;
		std::shared_ptr<BVH3Node> RChild = nullptr;

		AABB3f aabb;

		bool isLeaf()
		{
			return (LChild == nullptr) && (RChild == nullptr);
		}
	}; // struct BVH3Node

	template <typename T>
	class BVH3
	{
	public:
		BVH3() {}

		void build(const std::vector<size_t> &primitiveIndices_, const std::vector<AABB3<T>> &primitivesAABBs_)
		{
			if (primitiveIndices_.empty() || primitivesAABBs_.empty())
			{
				std::cout << "BVH build fault! primitiveIndices_ is empty!" return;
			}

			primitiveIndices = primitiveIndices_;
			primitivesAABBs = primitivesAABBs_;

			// compute root AABB
			for(auto aabb : primitivesAABBs)
			{
				
			}

		}

		std::vector<size_t> primitiveIndices;
		std::vector<AABB3<T>> primitivesAABBs;

		// root Node
		BVH3Node root;
		// AABB of root
		AABB3<T> rootAABB;

	}; // class BVH3

} // namespace HSim
