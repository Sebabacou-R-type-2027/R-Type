#pragma once
#include "CommandHandler.hpp"
#include "Packet.hpp"
#include "PacketCMD.hpp"
#include <asio.hpp>
#include <atomic>
#include <iostream>
#include <thread>

#ifndef CLIENT_HPP
#define CLIENT_HPP

using asio::ip::udp;

namespace client {
    /**
     * @brief Classe représentant un client réseau.
     *
     * Cette classe gère la connexion d'un client à un serveur via UDP et la création d'un client host,
     * l'envoi et la réception de paquets, ainsi que l'exécution de commandes.
     */
    class Client {
        public:
            /**
             * @brief Constructeur de la classe Client.
             *
             * Initialise la connexion avec le serveur à partir de son IP et de son port.
             *
             * @param io_context Contexte d'E/S pour ASIO.
             * @param server_ip Adresse IP du serveur.
             * @param server_port Port du serveur.
             */
            Client(asio::io_context& io_context, const std::string& server_ip, short server_port);

            /**
             * @brief Destructeur de la classe Client.
             *
             * Termine les threads et libère les ressources.
             */
            ~Client();

            /**
             * @brief Boucle principale de la logique du client.
             *
             * Cette méthode est utilisée pour gérer les opérations principales du client,
             * comme la gestion des commandes et la réception de messages.
             */
            void main_loop();

            /**
             * @brief Envoie un paquet au serveur.
             *
             * @param packet Référence vers le paquet à envoyer.
             */
            void send_packet(Packet& packet) const;

            /**
             * @brief Envoie un message sous forme de chaîne de caractères.
             *
             * @param message Le message à envoyer.
             */
            void send_message(const std::string &message);

            /**
             * @brief Gère la réception de messages du serveur.
             *
             * @param bytes_transferred Le nombre d'octets transférés.
             */
            void handle_receive(std::size_t bytes_transferred, const udp::endpoint& sender_endpoint);

            /**
             * @brief Insère des commandes dans la liste des commandes à exécuter.
             *
             * @param commands Map contenant les commandes à insérer.
             */
            void insertCommands(const std::map<std::string, std::string>& commands);

            /**
             * @brief Remplit la liste des commandes à envoyer.
             *
             * @param command La commande à ajouter à la liste.
             */
            void fillCommandsToSends(std::string command);

            int number_of_players_ = 0;  ///< Nombre de joueurs actuellement connectés.
            int my_id_in_lobby_;
            std::map<std::string, std::string> _commandsToDo;  ///< Commandes à effectuer.
            std::atomic<bool> is_running_;  ///< Indique si le client est en cours d'exécution.
        private:
            bool im_host = false;  ///< Indique si le client est l'hôte du serveur.
            asio::io_context& io_context_;  ///< Contexte d'E/S ASIO.
            udp::socket socket_;  ///< Socket UDP utilisée pour la communication avec le serveur.
            udp::endpoint remote_endpoint_;  ///< Endpoint du serveur (adresse et port).
            std::thread receive_thread_;  ///< Thread utilisé pour la réception des messages.
            std::thread send_thread_;  ///< Thread utilisé pour l'envoi des messages.
            std::unordered_map<std::string, udp::endpoint> players_endpoints_;  ///< Map des endpoints des joueurs connectés.
            std::mutex messages_mutex_;  ///< Mutex pour protéger l'accès aux messages reçus.
            std::unordered_map<int, std::pair<std::string, udp::endpoint>> received_messages_; ///< Map des messages reçus (avec un ID et un endpoint).
            int message_id_counter_ = 0; ///< Compteur d'ID pour les messages.
            std::condition_variable messages_condition_;  ///< Condition variable pour la synchronisation des messages reçus.
            std::array<char, 65535> recv_buffer_;  ///< Buffer pour stocker les données reçues.
            std::size_t recv_buffer_size_ = 0;  ///< Taille du buffer.
            std::unique_ptr<CommandHandler> command_handler_;  ///< Gestionnaire de commandes du client.
            std::map<std::string, std::string> _commandsSend;  ///< Commandes à envoyer en tant que joueur.
            std::thread send_commands_thread_;

            /**
             * @brief Boucle de réception des messages.
             *
             * Cette méthode tourne en boucle pour recevoir les messages provenant du serveur.
             */
            void receive_loop();

            /**
             * @brief Démarre la réception des messages du serveur.
             *
             * Cette méthode configure le socket pour commencer à recevoir les messages.
             */
            void start_receive();

            /**
             * @brief Gère les messages reçus du serveur.
             *
             * @param bytes_transferred Nombre d'octets reçus.
             */
            void manage_message(std::size_t bytes_transferred, const udp::endpoint& sender_endpoint);

            /**
             * @brief Envoie un message à un joueur spécifique.
             *
             * @param message Message à envoyer.
             * @param player_endpoint Endpoint du joueur destinataire.
             */
            void send_message_to_player(const std::string& message, const udp::endpoint& player_endpoint);


            void send_commands_to_all_clients();
    };
}

#endif //CLIENT_HPP
