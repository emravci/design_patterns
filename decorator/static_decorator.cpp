// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

// NOTES:
// in the book, tax_rate and discount is to be fed as non-type template parameter
// since that might lead to code bloat especially in the case of double
// it is not preferred here, but the code in the book is still given as comments

#include <iostream>
#include <string>

using Money = double;

class Book
{
	public:
	Book(std::string title, Money price) : title_{std::move(title)}, price_{price} {}
	const std::string& title() const { return title_; }
	Money price() const { return price_; }
	private:
	std::string title_;
	Money price_;
};

template<class Type>
concept PricedItem = requires(Type item)
{
	{item.price()} -> std::same_as<Money>;
};

// template<double discount, PricedItem Item>
template<PricedItem Item>
class Discounted
{	// using composition
	public:
	template<class ...Args>
	explicit Discounted(double discount, Args&& ...args) : factor_{1.0 - discount}, item_{std::forward<Args>(args)...} {}
	// explicit Discounted(Args&& ...args) : item_{std::forward<Args>(args)...} {}
	Money price() const { return item_.price() * factor_; }
	private:
	double factor_;
	Item item_;
};

// template<double tax_rate, PricedItem Item>
template<PricedItem Item>
class Taxed : private Item
{	// using inheritance
	public:
	template<class ...Args>
	explicit Taxed(double tax_rate, Args&& ...args) : factor_{1.0 + tax_rate}, Item{std::forward<Args>(args)...} {}
	// explicit Taxed(Args&& ...args) : Item{std::forward<Args>(args)...} {}
	Money price() const { return Item::price() * factor_; }
	private:
	double factor_;
};

int main()
{
	// Taxed<0.20, Discounted<0.20, Book>> effective_cpp{"Effective C++", 100.0};
	// Taxed<Discounted<Book>> effective_cpp{0.2, Discounted<Book>{0.2, Book{"Effective C++", 100.0}}};
	Taxed<Discounted<Book>> effective_cpp{0.2, // 20% tax rate
		0.2, // 20% discount
			"Effective C++", 100.0}; // book title and price
	std::cout << effective_cpp.price() << "\n";
	return 0;
}
