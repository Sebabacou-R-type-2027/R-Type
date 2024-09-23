#include "ClientSaver.hpp"

#include <iostream>

namespace server {
    ClientSaver::ClientSaver(const std::string& filename) {
        set_filename(filename);
        init_file();
    }

    std::string ClientSaver::get_filename() const {
        return this->filename_;
    }

    void ClientSaver::set_filename(const std::string& filename) {
        this->filename_ = "ERROR";
        if (filename.empty()) {
            this->filename_ = "ERROR";
            throw ClientSaverException("Filename cannot be empty", *this);
        }
        if (filename.find(".csv") == std::string::npos) {
            this->filename_ = "ERROR";
            throw ClientSaverException("Filename must be a .csv file", *this);
        }
        this->filename_ = filename;
        init_file();
    }

    void ClientSaver::save_client(const client& cli) {
        std::cout << "Saving client start: " << cli.get_ip() << ":" << cli.get_port() << std::endl;
        this->file.open(this->filename_, std::ios::out | std::ios::app);
        if (!this->file.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        this->file << cli.get_ip() << ","
            << cli.get_port() << ","
            << cli.get_nickname() << ","
            << cli.get_id() << ","
            << cli.get_password() << std::endl;
        this->file.close();
    }

    void ClientSaver::init_file() {
        const std::string header = "IP,Port,username,ID";

        this->file.open(this->filename_, std::ios::out | std::ios::app);
        if (!this->file.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        if (this->file.tellp()== 0) {
            this->file << header << std::endl;
        }
        this->file.close();
    }
}
