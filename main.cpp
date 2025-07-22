#include "client1.hpp"
#include "client2.hpp"
#include "server.hpp"
#include <thread>

int main() {
    Client1 client1;
    Client2 client2;
    Server server;

    std::thread client1_thread(&Client1::start, &client1);
    std::thread client2_thread(&Client2::start, &client2);
    std::thread server_thread(&Server::start, &server);

    client1.send_speed_request(); // Trigger the process

    client1_thread.join();
    client2_thread.join();
    server_thread.join();

    return 0;
}
