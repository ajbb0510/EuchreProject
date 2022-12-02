#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    BinaryPQ(COMP comp = COMP()) :
        base{ comp } {
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp } {
            for (InputIterator i = start; i < end; i++) {
                data.push_back(*i);
            }
            updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for(size_t i = size(); i > 0; i--) {
            fixdown((int)(i - 1));
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    virtual void push(const TYPE & val) {
        
        data.push_back(val);
        fixup((int)(data.size() - 1));
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        data[0] = data[data.size() - 1];
        data.pop_back();
        fixdown(0);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return data.front();
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()
    
    private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;
    //TODO: Add any additional member functions or data you require here.
    virtual void fixup(int k) {
        while ((k > 0) && this->compare(data[k/2], data[k])) {
            std::swap(data[k], data[k/2]);
            k /= 2;
        }
    }
    
    virtual void fixdown(int k) {
        while ((k * 2) < (int)data.size()) {
            int j = 2 * k;
            if ((j < (int)(data.size() - 1)) &&
                this->compare(data[j], data[j + 1])) {
                j++;
            }
            if (!this->compare(data[k], data[j])) {
                break;
            }
            std::swap(data[k], data[j]);
            k = j;
        }
    }

}; // BinaryPQ


#endif // BINARYPQ_H
