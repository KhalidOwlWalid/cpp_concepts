#include <spdlog/spdlog.h>

class Test {
public:

    Test(const int x): _x(x) {}

    void get_x_val() { 
        _x = 2;
    }  // Not const

    void get_x_val() const {
        // _x = 2; // Not possible as the parameter is read-only
    }

    const int get_const_x_val() {
        return _x;
    }


private:
    // Member variable for Test class
    int _x;
};

int main() {
    Test test(1);
    test.get_x_val();
    return 0;
}
