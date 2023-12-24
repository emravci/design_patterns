// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <numbers>
#include <memory>
#include <vector>

class Shape; 

class MaterialCostStrategy
{
	public:
	virtual ~MaterialCostStrategy() = default;
	// in the reference book there is one strategy overload per Shape family class (Circle, Square etc)
	// here cost computation only relies on the area and material type, there is no need for individual overloads
	virtual double cost(const Shape&) const = 0;
};

class Shape
{
	public:
	virtual ~Shape() = default;
	virtual double area() const = 0;
	virtual double cost() const = 0;
	protected:
	using strategy_type = std::unique_ptr<MaterialCostStrategy>;
};

class Circle : public Shape
{
	public:
	explicit Circle(double radius, Shape::strategy_type material_cost_strategy) : radius_{radius}, material_cost_strategy_{std::move(material_cost_strategy)}
	{
		// check the length is valid.
	}
	inline double radius() const { return radius_; }
	virtual double area() const override { return std::numbers::pi_v<double> * radius_ * radius_; }
	virtual double cost() const override { return material_cost_strategy_->cost(*this); }
	private:
	double radius_;
	Shape::strategy_type material_cost_strategy_;
};

class Square : public Shape
{
	public:
	explicit Square(double side, Shape::strategy_type material_cost_strategy) : side_{side}, material_cost_strategy_{std::move(material_cost_strategy)}
	{
		// check the length is valid.
	}
	inline double side() const { return side_; }
	virtual double area() const override { return side_ * side_; }
	virtual double cost() const override { return material_cost_strategy_->cost(*this); }
	private:
	double side_;
	Shape::strategy_type material_cost_strategy_;
};

class AluminumCostStrategy : public MaterialCostStrategy
{
	public:
	virtual double cost(const Shape& shape) const override
	{
		return cost_per_mm2 * shape.area();
	}
	private:
	double cost_per_mm2 = 2.0;
};

class SteelCostStrategy : public MaterialCostStrategy
{
	public:
	virtual double cost(const Shape& shape) const override
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
	shapes.emplace_back(std::make_unique<Circle>(2.5, std::make_unique<AluminumCostStrategy>()));
	shapes.emplace_back(std::make_unique<Square>(3.0, std::make_unique<SteelCostStrategy>()));
	shapes.emplace_back(std::make_unique<Circle>(4.0, std::make_unique<SteelCostStrategy>()));

	std::cout << total_cost(shapes) << "\n";
	return 0;
}
