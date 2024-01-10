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
{
	public:
	virtual ~ShapeConcept() = default;
	virtual double cost() const = 0;
};

template<class ShapeType>
class ShapeModel : public ShapeConcept
{
	public:
	using CostStrategy = std::function<double(const ShapeType&)>;
	explicit ShapeModel(ShapeType shape, CostStrategy cost_strategy) : shape_{std::move(shape)}, cost_strategy_{std::move(cost_strategy)} {}
	virtual double cost() const override
	{
		return cost_strategy_(shape_);
	}
	private:
	ShapeType shape_;
	CostStrategy cost_strategy_;
};

// template<class Shape>
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

// template<class Shape>
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

using Shapes = std::vector<std::unique_ptr<ShapeConcept>>;

double total_cost(const Shapes& shapes)
{
	double sum = 0.0;
	for(const auto& shape : shapes)
	{
		sum += shape->cost();
	}
	return sum;
}

int main()
{
	using CircleModel = ShapeModel<Circle>;
	using SquareModel = ShapeModel<Square>;

	Shapes shapes{};

	// shapes.emplace_back(std::make_unique<CircleModel>(Circle{2.5}, AluminumCostStrategy<Circle>{}));
	shapes.emplace_back(std::make_unique<CircleModel>(Circle{2.5}, AluminumCostStrategy{}));
	shapes.emplace_back(std::make_unique<SquareModel>(Square{3.0}, SteelCostStrategy{}));
	shapes.emplace_back(std::make_unique<CircleModel>(Circle{4.0}, SteelCostStrategy{}));

	std::cout << total_cost(shapes) << "\n";

	return 0;
}
