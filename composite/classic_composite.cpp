// Reference: 
// Erich Gamma et al.
// Design Patterns.
// Elements of reusable object oriented software.

#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
// #include <concepts>

class Shape
{
	public:
	virtual ~Shape() = default;
	virtual double area() const = 0;	
};

class Square : public Shape
{
	public:
	Square(double side) : side_{side} {}
	virtual double area() const override { return side_ * side_; }
	private:
	double side_;
};

class EquilateralTriangle : public Shape
{
	public:
	EquilateralTriangle(double side) : side_{side} {}
	virtual double area() const override { return std::sqrt(3.0) / 4.0 * side_ * side_; }
	private:
	double side_;
};

class CompositeShape : public Shape
{
	public:
	virtual ~CompositeShape() = default;
	void addShape(std::unique_ptr<Shape> shape) { components_.push_back(std::move(shape)); }
	virtual double area() const override;
	protected:
	CompositeShape() = default;
	private:
	std::vector<std::unique_ptr<Shape>> components_;
};

double CompositeShape::area() const
{
	double sum = 0.0;
	for(const auto& shape : components_)
	{
		sum += shape->area();
	}
	return sum;
}

class Polygon : public CompositeShape
{
	public:
	// instead of any class, a concept could be used
	template<class ...Args>
	Polygon(std::unique_ptr<Args> ...args) : CompositeShape{} 
	{	// fold expression
		(this->addShape(std::move(args)), ...);
	}
	private:
};

// class SeparateShapes : public CompositeShape {};

int main()
{
	std::unique_ptr<CompositeShape> polygon = std::make_unique<Polygon>(
		std::make_unique<EquilateralTriangle>(3.0),
		std::make_unique<Square>(3.0)
	);
	polygon->addShape(std::make_unique<Square>(3.0));
	std::cout << polygon->area() << "\n";
	return 0;
}
