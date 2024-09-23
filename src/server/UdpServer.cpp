#include "UdpServer.hpp"

#include <iostream>
#include <asio.hpp>

#include "client/client.hpp"
#include "client/ClientSaver.hpp"


UdpServer::UdpServer(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
    start_receive();
}

void UdpServer::start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                handle_receive(bytes_recvd);
            } else {
                start_receive();
            }
        });
}

void UdpServer::handle_receive(std::size_t bytes_transferred) {
    const auto message = std::string(recv_buffer_.data(), bytes_transferred);
    std::string client_str = remote_endpoint_.address().to_string() + ":" + std::to_string(remote_endpoint_.port());

    std::cout << "sender: " << client_str << std::endl;
    std::cout << "Authorised clients:" << std::endl;
    for (const auto& client : connected_clients_) {
        std::cout << "\t" << client << std::endl;
    }
    if (message.rfind("login ", 0) == 0) {
        std::string username, password;
        std::istringstream iss(message.substr(6));

        std::getline(iss, username, ' ');
        std::getline(iss, password);

        if (!username.empty() && !password.empty() && password.find(' ') == std::string::npos) {
            handle_new_connection(remote_endpoint_, username, password);
        } else {
            std::cout << "Invalid login format" << std::endl;
        }
    } else if (message == "logout") {
        handle_disconnect(remote_endpoint_);
    } else {
        auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_str](const server::client& cli) {
            return cli.get_ip() + ":" + cli.get_port() == client_str;
        });
        if (it != connected_clients_.end()) {
            std::cout << "Received: " << message << " from: " << client_str << std::endl;
        } else {
            std::cout << "Not login client: " << client_str << std::endl;
        }
    }

    start_receive();
}

void UdpServer::handle_new_connection(const udp::endpoint& client_endpoint, const std::string& username, const std::string& password) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());
    server::ClientSaver cs("clients.csv");

    auto it = std::find_if(connected_clients_.begin(), connected_clients_.end(), [&client_endpoint](const server::client& cli) {
        return cli.get_id() == std::hash<std::string>{}(client_endpoint.address().to_string() + std::to_string(client_endpoint.port()));
    });

    try {
        uint32_t id = cs.check_if_user_already_exists_in_db(username, password);
        if (id) {
            try {
                connected_clients_.emplace_back(client_address, client_port, username, password, id);
                std::cout << "New authorised client from db: " << client_address << ":" << client_port << std::endl;
            } catch (const server::client::ClientException& e) {
                std::cout << e.what() << std::endl;
            }
        } else if (it == connected_clients_.end()) {
            connected_clients_.emplace_back(client_address, client_port, username, password);
            std::cout << "New authorised client: " << client_address << ":" << client_port << std::endl;
            cs.save_client(connected_clients_.back());
        } else {
            std::cout << "Client already authorised: " << client_address << ":" << client_port << std::endl;
        }
    } catch (const server::ClientSaver::ClientSaverException& e) {
        std::cerr << e.what() << std::endl;
    } catch (const server::client::ClientException& e) {
        std::cout << e.what() << std::endl;
        std::cout << "Password or Username is not correct" << std::endl;
    }
}

void UdpServer::handle_disconnect(const udp::endpoint& client_endpoint) {
    std::string client_address = client_endpoint.address().to_string();
    std::string client_port = std::to_string(client_endpoint.port());

    auto it = std::remove_if(connected_clients_.begin(), connected_clients_.end(), [&client_address, &client_port](const server::client& cli) {
        return cli.get_ip() == client_address && cli.get_port() == client_port;
    });

    if (it != connected_clients_.end()) {
        connected_clients_.erase(it, connected_clients_.end());
        std::cout << "Client disconnected: " << client_address << ":" << client_port << std::endl;
    } else {
        std::cout << "Client not found: " << client_address << ":" << client_port << std::endl;
    }
}