#include <spdlog/spdlog.h>

// Uncomment the following to simulate the error if the example class inherits the Sensor class but is not in the 'friend' list
// #define SIMULATE_BUILD_ERROR

class Sensor {

    /*
    The friend declaration is a powerful tool, if you require that your inherited class have access to the variables
    of your base class, in this class Sensor_Example1 needs access to Sensor class private variable.
     */
    friend class Sensor_Example1;

public:
    ~Sensor() = default;


    std::string public_var {"Sensor Public Variable"};

    /*
    The protected access specifier allows class that inherits from Sensor (base class) to be able to access the private variable
    of the base class without needing a helper function (e.g. getters and setters). However, this is only allowed to be accessible
    within the class implementation itself. If this is within a local scope of a different implementation file, you will not be able
    to access it, similar to a private variable!
     */
protected:
    std::string protected_var {"Sensor Protected Variable"};

private:
    std::string private_var {"Sensor Private Variable"};

};

class Sensor_Example1: public Sensor {

public:
    ~Sensor_Example1() = default;

    void get_sensor_public_variable() const {
        spdlog::info("Inside Sensor Example 1: {}", public_var);
    };
    void set_sensor_public_variable() {
        public_var = example1_var;
    };

    void get_sensor_protected_variable() const {
        spdlog::info("Inside Sensor Example 1: {}", protected_var);
    };
    void set_sensor_protected_variable() {
        protected_var = example1_var;
    };

    void get_sensor_private_variable() const {
        spdlog::info("Inside Sensor Example 1: {}", private_var);
    };

    void set_sensor_private_variable() {
        private_var = example1_var;
    };

private:
    std::string example1_var {"Example 1 Sensor Variable"};

};

class Sensor_Example2: public Sensor {

public:
    ~Sensor_Example2() = default;

    void get_sensor_public_variable() const {
        spdlog::info("Inside Sensor Example 2: {}", public_var);
    };
    void set_sensor_public_variable() {
        public_var = example2_var;
    };

    void get_sensor_protected_variable() const {
        spdlog::info("Inside Sensor Example 2: {}", protected_var);
    };
    void set_sensor_protected_variable() {
        protected_var = example2_var;
    };

#ifdef SIMULATE_BUILD_ERROR
    void get_sensor_private_variable() const {
        spdlog::info("Inside Sensor Example 2: {}", private_var);
    };

    void set_sensor_private_variable() {
        private_var = example2_var;
    };
#endif // SIMULATE_BUILD_ERROR

private:
    std::string example2_var {"Example 2 Sensor Variable"};

};

int main() {
    Sensor_Example1 sensor_example;
    sensor_example.get_sensor_public_variable();
    sensor_example.set_sensor_public_variable();
    sensor_example.get_sensor_public_variable();

    sensor_example.get_sensor_protected_variable();
    sensor_example.set_sensor_protected_variable();
    sensor_example.get_sensor_protected_variable();

    sensor_example.get_sensor_private_variable();
    sensor_example.set_sensor_private_variable();
    sensor_example.get_sensor_private_variable();

    Sensor_Example2 sensor_example2;
    sensor_example2.get_sensor_public_variable();
    sensor_example2.set_sensor_public_variable();
    sensor_example2.get_sensor_public_variable();

    sensor_example2.get_sensor_protected_variable();
    sensor_example2.set_sensor_protected_variable();
    sensor_example2.get_sensor_protected_variable();

    return 0;
}