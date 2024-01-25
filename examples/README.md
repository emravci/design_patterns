# Expression Tree
* Type erasure pattern is used to create the abstraction (Expression). Type erasure implementation relies on three other design patterns; namely, external polymorphism, prototype and bridge.
* Arithmetic operations classes make use of composite design pattern, which helps to create expression tree.
* When overloaded operators used, object returned from the functions has only two children. However, with variadic constructor, it is easy and readable to create one with many children.
* Since value semantics is used instead of reference semantics, lazy evaluations may not be efficient.
* To overcome that, overloaded operators might be modified to trigger evaluations, so that instead of returning a large expression tree, only a node with two **leaf** children is returned.
* Also, other overloads could be provided, such as *Addition operator+(Addition, const Expression&)*, which would add Expression object on the right hand side as a children of Addition object on the left hand side.That would decrease the height of the expression tree and might improve performance due to less recursive call on *evaluate*.
