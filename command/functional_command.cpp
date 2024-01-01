// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <stack>
#include <functional>

struct CalculatorCommand
{
	using function_type = std::function<int(int)>;
	const function_type execute;
	const function_type undo;
};

CalculatorCommand make_add(int operand)
{
	struct Execute
	{
		const int operand_;
		int operator()(int i)
		{
			return i + operand_;
		}
	};
	struct Undo
	{
		const int operand_;
		int operator()(int i)
		{
			return i - operand_;
		}
	};
	CalculatorCommand calc_comm{Execute{operand}, Undo{operand}};
	return calc_comm;
}

CalculatorCommand make_subtract(int operand)
{
	struct Execute
	{
		const int operand_;
		int operator()(int i)
		{
			return i - operand_;
		}
	};
	struct Undo
	{
		const int operand_;
		int operator()(int i)
		{
			return i + operand_;
		}
	};
	CalculatorCommand calc_comm{Execute{operand}, Undo{operand}};
	return calc_comm;
}

class Calculator
{
	public:
	void compute(CalculatorCommand);
	void undo_last();
	int result() const { return current_; }
	void clear();
	private:	
	using CommandStack = std::stack<CalculatorCommand>;
	int current_ = 0;
	CommandStack stack_;
};

void Calculator::compute(CalculatorCommand command)
{
	current_ = command.execute(current_);
	stack_.push(std::move(command));
}

void Calculator::undo_last()
{
	if(stack_.empty() == true) return;
	auto command = std::move(stack_.top());
	stack_.pop();
	current_ = command.undo(current_);
}

void Calculator::clear()
{
	current_ = 0;
	CommandStack{}.swap(stack_);
}

int main()
{
	Calculator calculator{};

	calculator.compute(make_add(3));
	calculator.compute(make_add(7));
	calculator.compute(make_subtract(4));
	calculator.compute(make_subtract(2));
	calculator.undo_last();

	std::cout << calculator.result() << "\n";
	return 0;
}
