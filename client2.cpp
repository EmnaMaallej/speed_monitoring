#include <iostream>
#include <vsomeip/vsomeip.hpp>
#include <cstring>
#include <ctime>

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define SPEED_REQUEST_METHOD_ID 0x1000
#define RESPONSE_METHOD_ID 0x1001
#define CLIENT2_ID 0x2222

std::shared_ptr<vsomeip::application> app;

std::string getTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::ostringstream oss;
    oss << "[" << (ltm->tm_hour < 10 ? "0" : "") << ltm->tm_hour << ":"
        << (ltm->tm_min < 10 ? "0" : "") << ltm->tm_min << ":"
        << (ltm->tm_sec < 10 ? "0" : "") << ltm->tm_sec << "] ";
    return oss.str();
}

void on_speed_request(const std::shared_ptr<vsomeip::message> &_request) {
    std::string ts = getTimestamp();
    if (_request->get_method() == SPEED_REQUEST_METHOD_ID) {
        std::cout << ts << "=== CLIENT2 === Received SpeedRequest from Client1" << std::endl;

        uint32_t rpm = 3000; // Example RPM value
        std::shared_ptr<vsomeip::message> response = vsomeip::runtime::get()->create_response(_request);
        response->set_method(RESPONSE_METHOD_ID);
        std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
        payload->set_data((uint8_t*)&rpm, sizeof(rpm));
        response->set_payload(payload);
        app->send(response);
        std::cout << ts << "=== CLIENT2 === Sent Response (RPM = " << rpm << ") to Client1" << std::endl;
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("client2");
    app->init();
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, SPEED_REQUEST_METHOD_ID, on_speed_request);
    app->request_service(SERVICE_ID, INSTANCE_ID);
    app->offer_service(SERVICE_ID, INSTANCE_ID, RESPONSE_METHOD_ID);
    app->start();
}
