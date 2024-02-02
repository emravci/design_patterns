// References:
// [1] Andrei Alexandrescu. Modern C++ Design: Generic Programming and Design Patterns Applied.
// [2] Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>

template<class Type> class Function;

template<class ReturnType, class ...Args>
class Function<ReturnType(Args...)>
{	// type erasure
	public:
	template<class CallableType>
	Function(CallableType object) : pimpl_{std::make_unique<Model<CallableType>>(std::move(object))} {}
	// the following two ctors are used to create a Function from an object and its member
	template<class CallableType>
	Function(CallableType object, ReturnType(CallableType::*pMemberFcn)(Args&&...))
	{
		using MemberFunctionPointerType = ReturnType(CallableType::*)(Args&&...);
		pimpl_ = std::make_unique<MemberFunction<CallableType, MemberFunctionPointerType>>(std::move(object), pMemberFcn);
	}
	template<class CallableType>
	Function(CallableType object, ReturnType(CallableType::*pMemberFcn)(Args&&...) const)
	{
		using MemberFunctionPointerType = ReturnType(CallableType::*)(Args&&...) const;
		pimpl_ = std::make_unique<MemberFunction<CallableType, MemberFunctionPointerType>>(std::move(object), pMemberFcn);
	}
	Function(const Function& function) : pimpl_{function.pimpl_->clone()} {}
	Function& operator=(const Function& function)
	{
		pimpl_ = function.pimpl_->clone();
		return *this;
	}
	Function(Function&&) = default;
	Function& operator=(Function&&) = default;
	~Function() = default;
	ReturnType operator()(Args&&... args) { return pimpl_->operator()(std::forward<Args>(args)...); }
	private:
	// external polymorphism
	class Concept;
	template<class CallableType> class Model;
	// free functions are also handled by Model
	template<class CallableType, class MemberFunctionPointerType> class MemberFunction;
	// bridge
	std::unique_ptr<Concept> pimpl_;
};

// external polymorphism definition 
template<class ReturnType, class ...Args>
class Function<ReturnType(Args...)>::Concept
{
	public:
	virtual ~Concept() = default;
	virtual ReturnType operator()(Args&&...) = 0;
	// prototype
	virtual std::unique_ptr<Concept> clone() const = 0;
};

template<class ReturnType, class ...Args>
template<class CallableType>
class Function<ReturnType(Args...)>::Model : public Concept
{
	public:
	using SelfType = Model<CallableType>;
	explicit Model(const CallableType& object) : object_{object} {}
	explicit Model(CallableType&& object) : object_{std::move(object)} {}
	virtual ReturnType operator()(Args&&... args) override { return object_(std::forward<Args>(args)...); }
	// prototype
	virtual std::unique_ptr<Concept> clone() const override { return std::make_unique<SelfType>(*this); }
	private:
	CallableType object_;
};

template<class ReturnType, class ...Args>
template<class CallableType, class MemberFunctionPointerType> 
class Function<ReturnType(Args...)>::MemberFunction : public Concept
{
	public:
	using SelfType = MemberFunction<CallableType, MemberFunctionPointerType>;
	explicit MemberFunction(const CallableType& object, MemberFunctionPointerType pMemberFcn) : object_{object}, pMemberFcn_{pMemberFcn} {}
	explicit MemberFunction(CallableType&& object, MemberFunctionPointerType pMemberFcn) : object_{std::move(object)}, pMemberFcn_{pMemberFcn} {}
	virtual ReturnType operator()(Args&&... args) override { return (object_.*pMemberFcn_)(std::forward<Args>(args)...); }
	// prototype
	virtual std::unique_ptr<Concept> clone() const override { return std::make_unique<SelfType>(*this); }
	private:
	CallableType object_;
	MemberFunctionPointerType pMemberFcn_;
};

// functions, function objects etc
double divide(int lhs, int rhs)
{
	return static_cast<double>(lhs) / rhs;
}

struct StatelessDivide
{
	double operator()(int lhs, int rhs) const { return static_cast<double>(lhs) / rhs; }
};

struct DivideCommand
{
	public:
	DivideCommand(int lhs, int rhs) : lhs_{lhs}, rhs_{rhs} {}
	double execute() const { return static_cast<double>(lhs_) / rhs_; }
	private:
	int lhs_, rhs_;
};

int main()
{
	StatelessDivide divideStateless;
	Function<double(int, int)> divideClass{divideStateless};
	std::cout << divideClass(1, 5) << "\n";
	Function<double(int, int)> divideFreeFunction{&divide};
	std::cout << divideFreeFunction(2, 5) << "\n";
	DivideCommand divideCommand{3, 5};
	Function<double()> divideMemberFunction{divideCommand, &DivideCommand::execute};
	std::cout << divideMemberFunction() << "\n";
	Function<double(int, int)> divideLambda([](int lhs, int rhs){ return static_cast<double>(lhs) / rhs; });
	std::cout << divideLambda(4, 5) << "\n";
	return 0;
}
