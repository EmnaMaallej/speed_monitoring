#include "server.hpp"
#include <iostream>

Server::Server() {
    app_ = vsomeip::runtime::get()->create_application("Server");
    app_->register_message_handler(0x1234, 0x0002, 0x0002,
                                  std::bind(&Server::on_speed_value, this, std::placeholders::_1));
}

void Server::start() {
    app_->init();
    app_->start();
}

void Server::on_speed_value(const std::shared_ptr<vsomeip::message>& msg) {
    float speed_kmh = *reinterpret_cast<float*>(msg->get_payload()->get_data());
    process_speed(speed_kmh);
}

void Server::process_speed(float speed_kmh) {
    auto response = vsomeip::runtime::get()->create_response();
    response->set_service(0x1234);
    response->set_instance(0x0002);
    if (speed_kmh < 100) {
        response->set_method(0x0003); // Ack
    } else {
        response->set_method(0x0004); // Alert
        response->get_payload()->set_data("Speed over 100 km/h", 19);
    }
    app_->send(response, std::make_shared<vsomeip::endpoint>(client1_endpoint_, vsomeip::protocol_type::TCP));
}
