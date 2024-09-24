/// \file client.hpp
/// \brief file with the declaration of Client class

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <exception>
#include <string>
#include <cstdint>
#include <utility>
#include <asio/ip/udp.hpp>

/// \namespace server
/// \brief Namespace for the server
namespace server {
    /// \class client
    /// \brief Class to represent a client in the server
    class client {
        public:
            /// \class ClientException
            /// \brief Exception class for client
            ///
            /// Create the exception with the client information
            class ClientException : public std::exception {
                public:
                    /// \brief Constructor for ClientException
                    /// \param message the message to be displayed
                    /// \param cli the client that caused the exception
                    explicit ClientException(std::string message, const client& cli)
                        : client_error_("ClientError[" + cli.ip_ + ":" + cli.port_ + "]: "), msg_(std::move(message)) {}

                    /// \brief Returns the error message
                    /// \return the error message with the client information
                    [[nodiscard]] const char* what() const noexcept override {
                        return (client_error_ + msg_).c_str();
                    }

                private:
                    std::string client_error_; ///< Specific error message for the client
                    std::string msg_; ///< Error message
            };

            /// \brief Constructor for Client class
            /// \param ip the ip of the client
            /// \param port the port of the client
            /// \param username the username of the client
            /// \details The constructor will generate a unique id for the client
            client(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);

            /// \brief Destructor for Client class
            ~client() = default;

            /// \brief Get the ip of the client
            /// \return the ip of the client
            [[nodiscard]]
            std::string get_ip() const;

            /// \brief Get the port of the client
            /// \return the port of the client
            [[nodiscard]]
            std::string get_port() const;

            /// \brief Get the username of the client
            /// \return the username of the client
            [[nodiscard]]
            std::string get_nickname() const;

            /// \brief Set the username of the client
            /// \param username the username to be set
            void set_nickname(const std::string& username); // TODO: check if the username is unique

            /// \brief Get the id of the client
            /// \return the id of the client
            [[nodiscard]]
            uint32_t get_id() const;

            /// \brief Get the password of the client
            /// \return the password of the client hashed
            [[nodiscard]]
            std::string get_password() const;

            /// \brief Set the password of the client
            /// \param password the password to be set
            void set_password(const std::string& password); // TODO hash the password

            /// \brief Get the endpoint of the client
            /// \return the endpoint of the client
            [[nodiscard]]
            asio::ip::udp::endpoint get_endpoint() const;

            /// \brief Set the endpoint of the client
            /// \param endpoint the endpoint to be set
            void set_endpoint(const asio::ip::udp::endpoint& endpoint);

            /// \brief Check if the client is the host
            /// \return true if the client is the host, false otherwise
            [[nodiscard]]
            bool is_host() const;

            /// \brief Set the client as the host
            /// \param host true if the client is the host, false otherwise
            void set_host(bool host);

            /// \brief Check if the client is in game
            /// \return true if the client is in game, false otherwise
            [[nodiscard]]
            bool is_in_game() const;

            /// \brief Set the client as in game
            /// \param in_game true if the client is in game, false otherwise
            void set_in_game(bool in_game);

            /// \brief Set the client as ready
            /// \param ready true if the client is ready, false otherwise
            /// \details The client has to be in a lobby to be ready
            void set_ready(bool ready);

            /// \brief Check if the client is ready
            /// \return true if the client is ready, false otherwise
            [[nodiscard]] bool is_ready() const;

            void start_game();

            /// \brief Overload the << operator to print the client information
            /// \param os the output stream
            /// \param cli the client to be printed
            /// \return the output stream with the client information
            friend std::ostream& operator<<(std::ostream& os, const client& cli);

            /// \brief Overload the == operator to compare two clients
            /// \param cli1 the client to be compared
            /// \param cli2 the client to be compared
            /// \return true if the clients are the same, false otherwise
            friend bool operator==(const client& cli1, const client& cli2);

            /// \brief Overload the != operator to compare two clients
            /// \param cli1 the client to be compared
            /// \param cli2 the client to be compared
            /// \return true if the clients are different, false otherwise
            friend bool operator!=(const client& cli1, const client& cli2);

            /// \brief Overload the = operator to assign a client to another
            /// \param cli the client to be assigned
            /// \return the client assigned
            client& operator=(const client& cli) = default;

        private:
            /// \brief Generate a unique id for the client
            /// \details The id is generated using the ip, port and the current timestamp
            /// \return the unique id
            [[nodiscard]]
            uint32_t generate_id() const;

            std::string port_; ///< Port of the client
            std::string ip_; ///< IP of the client
            std::string username_; ///< username of the client, all username as to be unique
            std::string password_; ///< password of the client
            uint32_t id_; ///< Unique id of the client, max value is 2^32
            asio::ip::udp::endpoint endpoint_; ///< Endpoint of the client
            bool host_; ///< True if the client is the host
            bool in_game_; ///< True if the client is in game
            bool is_ready_ = false; ///< True if the client is ready

        };
}

#endif