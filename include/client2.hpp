#ifndef CLIENT2_HPP
#define CLIENT2_HPP

#include <vsomeip/vsomeip.hpp>

class Client2 {
public:
    Client2();
    void start();
    void on_speed_request(const std::shared_ptr<vsomeip::message>&);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::string client1_endpoint_ = "127.0.0.1:30000"; // Assume Client1 listens here
    float get_current_speed();
};

#endif
