// References: 
// [1] Erich Gamma et al. Design Patterns. Elements of reusable object oriented software.
// [2] Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

#include <iostream>
#include <cmath>
#include <tuple>

namespace Concept
{
	namespace Impl
	{
		template<class Type>
		concept HasAreaAsMemberFcn = requires(Type shape)
		{
			{shape.area()} -> std::same_as<double>;
		};

		template<class Type>
		concept HasAreaAsFreeFcn = requires(Type shape)
		{
			{area(shape)} -> std::same_as<double>;
		};	
	}
	template<class Type>
	concept Shape = Impl::HasAreaAsMemberFcn<Type> || Impl::HasAreaAsFreeFcn<Type>;
}

// primitive shapes

class Square
{
	public:
	Square(double side) : side_{side} {}
	double area() const { return side_ * side_; }
	private:
	double side_;
};

class EquilateralTriangle
{
	public:
	EquilateralTriangle(double side) : side_{side} {}
	double area() const { return std::sqrt(3.0) / 4.0 * side_ * side_; }
	private:
	double side_;
};

// Compound/Composite shapes

template<Concept::Shape ...ShapeTypes>
class Polygon
{
	public:
	Polygon(ShapeTypes&& ...shapes) : shapes_{std::forward<ShapeTypes>(shapes)...} {}
	double area() const;
	private:
	std::tuple<ShapeTypes...> shapes_;
};

template<Concept::Shape ...ShapeTypes>
double Polygon<ShapeTypes...>::area() const
{
	return std::apply([](auto&& ...shapes)
		{
			return (shapes.area() + ...);
		},
		shapes_);
}

// free function impl

double area(const Square& square)
{
	return square.area();
}

double area(const EquilateralTriangle& triangle)
{
	return triangle.area();
}

template<Concept::Shape ...ShapeTypes>
double area(const std::tuple<ShapeTypes...>& polygon)
{
	return std::apply([](auto&& ...shapes)
		{
			return (area(shapes) + ...);
		},
		polygon);
}

int main()
{
	Polygon<Polygon<Square, EquilateralTriangle>, Square> polygon{{3.0, 3.0}, 3.0};
	std::cout << polygon.area() << "\n";
	// free function form
	std::tuple<Square, std::tuple<Square, EquilateralTriangle>> polygonTuple{3.0, {3.0, 3.0}};
	std::cout << area(polygonTuple) << "\n";
	return 0;
}
