# Abstractions

## __Storable Polymorphic Types__ ##
In designing my database, I have created a pure interface that is to be inherited by different types of storable structures. I believe that what we're storing are not the same and all have different query information, it was best to isolate the implementation details.\
It was also very similar to the handleInput strategy pattern, so I implemented it very similarly. This way, when a new storable type is introduced, I do not have have to change the add function and all I have to do is create a new Storable subclass.

## __BlockVisitor -> BlockIterator__ ##
I am not sure if this is the right move, but I think it's better to segregate the implementation details of iteration. The BlockVisitor is currently defined as a function pointer with a specific type signature, but this doesn't provide full flexibility.\
I also don't understand the indirection it is doing. Storage should inherit BlockIterator and so should the future IndexIterator, which means that they must both implement the each() method.\
This means that when calling .each(), it must have a BlockVisitor function known. This can be solved by OxE or by having a method within class that has that signature.\
By doing this, we create one method + one class for every new iteration we come up with.

Would it not be better to pass in the BlockIterator directly to a single API such as read and use polymorphism to dictate which route to take?\
This way, whe nwe have more than one iteration in the future, we can create more classes, and when we want to use a specific iteration pattern but only read certain blocks, we can write functions within those classes.
