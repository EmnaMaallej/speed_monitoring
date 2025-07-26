#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <thread>
#include <chrono>

#define SERVICE_ID 0x0002
#define INSTANCE_ID 0x0001
#define METHOD_ID 0x0003

class Client2 {
public:
    Client2() {
        app_ = vsomeip::runtime::get()->create_application("client2");
    }

    bool init() {
        if (!app_->init()) {
            std::cerr << "Client2: Failed to initialize application" << std::endl;
            return false;
        }

        app_->register_message_handler(
            SERVICE_ID, INSTANCE_ID, METHOD_ID,
            std::bind(&Client2::on_speed_request, this, std::placeholders::_1)
        );

        app_->offer_service(SERVICE_ID, INSTANCE_ID);
        std::cout << "Client2: Offering svc_SpeedRequest (0x0002, 0x0001)" << std::endl;
        return true;
    }

    void start() {
        app_->start();
    }

private:
    void on_speed_request(const std::shared_ptr<vsomeip::message> &_request) {
        std::cout << "Client2: Received SpeedRequest" << std::endl;

        // Create response
        std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(_request);
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();

        // Mock speed data: 3000 tour/min (RPM)
        uint32_t speed_tour_min = 3000;
        std::vector<vsomeip::byte_t> payload_data;
        payload_data.push_back(static_cast<vsomeip::byte_t>(speed_tour_min >> 24));
        payload_data.push_back(static_cast<vsomeip::byte_t>(speed_tour_min >> 16));
        payload_data.push_back(static_cast<vsomeip::byte_t>(speed_tour_min >> 8));
        payload_data.push_back(static_cast<vsomeip::byte_t>(speed_tour_min));
        payload->set_data(payload_data);

        response->set_payload(payload);
        app_->send(response);
        std::cout << "Client2: Sent speed response: " << speed_tour_min << " tour/min" << std::endl;
    }

    std::shared_ptr<vsomeip::application> app_;
};

int main() {
    Client2 client2;
    if (client2.init()) {
        client2.start();
    } else {
        std::cerr << "Client2: Initialization failed" << std::endl;
        return 1;
    }
    return 0;
}
