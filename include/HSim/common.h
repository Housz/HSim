#pragma once

#include <iostream>
#include <functional>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>
#include <utility>
#include <memory>
#include <array>
#include <queue>
#include <thread>
#include <mutex>
#include <random>


const double PI = 			3.14159265358979323846264338327950288;
const double PI_DOUBLE = 	PI * 2;
const double PI_QUAD = 		PI * 4;
const double PI_HALF = 		1.57079632679489661923132169163975144;
const double PI_QUARTER = 	0.785398163397448309615660845819875721;

const double EPSILON = std::numeric_limits<double>::epsilon();
const float MAX_FLOAT = std::numeric_limits<float>::max();
const double MAX_DOUBLE = std::numeric_limits<double>::max();
