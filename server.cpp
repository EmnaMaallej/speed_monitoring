#include <iostream>
#include <string>
#include <vsomeip/vsomeip.hpp>
#include <cstring> // Added for memcpy

#define SERVICE_ID 0x1234
#define INSTANCE_ID 0x5678
#define SPEED_VALUE_METHOD_ID 0x1002
#define ACK_METHOD_ID 0x1003
#define NOTIFICATION_EVENT_ID 0x8001
#define SERVER_ID 0x3333

std::shared_ptr<vsomeip::application> app;

void on_speed_value(const std::shared_ptr<vsomeip::message> &_request) {
    if (_request->get_method() == SPEED_VALUE_METHOD_ID) {
        // Handle SpeedValue from Client1
        std::shared_ptr<vsomeip::payload> payload = _request->get_payload();
        float kmh;
        memcpy(&kmh, payload->get_data(), sizeof(kmh));
        std::cout << "Server: Received SpeedValue = " << kmh << " km/h" << std::endl;

        // Send Ack
        std::shared_ptr<vsomeip::message> ack = vsomeip::runtime::get()->create_response(_request);
        ack->set_method(ACK_METHOD_ID);
        std::shared_ptr<vsomeip::payload> ack_payload = vsomeip::runtime::get()->create_payload();
        bool ack_value = true;
        ack_payload->set_data((uint8_t*)&ack_value, sizeof(ack_value));
        ack->set_payload(ack_payload);
        app->send(ack);
        std::cout << "Server: Sent Ack" << std::endl;

        // Analyze speed and send Notification
        std::string notification = (kmh < 100.0) ? "No Alert" : "Alert";
        std::shared_ptr<vsomeip::message> notif_msg = vsomeip::runtime::get()->create_notification();
        notif_msg->set_service(SERVICE_ID);
        notif_msg->set_instance(INSTANCE_ID);
        notif_msg->set_method(NOTIFICATION_EVENT_ID);
        std::shared_ptr<vsomeip::payload> notif_payload = vsomeip::runtime::get()->create_payload();
        notif_payload->set_data((uint8_t*)notification.c_str(), notification.length());
        notif_msg->set_payload(notif_payload);
        app->send(notif_msg);
        std::cout << "Server: Sent Notification = " << notification << std::endl;
    }
}

int main() {
    app = vsomeip::runtime::get()->create_application("server");
    app->init();
    app->register_message_handler(SERVICE_ID, INSTANCE_ID, SPEED_VALUE_METHOD_ID, on_speed_value);
    app->offer_service(SERVICE_ID, INSTANCE_ID, SERVER_ID);
    app->offer_event(SERVICE_ID, INSTANCE_ID, NOTIFICATION_EVENT_ID, {NOTIFICATION_EVENT_ID});
    app->start();
}
