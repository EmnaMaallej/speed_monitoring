#include "client1.hpp"
#include <iostream>

Client1::Client1() {
    app_ = vsomeip::runtime::get()->create_application("Client1");
    app_->register_message_handler(vsomeip::ANY_SERVICE, vsomeip::ANY_INSTANCE, vsomeip::ANY_METHOD,
                                  std::bind(&Client1::on_response, this, std::placeholders::_1));
    app_->register_message_handler(0x1234, 0x0002, vsomeip::ANY_METHOD,
                                  std::bind(&Client1::on_server_response, this, std::placeholders::_1));
}

void Client1::start() {
    app_->init();
    app_->start();
}

void Client1::send_speed_request() {
    auto request = vsomeip::runtime::get()->create_request();
    request->set_service(0x1234);
    request->set_instance(0x0001);
    request->set_method(0x0001);
    app_->send(request, std::make_shared<vsomeip::endpoint>(client2_endpoint_, vsomeip::protocol_type::TCP));
}

void Client1::on_response(const std::shared_ptr<vsomeip::message>& msg) {
    float speed_tourmin = *reinterpret_cast<float*>(msg->get_payload()->get_data());
    float speed_kmh = convert_tourmin_to_kmh(speed_tourmin);
    send_speed_to_server(speed_kmh);
}

float Client1::convert_tourmin_to_kmh(float tourmin) {
    return tourmin * 0.1885; // Placeholder conversion factor
}

void Client1::send_speed_to_server(float speed_kmh) {
    auto msg = vsomeip::runtime::get()->create_request();
    msg->set_service(0x1234);
    msg->set_instance(0x0002);
    msg->set_method(0x0002);
    msg->get_payload()->set_data(&speed_kmh, sizeof(speed_kmh));
    app_->send(msg, std::make_shared<vsomeip::endpoint>(server_endpoint_, vsomeip::protocol_type::TCP));
}

void Client1::on_server_response(const std::shared_ptr<vsomeip::message>& msg) {
    if (msg->get_method() == 0x0003) {
        std::cout << "Received Ack" << std::endl;
    } else if (msg->get_method() == 0x0004) {
        std::cout << "Alert: Speed over 100 km/h" << std::endl;
    }
}
