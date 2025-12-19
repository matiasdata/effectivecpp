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
    // {
    //     return Rational(lhs.getnum() * rhs.getnum(), lhs.getden() * rhs.getden());
    // } 
    {
        return doMultiply(lhs,rhs);
    }
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
