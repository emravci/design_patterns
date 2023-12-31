Reference : Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.

With strategy, it is still difficult to add new operations but it is easier to add new types since the properties of OOP is preserved. On the other hand, implementation of the function can be changed easily even during runtime, when implemented by means of a base class.
# classic implementation
* In this example, it does not require to overload the function (MaterialCostStrategy::cost here) for different Shape family classes (Circle, Square). However, as in the example in the book, it may. When it does and when a new type is added, MaterialCostStrategy abstract class needs to be modified, which would trigger recompilation of any class derived from MaterialCostStrategy. A solution to that is to create an abstract and concrete class for each type such as MaterialCostCircleStrategy, MaterialCostSquareStrategy etc. In order not to create multiple abstract classes, C++ templates can be employed such as MaterialCostStrategy< Circle >, MaterialCostStrategy< Square > etc. With this approach, the number of base classes are the same; however, they are compiler-generated. Most of the work also remains the same because classes derived from those still need implementation.
* When implemented by means of a base class, the performance will certainly take a hit by the additional runtime indirection. The performance is also affected adversely by many manual allocations, the resulting memory fragmentation, and the various indirections due to numerous pointers.
# policy-based design
* It lacks flexibility to adapt strategy at runtime.
* There is one instantiation of Shape family classes for each Strategy, Circle<SteelCostStrategy> for instance.
* Class templates usually completely reside in header files. Therefore, opportunity to hide implementation details in source files might be lost.
* This implementation does not suffer from performance penalty of the classic implementation. 
