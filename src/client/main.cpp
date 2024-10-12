/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include "game/Game.hpp"
#include "networkClient/Client.hpp"
#include "thread"

// void run_network(cshar* argv[]){
    // asio::io_context io_context;
    // client::Client client(io_context, argv[1], std::stoi(argv[2]));

    // client.main_loop();
// }

// void run_network(client::Client& client) {
    // client.main_loop();
// }

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./r-type_client <host> <port>" << std::endl;
        return 1;
    }

    asio::io_context io_context;
    client::Client client(io_context, argv[1], std::stoi(argv[2]));

    rtype::Game game("R-Type", 1920, 1080, client);

    try {
        std::thread network_thread(&client::Client::main_loop, &client);
        game.run();
        network_thread.join();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid port number: " << argv[2] << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Port number out of range: " << argv[2] << std::endl;
        return 1;
    }

    return 0;
}
