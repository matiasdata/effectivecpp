#include <iostream>
#include <string>

template <typename T>
class Base
{
public:
    Base(T val_) : val{val_} {std::cout << "Creating Base object.\n";}
    virtual ~Base(){std::cout << "Destructing Base object.\n";}
    void sendmessage(const std::string& info) const {std::cout << "[Base] Sending message: " << info << " with value " << val << "\n";}
private:
    T val;
};

template <typename T>
class Derived : public Base<T>
{
public:
    // using Base<T>::sendmessage; //alternative 2
    Derived(T val_) : Base<T>{val_} {std::cout << "Creating Derived object.\n";}
    ~Derived(){std::cout << "Destructing Derived object.\n";}
    void sendmessageDerived(const std::string& info) const 
    {
        std::cout << "Derived class sending message...\n";
        // sendmessage(info); // error: unknown identifier unless using alternative 2, add a using declaration
        this->sendmessage(info); // alternative one, use this->
        //Base<T>::sendmessage(info); // alternative 3, least preferred because of virtual binding.
    }


};


int main()
{
    Derived<int> b1(15);
    b1.sendmessageDerived("Hola");
    return 0;
}