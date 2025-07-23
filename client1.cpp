#include <iomanip>
#include <iostream>
#include <sstream>
#include <vsomeip/vsomeip.hpp>
#include <cstring> 
#include <thread>  

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define SPEED_REQUEST_METHOD_ID 0x1000
#define RESPONSE_METHOD_ID 0x1001
#define SPEED_VALUE_METHOD_ID 0x1002
#define ACK_METHOD_ID 0x1003
#define NOTIFICATION_EVENT_ID 0x8001
#define CLIENT1_ID 0x1111

std::shared_ptr<vsomeip::application> app;

void on_message(const std::shared_ptr<vsomeip::message> &_response) {
    if (_response->get_method() == RESPONSE_METHOD_ID) {
        // Handle Response (tour/min) from Client2
        std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
        uint32_t rpm;
        memcpy(&rpm, payload->get_data(), sizeof(rpm));
        std::cout << "Client1: Received RPM = " << rpm << std::endl;

        // Convert tour/min to km/h (assuming wheel circumference = 2m)
        float kmh = (rpm * 2 * 3.14159 * 60) / 1000.0;
        std::cout << "Client1: Converted to km/h = " << kmh << std::endl;

        // Send SpeedValue to Server
        std::shared_ptr<vsomeip::message> speed_msg = vsomeip::runtime::get()->create_request();
        speed_msg->set_service(SERVICE_ID);
        speed_msg->set_instance(INSTANCE_ID);
        speed_msg->set_method(SPEED_VALUE_METHOD_ID);
        std::shared_ptr<vsomeip::payload> speed_payload = vsomeip::runtime::get()->create_payload();
        speed_payload->set_data((uint8_t*)&kmh, sizeof(kmh));
        speed_msg->set_payload(speed_payload);
        app->send(speed_msg);
    } else if (_response->get_method() == ACK_METHOD_ID) {
        // Handle Ack from Server
        std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
        bool ack;
        memcpy(&ack, payload->get_data(), sizeof(ack));
        std::cout << "Client1: Received Ack = " << (ack ? "true" : "false") << std::endl;
    } else if (_response->get_method() == NOTIFICATION_EVENT_ID) {
        // Handle Notification from Server
        std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
        std::string notification((char*)payload->get_data(), payload->get_length());
        std::cout << "Client1: Received Notification = " << notification << std::endl;
    }
}

void send_speed_request() {
    std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
    request->set_service(SERVICE_ID);
    request->set_instance(INSTANCE_ID);
    request->set_method(SPEED_REQUEST_METHOD_ID);
    std::shared_ptr<vsomeip::payload> payload = vsomeip::runtime::get()->create_payload();
    request->set_payload(payload);
    app->send(request);
    std::cout << "Client1: Sent SpeedRequest" << std::endl;
}

int main() {
    app = vsomeip::runtime::get()->create_application("client1");
    app->init();
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, RESPONSE_METHOD_ID, on_message);
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, ACK_METHOD_ID, on_message);
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, NOTIFICATION_EVENT_ID, on_message);
    app->offer_service(SERVICE_ID, INSTANCE_ID, CLIENT1_ID);
    app->start();

    // Periodically send SpeedRequest
    while (true) {
        send_speed_request();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
