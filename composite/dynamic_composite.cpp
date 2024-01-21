// References: 
// [1] Erich Gamma et al. Design Patterns. Elements of reusable object oriented software.
// [2] Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>
#include <cmath>
#include <vector>

template<class Shape>
concept ShapeConcept = requires(Shape shape)
{
	{shape.area()} -> std::same_as<double>;
};

class Shape
{	// type erasure
	public:
	template<ShapeConcept ShapeType>
	Shape(ShapeType shape) : pimpl_{std::make_unique<Model<ShapeType>>(std::move(shape))} {}
	Shape(const Shape& shape) : pimpl_{shape.pimpl_->clone()} {}
	Shape& operator=(const Shape& shape)
	{
		pimpl_ = shape.pimpl_->clone();
		return *this;
	}
	Shape(Shape&&) = default;
	Shape& operator=(Shape&&) = default;
	~Shape() = default;
	double area() const { return pimpl_->area(); }
	private:
	// external polymorphism
	class Concept
	{
		public:
		virtual ~Concept() = default;
		virtual double area() const = 0;
		// prototype
		virtual std::unique_ptr<Concept> clone() const = 0;
	};
	template<ShapeConcept ShapeType>
	class Model : public Concept
	{
		public:
		using self_type = Model<ShapeType>;
		explicit Model(const ShapeType& shape) : shape_{shape} {}
		explicit Model(ShapeType&& shape) : shape_{std::move(shape)} {}
		virtual double area() const override { return shape_.area(); }
		// prototype
		virtual std::unique_ptr<Concept> clone() const override { return std::make_unique<self_type>(*this); }
		private:
		ShapeType shape_;
	};
	// bridge
	std::unique_ptr<Concept> pimpl_;
};

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

// Compound/Composite shape

class Polygon
{
	public:
	template<ShapeConcept ...ShapeTypes>
	Polygon(ShapeTypes ...shapes) : shapes_{std::move(shapes)...} {}
	double area() const;
	private:
	std::vector<Shape> shapes_;
};

double Polygon::area() const
{
	double sum = 0.0;
	for(const auto& shape : shapes_)
	{
		sum += shape.area();
	}
	return sum;
}

int main()
{
	EquilateralTriangle triangle{3.0};
	Square square{3.0};
	Polygon pentagon{triangle, square};
	Polygon polygon{pentagon, square};
	std::cout << polygon.area() << "\n";
	return 0;
}
