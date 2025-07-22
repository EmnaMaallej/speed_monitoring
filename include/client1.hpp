#ifndef CLIENT1_HPP
#define CLIENT1_HPP

#include <vsomeip/vsomeip.hpp>
#include <functional>

class Client1 {
public:
    Client1();
    void start();
    void send_speed_request();
    void on_response(const std::shared_ptr<vsomeip::message>&);
    void send_speed_to_server(float speed_kmh);
    void on_server_response(const std::shared_ptr<vsomeip::message>&);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::string client2_endpoint_ = "127.0.0.1:30001";
    std::string server_endpoint_ = "127.0.0.1:30002";
    float convert_tourmin_to_kmh(float tourmin);
};

#endif
