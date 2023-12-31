// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <numbers>
#include <memory>
#include <vector>

class Shape
{
	public:
	virtual ~Shape() = default;
	virtual double area() const = 0;
	virtual double cost() const = 0;
};

template<class MaterialCostStrategy>
class Circle : public Shape
{
	public:
	explicit Circle(double radius, MaterialCostStrategy material_cost_strategy) : radius_{radius}, material_cost_strategy_{std::move(material_cost_strategy)}
	{
		// check the length is valid.
	}
	inline double radius() const { return radius_; }
	virtual double area() const override { return std::numbers::pi_v<double> * radius_ * radius_; }
	virtual double cost() const override { return material_cost_strategy_.cost(*this); }
	private:
	double radius_;
	MaterialCostStrategy material_cost_strategy_;
};

template<class MaterialCostStrategy>
class Square : public Shape
{
	public:
	explicit Square(double side, MaterialCostStrategy material_cost_strategy) : side_{side}, material_cost_strategy_{std::move(material_cost_strategy)}
	{
		// check the length is valid.
	}
	inline double side() const { return side_; }
	virtual double area() const override { return side_ * side_; }
	virtual double cost() const override { return material_cost_strategy_.cost(*this); }
	private:
	double side_;
	MaterialCostStrategy material_cost_strategy_;
};

class AluminumCostStrategy
{
	public:
	double cost(const Shape& shape) const
	{
		return cost_per_mm2 * shape.area();
	}
	private:
	double cost_per_mm2 = 2.0;
};

class SteelCostStrategy
{
	public:
	double cost(const Shape& shape) const
	{
		return cost_per_mm2 * shape.area();
	}
	private:
	double cost_per_mm2 = 5.0;
};

using Shapes = std::vector<std::unique_ptr<Shape>>;

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
	Shapes shapes{};
	shapes.emplace_back(std::make_unique<Circle<AluminumCostStrategy>>(2.5, AluminumCostStrategy{}));
	shapes.emplace_back(std::make_unique<Square<SteelCostStrategy>>(3.0, SteelCostStrategy{}));
	shapes.emplace_back(std::make_unique<Circle<SteelCostStrategy>>(4.0, SteelCostStrategy{}));

	std::cout << total_cost(shapes) << "\n";
	return 0;
}
