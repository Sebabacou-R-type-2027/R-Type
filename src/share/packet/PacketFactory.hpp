#ifndef PACKETFACTORY_HPP
#define PACKETFACTORY_HPP

#include <cstdint>
#include <exception>
#include <memory>
#include <string>

#include <asio/ip/udp.hpp>

class Packet;

class PacketFactory {
    public:
        /// \class PacketFactoryException
        /// \brief Exception class for packet
        ///
        /// Create the exception with the packet information
        class PacketFactoryException : public std::exception {
            public:
                /// \brief Constructor for PacketFactoryException
                /// \param message the message to be displayed
                /// \param id the id of the packet that caused the exception
                explicit PacketFactoryException(std::string message, const uint16_t id)
                    : packet_error_("PacketFactoryError[" + std::to_string(id) + "]: "), msg_(std::move(message)) {}

                /// \brief Returns the error message
                /// \return the error message with the packet information
                [[nodiscard]]
                const char* what() const noexcept override {
                    full_message_ = packet_error_ + msg_;
                    return full_message_.c_str();
                }

            private:
                std::string packet_error_; ///< Specific error message for the packet
                std::string msg_; ///< Error message
                mutable std::string full_message_; ///< Full error message
        };

        /// \enum TypePacket
        /// \brief Enum for the type of the packet
        enum TypePacket {
            PING = 0,
            ACK = 1,
            CMD = 2,
            CMDP = 3,
        };

        /// \brief Create a packet
        /// \param type the type of the packet
        /// \return the packet created as a unique_ptr
        static std::unique_ptr<Packet> create_packet(TypePacket type, asio::ip::udp::socket& socket_);

        static void up_id();


    private:
        static uint16_t id_;

};

#endif //PACKETFACTORY_HPP
