#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <exception>
#include <string>
#include <variant>
#include "PacketFactory.hpp"


/// \class Packet
/// \brief Class that represents a packet
/// \details The packet is the base class for all the packets
/// \details You always need to create a child class to use the packet
class Packet {
    public:
        struct uint24_t {
            uint8_t data[3];
        };
        /// \class PacketException
        /// \brief Exception class for packet
        ///
        /// Create the exception with the packet information
        class PacketException : public std::exception {
            public:
                /// \brief Constructor for PacketException
                /// \param message the message to be displayed
                /// \param id the id of the packet that caused the exception
                explicit PacketException(std::string message, const std::variant<uint8_t, uint16_t> id)
                    : packet_error_("PacketError[" + std::to_string(std::get<uint16_t>(id)) + "]: "), msg_(std::move(message)) {}

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


        /// \brief Send the packet
        void send_packet(); // TODO : check with ewen for how the send work;

        /// \brief Constructor for Packet class
        /// \param type the type of the packet
        /// \param id the id of the packet
        Packet(PacketFactory::TypePacket type, uint16_t id);

        /// \brief Destructor for Packet class
        virtual ~Packet() = default;

        /// \brief Get the size of the packet
        /// \return the size of the packet
        [[nodiscard]]
        uint32_t get_size() const;

        /// \brief Get the id of the packet
        /// \return the id of the packet
        [[nodiscard]]
        uint16_t get_idp() const;

        /// \brief Get the type of the packet
        /// \return the type of the packet
        [[nodiscard]]
        uint8_t get_type() const;

        /// \brief Get the data of the packet
        /// \return the data of the packet
        [[nodiscard]]
        std::string get_data() const;

        /// \brief Set the size of the packet
        /// \details The size of the packet is defined by the size of the data and the size of the id and type
        void set_size();

        /// \brief Set the id of the packet
        /// \details The id of the packet is generated by the factory
        void set_id(uint16_t id);

        /// \brief Set the type of the packet
        /// \details The type of the packet is defined by the child class
        void set_type(uint8_t type);

        /// \brief Set the data of the packet
        virtual void format_data() = 0;

        /// \brief Convert a number to uint24_t
        /// \param number the number to be converted
        /// \return the number converted to uint24_t
        [[nodiscard]]
        static uint24_t convert_to_uint24(uint32_t number);


        /// \brief Overload the operator<< for the packet
        /// \param os the output stream
        /// \param packet the packet to be displayed
        /// \return the output stream with the packet displayed
        friend std::ostream& operator<<(std::ostream& os, const Packet& packet);

        /// \brief Overload the operator<< for the uint24_t
        /// \param os the output stream
        /// \param value the uint24_t to be displayed
        /// \return the output stream with the uint24_t displayed
        friend std::ostream& operator<<(std::ostream& os, const uint24_t& value);

    protected:
        std::string data_; ///< The data of the packet
        bool is_data_set_ = false; ///< Check if the data is set

    private:

        /// \brief Get the size type of the packet
        /// \param number the number to be checked
        /// \return the size type of the packet
        [[nodiscard]]
        int get_size_varint(uint32_t number);

        std::variant<uint8_t, uint16_t, uint24_t> size_; ///< The size of the packet
        std::variant<uint8_t, uint16_t> idp_; ///< The id of the packet
        uint8_t type_; ///< The type of the packet
};

#endif //PACKET_HPP
