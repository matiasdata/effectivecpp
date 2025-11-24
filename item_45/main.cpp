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
        std::cout << "Generalized copy constructor: SmartPtr<" << typeid(U).name() << "> -> SmartPtr<" << typeid(T).name() << ">" << std::endl;
    }

    T* get() const {return heldPtr;}
private:
    T* heldPtr;
};

class Top { /* ... */ };
class Middle: public Top { /* ... */ };
class Bottom: public Middle { /* ... */ };

int main()
{

SmartPtr<Middle> middlePtr(new Middle); 
// Conversion 1: Derived -> Base (Middle* -> Top*)
SmartPtr<Top> topPtr1 = middlePtr; 
// Conversion 2: Derived -> Base (Bottom* -> Top*) via temporary
SmartPtr<Top> topPtr2 = SmartPtr<Bottom>(new Bottom); 
// Conversion 3: Non-const -> Const
SmartPtr<const Top> constTopPtr = topPtr1;

}