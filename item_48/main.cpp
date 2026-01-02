#include <iostream>

template <uint64_t N>
struct Factorial
{
    static constexpr uint64_t value = N * Factorial<N-1>::value;
};

template <>
struct Factorial<0>
{
    static constexpr uint64_t value = 1;
};

/*
static constexpr (Values) for TMP:
    Used in structs/classes to define compile-time global constant associated
    with the template type. Because they 
    are 'static' it means the value belongs to the type itself.  Because they 
    are 'constexpr', they are calculated by the compiler at compile time, not 
    calculated at run time by the CPU.
*/


template <size_t N>
struct VectorAdder
{
    static void add(int* a, int* b)
    {
        a[N-1] += b[N-1]; // add the current element
        VectorAdder<N-1>::add(a,b); // recursively call the previous index
    }
};

template <>
struct VectorAdder<0>
{
    static void add(int* a, int* b)
    {
        // base case: do nothing (loop is finished).
    }
};


// modern alternative, does not need template specialization and thus can be written as a normal template function, no need for structs. 
// looks like a standard function.

template <typename T, size_t N>
void fastAdd(T* a, T* b)
{
    if constexpr (N > 0) {
        a[N-1] += b[N-1]; // add the current element
        fastAdd<T,N-1>(a,b); // recursively call the previous index
    }
}

int main()
{
    std::cout << "Factorial<20>::value = " << Factorial<20>::value << "\n"; // compile time recursive calculation with template metaprogramming (TMP)
    

    int i1[2] = {1, 2};
    int i2[2] = {10, 20};

    VectorAdder<2>::add(i1,i2);

    std::cout << "Int sum result: " << i1[0] << ", " << i1[1] << " using VectorAdder<2>::add \n";

    fastAdd<int, 2>(i1, i2);

    std::cout << "Int sum result: " << i1[0] << ", " << i1[1] << " using fastAdd \n";



    double v1[3] = {1.5, 2.5, 3.5};
    double v2[3] = {10.1, 20.1, 30.1};

    // At compile time, this becomes:
    // v1[2] += v2[2];
    // v1[1] += v2[1];
    // v1[0] += v2[0];

    fastAdd<double,3>(v1, v2);

    std::cout << "Double result: " << v1[0] << ", " << v1[1] << ", " << v1[2] << "\n";

    return 0;
}


/*
TEMPLATE METAPROGRAMMING (TMP)

 1. THE CONCEPT:
 TMP is the process of writing template-based programs that execute INSIDE the 
 compiler. The "output" of a template metaprogram is not a binary, but rather 
 specific C++ source code instantiated from templates, which is then compiled 
 normally. It was "discovered" as a side effect of the template system, not 
 explicitly designed.

 2. KEY STRENGTHS:
 - Shifting Work to Compile-time: Moves calculations and logic from runtime to 
   compilation, resulting in smaller executables and faster execution.
 - Early Error Detection: Allows the compiler to catch logic errors (like 
   invalid unit combinations) that would otherwise only be found during testing.
 - Turing Completeness: TMP can compute anything a normal program can, using 
   recursive template instantiation instead of loops and specialization instead 
   of if/else.

 3. PRACTICAL EXAMPLES:
 - Dimensional Analysis: Ensuring mass and velocity are never added together.
 - Expression Templates: Optimizing matrix math by eliminating temporary 
   objects and merging loops (e.g., m1 * m2 * m3).
 - Policy-Based Design: Generating hundreds of custom class variants (like 
   smart pointers) from a few policy templates.

 4. THE TRADE-OFFS:
 - Syntax: Often unintuitive and uses functional programming patterns.
 - Compilation Speed: Shifting work to the compiler significantly increases 
   build times.
 - Tool Support: Extremely difficult to debug compared to standard C++.

 SUMMARY:
 TMP is a powerful tool for library developers to provide highly efficient, 
 type-safe, and customizable code. While not for everyone, it enables 
 optimizations and safety checks that are impossible in "normal" C++.
*/

/*
 THE ENUM HACK vs. STATIC CONSTEXPR

 1. THE ENUM HACK (C++98):
 Used to define compile-time constants inside a class when older compilers 
 insisted on allocating memory for 'static const int'. Because enums are 
 pure symbols, they occupy no memory and their address cannot be taken.

 2. WHY IT WAS USED:
 - Guaranteed zero memory overhead (no lvalue created).
 - Provided a reliable compile-time constant for array sizes and templates.
 - Strictly forbade pointer access to the value.

 3. THE MODERN REPLACEMENT (static constexpr):
 C++11 introduced 'static constexpr' to provide a formal way to define 
 compile-time constants.
 - Supports non-integers (double, float, and literal objects).
 - Clearer intent than using an enum for math results.
 - The standard choice for modern Template Metaprogramming (TMP).

 Use 'static constexpr' for modern code. Use the "enum hack" only for 
 legacy C++98 compatibility or to strictly prevent address-of operations.

*/

/*
 WHY 'IF CONSTEXPR' IS MANDATORY FOR TMP
 
 1. COMPILATION OF ALL BRANCHES:
 A regular 'if' requires the compiler to generate valid code for both 
 branches. In recursive templates, this leads to infinite instantiation 
 because the compiler tries to generate fastAdd<-1>, fastAdd<-2>, etc.
 'if constexpr' is a compile-time instruction. The compiler evaluates
 the condition and discards the branch that is false. The code in the
discarded branch is not even turned into machine code.

 2. DISCARDING STATEMENTS:
 'if constexpr' allows the compiler to discard the 'true' branch entirely 
 if the condition is false. This "dead code" is never instantiated, 
 allowing the recursion to terminate safely.

 3. TYPE VALIDITY:
 'if constexpr' is also used when a branch contains code that wouldn't 
 even compile for certain types (e.g., calling .length() on an int). 
 A regular 'if' would cause a compiler error; 'if constexpr' ignores it.

*/

/*
 FUNCTION TEMPLATES vs. STRUCT TEMPLATES
 
 1. HISTORICAL CONTEXT:
 Historically, Template Metaprogramming (TMP) relied on Structs because 
 they allowed "Partial Specialization" to define base cases. Functions 
 were harder to specialize without causing "ambiguous call" errors.

 2. THE C++17 SHIFT:
 'if constexpr' allows Function Templates to handle their own base cases. 
 This makes 'Template Structs' (like the old Item 48 examples) largely 
 obsolete for simple recursive logic.

 3. CLEANER SYNTAX:
 Using a function template (fastAdd<N>) is more natural than a struct 
 (VectorAdder<N>::add) because it avoids the need for the static 'add' 
 wrapper and extra boilerplate code.
*/

/*
 ASSEMBLY ANALYSIS OF TMP UNROLLING

 Using -O3 with VectorAdder<2> results in:
 1. INLINING: The recursive template calls are removed. The 'call' 
    instructions are replaced by direct data manipulation.
 2. UNROLLING: The loop logic is 'flattened'. Instead of a loop counter, 
    the compiler generates specific instructions for index 0 and index 1.
 3. LEA TRICK: The compiler often uses 'lea' (Load Effective Address) 
    to perform addition and register movement in a single clock cycle.
 4. CONSTANT FOLDING: If array values are known at compile-time, the 
    compiler performs the math itself, leaving only 'mov' instructions 
    with the final results in the binary.

*/