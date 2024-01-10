// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <string>
#include <memory>

class Animal
{
	public:
	virtual ~Animal() = default;
	virtual void make_sound() const = 0;
	virtual std::unique_ptr<Animal> clone() const = 0;
};

class Sheep : public Animal
{
	public:
	explicit Sheep(std::string name) : name_{std::move(name)} {}
	virtual void make_sound() const override 
	{
		std::cout << "baa\n";
	}
	virtual std::unique_ptr<Animal> clone() const override
	{
		return std::make_unique<Sheep>(*this);
	}
	private:
	std::string name_;
};

int main()
{
	std::unique_ptr<Animal> dolly = std::make_unique<Sheep>("dolly");
	dolly->make_sound();
	auto clone_of_dolly = dolly->clone();
	clone_of_dolly->make_sound();
	return 0;
}
