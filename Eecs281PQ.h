#ifndef EECS281_PQ_H
#define EECS281_PQ_H

#include <functional>
#include <iterator>
#include <vector>

// A simple interface that implements a generic max-priority queue.
// Runtime specifications assume constant time comparison and copying.
// The priority queue may use a custom comparator specified by the COMP type.
// The semantics of the comparator are as shown in this example:
//   COMP comp; // an instance of the comparator
//   comp(val1, val2); // returns true if val1 is "less extreme" than val2
template<typename TYPE, typename COMP = std::less<TYPE>>
class Eecs281PQ {
public:
    virtual ~Eecs281PQ() {}

    // Description: Add a new element to the priority queue.
    virtual void push(const TYPE& val) = 0;

    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    virtual void pop() = 0;

    // Description: Return the most extreme (defined by 'compare') element of
    //              the priority queue.
    virtual const TYPE& top() const = 0;

    // Description: Get the number of elements in the priority queue.
    virtual std::size_t size() const = 0;

    // Description: Return true if the priority queue is empty.
    virtual bool empty() const = 0;

    // Description: Notifies the PQ that all element priorities may have changed.
    //              You must reorder the data so that the PQ invariant is restored.
    //              Each derived PQ will have to implement this appropriately.
    virtual void updatePriorities() = 0;

protected:
    Eecs281PQ() {}
    Eecs281PQ(const COMP &comp) : compare(comp) {}

    // Important! Recall that you must always have an instance of a functor
    // to use it. Use this member variable, which is an instance of the
    // comparator type COMP, to compare elements of type TYPE.
    //
    // You can use this in the derived classes with:
    //   this->compare(Thing1, Thing2)
    // (If you just use compare without the this->, the compiler won't
    // know to look for a compare member in the base class.)
    //
    // With the default compare function (std::less), this will
    // tell you if Thing1 is lower priority than Thing2.
    COMP compare;
}; // Eecs281PQ


#endif
