Reference : Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.
# classic implementation (intrusive)
* With the addition of accept function, now it is easy to add operations. However, all Shape family classes including the abstract base class Shape need to be modified.
* Another weakness is that even if the implemantation is same for different shapes, visit functions for different shapes need to be implemented. For instance, Translate visitor would be same for all Shape family.
* Another is that return type of visit function is determined in the abstract base class (ShapeVisitor here). Common approach (as implemented here) is to store the result in the Visitor family classes and access later.
* Another disadvantage is that it becomes difficult to add new types.
* Lastly, if another layer is added to Shape family classes and accept is forgotten to be overridden visitor would be applied to wrong types.
# variant implementation (nonintrusive)
Procedural programming is good at adding operations. This solution exploits that fact.
* This approach is nonintrusive, that is, it does not require any change in the Shape family classes.
* There is no need to have a abstract base class or any virtual functions either for Shape or for Visitor family classes.
* Variant takes up the space needed by the largest object so care must be taken.
* There is no need to use unique_ptr as in classic implementation unless there is a problem with size of std::variant (Shape) object (too large). Then, pointers, or proxy object can be used inside std::variant.
* There is no need to implement an alternative operator() for every class because there is no abstract base class / are no pure virtual functions.
* Return type can be customised because it is not determined by the abstract class anymore. 
* Instead of std::visit, std::get_if with lots of if, else if statements could be used to improve performance. However, that would decrease maintainability, readability etc. Also, the performance improvement can be compiler implementation dependent.
