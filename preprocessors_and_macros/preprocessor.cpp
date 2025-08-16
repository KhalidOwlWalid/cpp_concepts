#include <iostream>

#define HELLO_WORLD "Hello, world!"
#define FUNC1

#ifdef FUNC1
void func1() {
    // std::cout << "I am inside function 1" << std::endl;
}
#endif

#ifdef FUNC2
void func2() {
    // std::cout << "I am inside function 2" << std::endl;
}
#endif

inline void inline_func() {std::cout << "Inline function" << std::endl; }

int main() {
    // std::cout << HELLO_WORLD << std::endl;
    inline_func();
    return 0;
}
