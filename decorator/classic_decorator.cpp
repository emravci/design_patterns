// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>
#include <string>
#include <cmath>

using Money = double;

class Item
{
	public:
	virtual ~Item() = default;
	virtual Money price() const = 0;
};

class DecoratedItem : public Item
{
	public:
	explicit DecoratedItem(std::unique_ptr<Item> item) : item_{std::move(item)}
	{
		if(!item_) throw std::invalid_argument("Invalid item!");
	}
	protected:
	Item& item() { return *item_; }
	const Item& item() const { return *item_; }
	private:
	std::unique_ptr<Item> item_;
};

class CppBook : public Item
{
	public:
	CppBook(std::string title, Money price) : title_{std::move(title)}, price_{price} {}
	const std::string& title() const { return title_; }
	virtual Money price() const override { return price_; }
	private:
	std::string title_;
	Money price_;
};

class ConferenceTicket : public Item
{
	public:
	ConferenceTicket(std::string title, Money price) : title_{std::move(title)}, price_{price} {}
	const std::string& title() const { return title_; }
	virtual Money price() const override { return price_; }
	private:
	std::string title_;
	Money price_;
};

class Discounted : public DecoratedItem
{
	public:
	Discounted(double discount, std::unique_ptr<Item> item) : DecoratedItem{std::move(item)}, factor_{1.0 - discount} 
	{
		if(discount < 0.0 || discount > 1.0) throw std::invalid_argument("Invalid discount!");
	}
	virtual Money price() const override { return item().price() * factor_; }
	private:
	double factor_;
};

class Taxed : public DecoratedItem
{
	public:
	Taxed(double tax_rate, std::unique_ptr<Item> item) : DecoratedItem{std::move(item)}, factor_{1.0 + tax_rate} 
	{
		if(!std::isfinite(tax_rate) || tax_rate < 0.0) throw std::invalid_argument("Invalid tax rate!");
	}
	virtual Money price() const override { return item().price() * factor_; }
	private:
	double factor_;
};

int main()
{
	std::unique_ptr<Item> effective_cpp = 
		std::make_unique<Taxed>(0.20, // 20% tax rate
			std::make_unique<Discounted>(0.20, // 20% discount 
				std::make_unique<CppBook>("Effective C++", 100.0)));
	std::cout << effective_cpp->price() << "\n";
	return 0;
}
