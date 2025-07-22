#include "client2.hpp"
#include <iostream>

Client2::Client2() {
    app_ = vsomeip::runtime::get()->create_application("Client2");
    app_->register_message_handler(0x1234, 0x0001, 0x0001,
                                  std::bind(&Client2::on_speed_request, this, std::placeholders::_1));
}

void Client2::start() {
    app_->init();
    app_->start();
}

void Client2::on_speed_request(const std::shared_ptr<vsomeip::message>& request) {
    float speed_tourmin = get_current_speed(); // Simulated
    auto response = vsomeip::runtime::get()->create_response(request);
    response->get_payload()->set_data(&speed_tourmin, sizeof(speed_tourmin));
    app_->send(response, std::make_shared<vsomeip::endpoint>(client1_endpoint_, vsomeip::protocol_type::TCP));
}

float Client2::get_current_speed() {
    return 120.0f; // Simulated speed in tour/min
}
