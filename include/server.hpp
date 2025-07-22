#ifndef SERVER_HPP
#define SERVER_HPP

#include <vsomeip/vsomeip.hpp>

class Server {
public:
    Server();
    void start();
    void on_speed_value(const std::shared_ptr<vsomeip::message>&);

private:
    std::shared_ptr<vsomeip::application> app_;
    std::string client1_endpoint_ = "127.0.0.1:30000"; // Assume Client1 listens here
    void process_speed(float speed_kmh);
};

#endif
