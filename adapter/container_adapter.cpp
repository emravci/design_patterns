#include <iostream>
#include <array>

template<class Type, std::size_t N>
class CircularBuffer
{
	public:
	CircularBuffer() = default;
	Type popFront() { return (*this)[out_++]; }
	void pushBack(const Type& element) { (*this)[in_++] = element; }
	private:
	class RangedInteger;
	Type& operator[](const RangedInteger& at) { return elements_[at.value()]; }
	RangedInteger in_;
	RangedInteger out_;
	std::array<Type, N> elements_;
};

template<class Type, std::size_t N>
class CircularBuffer<Type, N>::RangedInteger
{
	public:
	RangedInteger() : value_{0} {}
	std::size_t value() const { return value_; }
	RangedInteger operator++(int);
	private:
	bool isSafeForIncrement() const { return (value_ + 1) != N; }
	void wrapToBeginning() { value_ = 0; }
	std::size_t value_;
};

template<class Type, std::size_t N>
typename CircularBuffer<Type, N>::RangedInteger CircularBuffer<Type, N>::RangedInteger::operator++(int)
{
	RangedInteger valueBeforeIncrement = *this;
	if(isSafeForIncrement()) { ++value_; }
	else { wrapToBeginning(); }
	return valueBeforeIncrement;
}

int main()
{
	constexpr std::size_t bufferSize = 5;
	CircularBuffer<int, bufferSize> buffer;
	for(int i = 0; i < bufferSize; ++i) { buffer.pushBack(i); }
	for(int i = 0; i < bufferSize; ++i) { std::cout << buffer.popFront() << "\n"; }
	return 0;
}
