#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP> {
    using base = Eecs281PQ<TYPE, COMP>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            Node(const TYPE &val)
                : elt(val), child(nullptr), sibling(nullptr) {
            }

            // Description: Allows access to the element at that Node's position.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &operator*() const { return elt; }
            const Node *sibling_ptr() const { return sibling; }
            const Node *child_ptr() const { return child; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *previous;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    PairingPQ(COMP comp = COMP()) :
        base{ comp } {
            root = nullptr;
            numnodes = 0;
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP comp = COMP()) :
        base{ comp } {
            for (InputIterator i = start; i != end; i++) {
                push(*i);
            }

    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        base{ other.compare } {
            root = nullptr;
            *this = other;
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ& operator=(const PairingPQ& rhs) {
        for (size_t i = 0; i < pushorder.size(); i++) {
            delete pushorder[i];
            pushorder[i] = nullptr;
        }
        root = copy(rhs.root);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        for (size_t i = 0; i < pushorder.size(); i++) {
            delete pushorder[i];
            pushorder[i] = nullptr;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        if (root != nullptr) {
            Node *violation = findviolations(root);
            while (violation != nullptr) {
                root = meld(root, violation);
                violation = findviolations(root);
            }
        }
        
    
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This has been provided for you,
    //              in that you should implement push functionality in the
    //              addNode function.
    // Runtime: Amortized O(1)
    virtual void push(const TYPE & val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        if (root->child != nullptr) {
            root->child->previous = nullptr;
        }
        numnodes--;
        Node *node1 = root->child;
        root->child = nullptr;
        root->sibling = nullptr;
        root->previous = nullptr;
        
        if (node1 == nullptr) {
            root = nullptr;
            return;
        }
        
        if (node1->child == nullptr && node1->sibling == nullptr) {
            root = node1;
            return;
        }
        
        std::deque<Node*> multipass;
        
        while (node1 != nullptr) {
            Node *node2 = node1->sibling;
            node1->previous = nullptr;
            node1->sibling = nullptr;
            multipass.push_back(node1);
            node1 = node2;
        }
        
        
        while (multipass.size() > 1) {
            Node *node2 = multipass.front();
            multipass.pop_front();
            Node *node3 = multipass.front();
            multipass.pop_front();
            Node *newNode = meld(node2, node3);
            multipass.push_back(newNode);
        }

        root = multipass.front();
        
        
        
        
       
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the priority_queue.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numnodes;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return root == nullptr;
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE & new_value) {
        node->elt = new_value;
        if (node->previous->child == node) {
            node->previous->child = node->sibling;
            if (node->sibling != nullptr) {
                node->sibling->previous = node->previous;
            }
            node->previous = nullptr;
            node->sibling = nullptr;
        }
        else {
            if (node->previous != nullptr) {
                node->previous->sibling = node->sibling;
            }
            if (node->sibling != nullptr) {
            node->sibling->previous = node->previous;
            }
            node->sibling = nullptr;
            node->previous = nullptr;
        }
        root = meld(root, node);
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: Amortized O(1)
    Node* addNode(const TYPE & val) {
        Node *n = new Node(val);
        if (root == nullptr) {
            root = n;
        }
        else {
            root = meld(n, root);
        }
        numnodes++;
        pushorder.push_back(n);
        return n;
    } // addNode()

    virtual Node* meld(Node *root1, Node *root2) {
        if (!this->compare(root1->elt, root2->elt)) {
            root2->sibling = root1->child;
            if (root1->child != nullptr) {
                root1->child->previous = root2;
            }
            root2->previous = root1;
            root1->child = root2;
            return root1;
        }
        else {
            root1->sibling = root2->child;
            if (root2->child != nullptr) {
                root2->child->previous = root1;
            }
            root1->previous = root2;
            root2->child = root1;
            return root2;
        }
    }
    virtual Node * copy(Node* root) {
        if (root == nullptr) {
            return nullptr;
        }
        else {
            Node *n = new Node(root->elt);
            if  ((n->child = copy(root->child)) != nullptr) {
                n->child->previous = n;
            }
            if ((n->sibling = copy(root->sibling))!= nullptr) {
                n->sibling->previous = n;
            }
            return n;
        }
    }
    
    virtual Node* findviolations(Node* parent) {
        Node *child = parent->child;
        while (child != nullptr) {
            if (this->compare(parent->elt, child->elt)) {
                if (child->previous->child == child) {
                    child->previous->child = child->sibling;
                }
                else {
                    child->previous->sibling = child->sibling;
                }
                if (child->sibling != nullptr) {
                    child->sibling->previous = child->previous;
                }
                child->sibling = nullptr;
                child->previous = nullptr;
                
                return child;
            }
            // Check child's child
            Node *n = findviolations(child);
            if (n != nullptr) {
                return n;
            }
            child = child->sibling;
        }
    
        return nullptr;
    }

    private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    Node *root;
    size_t numnodes;
    std::vector<Node*> pushorder;
};


#endif // PAIRINGPQ_H
