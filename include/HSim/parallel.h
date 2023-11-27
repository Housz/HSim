#pragma once

#include <HSim/common.h>

#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_reduce.h>

namespace HSim
{

	template <typename IteratorType, typename Function>
	void parallelFor(IteratorType begin, IteratorType end, const Function &func)
	{
		tbb::parallel_for_each(begin, end, func);
	}

	template <typename IteratorType, typename T>
	void parallelFill(IteratorType begin, IteratorType end, T value)
	{
		tbb::parallel_for_each(begin, end, [&](auto &i){
			i = value;
		});
	}

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

} // namespace HSim
