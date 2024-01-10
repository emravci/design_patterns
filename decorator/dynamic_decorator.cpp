// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>

using Money = double;

class Item
{	// type erasure implementation
	public:
	template<class Type>
	Item(Type item) : pimpl_{std::make_unique<Model<Type>>(std::move(item))} {}
	Item(const Item& item) : pimpl_{item.pimpl_->clone()} {}
	Item& operator=(const Item& item) 
	{
		pimpl_ = item.pimpl_->clone();
		return *this;
	}
	~Item() = default;
	Item(Item&&) = default;
	Item& operator=(Item&&) = default;
	Money price() const { return pimpl_->price(); }
	private:
	// external polymorphism
	struct Concept
	{
		virtual ~Concept() = default;
		virtual Money price() const = 0;
		// prototype
		virtual std::unique_ptr<Concept> clone() const = 0;
	};
	template<class Type>
	struct Model : Concept
	{
		using self_type = Model<Type>;
		explicit Model(const Type& item) : item_{item} {}
		explicit Model(Type&& item) : item_{std::move(item)} {}
		virtual Money price() const override { return item_.price(); }
		// prototype
		virtual std::unique_ptr<Concept> clone() const override { return std::make_unique<self_type>(*this); }
		Type item_;
	};
	// bridge
	std::unique_ptr<Concept> pimpl_;
};

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

class Discounted
{
	public:
	Discounted(double discount, Item item) : factor_{1.0 - discount}, item_{std::move(item)} {}
	Money price() const { return item_.price() * factor_; }
	private:
	double factor_;
	Item item_;
};

class Taxed
{
	public:
	Taxed(double tax_rate, Item item) : factor_{1.0 + tax_rate}, item_{std::move(item)} {}
	Money price() const { return item_.price() * factor_; }
	private:
	double factor_;
	Item item_;
};

int main()
{
	Item effective_cpp{Taxed{0.2, Discounted{0.2, Book{"Effective C++", 100.0}}}};
	std::cout << effective_cpp.price() << "\n";
	return 0;
}
