Reference : Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.
# classic bridge
* Bridge pattern is used to seperate classes not connect them.
* It is used to reduce physical dependencies.
* In strategy, variation point is controlled externally (by the user). In bridge, it is controlled internally.
* Since bridge introduces an additional indirection, it has a performance overhead.
* Depending on whether to use an abstraction, there is a virtual function call overhead.
* Additional dynamic memory allocation also costs.
* Code complexity also increases.
# pointer to implementation
* This one has the same advantages and disadvantages.
* However, when implemented as a partial bridge, it may improve performance. Via bridge pattern, the size of the type can be reduced, this can prove to be very valuable for instance if the type is to be used inside std::variant.
* Also with patrial bridge, the class might be cache-friendly, which would also increase the performance.
