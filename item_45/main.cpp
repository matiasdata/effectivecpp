#include <iostream>

template <typename T>
class SmartPtr {
public:
    explicit SmartPtr(T* ptr) : heldPtr{ptr}
    {
        std::cout << "Constructor for SmartPtr<" << typeid(T).name() << ">\n";
    }

    SmartPtr(const SmartPtr& other) : heldPtr{other.get()}
    {
        std::cout << "Copy constructor for SmartPtr<" << typeid(T).name() << ">\n";
    }

    template <typename U>
    SmartPtr(const SmartPtr<U>& other) : heldPtr{other.get()}
    {
        std::cout << "Generalized copy constructor: SmartPtr<" << typeid(U).name() << "> -> SmartPtr<" << typeid(T).name() << ">\n";
    }

    T* get() const {return heldPtr;}
private:
    T* heldPtr;
};

class Top { /* ... */ };
class Middle: public Top { /* ... */ };
class Bottom: public Middle { /* ... */ };

template <typename T>
class Widget
{
public:
    explicit Widget(const T& obj_) : obj{obj_} 
    {
        std::cout << "Constructor for Widget<" << typeid(T).name() << ">\n";
    }
    Widget(const Widget& w) : obj{w.obj} 
    {
        std::cout << "Copy constructor for Widget<" << typeid(T).name() << ">\n";
    }

    
    template <typename U>
    Widget(const Widget<U> & w) : obj{static_cast<T>(w.obj)} // alternatively use parenthesis, as {} won't accept narrowing conversions.
    {
        std::cout << "Generalized copy constructor for Widget<" << typeid(T).name() << "> from Widget<"<<typeid(U).name() << "\n";
    }

private:
    T obj;
    
    // This template friend declaration grants access to the private members
    // (like 'obj') of ALL other Widget instantiations.
    // e.g., it lets Widget<double>'s constructor read Widget<int>'s private 'obj'.
    template <typename U>
    friend class Widget;
};

int main()
{
    SmartPtr<Middle> middlePtr(new Middle); 
    // Conversion 1: Derived -> Base (Middle* -> Top*)
    SmartPtr<Top> topPtr1 = middlePtr; 
    // Conversion 2: Derived -> Base (Bottom* -> Top*) via temporary
    SmartPtr<Top> topPtr2 = SmartPtr<Bottom>(new Bottom); 
    // Conversion 3: Non-const -> Const
    SmartPtr<const Top> constTopPtr = topPtr1;

    Widget<int> wi(1);
    Widget<int> wi2 = wi;
    Widget<double> wd = wi;

}

/*
Problem:
Standard C++ treats different template instantiations (like SmartPtr<Top> and
SmartPtr<Middle>) as unrelated classes, preventing implicit conversions
(Derived -> Base, Non-const -> Const) that occur with built-in pointers (T*).

Solution: Member Function Templates (Generalized Copy Constructor)
A member template is used to create a "generalized copy constructor":
template<typename U> SmartPtr(const SmartPtr<U>& other);

The implementation: initializes T* heldPtr with U* other.get().
This acts as a compile-time filter: the conversion only compiles if an
implicit conversion exists between U* and T* (e.g., Derived* to Base*).

Critical Rule:
Declaring a generalized copy/assignment template does not suppress the
compiler's generation of the normal copy constructor/assignment operator.
You must explicitly declare both the normal and the generalized member
template versions to control all aspects of copying and assignment.
*/