#ifndef TYPETRAITSUTILS_HPP
#define TYPETRAITSUTILS_HPP

#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

namespace registration {

// Returns "integral", "floating-point", "string", or "pointer"
template <typename TypeParam>
std::string describeType() {
    if constexpr (std::is_pointer_v<TypeParam>) {
        return "pointer";
    } else if constexpr (std::is_same_v<TypeParam, std::string>) {
        return "string";
    } else if constexpr (std::is_integral_v<TypeParam>) {
        return "integral";
    } else if constexpr (std::is_floating_point_v<TypeParam>) {
        return "floating-point";
    } else {
        return "other";
    }
}

// True when TypeParam is integral or floating-point
template <typename TypeParam>
bool isNumeric() {
    return (std::is_integral_v<TypeParam> || std::is_floating_point_v<TypeParam>);
}

// True when both types are the same
template <typename FirstType, typename SecondType>
bool areSameType() {
    return std::is_same_v<FirstType, SecondType>;
}

// Convert a numeric value to string.
// Integral    -> std::to_string (plain integer representation).
// Float/double -> formatted to 2 decimal places via ostringstream.
template <typename NumericType>
std::string numericToString(NumericType val) {
    static_assert(std::is_arithmetic_v<NumericType>,
                  "numericToString requires a numeric type");
    if constexpr (std::is_integral_v<NumericType>) {
        return std::to_string(val);
    } else {
        std::ostringstream oss;
        constexpr int kPrecisionTwo = 2;
        oss << std::fixed << std::setprecision(kPrecisionTwo) << static_cast<double>(val);
        return oss.str();
    }
}

// Add two arithmetic values; if both are integral return long long,
// otherwise return double (the common arithmetic type).
template <typename FirstType, typename SecondType>
auto safeAdd(FirstType lhs, SecondType rhs) {
    static_assert(std::is_arithmetic_v<FirstType> && std::is_arithmetic_v<SecondType>,
                  "safeAdd requires numeric types");
    if constexpr (std::is_integral_v<FirstType> && std::is_integral_v<SecondType>) {
        return (static_cast<long long>(lhs) + static_cast<long long>(rhs));
    } else {
        return (static_cast<double>(lhs) + static_cast<double>(rhs));
    }
}

// Return the zero/default value for a type (0 for numbers, "" for string)
template <typename TypeParam>
TypeParam getZero() {
    return TypeParam{};
}

} // namespace registration

#endif // TYPETRAITSUTILS_HPP
