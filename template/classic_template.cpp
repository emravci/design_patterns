// References: 
// [1] Erich Gamma et al. Design Patterns. Elements of reusable object oriented software.
// [2] Gerald Jay Sussman, Hal Abelson, and Julie Sussman. Structure and Interpretation of Computer Programs. MIT Press.

#include <iostream>

class Sum
{
	public:
	Sum(long long begin, long long closedEnd, long long increment) : iterator_{begin, closedEnd, increment} {}
	virtual ~Sum() = default;
	// evaluate() is a template method
	double evaluate() const;
	private:
	// polymorphic behaviour of evaluate() is controlled by doEvaluate()
	virtual double doEvaluate(long long index) const  = 0;
	class Iterator
	{
		public:
		Iterator(long long begin, long long closedEnd, long long increment) : begin_{begin}, closedEnd_{closedEnd}, increment_{increment}, index_{begin} {}
		// begin and next functions modify mutable variable
		void begin() const { index_ = begin_; }
		void next() const { index_ += increment_; }
		bool isDone() const { return index_ > closedEnd_; }
		long long index() const { return index_; }
		private:
		const long long begin_;
		const long long closedEnd_;
		const long long increment_;
		mutable long long index_;
	};
	Iterator iterator_;
};

double Sum::evaluate() const
{
	double sum = 0.0;
	for(iterator_.begin(); iterator_.isDone() == false; iterator_.next())
	{
		sum += doEvaluate(iterator_.index());
	}
	return sum;
}

class SimpleSum : public Sum
{
	public:
	SimpleSum(long long begin, long long closedEnd, long long increment) : Sum{begin, closedEnd, increment} {}
	private:
	virtual double doEvaluate(long long) const override;
};

double SimpleSum::doEvaluate(long long index) const
{
	return index;
}

class SquarredSum : public Sum
{
	public:
	SquarredSum(long long begin, long long closedEnd, long long increment) : Sum{begin, closedEnd, increment} {}
	private:
	virtual double doEvaluate(long long) const override;
};

double SquarredSum::doEvaluate(long long index) const
{
	return index * index;
}

class LeibnitzSum : public Sum
{
	public:
	LeibnitzSum(long long begin, long long closedEnd, long long increment) : Sum{begin, closedEnd, increment} {}
	private:
	virtual double doEvaluate(long long) const override;
};

double LeibnitzSum::doEvaluate(long long index) const
{
	return 1.0 / index / (index + 2);
}

int main()
{
	SimpleSum simpleSum{1, 5, 1};
	std::cout << simpleSum.evaluate() << "\n";
	SquarredSum squarredSum{1, 5, 1};
	std::cout << squarredSum.evaluate() << "\n";
	LeibnitzSum piOverEight(1, 5000, 4);
	std::cout << piOverEight.evaluate() * 8 << "\n";
	return 0;
}
