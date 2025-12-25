#include <vector>
#include <list>
#include <iostream>
#include <iterator>
#include <concepts>

/* 
TAGS: Empty structs used solely for overload resolution.
Note: In the STL, random_access_iterator_tag inherits from bidirectional_iterator_tag.
*/
struct slow_tag{};
struct fast_tag{};

/* 
TRAITS CLASS: Provides a uniform interface to get type info.
*/

template <typename IterT>
struct my_iterator_traits
{
    // By default, assume the iterator defines its own category
    typedef typename IterT::my_category my_category;
};


// Partial specialization for built-in pointers (they are always "fast")
template <typename T>
struct my_iterator_traits<T*>
{
    typedef fast_tag my_category;
};

/*
WORKER FUNCTIONS (doAdvance): Overloaded based on tags.
*/

template <typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, fast_tag)
{
    std::cout << "Using fast O(1) advance\n";
    iter += d;
}

template <typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, slow_tag)
{
    std::cout << "Using slow O(n) advance\n";
    if (d >= 0)
    {
        while(d--) ++iter;
    } 
    else 
    {
        while(d++) --iter;
    }
}

/* 
MASTER FUNCTION (advance): The public API.
*/

template <typename IterT, typename DistT>
void myAdvance(IterT& iter, DistT d)
{
    doAdvance(iter,d, typename my_iterator_traits<IterT>::my_category());
}

// User-defined iterator for a linked list

struct MyListIter
{
    typedef slow_tag my_category; // Defining the trait locally
    int* ptr;
    void operator++(){++ptr;}
    void operator--(){--ptr;}
};

// Modern alternative using C++ Concepts:

template <typename IterT, typename DistT>
concept RandomAccess = requires(IterT iter, DistT d)
{
    iter += d; // the type must support the += operator
};

template <typename IterT, typename DistT>
requires RandomAccess<IterT, DistT>
void my_advance(IterT& iter, DistT d)
{
    std::cout << "Using fast O(1) advance with Concepts.\n";
    iter += d;
}

// Version for everything else (General case)
template<typename IterT, typename DistT>
void my_advance(IterT& iter, DistT d) {
    std::cout << "Using slow O(n) advance with Concepts.\n";
    while (d--) ++iter;
}


int main()
{
    // Case 1: Built-in pointer (Uses specialization)
    int arr[] = {1, 2, 3, 4, 5};
    int* p = arr;
    myAdvance(p, 3);
    my_advance(p, 3);

    // Case 2: Custom iterator (Uses internal typedef)
    MyListIter li;
    myAdvance(li, 2);
    my_advance(li, 2);
    return 0;
}

/*

Traits classes are a technique used in C++ to retrieve information about a type during compilation. 

They allow you to write generic code that can adapt its behavior based on the properties of the types
it is operating on, without requiring those types to share a common base class.
 
1. The Core Problem
 
Some algorithms can be implemented more efficiently if a type has certain capabilities
(e.g., std::advance can jump in $O(1)$ time for a std::vector but needs $O(n)$ time for a std::list).
However, a simple if statement won't work because:Runtime Cost: Checking type properties at runtime 
is slow.

Compilation Failure: If you write iter += d inside an if block, the code won't compile for iterators
that don't support +=, even if that branch is never executed.

2. How Traits Work

Traits are implemented as a template struct (e.g., std::iterator_traits<T>). For User-Defined Types: 
The traits template "parrots" a typedef defined inside the class. For Built-in Types (Pointers): 
The traits template uses partial template specialization to provide information for types like T*,
which cannot have internal typedefs.
 
3. Tag Dispatching

To use traits effectively, C++ uses Tag Dispatching. You create overloaded "worker" functions that 
take a "tag" object as a parameter. The "master" function calls the worker and passes a temporary 
tag object retrieved from the traits class. The compiler chooses the correct overload at compile time.

*/
