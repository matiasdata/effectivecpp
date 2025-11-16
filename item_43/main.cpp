#include <iostream>
#include <string>

template <typename T>
class Base
{
public:
    Base(T val_) : val{val_} {std::cout << "Creating Base object.\n";}
    virtual ~Base(){std::cout << "Destructing Base object.\n";}
    void sendMessage(const std::string& info) const {std::cout << "[Base] Sending message: " << info << " with value " << val << "\n";}
private:
    T val;
};

template <typename T>
class Derived : public Base<T>
{
public:
    // using Base<T>::sendMessage; //alternative 2
    Derived(T val_) : Base<T>{val_} {std::cout << "Creating Derived object.\n";}
    ~Derived(){std::cout << "Destructing Derived object.\n";}
    void sendMessageDerived(const std::string& info) const 
    {
        std::cout << "Derived class sending message...\n";
        // sendMessage(info); // error: unknown identifier unless using alternative 2, add a using declaration
        this->sendMessage(info); // alternative one, use this->
        //Base<T>::sendMessage(info); // alternative 3, least preferred because of virtual binding, if sendMessage() is virtual this calls the base member function.
    }


};


int main()
{
    Derived<int> b1(15);
    b1.sendMessageDerived("Hola");
    return 0;
}


/*
When a class template derives from a base class template (e.g., 
LoggingMsgSender<T> : MsgSender<T>), the compiler does NOT automatically 
look in the base class for member names. This is because the base class 
template may be specialized, and such specializations might remove or 
change those members. As a result, unqualified calls like sendMessage(info) 
fail to compile.

To access base class members in a derived class template, you must make 
those names visible using one of:
  1) this->sendMessage(info);
  2) using Base<T>::sendMessage;
  3) Base<T>::sendMessage(info);  // no virtual dispatch

These techniques inform the compiler that the name should be treated as 
dependent on the base class and looked up during template instantiation.
*/

