// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include "Person.h"

struct Person::Impl
{
	std::string address_;
	std::string city_;
	std::string country_;
};

Person::Person() : pimpl_{std::make_unique<Impl>()} {}

// dtor is defined in source file because definition of Impl class is not in the header file
Person::~Person() = default;

Person::Person(const Person& other) : pimpl_{std::make_unique<Impl>(*other.pimpl_)} {}

Person& Person::operator=(const Person& other)
{
	*pimpl_ = *other.pimpl_;
	return *this;
}

Person::Person(Person&& other) : pimpl_{std::make_unique<Impl>(std::move(*other.pimpl_))} {}

Person& Person::operator=(Person&& other)
{
	*pimpl_ = std::move(*other.pimpl_);
	return *this;
}

