#ifndef CLIENTSAVER_HPP
#define CLIENTSAVER_HPP

#include <fstream>

#include "client.hpp"

/// \namespace server
/// \brief Namespace for the server
namespace server {
    /// \class ClientSaver
    /// \brief Class to save the client information
    class ClientSaver {
        public:
            class ClientSaverException : public std::exception {
                public:
                    /// \brief Constructor for ClientSaverException
                    /// \param message the message to be displayed
                    /// \param clis the client saver that caused the exception
                    explicit ClientSaverException(std::string message, const ClientSaver& clis)
                        : error_("ClientSaverError[" + clis.filename_ + "]: "), msg_(std::move(message)) {}

                    /// \brief Returns the error message
                    /// \return the error message with the filename
                    [[nodiscard]] const char* what() const noexcept override {
                        return (error_ + msg_).c_str();
                    }

                private:
                    std::string error_; ///< Specific error message for the client saver
                    std::string msg_; ///< Error message
            };

            /// \brief Constructor for ClientSaver
            /// \param filename the filename to save the client information
            /// \details explicit constructor to avoid implicit conversion, meaning that the constructor will only be called when the filename is passed as an argument
            explicit ClientSaver(const std::string& filename);

            /// \brief Destructor for ClientSaver
            ~ClientSaver() = default;

            /// \brief Get the filename
            /// \return the filename to save the client information
            [[nodiscard]]
            std::string get_filename() const;

            /// \brief Set the filename
            /// \param filename the filename to save the client information
            void set_filename(const std::string& filename);

            /// \brief Save the client information
            /// \param cli the client to save
            void save_client(const client& cli);

      private:
            /// \brief Initialize the file
            /// \details Open the file and write the header
            void init_file();

            std::ofstream file; ///< The file to save the client information
            std::string filename_; ///< The filename to save the client information
    };
}

#endif //CLIENTSAVER_HPP
