Reference : Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

In a way, adapter class works as an indirection and maps from one set of functions to to another one. Note that it is not strictly necessary to map from one to one.
# object adapter
* Composition is preferred over inheritance.
* It provides much more flexibility than class adapter does.
# class adapter
* If a virtual function have to be overridden,
* If access to a protected member function is needed,
* If the adapted type to be constructed before another base class is necessary,
* If sharing a common virtual base class or overriding the construction of a virtual base class is needed,
* If significant advantage could be taken from the Empty Base Optimization (EBO), class adapter would be preferred.
