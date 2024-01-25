// References: 
// [1] Erich Gamma et al. Design Patterns. Elements of reusable object oriented software.
// [2] Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>
#include <vector>

template<class Expression>
concept ExpressionConcept = requires(Expression expression)
{
	{expression.evaluate()} -> std::same_as<double>;
};

class Expression
{	// type erasure implementation
	public:
	template<ExpressionConcept ExpressionType>
	Expression(ExpressionType expression) : pimpl_{std::make_unique<Model<ExpressionType>>(std::move(expression))} {}
	Expression(const Expression& expression) : pimpl_{expression.pimpl_->clone()} {}
	Expression& operator=(const Expression& expression) 
	{
		pimpl_ = expression.pimpl_->clone();
		return *this;
	}
	~Expression() = default;
	Expression(Expression&&) = default;
	Expression& operator=(Expression&&) = default;
	double evaluate() const { return pimpl_->evaluate(); }
	private:
	// external polymorphism
	struct Concept
	{
		virtual ~Concept() = default;
		virtual double evaluate() const = 0;
		// prototype
		virtual std::unique_ptr<Concept> clone() const = 0;
	};
	template<ExpressionConcept ExpressionType>
	struct Model : Concept
	{
		using self_type = Model<ExpressionType>;
		explicit Model(const ExpressionType& expression) : expression_{expression} {}
		explicit Model(ExpressionType&& expression) : expression_{std::move(expression)} {}
		virtual double evaluate() const override { return expression_.evaluate(); }
		// prototype
		virtual std::unique_ptr<Concept> clone() const override { return std::make_unique<self_type>(*this); }
		ExpressionType expression_;
	};
	// bridge
	std::unique_ptr<Concept> pimpl_;
};

// primitive expression (value expression)

class Value
{
	public:
	Value(double value) : value_{value} {}
	double evaluate() const { return value_; }
	private:
	double value_;
};

// composite expression types

class Addition
{
	public:
	Addition() = delete;
	template<ExpressionConcept ExpressionType> Addition(ExpressionType) = delete;
	template<ExpressionConcept ...ExpressionTypes>
	Addition(ExpressionTypes ...expressions) : expressions_{std::move(expressions)...} {}
	double evaluate() const;
	private:
	std::vector<Expression> expressions_;
};

double Addition::evaluate() const
{
	double result = 0.0;
	for(const auto& expression : expressions_)
	{
		result += expression.evaluate();
	}
	return result;
}

class Multiplication
{
	public:
	Multiplication() = delete;
	template<ExpressionConcept ExpressionType> Multiplication(ExpressionType) = delete;
	template<ExpressionConcept ...ExpressionTypes>
	Multiplication(ExpressionTypes ...expressions) : expressions_{std::move(expressions)...} {}
	double evaluate() const;
	private:
	std::vector<Expression> expressions_;
};

double Multiplication::evaluate() const
{
	double result = 1.0;
	for(const auto& expression : expressions_)
	{
		result *= expression.evaluate();
	}
	return result;
}

class Subtraction
{
	public:
	Subtraction() = delete;
	template<ExpressionConcept ExpressionType> Subtraction(ExpressionType) = delete;
	template<ExpressionConcept ...ExpressionTypes>
	Subtraction(ExpressionTypes ...expressions) : expressions_{std::move(expressions)...} {}
	double evaluate() const;
	private:
	std::vector<Expression> expressions_;
};

double Subtraction::evaluate() const
{
	double result = expressions_[0].evaluate();
	for(std::size_t i = 1, n = expressions_.size(); i < n; ++i)
	{
		result -= expressions_[i].evaluate();
	}
	return result;
}

class Division
{
	public:
	Division() = delete;
	template<ExpressionConcept ExpressionType> Division(ExpressionType) = delete;
	template<ExpressionConcept ...ExpressionTypes>
	Division(ExpressionTypes ...expressions) : expressions_{std::move(expressions)...} {}
	double evaluate() const;
	private:
	std::vector<Expression> expressions_;
};

double Division::evaluate() const
{
	double result = expressions_[0].evaluate();
	for(std::size_t i = 1, n = expressions_.size(); i < n; ++i)
	{
		result /= expressions_[i].evaluate();
	}
	return result;
}

Addition operator+(const Expression& lhs, const Expression& rhs)
{
	return {lhs, rhs};
}

Subtraction operator-(const Expression& lhs, const Expression& rhs)
{
	return {lhs, rhs};
}

Multiplication operator*(const Expression& lhs, const Expression& rhs)
{
	return {lhs, rhs};
}

Division operator/(const Expression& lhs, const Expression& rhs)
{
	return {lhs, rhs};
}

int main()
{
	Value one{1.0};
	Value two{2.0};
	Value three{3.0};
	Value four{4.0};
	auto eight = Addition{one, three, four};
	auto thirtyTwo = eight * four;
	auto sixteen = thirtyTwo / two;
	std::cout << sixteen.evaluate() << "\n";
	return 0;
}
