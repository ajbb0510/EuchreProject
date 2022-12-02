#ifndef SORTEDPQ_H
#define SORTEDPQ_H

#include "Eecs281PQ.h"
#include <algorithm>
#include <iostream>


// A specialized version of the priority queue ADT that is implemented with an
// underlying sorted array-based container.
// Note: The most extreme element should be found at the back of the
// 'data' container, such that traversing the iterators yields the elements in
// sorted order.
template<typename TYPE, typename COMP = std::less<TYPE>>
class SortedPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    SortedPQ(COMP comp = COMP()) :
        base{ comp } {
    } // SortedPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n log n) where n is number of elements in range.
    template<typename InputIterator>
    SortedPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp }  {
            for (InputIterator i = start; i < end; i++) {
                data.push_back(*i);
            }
            sort(data.begin(), data.end(), this->compare);
    }


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~SortedPQ() {
    } // ~SortedPQ()


    // Description: Add a new element to the PQ.
    // Runtime: O(n)
    virtual void push(const TYPE & val) {
        auto i = data.begin();
        while(i != data.end() && this->compare(*i, val)) {
            i++;
        }
        data.insert(i, val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the PQ.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(1)
    virtual void pop() {
        data.pop_back();
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return data.back();
    } // top()


    // Description: Get the number of elements in the PQ.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the PQ is empty.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


    // Description: Notifies the PQ that all element priorities may have changed.
    //              You must reorder the data so that the PQ invariant is restored.
    // Runtime: O(n log n)
    virtual void updatePriorities() {
        sort(data.begin(), data.end(),this->compare);
    } // updatePriorities()


private:
    // Note: This vector *must* be used your PQ implementation.
    std::vector<TYPE> data;
    
    //TODO: Add any additional member functions or data you require here.

}; // SortedPQ

#endif // SORTEDPQ_H
