//
// Created by shooting_star_t33 on 9/18/24.
//

#include "UdpServer.hpp"

#include <iostream>
#include <asio.hpp>


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
    std::cout << "Received: " << std::string(recv_buffer_.data(), bytes_transferred) << std::endl;
    start_receive(); // Continue to receive the next message
}