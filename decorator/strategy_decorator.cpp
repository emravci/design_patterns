// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>
#include <string>
#include <cmath>

using Money = double;

class PriceStrategy
{
	public:
	virtual ~PriceStrategy() = default;
	virtual Money update(Money) const = 0;
};

class DecoratedPriceStrategy : public PriceStrategy
{
	public:
	explicit DecoratedPriceStrategy(std::unique_ptr<PriceStrategy> price_strategy) : price_strategy_{std::move(price_strategy)} 
	{
		if(!price_strategy_) throw std::invalid_argument("Invalid price strategy!");
	}
	protected:
	PriceStrategy& price_strategy() { return *price_strategy_; }
	const PriceStrategy& price_strategy() const { return *price_strategy_; }
	private:
	std::unique_ptr<PriceStrategy> price_strategy_;
};

class StandardPriceStrategy : public PriceStrategy
{
	public:
	virtual Money update(Money price) const override { return price; }
};

class DiscountedPriceStrategy : public DecoratedPriceStrategy
{
	public:
	DiscountedPriceStrategy(double discount, std::unique_ptr<PriceStrategy> price_strategy) : factor_{1.0 - discount}, DecoratedPriceStrategy{std::move(price_strategy)} 
	{
		if(discount < 0.0 || discount > 1.0) throw std::invalid_argument("Invalid discount!");
	}
	virtual Money update(Money price) const override { return price_strategy().update(price) * factor_; }
	private:
	double factor_;
};

class TaxedPriceStrategy : public DecoratedPriceStrategy
{
	public:
	TaxedPriceStrategy(double tax_rate, std::unique_ptr<PriceStrategy> price_strategy) : factor_{1.0 + tax_rate}, DecoratedPriceStrategy{std::move(price_strategy)} 
	{
		if(!std::isfinite(tax_rate) || tax_rate < 0.0) throw std::invalid_argument("Invalid tax rate!");
	}
	virtual Money update(Money price) const override { return price_strategy().update(price) * factor_; }
	private:
	double factor_;
};

class Item
{
	public:
	virtual ~Item() = default;
	virtual Money price() const = 0;
};

class Book : public Item
{
	public:
	Book(std::string title, Money price, std::unique_ptr<PriceStrategy> price_strategy) : title_{std::move(title)}, price_{price}, price_strategy_{std::move(price_strategy)} {}
	const std::string& title() const { return title_; }
	virtual Money price() const override { return price_strategy_->update(price_); }
	private:
	std::string title_;
	Money price_;
	std::unique_ptr<PriceStrategy> price_strategy_;
};

int main()
{
	std::unique_ptr<Item> effective_cpp = std::make_unique<Book>("Effective C++", 100.0,
		std::make_unique<TaxedPriceStrategy>(0.20,
			std::make_unique<DiscountedPriceStrategy>(0.20,
				std::make_unique<StandardPriceStrategy>())));
	std::cout << effective_cpp->price() << "\n";
	return 0;
}
