// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <stack>
#include <memory>

class CalculatorCommand
{
	public:
	virtual ~CalculatorCommand() = default;
	virtual int execute(int i) const = 0;
	virtual int undo(int i) const = 0;
};

class Add : public CalculatorCommand
{
	public:
	explicit Add(int operand) : operand_{operand} {}
	virtual int execute(int i) const override { return i + operand_; }
	virtual int undo(int i) const override { return i - operand_; }
	private:
	int operand_;
};

class Subtract : public CalculatorCommand
{
	public:
	explicit Subtract(int operand) : operand_{operand} {}
	virtual int execute(int i) const override { return i - operand_; }
	virtual int undo(int i) const override { return i + operand_; }
	private:
	int operand_;
};

class Calculator
{
	public:
	using CommandPtrType = std::unique_ptr<CalculatorCommand>;
	void compute(CommandPtrType);
	void undo_last();
	int result() const { return current_; }
	void clear();
	private:	
	using CommandStack = std::stack<CommandPtrType>;
	int current_ = 0;
	CommandStack stack_;
};

void Calculator::compute(Calculator::CommandPtrType command)
{
	current_ = command->execute(current_);
	stack_.push(std::move(command));
}

void Calculator::undo_last()
{
	if(stack_.empty() == true) return;
	auto command = std::move(stack_.top());
	stack_.pop();
	current_ = command->undo(current_);
}

void Calculator::clear()
{
	current_ = 0;
	CommandStack{}.swap(stack_);
}

int main()
{
	Calculator calculator{};
	calculator.compute(std::make_unique<Add>(3));
	calculator.compute(std::make_unique<Add>(7));
	calculator.compute(std::make_unique<Subtract>(4));
	calculator.compute(std::make_unique<Subtract>(2));
	calculator.undo_last();
	std::cout << calculator.result() << "\n";
	return 0;
}
