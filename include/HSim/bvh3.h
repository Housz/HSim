#pragma once

#include <HSim/aabb3.h>

namespace HSim
{
	struct BVH3Node;
	using BVH3Node_Ptr = std::shared_ptr<BVH3Node>;

	struct BVH3Node
	{
		BVH3Node() {}

		BVH3Node(std::shared_ptr<BVH3Node> LChild_, std::shared_ptr<BVH3Node> RChild_)
			: LChild(LChild_), RChild(RChild_) {}

		BVH3Node_Ptr LChild = nullptr;
		BVH3Node_Ptr RChild = nullptr;

		AABB3f aabb;
		size_t primitiveIndex; // if isLeaf

		bool isLeaf()
		{
			return (LChild == nullptr) && (RChild == nullptr);
		}

	}; // struct BVH3Node

	template <typename T = float>
	class BVH3
	{
	public:
		using indexIterType = std::vector<size_t>::iterator;

		BVH3()
		{
			rootNode = std::make_shared<BVH3Node>();
		}

		void reset()
		{
			rootNode = std::make_shared<BVH3Node>();
			rootAABB.reset();
		}

		void build(std::vector<size_t> &primitiveIndices_, const std::vector<AABB3<T>> &primitivesAABBs_)
		{
			if (primitiveIndices_.empty() || primitivesAABBs_.empty())
			{
				std::cout << "BVH build fault! primitiveIndices_ is empty!" << std::endl;
				return;
			}

			primitiveIndices = primitiveIndices_;
			primitivesAABBs = primitivesAABBs_;

			// compute root AABB
			rootAABB.reset();
			for (auto aabb : primitivesAABBs)
			{
				rootAABB.merge(aabb);
			}

			// debug
			// std::cout << "rootAABB";
			// std::cout << rootAABB.lowerCorner;
			// std::cout << rootAABB.upperCorner;

			rootNode->aabb = rootAABB;

			indexIterType indexBegin = primitiveIndices.begin();
			indexIterType indexEnd = primitiveIndices.end();

			auto depth = recursiveBuid(rootNode, indexBegin, indexEnd, 0);
		}

		//  primitiveIndices begin iter, end iter,

		// size_t recursiveBuid(BVH3Node_Ptr rootNode, std::vector<size_t> &primitiveIndices, size_t currDepth)
		size_t recursiveBuid(BVH3Node_Ptr currNode, indexIterType indexBegin, indexIterType indexEnd, size_t currDepth)
		{

			// leaf node
			// if (primitiveIndices.size() == 1)

			size_t currIndicesSize = indexEnd - indexBegin;
			
			std::cout << "\ncurrIndicesSize: " << currIndicesSize << std::endl;

			if (currIndicesSize == 1)
			{
				currNode->primitiveIndex = primitiveIndices[0];
				currNode->aabb = primitivesAABBs[primitiveIndices[0]];

				return currDepth + 1;
			}

			// Compute node AABB by merging all children of the curr node.
			AABB3f nodeAABB;
			// for (size_t i = 0; i < primitiveIndices.size(); i++)
			// {
			// 	auto currPrimitiveIndex = primitiveIndices[i];
			// 	auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
			// 	nodeAABB.merge(currPrimitivesAABB);
			// }

			for (auto iter = indexBegin; iter != indexEnd; iter++)
			{
				auto currPrimitiveIndex = *iter;
				auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
				nodeAABB.merge(currPrimitivesAABB);
			}

			// debug
			// std::cout << "depth: " << currDepth;
			// std::cout << "\nnodeAABB\n" << nodeAABB.lowerCorner;
			// std::cout << nodeAABB.upperCorner;

			// Choose the axis with the longest span as the spliting axis
			auto nodeAABBSize = nodeAABB.upperCorner - nodeAABB.lowerCorner;
			size_t axis; // 0: x, 1: y, 2: z
			if (nodeAABBSize.x > nodeAABBSize.y && nodeAABBSize.x > nodeAABBSize.z)
			{
				axis = 0;
			}
			else
			{
				axis = (nodeAABBSize.y > nodeAABBSize.z) ? 1 : 2;
			}

			float pivotPosition = 0.5 * (nodeAABB.upperCorner[axis] + nodeAABB.lowerCorner[axis]);

			// debug
			std::cout << "depth: " << currDepth << std::endl;
			for (auto primitiveIndex : primitiveIndices)
			{
				std::cout << primitiveIndex << " ";
			}

			// split
			auto splitIter = qsplit(indexBegin, indexEnd, pivotPosition, axis);

			std::cout << "\nqsplited:\n";
			for (auto primitiveIndex : primitiveIndices)
			{
				std::cout << primitiveIndex << " ";
			}
			std::cout << "\nsplitIter: " << splitIter - indexBegin << std::endl;

			std::cout << indexBegin - indexBegin << " " << splitIter - indexBegin << " " << indexEnd - indexBegin << std::endl << std::endl;

			BVH3Node_Ptr LChildNode = std::make_shared<BVH3Node>();
			BVH3Node_Ptr RChildNode = std::make_shared<BVH3Node>();

			size_t depthL = recursiveBuid(LChildNode, indexBegin, splitIter, currDepth + 1);
			size_t depthR = recursiveBuid(RChildNode, splitIter, indexEnd, currDepth + 1);

			currNode->aabb = nodeAABB;
			currNode->LChild = LChildNode;
			currNode->RChild = RChildNode;

			return std::max(depthL, depthR);
		}

		indexIterType qsplit(indexIterType indexBegin, indexIterType indexEnd, float pivotPosition, size_t axis)
		{
			auto splitIter = indexBegin;
			for (auto iter = indexBegin; iter != indexEnd; iter++)
			{
				auto currPrimitiveIndex = *iter;
				auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
				float center = 0.5 * (currPrimitivesAABB.lowerCorner[axis] + currPrimitivesAABB.upperCorner[axis]);

				if (center < pivotPosition)
				{
					std::swap(*iter, *splitIter);
					splitIter++;
				}
			}

			// ill condition
			if (splitIter == indexBegin || splitIter == indexEnd)
			{
				// Divide equally
				splitIter += (indexEnd - indexBegin) / 2;
			}

			return splitIter;
		}

		std::vector<size_t> primitiveIndices;
		std::vector<AABB3<T>> primitivesAABBs;

		// root Node
		BVH3Node_Ptr rootNode;
		// AABB of root
		AABB3<T> rootAABB;

	}; // class BVH3

} // namespace HSim
