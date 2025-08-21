#include <spdlog/spdlog.h>

#include <vector>
#include <memory>
#include <spdlog/spdlog.h>

class DAQ_Backend;

/*
 * In building an application, we want the application to be providing all of the front-end level interface
 * In the case of a Data Acquisition application, we want to be able to retrieve data using some sort of a backend
 * that should be abstracted
 *
 * In the case of DAQ, you can have different backends that is specific for:
 *   - Real-time analysis
 *   - Data playback  
 *   - Simulation (made up) data to test some features
 *
 * We can imagine that for each of this mode, there will always be specific abstract interface that works for all of them. For instance:
 *   * setup()      - Mode specific implementation for setting up any channels
 *   * init()       - Initialize each modes with their channels (e.g. in data playback, read all of the data from the supported file)
 *   * update()     - Handle the user inputs (event driven or callbacks)
 *   * shutdown()   - What happens if the user closes this mode? How do we clear the data? Should we save unsaved data if we are recording the data in real time?
 *
 * With that, each of these functions, will need to be purely virtual, defined as follow:
 *   virtual <return type> <function name> = 0;
 *
 * Do not get confused between using only virtual and virtual = 0. The latter means pure virtual which means that each function defined with this
 * needs to be overriden! As for the former, the user can decide to override them or use the default implementation which can be implemented in the parent class.
 * If however, you wish to tell the developer that they should not discard the 
 */
class DAQ_App {

public:

    enum class DAQ_MODE {
        INVALID=-1,
        REAL_TIME=0,
        DATA_PLAYBACK = 1,
        SIMULATION = 2,
    };

    // DAQ state implementation
    DAQ_App();
    bool init();
    void update();
    void shutdown();

    // API
    const std::vector<float> &get_latest_data() const;
    uint32_t get_sample_rate_ms();

private:
    DAQ_Backend *_create_backend(DAQ_MODE mode);

    // Variable that stores the backend pointer
    std::unique_ptr<DAQ_Backend> _daq_backend;
    DAQ_MODE _current_daq_mode;

};

// class DAQ_Backend {

// };

int main() {

}