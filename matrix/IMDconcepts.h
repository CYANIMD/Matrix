#ifndef __IMD_CONCEPTS_
#define __IMD_CONCEPTS_

#include <concepts>

namespace IMD {
	template<typename T>
	concept Integral = std::is_integral_v<T>;
	template<typename T>
	concept Floating = std::is_floating_point_v<T>;
	template<typename T>
	concept Numeric = Integral<T> || Floating<T>;
	template<typename T, typename T1>
	using ResultTypeSum = decltype(std::declval<T>() + std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeMinus = decltype(std::declval<T>() - std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeProduct = decltype(std::declval<T>()* std::declval<T1>());
	template<typename T, typename T1>
	using ResultTypeDivision = decltype(std::declval<T>() / std::declval<T1>());
}

#endif // !__IMD_CONCEPTS_

