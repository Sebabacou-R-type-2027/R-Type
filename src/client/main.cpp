#include "networkClient/Client.hpp"

int main(int argc, char* argv[]) {
    asio::io_context io_context;

    // Remplace "127.0.0.1" et 9000 avec l'adresse IP et le port du serveur
    if (argc != 3) {
        std::cerr << "Usage: ./r-type_client <host> <port>" << std::endl;
        return 1;
    }
    client::Client client(io_context, argv[1], std::stoi(argv[2]));

    // Lancement de la boucle principale du client
    client.main_loop();

    return 0;
}