#include <string>
#include "spdlog/spdlog.h"

class Animal {

public:
    Animal(std::string name, const int age) : 
        _name(name), _age(age) {};

    ~Animal() = default;

    [[nodiscard]] virtual bool init() {
        spdlog::info("Animal init function");
        return true;
    };

    const std::string get_name() {
        return _name;
    }

    void set_name(const std::string name) {
        _name = name;
    }

private:
    std::string _name;
    int _age;
};

// Genus
class Canis: public Animal {

public:
    Canis(std::string name, const int age, const std::string type) : 
        Animal(name, age), _type(type) {}

    ~Canis() = default;

    bool init() override {
        spdlog::info("Canis init function");
        return true;
    };

    virtual void sound() {
        spdlog::info("[{}]: *Typical canis sound*", _type);
    }
private:
    std::string _type;
};

class Dog: public Canis {

public:
    Dog(std::string name, const int age) :
        Canis(name, age, "Dog") {}
    
    ~Dog() = default;

    // Once you have override this as final, if dog class gets inherited
    // the child class will not be able to override this function anymore
    // If you do try, you will get the following error
    // error: virtual function ‘virtual bool Coyotes::init()’ overriding final function
    bool init() override final {
        spdlog::info("Dog init function");
        return true;
    };

    void sound() override {
        spdlog::info("[Dog]: Woof");
    };
};

class Coyotes: public Canis {

public:
    Coyotes(std::string name, const int age) :
        Canis(name, age, "Coyotes") {}
    
    ~Coyotes() = default;

    bool init() override final {
        spdlog::info("Coyotes init function");
        return true;
    };

};

int main() {
    spdlog::info("Virtual Class Example");
    Dog dog("Fluffy", 3);
    dog.init();
    dog.sound();

    Coyotes coyotes("Jack", 2);
    coyotes.init();
    coyotes.sound();

    return 0;
}