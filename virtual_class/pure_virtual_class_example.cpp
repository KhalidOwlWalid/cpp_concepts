#include <spdlog/spdlog.h>

// To further understand why one would use pure virtual function, please look at factory_pattern example

// Uncomment the following definition, and try building again, to simulate the problem when inheriting abstract type
// error: cannot declare variable ‘circle’ to be of abstract type ‘Circle’
// note:   because the following virtual functions are pure within ‘Circle’:
// note:     ‘virtual void Shape::draw()’
// #define SHOW_BUILD_FAIL_DUE_TO_NOT_OVERRIDING_VIRTUAL_FUNCTION

// Comment this out when you uncomment the above
#define SHOW_WORKING_PURE_VIRTUAL_FUNCTION

class Shape {
public:
    Shape() { spdlog::info("Constructing shape"); }

#ifdef SHOW_BUILD_FAIL_DUE_TO_NOT_OVERRIDING_VIRTUAL_FUNCTION
    virtual void draw() = 0; // Pure virtual function
#elif defined(SHOW_WORKING_PURE_VIRTUAL_FUNCTION)
    virtual void draw() = 0;
#else
    /*
    The use of [[nodiscard]] here implies that if your inherited class does not override this function,
    it will simply complain, but there would be no problem as there is still a default implementation
    Use this, if lets say you wish to not use a pure virtual function since it is not an important 
    attributes that the inherited class should have but still useful for them to override!
    */
    [[nodiscard]] virtual void draw() { spdlog::info("I am using the default draw implementation"); }
#endif
    ~Shape() = default;
};

class Circle: public Shape {
public:
    Circle() { spdlog::info("Constructing circle"); };

    // Since we have defined the draw function to be pure virtual (using the = 0)
    // it now becomes an abstract type, hence ALL of the pure virtual function NEEDS
    // to be overriden. If not, the build will fail!
#ifdef SHOW_BUILD_FAIL_DUE_TO_NOT_OVERRIDING_VIRTUAL_FUNCTION
    // Intentionally not overriding the draw function to invoke the error
#elif defined(SHOW_WORKING_PURE_VIRTUAL_FUNCTION)
    virtual void draw() { spdlog::info("I am overriding the pure virtual function"); }
#else
    // Will use the default draw implementation
#endif
    ~Circle() = default;
};

int main() {
    Circle circle;
    circle.draw();
    return 0;
}
