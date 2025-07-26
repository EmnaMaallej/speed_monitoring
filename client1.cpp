#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define SERVICE_ID 0x0002
#define INSTANCE_ID 0x0001
#define METHOD_ID 0x0003
#define WHEEL_CIRCUMFERENCE 2.1 // meters

class Client1 {
public:
    Client1() {
        app_ = vsomeip::runtime::get()->create_application("client1");
    }

    bool init() {
        if (!app_->init()) {
            std::cerr << "Client1: Failed to initialize application" << std::endl;
            return false;
        }

        app_->register_state_handler(
            std::bind(&Client1::on_state, this, std::placeholders::_1)
        );

        app_->register_message_handler(
            SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&Client1::on_response, this, std::placeholders::_1)
        );

        app_->request_service(SERVICE_ID, INSTANCE_ID);
        std::cout << "Client1: Requesting svc_SpeedRequest (0x0002, 0x0001)" << std::endl;
        return true;
    }

    void start() {
        app_->start();
        send_requests();
    }

private:
    void on_state(vsomeip::state_type_e _state) {
        if (_state == vsomeip::state_type_e::ST_REGISTERED) {
            std::cout << "Client1: Application registered" << std::endl;
        }
    }

    void on_response(const std::shared_ptr<vsomeip::message> &_response) {
        auto payload = _response->get_payload();
        auto data = payload->get_data();
        uint32_t speed_tour_min = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        std::cout << "Client1: Received speed: " << speed_tour_min << " tour/min" << std::endl;

        // Convert to km/h
        double speed_kmh = speed_tour_min * WHEEL_CIRCUMFERENCE * 60.0 / 1000.0;
        std::cout << "Client1: Converted speed: " << speed_kmh << " km/h" << std::endl;
    }

    void send_requests() {
        while (true) {
            std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
            request->set_service(SERVICE_ID);
            request->set_instance(INSTANCE_ID);
            request->set_method(METHOD_ID);

            std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
            std::vector<vsomeip::byte_t> payload_data; // Empty payload for request
            payload->set_data(payload_data);
            request->set_payload(payload);

            app_->send(request);
            std::cout << "Client1: Sent SpeedRequest" << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    std::shared_ptr<vsomeip::application> app_;
};

int main() {
    Client1 client1;
    if (client1.init()) {
        client1.start();
    } else {
        std::cerr << "Client1: Initialization failed" << std::endl;
        return 1;
    }
    return 0;
}
