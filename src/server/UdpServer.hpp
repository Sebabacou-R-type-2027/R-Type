#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <asio.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "lobby/Lobby.hpp"
#include "matchmaking/MatchmakingSystem.hpp"
#include "matchmaking/MatchmakingQueue.hpp"
#include "client/client.hpp"
#include "client/ClientSaver.hpp"
#include "Packet.hpp"
#include "PacketPing.hpp"

using asio::ip::udp;

/**
 * @class UdpServer
 * @brief La classe UdpServer gère la communication UDP entre le serveur et les clients ainsi que la gestion des lobbies.
 */
class UdpServer {
    public:
        /**
         * @brief Constructeur de la classe UdpServer.
         * @param io_context Contexte d'entrée/sortie utilisé par Asio.
         * @param port Port sur lequel le serveur écoute les connexions.
         */
        UdpServer(asio::io_context &io_context, short port);

        /**
         * @brief Destructeur de la classe UdpServer.
         */
        ~UdpServer();

        /**
         * @brief Boucle principale du serveur pour gérer les communications et les événements.
         */
        void server_loop();

        /**
         * @brief Gère un message reçu d'un client.
         * @param message Le message reçu.
         * @param client_endpoint L'adresse du client qui a envoyé le message.
         */
        void handle_client_message(const std::string& message, const asio::ip::udp::endpoint& client_endpoint, std::size_t bytes_recv);

        /**
         * @brief Récupère un client basé sur son endpoint.
         * @param client_endpoint L'adresse du client.
         * @return Le client correspondant à l'endpoint donné.
         */
        server::client& get_client(const asio::ip::udp::endpoint& client_endpoint);

    private:
        /**
         * @brief Démarre la réception des messages du client.
         */
        void start_receive();

        /**
         * @brief Boucle dédiée à la réception de messages dans un thread séparé.
         */
        void receive_loop();

        /**
         * @brief Gère la réception des messages du client.
         * @param bytes_transferred Le nombre d'octets reçus.
         */
        void handle_receive(std::size_t bytes_transferred);

        /**
         * @brief Gère une nouvelle connexion client.
         * @param client_endpoint L'adresse du client qui se connecte.
         */

       void handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password);

        /**
         * @brief Gère la déconnexion d'un client.
         * @param client_endpoint L'adresse du client qui se déconnecte.
         */
        void handle_disconnect(const udp::endpoint& client_endpoint);

        /**
         * @brief Envoie un ping à un client pour choisir un hôte.
         * @param client_endpoint L'adresse du client.
         */
        void ping_to_choose_host(const udp::endpoint &client_endpoint);

        /**
         * @brief Crée un nouveau lobby.
         * @param message Le message contenant les informations pour créer le lobby.
         */
        void create_lobby(const std::string& message);

        /**
         * @brief Permet à un client de rejoindre un lobby existant.
         * @param message Le message contenant les informations pour rejoindre le lobby.
         */
        void join_lobby(const std::string& message);

        /**
         * @brief Permet à un client de quitter un lobby.
         * @param message Le message contenant les informations pour quitter le lobby.
         */
        void leave_lobby(std::string message);

        void logout(const std::string& message);

        /**
         * @brief Envoie un ping à un client pour vérifier sa connexion.
         * @param client_endpoint L'adresse du client.
         */
        void send_ping(const udp::endpoint& client_endpoint);

        /**
         * @brief Démarre une nouvelle partie.
         * @param message Le message contenant les informations nécessaires pour démarrer la partie.
         */
        void start_game(const std::string& message);
        void send_message(const std::string& message, const udp::endpoint& endpoint);
        void add_client_to_matchmaking(const std::string& message);
        void handle_matchmaking_queue();
        void send_lobby_info(const std::string& message);
        bool everyone_ready(std::vector<server::client> clients);
        void lauch_game(const std::vector<server::client>& clients);
        std::chrono::milliseconds set_elapsed_time(std::vector<server::client>& clients);
        void handle_login(const std::string& message);
        void get_best_score_cli(const server::client& client);
        void get_best_score_cli(std::string message);
        void new_score_cli(std::string message);
        void execute_function(const std::string& message, std::string client_str);

        asio::io_context& io_context_; ///< Contexte d'entrée/sortie d'Asio.
        udp::socket socket_; ///< Socket UDP pour gérer les connexions.
        udp::endpoint remote_endpoint_; ///< Endpoint du client distant.
        std::array<char, 65535> recv_buffer_; ///< Buffer pour stocker les messages reçus.
        std::size_t recv_buffer_size_ = 0; ///< Taille du buffer.
        std::vector<server::client> connected_clients_; ///< Liste des clients connectés.
        std::map<int, Lobby> lobbies_; ///< Liste des lobbies disponibles.
        std::thread receive_thread_; ///< Thread pour la réception des messages.
        std::mutex messages_mutex_; ///< Mutex pour protéger l'accès aux messages.
        std::condition_variable messages_condition_; ///< Condition variable pour la gestion des messages reçus.
        std::unordered_map<int, std::pair<std::array<char, 65535>, udp::endpoint>> received_messages_; ///< Map des messages reçus avec leur ID et leur endpoint.
        int message_id_counter_ = 0; ///< Compteur d'ID pour les messages.
        int lobby_id_ = 0; ///< Compteur d'ID pour les lobbies.

        std::map<std::string, std::function<void(const std::string&)>> function_map_; ///< Map des fonctions pour gérer les différents types de messages.
        MatchmakingSystem matchmaking_system_; ///< Système de matchmaking pour gérer les parties.
};

#endif //UDPSERVER_HPP
