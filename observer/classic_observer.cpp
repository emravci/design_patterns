// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

// Note that this code is almost identical to the one given in the book.

#include <iostream>
#include <string>
#include <set>

template<class Observed, class StateTag>
class Observer
{
	public:
	virtual ~Observer() = default;
	virtual void update(const Observed&, StateTag) = 0;
};

class Person
{
	public:
	enum StateChange
	{
		forename_changed,
		surname_changed,
		address_changed
	};
	using PersonObserver = Observer<Person, StateChange>;
	explicit Person(std::string forename, std::string surname) : forename_{std::move(forename)}, surname_{std::move(surname)} {}
	bool attach(PersonObserver*);
	bool detach(PersonObserver*);
	void notify(StateChange);
	void forename(std::string);
	void surname(std::string);
	void address(std::string);
	const std::string& forename() const { return forename_; }
	const std::string& surname() const { return surname_; }
	const std::string& address() const { return address_; }
	private:
	std::string forename_;
	std::string surname_;
	std::string address_;
	std::set<PersonObserver*> observers_;
};

bool Person::attach(PersonObserver* observer)
{
	auto [pos, success] = observers_.insert(observer);
	return success;
}

bool Person::detach(PersonObserver* observer)
{
	return (observers_.erase(observer)) > 0;
}

void Person::notify(StateChange property)
{	// in the book, an implementation not very straightforward presented
	// in order to be able to detect detach ops during iteration
	for(const auto& observer : observers_)
	{
		observer->update(*this, property);
	}
}

void Person::forename(std::string forename)
{
	forename_ = std::move(forename);
	notify(forename_changed);
}

void Person::surname(std::string surname)
{
	surname_ = std::move(surname);
	notify(surname_changed);
}

void Person::address(std::string address)
{
	address_ = std::move(address);
	notify(address_changed);
}

class NameObserver : public Observer<Person, Person::StateChange>
{
	public:
	virtual void update(const Person&, Person::StateChange) override;
};

void NameObserver::update(const Person& person, Person::StateChange property)
{
	if(property == Person::forename_changed || property == Person::surname_changed)
	{
		std::cout << "Updated name of the person is " << person.forename() << " " << person.surname() << "!\n";
	}
}

class AddressObserver : public Observer<Person, Person::StateChange>
{
	public:
	virtual void update(const Person&, Person::StateChange) override;
};

void AddressObserver::update(const Person& person, Person::StateChange property)
{
	if(property == Person::address_changed)
	{
		std::cout << "Address of " << person.forename() << " " << person.surname() << " has been changed!\n";
	}
}

int main()
{
	NameObserver name_observer;
	AddressObserver address_observer;

	Person tony("Tony", "Stark");
	Person alanna("Alanna", "Mitsopolis");

	tony.attach(&name_observer);
	alanna.attach(&name_observer);
	alanna.attach(&address_observer);

	tony.forename("Tony Ironman");
	alanna.forename("Alanna White-Widow");

	tony.address("Stark Industries");
	alanna.address("Earth");

	return 0;
}
