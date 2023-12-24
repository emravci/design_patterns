// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <variant>
#include <vector>
#include <numbers>

class Circle 
{
	public:
	explicit Circle(double radius) : radius_{radius} {
		// check if the value is valid
	}
	inline double radius() const
	{
		return radius_;
	}
	private:
	double radius_;
};

class Square
{
	public:
	explicit Square(double side) : side_{side} {
		// check if the value is valid
	}
	inline double side() const
	{
		return side_;
	}
	private:
	double side_;
};

struct Area 
{
	public:
	inline double operator()(const Circle& c)
	{
		return std::numbers::pi_v<double> * c.radius() * c.radius();
	}
	inline double operator()(const Square& s)
	{
		return s.side() * s.side();
	}
};

struct Perimeter
{
	inline double operator()(const Circle& c)
	{
		return 2.0 * std::numbers::pi_v<double> * c.radius();
	}
	inline double operator()(const Square& s)
	{
		return 4.0 * s.side();
	}
};

using Shape = std::variant<Circle, Square>;
using Shapes = std::vector<Shape>;

double total_area(const Shapes& shapes)
{
	double sum = 0.0;
	Area area{};
	for(const auto& shape : shapes)
	{
		sum += std::visit(area, shape);
	}
	return sum;
}

double total_perimeter(const Shapes& shapes)
{
	double sum = 0.0;
	Perimeter perimeter{};
	for(const auto& shape : shapes)
	{
		sum += std::visit(perimeter, shape);
	}
	return sum;
}

int main()
{
	Shapes shapes;
	shapes.emplace_back(Circle{2.5});
	shapes.emplace_back(Square{3.0});
	shapes.emplace_back(Circle{4.0});

	std::cout << total_area(shapes) << "\n";
	std::cout << total_perimeter(shapes) << "\n";
	return 0;
}
