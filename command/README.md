Reference : Klaus Iglberg. C++ Software Design. Design principles and patterns for high-quality software.
# classic implementation
* The only real disadvantage is the runtime performance overhead due to the additional indirection.
# functional implementation
* Since there are two functions (execute and undo) in this case, handling two function objects is a little bit problematic. Here, a struct holding two function objects is created.
* For difference commands new make_command free functions could be added non-intrusively.
* Another solution that provides better encapsulation is converting public ctor of the struct into private, and making make_command functions public static member of the struct. However, this solution is intrusive.
