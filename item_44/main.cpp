#include <iostream>
#include <string>

template <int N>
class WorkerBloat
{
public:
    void work() const
    {
        // In a real world example, this will be a complex algorithm with several lines
        std::cout << "Working with N = " << N << "\n";
    }
};

class WorkerBase
{
protected:
    void workImpl(int N) const
    {
        // One single copy of this script will exist in the binary
        std::cout << "Working with N = " << N << "\n";
    }
};

template <int N>
class Worker : private WorkerBase
{
public:
    void work() const
    {
        // Thin wrapper and implicitely inlined
        workImpl(N);
    }
};  



int main()
{
    WorkerBloat<5> wb1;
    WorkerBloat<10> wb2;
    wb1.work();
    wb2.work();
    // The compiler will duplicate the code to generate two functions:
    // Worker<5>::work()
    // Worker<10>::work()

    Worker<5> w1;
    Worker<10> w2;
    w1.work();
    w2.work();
    // The compiler will only produce
    // WorkerBase::workImpl(int)
    // The functions:
    // Worker<5>::work()
    // Worker<10>::work()
    // are tiny (just one line) and usually inlined (replaces this function call with direct calls), thus there is no bloat.
    // After inlining, Worker<N>::work() often produces zero code the call is replaced by a direct call to workImpl with the
    // integer N.
    return 0;
}


/*

Template instantiations duplicate code whenever the template parameter
affects *any* part of the implementation. When many instantiations differ
only in a constant (e.g., N in Worker<N>), each instantiation generates
its own full copy of the function body → code bloat.

The solution is to:
1. Move all parameter-independent logic into a non-templated base class.
2. Let the templated class contain only a tiny wrapper that forwards the
   template argument to the shared implementation.

This reduces bloat because:
- The large implementation exists only once (in WorkerBase).
- Worker<N>::work() becomes a trivial one-line wrapper.
- Trivial wrappers are usually inlined and often generate zero machine code.
- All Worker<N> instances share the same implementation of workImpl(int).

This preserves type safety (Worker<5> and Worker<10> remain distinct types)
while avoiding replicated object code in the binary.

*/