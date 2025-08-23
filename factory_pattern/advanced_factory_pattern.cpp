#include <spdlog/spdlog.h>

#include <vector>
#include <memory>
#include <spdlog/spdlog.h>

/*
 * In building an application, we want the application to be providing all of the front-end level interface
 * In the case of a Data Acquisition application, we want to be able to retrieve data using some sort of a backend
 * that should be abstracted
 *
 * In the case of DAQ, you can have different backends that is specific for each protocol since every one of them have different
 * ways of being setup and initialized (e.g. UDP requires the socket to be binded if server or connected if client)
*
 * We can imagine that for each of this mode, there will always be specific abstract interface that works for all of them. For instance:
 *   * setup()      - Mode specific implementation for setting up any protocols
 *   * init()       - Initialize each modes with their channels (setup UART driver etc.)
 *   * update()     - Handle different messages streaming in (event driven or callbacks)
 *   * shutdown()   - What happens if the user closes this mode? How do we clear the data? Should we save unsaved data if we are recording the data in real time?
 *
 * With that, each of these functions, will need to be purely virtual, defined as follow:
 *   virtual <return type> <function name> = 0;
 *
 * Do not get confused between using only virtual and virtual = 0. The latter means pure virtual which means that each function defined with this
 * needs to be overriden! As for the former, the user can decide to override them or use the default implementation which can be implemented in the parent class.
 * If however, you wish to tell the developer that they should not discard the 
 */

// Uncommend this to see how each backend behaves if we use the default implementation from the backend
#define UDP_OVERRIDE_BACKEND
#define MAVLINK_OVERRIDE_BACKEND

#define ALLOW_MULTIPLE_BACKENDS

class DAQ_Backend;
class DAQ_Mavlink;
class DAQ_UDP;

enum class DAQ_Protocol {
    INVALID = -1,
    MAVLINK = 0,
    UDP = 1,
};

enum class DAQ_Mode {
    INVALID=-1,
    REAL_TIME=0,
    DATA_PLAYBACK = 1,
    SIMULATION = 2,
};

class DAQ_App {

public:

    // DAQ state implementation
    DAQ_App() = default;

#ifdef ALLOW_MULTIPLE_BACKENDS
    bool init(const std::vector<DAQ_Protocol> protocols);
#else
    bool init(const DAQ_Protocol protocol);
#endif

    void update();
    void shutdown();

    bool is_running() { return _is_running; };

    void set_running_flag(const bool flag) {
        _is_running = flag;
    }

    // API
    const std::vector<float> &get_latest_data() const {};
    uint32_t get_sample_rate_ms() {};

    std::unique_ptr<DAQ_Backend> _create_backend(DAQ_Protocol protocol);

#ifdef ALLOW_MULTIPLE_BACKENDS
    std::vector<std::unique_ptr<DAQ_Backend>> _daq_backends;
#else
    std::unique_ptr<DAQ_Backend> _daq_backend;
#endif // ALLOW_MULTIPLE_BACKENDS
    

private:

    // Variable that stores the backend pointer
    DAQ_Mode _current_daq_mode;
    bool _is_running = false;

};

class DAQ_Backend {

friend class DAQ_UDP;
friend class DAQ_Mavlink;

public:
    DAQ_Backend() = default;
    ~DAQ_Backend() = default;

    // Abstract function that each function should have in their backend implementation
    virtual void setup() = 0;
    virtual bool init() {
        spdlog::info("Using default init() implementation for {}", protocol_type());
    };
    virtual void update() {
        spdlog::info("Backend for {} has not been implemented. Using the default update() function.", protocol_type());
        ++_backend_counter;
        spdlog::info("Backend counter for {}: {}", protocol_type(), _backend_counter);
    };

    virtual void shutdown() {
        spdlog::info("Using default shutdown() implementation for {}", protocol_type());
    };

    virtual const char *protocol_type() const = 0;

protected:
    size_t _backend_counter;
};

class DAQ_UDP: public DAQ_Backend {

public:
    DAQ_UDP() = default;
    ~DAQ_UDP() = default;

    // Overriding the virtual function from parent class
    void setup() override {};


#ifdef UDP_OVERRIDE_BACKEND
    bool init() override {
        spdlog::info("{} init() function. Initialize protocol specific socket/implementation.", protocol_type());
    };
    void update() override {
        spdlog::info("{} update() function. Process data here.", protocol_type());
    };
    void shutdown() override {
        spdlog::info("{} shutdown() function. Implement protocol specific cleanup here.", protocol_type());
    };
#endif


    const char *protocol_type() const override { return "UDP"; }

    // Protocol specific functions or variables
    void parse_udp_packets();
    void receive_udp_packets();

};

class DAQ_Mavlink: public DAQ_Backend {

public:
    DAQ_Mavlink() = default;
    ~DAQ_Mavlink() = default;

    // Overriding the virtual function from parent class
    void setup() override {};

#ifdef MAVLINK_OVERRIDE_BACKEND
    bool init() override {
        spdlog::info("{} init() function. Initialize protocol specific socket/implementation.", protocol_type());
    }
    void update() override {
        spdlog::info("{} update() function. Process data here.", protocol_type());
    };
    void shutdown() override {
        spdlog::info("{} shutdown() function. Implement protocol specific cleanup here.", protocol_type());
    };
#endif

    const char *protocol_type() const override { return "Mavlink"; }

    // Protocol specific function from parent class
    void send_heartbeat() {};
    void listen_to_heartbeat() {};
    void send_mavlink_packets() {};

};

std::unique_ptr<DAQ_Backend> DAQ_App::_create_backend(DAQ_Protocol protocol) {
    switch(protocol) {
        case DAQ_Protocol::MAVLINK:
            return std::make_unique<DAQ_Mavlink>();
        case DAQ_Protocol::UDP:
            return std::make_unique<DAQ_UDP>();
        default:
            spdlog::info("Protocol provided either does not exist or has not been implemented");
            return nullptr;
    }
}

#ifdef ALLOW_MULTIPLE_BACKENDS
bool DAQ_App::init(const std::vector<DAQ_Protocol> protocols) {

    for (size_t i = 0; i < protocols.size(); i++) {
        std::unique_ptr<DAQ_Backend> backend_ptr;
        backend_ptr = _create_backend(protocols[i]);

        if (backend_ptr == nullptr) {
            spdlog::warn("Fails to create backend for this protocol");
            continue;
        } else {
            backend_ptr->init();
        }

        _daq_backends.push_back(std::move(backend_ptr));
    }

    _is_running = true;

    return true;
}
#else
bool DAQ_App::init(const DAQ_Protocol protocol) {

    _daq_backend = _create_backend(protocol);

    if (_daq_backend == nullptr) {
        spdlog::info("Fails to initialize the backend. The protocol provided is invalid");
        return false;
    }

    spdlog::info("DAQ initialized for the following protocol: {}", _daq_backend->protocol_type());

    if (_daq_backend->init()) {
        spdlog::info("{} protocol has been initialized", _daq_backend->protocol_type());
    }
    
    _is_running = true;

    return true;
    // In the init fuction for the application, we need to setup the necessary backend
}
#endif // ALLOW_MULTIPLE_BACKENDS

void DAQ_App::update() {

#ifdef ALLOW_MULTIPLE_BACKENDS
    for (size_t i = 0; i < _daq_backends.size(); i++) {
        _daq_backends[i]->update();
    }
#else
    _daq_backend->update();
#endif

}

void DAQ_App::shutdown() {

#ifdef ALLOW_MULTIPLE_BACKENDS
    for (size_t i = 0; i < _daq_backends.size(); i++) {
        _daq_backends[i]->shutdown();
    }
#else
    _daq_backend->shutdown();
#endif


}

int main() {

    DAQ_App daq_app;

#ifdef ALLOW_MULTIPLE_BACKENDS

    std::vector<DAQ_Protocol> protocols_to_use {DAQ_Protocol::UDP, DAQ_Protocol::MAVLINK};

    if (!daq_app.init(protocols_to_use)) {
        spdlog::error("DAQ fails to initialize due to invalid protocol. Force closing the application.");
        return 0;
    }

#else
    DAQ_Protocol protocol_type = DAQ_Protocol::UDP;
    if (!daq_app.init(protocol_type)) {
        spdlog::error("DAQ fails to initialize due to invalid protocol. Force closing the application.");
        return 0;
    };
#endif

    if (daq_app.is_running()) {
        for (size_t i = 0; i < 5; i++) {
            daq_app.update();
        }
        daq_app.set_running_flag(false);
    }

    daq_app.shutdown();

    return 0;
}