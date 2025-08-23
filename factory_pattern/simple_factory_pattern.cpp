// Example taken from
// https://www.geeksforgeeks.org/system-design/factory-method-pattern-c-design-patterns/
#include <spdlog/spdlog.h>

class Shape {

public:
    Shape() { spdlog::info("Calling shape constructor"); }
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

class Circle: public Shape {
public:
    void draw() override {
        spdlog::info("Drawing circle");
    }
};

class Triangle: public Shape {
public:
    void draw() override {
        spdlog::info("Drawing triangle");
    }
};

class ShapeFactory {
public:
    virtual Shape* create_shape_with_raw_ptr() = 0; // Pure virtual function (defined by the = 0), class is now considered abstract class
    virtual ~ShapeFactory() = default;
    virtual std::unique_ptr<Shape> create_shape_with_unique_ptr() = 0;
    virtual void test() = 0;
};

class CircleFactory : public ShapeFactory {
public:

    Shape* create_shape_with_raw_ptr() override {
        return new Circle();
    }

    std::unique_ptr<Shape> create_shape_with_unique_ptr() override {
        return std::make_unique<Circle>();
    }

    void test() override {
        spdlog::info("Overriding needed");
    }
};

class TriangleFactory : public ShapeFactory {
public:

    Shape* create_shape_with_raw_ptr() override {
        return new Triangle();
    }

    std::unique_ptr<Shape> create_shape_with_unique_ptr() override {
        return std::make_unique<Triangle>();
    }
};

int main() {
    // The following error occurs because as soon as we added a single pure virtual function (see = 0), it is considered as an abstract class
    // Constructing it using the following will only result in an error, hence that is the main reason why, in a lot of other virtual
    // examples, we would inherit this directly on the children class. The children class poses no problem since it is no longer an abstract class
    // when it has overriden the function.
    // For more examples on pure virtual functions, please refer to the "virtual class" examples!
    // ShapeFactory factory; // Error: object of abstract class type "ShapeFactory" is not allowed
    ShapeFactory* factory_raw_ptr;
    
    // Use of new will mean that we will need to handle the heap allocation accordingly with the delete keyword
    factory_raw_ptr = new CircleFactory();
    Shape* circle_raw_ptr = factory_raw_ptr->create_shape_with_raw_ptr();
    circle_raw_ptr->draw();
    delete circle_raw_ptr;
    delete factory_raw_ptr;

    // For a more modern approach, use smart pointers
    // This avoids the need to use the delete keyword to clean up memory and a lot safer
    auto factory_unique_ptr = std::make_unique<CircleFactory>();
    std::unique_ptr<Shape> circle_unique_ptr = factory_unique_ptr->create_shape_with_unique_ptr();
    circle_unique_ptr->draw();

    return 0;

}