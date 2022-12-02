#ifndef POORMANPQ_H
#define POORMANPQ_H

#include "Eecs281PQ.h"

#include <utility> // needed for swap()

#define UNKNOWN -1

// A specialized version of the priority queue ADT that is implemented with an
// underlying unordered array-based container that is linearly searched
// for the most extreme element every time it is needed.
//
// Since top() is almost always followed by pop(), when top() has to find
// the most extreme element, it remembers that index so that pop() does
// does not have to search again.  Note the use of the mutable keyword for
// the extreme variable, which allows it to be updated even in a const function.
//
// THE POORMAN PQ IS IMPLEMENTED FOR YOU. DO NOT CHANGE THIS FILE.
// TODO: Read and understand this priority queue implementation!
// Pay particular attention to how the constructors and findExtreme()
// are written, especially the use of this->compare.

template<typename TYPE, typename COMP = std::less<TYPE>>
class PoormanPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    PoormanPQ(COMP comp = COMP()) :
        base{ comp }, extreme{ UNKNOWN } {
    } // PoormanPQ()


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PoormanPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp }, data{ start, end }, extreme{ UNKNOWN } {
    } // PoormanPQ()


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~PoormanPQ() {
    } // ~PoormanPQ()


    // Description: Does nothing for this implementation, as items can never be
    //              'out of order'. This function is implemented for you.
    // Runtime: O(1)
    virtual void updatePriorities() {
        extreme = UNKNOWN;
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: Amortized O(1)
    virtual void push(const TYPE & val) {
        data.push_back(val);
        extreme = UNKNOWN;
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the PQ.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(n)
    virtual void pop() {
        if (extreme == UNKNOWN)
            findExtreme();
        std::swap(data[extreme], data.back());
        data.pop_back();
        extreme = UNKNOWN;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ.
    // Runtime: O(n)
    virtual const TYPE & top() const {
        if (extreme == UNKNOWN)
            findExtreme();
        return data[extreme];
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()

    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    std::vector<TYPE> data;

    mutable int extreme;
    void findExtreme() const {
        unsigned int index = 0;

        for (unsigned int i = 1; i < data.size(); ++i)
            if (this->compare(data[index], data[i]))
                index = i;

        extreme = index;
    } // findExtreme()
}; // PoormanPQ

#endif // POORMANPQ_H
