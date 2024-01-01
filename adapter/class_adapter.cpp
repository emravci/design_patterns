// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <vector>
#include <memory>

class Shape
{
	public:
	virtual ~Shape() = default;
	virtual double area() const = 0;
};

class Circle : public Shape
{
	public:
	explicit Circle(double radius) : radius_{radius}
	{
		// check the length is valid.
	}
	inline double radius() const { return radius_; }
	virtual double area() const override { return std::numbers::pi_v<double> * radius_ * radius_; }
	private:
	double radius_;
};

class Square : public Shape
{
	public:
	explicit Square(double side) : side_{side}
	{
		// check the length is valid.
	}
	inline double side() const { return side_; }
	virtual double area() const override { return side_ * side_; }
	private:
	double side_;
};

namespace ReadOnly
{
	class Rectangle
	{
		public:
		using pair_type = std::pair<double, double>;
		Rectangle(double side, double other_side) : sides_{side, other_side} {}
		inline const pair_type& sides() const { return sides_; }
		private:
		pair_type sides_;
	};

	double area(const Rectangle& rectangle)
	{
		return rectangle.sides().first * rectangle.sides().second;
	}
}

class Rectangle : public Shape, private ReadOnly::Rectangle
{
	public:
	Rectangle(double side, double other_side) : ReadOnly::Rectangle{side, other_side} {}
	virtual double area() const override { return ReadOnly::area(*this); }
};

using Shapes = std::vector<std::unique_ptr<Shape>>;

double total_area(const Shapes& shapes)
{
	double sum = 0.0;
	for(const auto& shape : shapes)
	{
		sum += shape->area();
	}
	return sum;
}

int main()
{
	Shapes shapes;
	shapes.emplace_back(std::make_unique<Circle>(3.));
	shapes.emplace_back(std::make_unique<Square>(4.));
	shapes.emplace_back(std::make_unique<Rectangle>(5., 3.));

	std::cout << total_area(shapes) << "\n";
	return 0;
}
