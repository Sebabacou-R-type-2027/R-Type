#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

int main() {
    try {
        asio::io_context io_context;

        // Résoudre l'adresse du serveur
        udp::resolver resolver(io_context);
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "localhost", "4242");

        // Créer et ouvrir le socket
        udp::socket socket(io_context);
        socket.open(udp::v4());

        // Message à envoyer
        std::string message = "";

        while (true) {
            std::cout << "Entrez un message à envoyer (ou 'exit' pour quitter) : ";
            std::getline(std::cin, message);

            if (message == "exit") {
                break; // Quitter la boucle si l'utilisateur entre 'exit'
            }

            // Envoyer le message
            socket.send_to(asio::buffer(message), *endpoints.begin());

            std::cout << "Message envoyé : " << message << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}