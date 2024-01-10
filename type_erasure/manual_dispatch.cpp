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

class Shape
{
	public:
	template<class ShapeType, class CostStrategy>
	Shape(ShapeType shape, CostStrategy cost_strategy) : pimpl_{new OwningModel<ShapeType>(std::move(shape), std::move(cost_strategy)), [](void* shape_ptr)
	{
		using ModelType = OwningModel<ShapeType>;
		auto* const model = static_cast<ModelType*>(shape_ptr);
		delete model;
	}}, cost_([](void* shape_ptr) -> double
	{
		using ModelType = OwningModel<ShapeType>;
		auto* const model = static_cast<ModelType*>(shape_ptr);
		// in the book, std::function wasnt used, which changes the following line into (*model->cost_strategy_)(model->shape);
		return model->cost_strategy_(model->shape_);
	}), clone_([](void* shape_ptr) -> void*
	{	// this is only used internally while constructing unique_ptr, therefore, return new does not lead to leak
		using ModelType = OwningModel<ShapeType>;
		auto* const model = static_cast<ModelType*>(shape_ptr);
		return new ModelType(*model);
	}) {}
	Shape(const Shape& other) : pimpl_{clone_(other.pimpl_.get()), other.pimpl_.get_deleter()}, cost_{other.cost_}, clone_{other.clone_} {}
	Shape& operator=(const Shape& other)
	{	// copy and swap
		using std::swap;
		Shape copy(other);
		swap(pimpl_, copy.pimpl_);
		swap(cost_, copy.cost_);
		swap(clone_, copy.clone_);
		return *this;
	}
	~Shape() = default;
	Shape(Shape&&) = default;
	Shape& operator=(Shape&&) = default;
	private:
	template<class ShapeType>
	struct OwningModel
	{	// the following type definition could be another template type of this class to improve performance		
		using CostStrategy = std::function<double(const ShapeType&)>;
		OwningModel(ShapeType shape, CostStrategy cost_strategy) : shape_{std::move(shape)}, cost_strategy_{std::move(cost_strategy)} {}
		ShapeType shape_;
		CostStrategy cost_strategy_;
	};
	using DtorFcnType = void(void*);
	using CostFcnType = double(void*);
	using CloneFcnType = void*(void*);
	std::unique_ptr<void, DtorFcnType*> pimpl_;
	CostFcnType* cost_;
	CloneFcnType* clone_;
	friend double cost(const Shape& shape)
	{
		return (*shape.cost_)(shape.pimpl_.get());
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
