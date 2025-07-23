#include <iostream>
#include <vsomeip/vsomeip.hpp>

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define SPEED_REQUEST_METHOD_ID 0x1000
#define RESPONSE_METHOD_ID 0x1001
#define CLIENT2_ID 0x2222

std::shared_ptr<vsomeip::application> app;

void on_speed_request(const std::shared_ptr<vsomeip::message> &_request) {
    if (_request->get_method() == SPEED_REQUEST_METHOD_ID) {
        std::cout << "Client2: Received SpeedRequest" << std::endl;

        // Simulate RPM value
        uint32_t rpm = 3000; // Example value
        std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(_request);
        response->set_method(RESPONSE_METHOD_ID);
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        payload->set_data((uint8_t*)&rpm, sizeof(rpm));
        response->set_payload(payload);
        app->send(response);
        std::cout << "Client2: Sent Response (RPM = " << rpm << ")" << std::endl;
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("client2");
    app->init();
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, SPEED_REQUEST_METHOD_ID, on_speed_request);
    app->offer_service(SERVICE_ID, INSTANCE_ID, CLIENT2_ID);
    app->start();
}
