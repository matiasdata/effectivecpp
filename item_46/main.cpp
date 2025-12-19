#include <iostream>
#include <numeric>
#include <stdexcept>

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
    Rational<T>& operator+=(const Rational<T>& other)
    {
        T pden = den*other.den;
        T pnum = num*other.den + other.num*den;
        num = pnum;
        den = pden;
        return *this;
    }
private:
    T num, den;
};

template <typename T>
inline const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs){
    return Rational(lhs.getnum() * rhs.getnum(), lhs.getden() * rhs.getden());
}
// Keep operator* as a non-member non-friend.
// The getter calls don’t hurt efficiency (they’ll inline). 
// Returning an int involves no heap allocation, no hidden copies — it’s literally just loading an integer from memory.
// This way, your Rational remains more encapsulated and extensible.

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
    std::cout << "Result = " << result << "\n";
    result = 2 * oneFourth; // no operator "*" matches these operands
    std::cout << "Result = " << result << "\n";
}
