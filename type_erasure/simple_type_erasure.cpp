// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <numbers>
#include <functional>
#include <vector>
#include <memory>

class Circle
{
	public:
	explicit Circle(double radius) : radius_{radius} {}
	double radius() const { return radius_; }
	double area() const { return std::numbers::pi_v<double> * radius_ * radius_; }
	private:
	double radius_;
};

class Square
{
	public:
	explicit Square(double side) : side_{side} {}
	double side() const { return side_; }
	double area() const { return side_ * side_; }
	private:
	double side_;
};

class ShapeConcept
{	// external polymorphism
	public:
	virtual ~ShapeConcept() = default;
	virtual double cost() const = 0;
	// prototype
	virtual std::unique_ptr<ShapeConcept> clone() const = 0;
};

template<class ShapeType>
class OwningShapeModel : public ShapeConcept
{	// external polymorphism
	public:
	// the following type definition could be another template parameter of this class to improve performance
	using CostStrategy = std::function<double(const ShapeType&)>;
	explicit OwningShapeModel(ShapeType shape, CostStrategy cost_strategy) : shape_{std::move(shape)}, cost_strategy_{std::move(cost_strategy)} {}
	virtual double cost() const override
	{
		return cost_strategy_(shape_);
	}
	virtual std::unique_ptr<ShapeConcept> clone() const override
	{	// prototype
		return std::make_unique<OwningShapeModel<ShapeType>>(*this);
	}
	private:
	ShapeType shape_;
	CostStrategy cost_strategy_;
};

class Shape
{	// type erasure
	public:
	template<class ShapeType, class CostStrategy>
	Shape(ShapeType shape, CostStrategy cost_strategy) : pimpl_{std::make_unique<OwningShapeModel<ShapeType>>(std::move(shape), std::move(cost_strategy))} {}
	Shape(const Shape& other) : pimpl_{other.pimpl_->clone()} {}
	Shape& operator=(const Shape& other)
	{	// copy and swap
		Shape copy(other);
		pimpl_.swap(copy.pimpl_);
		return *this;
	}
	~Shape() = default;
	Shape(Shape&&) = default;
	Shape& operator=(Shape&&) = default;
	private:
	// bridge
	std::unique_ptr<ShapeConcept> pimpl_;
	// the following could be implemented as member function as well
	friend double cost(const Shape& shape)
	{	// hidden friend to be injected enclosing namespace
		return shape.pimpl_->cost();
	}
};

class AluminumCostStrategy
{
	public:
	template<class Shape>
	double operator()(const Shape& shape) const
	{
		return cost_per_mm2_ * shape.area();
	}
	private:
	double cost_per_mm2_ = 2.0;
};

class SteelCostStrategy
{
	public:
	template<class Shape>
	double operator()(const Shape& shape) const
	{
		return cost_per_mm2_ * shape.area();
	}
	private:
	double cost_per_mm2_ = 5.0;
};

using Shapes = std::vector<Shape>;

double total_cost(const Shapes& shapes)
{
	double sum = 0.0;
	for(const auto& shape : shapes)
	{
		sum += cost(shape);
	}
	return sum;
}

int main()
{
	Shapes shapes{};

	shapes.emplace_back(Circle{2.5}, AluminumCostStrategy{});
	shapes.emplace_back(Square{3.0}, SteelCostStrategy{});
	shapes.emplace_back(Circle{4.0}, SteelCostStrategy{});

	std::cout << total_cost(shapes) << "\n";

	return 0;
}
