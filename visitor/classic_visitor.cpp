// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <vector>
#include <memory>
#include <numbers>

class Circle;
class Square;

class ShapeVisitor
{
	public:
	virtual ~ShapeVisitor() = default;
	virtual void visit(const Circle&) = 0;
	virtual void visit(const Square&) = 0;
};

class Shape
{
	public:
	virtual ~Shape() = default;
	virtual void accept(ShapeVisitor&) = 0;
};

class Circle : public Shape
{
	public:
	explicit Circle(double radius) : radius_{radius} {
		// check if the value is valid
	}
	virtual void accept(ShapeVisitor& v) override
	{
		v.visit(*this);
	}
	inline double radius() const
	{
		return radius_;
	}
	private:
	double radius_;
};

class Square : public Shape
{
	public:
	explicit Square(double side) : side_{side} {
		// check if the value is valid
	}
	virtual void accept(ShapeVisitor& v) override
	{
		v.visit(*this);
	}
	inline double side() const
	{
		return side_;
	}
	private:
	double side_;
};

class Area : public ShapeVisitor
{
	public:
	virtual void visit(const Circle& c) override
	{
		value_ = std::numbers::pi_v<double> * c.radius() * c.radius();
	}
	virtual void visit(const Square& s) override
	{
		value_ = s.side() * s.side();
	}
	inline double value() const
	{
		return value_;
	}
	private:
	double value_;
};

class Perimeter : public ShapeVisitor
{
	public:
	virtual void visit(const Circle& c) override
	{
		value_ = 2.0 * std::numbers::pi_v<double> * c.radius();
	}
	virtual void visit(const Square& s) override
	{
		value_ = 4.0 * s.side();
	}
	inline double value() const
	{
		return value_;
	}
	private:
	double value_;
};

using Shapes = std::vector<std::unique_ptr<Shape>>;

double total_area(const Shapes& shapes)
{
	double sum = 0.0;
	Area area{};
	for(const auto& shape : shapes)
	{
		shape->accept(area);
		sum += area.value();
	}
	return sum;
}

double total_perimeter(const Shapes& shapes)
{
	double sum = 0.0;
	Perimeter perimeter{};
	for(const auto& shape : shapes)
	{
		shape->accept(perimeter);
		sum += perimeter.value();
	}
	return sum;
}

int main()
{
	Shapes shapes;
	shapes.emplace_back(std::make_unique<Circle>(2.5));
	shapes.emplace_back(std::make_unique<Square>(3.0));
	shapes.emplace_back(std::make_unique<Circle>(4.0));

	std::cout << total_area(shapes) << "\n";
	std::cout << total_perimeter(shapes) << "\n";

	return 0;
}
