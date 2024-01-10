// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>

class Engine
{
	public:
	virtual ~Engine() = default;
	virtual void start() = 0;
	virtual void stop() = 0;
};

class ElectricEngine : public Engine
{
	public:
	virtual void start() override {}
	virtual void stop() override {}
};

class CombustionEngine : public Engine
{
	public:
	virtual void start() override {}
	virtual void stop() override {}
};

class Car
{
	protected:
	explicit Car(std::unique_ptr<Engine> engine) : pimpl_{std::move(engine)} {}
	public:
	virtual ~Car() = default;
	protected:
	Engine* get_engine() { return pimpl_.get(); }
	const Engine* get_engine() const { return pimpl_.get(); }
	private:
	std::unique_ptr<Engine> pimpl_;
};

class ElectricCar : public Car
{
	public:
	explicit ElectricCar() : Car{std::make_unique<ElectricEngine>()} {}
	private:
};

int main()
{
	return 0;
}
