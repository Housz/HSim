#pragma once

#include <iostream>
#include <functional>

#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>

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

} // namespace HSim
