#pragma once

#include <HSim/common.h>

#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/blocked_range3d.h>


namespace HSim
{

	template <typename IteratorType, typename Function>
	void parallelForEach(IteratorType begin, IteratorType end, const Function &func)
	{
		tbb::parallel_for_each(begin, end, func);
	}

	template <typename IteratorType, typename T>
	void parallelFillEach(IteratorType begin, IteratorType end, T value)
	{
		tbb::parallel_for_each(begin, end, [&](auto &i){
			i = value;
		});
	}

	template <typename IndexType, typename Function>
	void parallelFor(IndexType startIndex, IndexType endIndex, Function& func)
	{
		if(startIndex > endIndex)
			return;

		tbb::parallel_for(startIndex, endIndex, func);
	}

	
	// void parallelFill()
	// {

	// }


	template <typename IteratorType, typename T, typename Function, typename Reduce>
	T parallelReduce(
		IteratorType begin, IteratorType end,
		T identity,
		const Function& func, 
		const Reduce& reduce
	)
	{
		size_t n = end - begin;
		
		return tbb::parallel_reduce(
			tbb::blocked_range<size_t>(0, n),
			identity,
			func,
			reduce
		);
	}

	template <typename T, typename Function, typename Reduce>
	T parallelReduce(
		size_t begin, size_t end,
		T identity,
		const Function& func, 
		const Reduce& reduce
	)
	{
		return tbb::parallel_reduce(
			tbb::blocked_range<size_t>(begin, end),
			identity,
			func,
			reduce
		);
	}

} // namespace HSim
