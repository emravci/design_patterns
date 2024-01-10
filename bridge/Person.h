// Reference: 
// Klaus Iglberg. 
// C++ Software Design. 
// Design principles and patterns for high-quality software.

#include <iostream>
#include <memory>


class Person
{
	public:
	Person();
	~Person();
	Person(const Person&);
	Person& operator=(const Person&);
	Person(Person&&);
	Person& operator=(Person&&);
	inline int year_of_birth() const { return year_of_birth_; }
	private:
	// frequently accessed data members
	std::string forename_;
	std::string lastname_;
	int year_of_birth_;
	// just declare here define in source file 
	// infrequently accessed data members
	struct Impl;
	const std::unique_ptr<Impl> pimpl_;
};
