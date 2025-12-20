#include <iostream>
#include <numeric>
#include <stdexcept>

template<typename T> class Rational; // declare rational template 

template<typename T>
const Rational<T> doMultiply( const Rational<T>& lhs,const Rational<T>& rhs); // declare helper template

template <typename T>
class Rational
{
public:
    Rational(const T& n = 0, const T& d = 1) // not explicit, so it can implicitely convert an integer into a rational
    {
        if (d==0) throw std::invalid_argument("Denominator can't be zero");
        num = n;
        den = d;
    }
    const T getnum() const {return num;}
    const T getden() const {return den;}
    Rational& operator+=(const Rational& other) // inside a template class, one can write Rational (shorthand) instead of Rational<T>, both are valid.
    {
        T pden = den*other.den;
        T pnum = num*other.den + other.num*den;
        num = pnum;
        den = pden;
        return *this;
    }
    friend const Rational operator*(const Rational& lhs, const Rational& rhs) // define operator* function so that main compiles 
    {return doMultiply(lhs,rhs);} // call helper function (defer implementation), useful for complex functions, not particularly in this example

    // {return Rational(lhs.getnum() * rhs.getnum(), lhs.getden() * rhs.getden());} // implementation for simple functions


private:
    T num, den;
};

template <typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs){
    return Rational(lhs.getnum() * rhs.getnum(), lhs.getden() * rhs.getden());
}


template <typename T>
std::ostream& operator<<(std::ostream& os, const Rational<T>& r) {
    return os << r.getnum()<< "/" << r.getden();
}


int main()
{
    Rational<int> oneFourth(1, 4);
    Rational<int> result;
    result = oneFourth * 2; // error: no operator "*" matches these operands, operand types are: int * Rational<int>
    // candidate function template "operator*" failed deduction
    // this happens if we don't declare the function inside the class
    std::cout << "Result = " << result << "\n";
    result = 2 * oneFourth; // no operator "*" matches these operands
    std::cout << "Result = " << result << "\n";
}


/*
 Implicit Conversions in Class Templates

 1. THE CORE CONFLICT:
 Template argument deduction and implicit type conversion are mutually exclusive 
 processes. When a compiler looks at a function template, it must deduce the 
 template types (T) based exactly on the provided arguments. It will NOT 
 apply constructors or conversion operators to make an argument "fit" a 
 template parameter during this deduction phase.

 2. THE PROBLEM WITH NON-MEMBERS:
 For functions where you want implicit conversion to apply to ALL arguments 
 (like operator* where '2 * rational' and 'rational * 2' should both work), 
 the function must be a non-member. However, if that non-member is a 
 template, the "No-Conversion-During-Deduction" rule prevents calls like 
 '2 * rational' from ever succeeding.

 3. THE "FRIEND INJECTION" SOLUTION:
 To fix this, we declare the function as a friend inside the class template. 
 This technique uses the class instantiation as a "function factory":
 - When the class Template<T> is instantiated, the compiler is forced to 
 emit a concrete, NON-TEMPLATE function signature for that specific T.
 - Because the resulting function is a "normal" function (not a template), 
 the compiler is now permitted to use implicit type conversions on any 
 argument to match the signature.

 4. THE LINKING CAVEAT:
 Because this friend function is "injected" into the surrounding namespace 
 via the class instantiation, it exists as a specific non-template function. 
 If you define it outside as a template, the linker cannot connect the two. 
 Therefore, the implementation must be defined inside the class body to link 
 correctly.

 EXAMPLE SUMMARY:
 - Standalone Template: Fails because deduction forbids conversion.
 - Member Function: Fails because '2' (on the left) cannot call a member.
 - Inlined Friend: Succeeds because it's a non-member "normal" function 
 spawned by the class, allowing '2' to be converted to the required type.
*/