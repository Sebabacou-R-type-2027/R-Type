#include "ClientSaver.hpp"
#include "client.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

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

    void ClientSaver::save_client(const client& cli) const {
        std::ofstream outfile(this->filename_, std::ios::out | std::ios::app);

        if (!outfile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }

        if (check_if_user_already_exists_in_db(cli.get_nickname(), cli.get_password()) != 0) {
            change_field(cli.get_id(), "BestScore", std::to_string(cli.get_best_score()));
            change_field(cli.get_id(), "Username", cli.get_nickname());
            change_field(cli.get_id(), "Password", cli.get_password());
            return;
        }
        outfile << cli.get_id() << ","
            << cli.get_nickname() << ","
            << cli.get_password() << ","
            << cli.get_best_score() << std::endl;
        outfile.close();
    }

    std::string ClientSaver::get_username_by_id(uint32_t id) const {
        std::ifstream infile(this->filename_);
        std::string line;
        std::istringstream iss(line);
        std::string saved_username, saved_id;

        if (!infile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        while (std::getline(infile, line)) {
            std::getline(iss, saved_id, ',');
            if (std::stoul(saved_id) == id) {
                infile.close();
                return saved_username;
            }
        }
        infile.close();
        return "";
    }

    uint32_t ClientSaver::get_best_score_by_id(uint32_t id) const {
        std::ifstream infile(this->filename_);
        std::string line;
        std::istringstream iss(line);
        std::string saved_best_score, saved_id;

        if (!infile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        while (std::getline(infile, line)) {
            std::getline(iss, saved_id, ',');
            if (std::stoul(saved_id) == id) {
                infile.close();
                return std::stoul(saved_best_score);
            }
        }
        infile.close();
        return 0;
    }

    uint32_t ClientSaver::check_if_user_already_exists_in_db(const std::string &username, const std::string &password) const {
        std::ifstream infile(this->filename_);
        std::string saved_username, saved_password, id, line;
        size_t pos1, pos2;
        if (!infile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        std::getline(infile, line);
        while (std::getline(infile, line)) {
            pos1 = line.find(',');
            if (pos1 == std::string::npos) {
                throw ClientSaverException("Error parsing line: first comma not found", *this);
            }
            id = line.substr(0, pos1);
            pos2 = line.find(',', pos1 + 1);
            if (pos2 == std::string::npos) {
                throw ClientSaverException("Error parsing line: second comma not found", *this);
            }
            saved_username = line.substr(pos1 + 1, pos2 - pos1 - 1);
            saved_password = line.substr(pos2 + 1);
            if (saved_username == username) {
                if (saved_password == client::hash_password(password)) {
                    infile.close();
                    return std::stoul(id);
                }
                throw client::ClientException("Password does not match", username);
            }
        }
        infile.close();
        return 0;
    }

void ClientSaver::change_field(uint32_t id, const std::string& field, const std::string& value) const {
        std::ifstream infile(this->filename_);
        std::vector<std::string> lines;
        std::string line;

        bool found = false;
        if (!infile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }

        std::getline(infile, line);
        lines.push_back(line);

        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string saved_id, saved_username, saved_password, saved_best_score;

            std::getline(iss, saved_id, ',');
            std::getline(iss, saved_username, ',');
            std::getline(iss, saved_password, ',');
            std::getline(iss, saved_best_score, ',');

            if (std::stoul(saved_id) == id) {
                found = true;
                if (field == "Username") {
                    saved_username = value;
                } else if (field == "Password") {
                    saved_password = value;
                } else if (field == "BestScore") {
                    saved_best_score = value;
                } else {
                    throw ClientSaverException("Invalid field name", *this);
                }
                line = saved_id + "," + saved_username + "," + saved_password + "," + saved_best_score;
            }
            lines.push_back(line);
        }
        infile.close();

        std::ofstream outfile(this->filename_);
        if (!outfile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        for (const auto& l : lines) {
            outfile << l << "\n";
        }
        outfile.close();
    }

    void ClientSaver::init_file() const
    {
        const std::string header = "ID,Username,Password,BestScore";

        std::ofstream outfile(this->filename_, std::ios::out | std::ios::app);
        if (!outfile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }
        if (outfile.tellp() == 0) {
            outfile << header << std::endl;
        }
        outfile.close();
    }


    std::vector<std::pair<std::string, uint32_t>> ClientSaver::get_all_best_scores() const {
        std::map<std::string, uint32_t> best_scores;
        std::ifstream infile(this->filename_);
        std::string line, saved_username, saved_best_score;
        size_t pos1, pos2, pos3;

        if (!infile.is_open()) {
            throw ClientSaverException("Cannot open or create file", *this);
        }

        std::getline(infile, line);  // Skip header
        while (std::getline(infile, line) && best_scores.size() < 10) {
            pos1 = line.find(',');
            if (pos1 == std::string::npos) {
                throw ClientSaverException("Error parsing line: first comma not found", *this);
            }
            pos2 = line.find(',', pos1 + 1);
            if (pos2 == std::string::npos) {
                throw ClientSaverException("Error parsing line: second comma not found", *this);
            }
            pos3 = line.find(',', pos2 + 1);
            if (pos3 == std::string::npos) {
                throw ClientSaverException("Error parsing line: third comma not found", *this);
            }
            saved_username = line.substr(pos1 + 1, pos2 - pos1 - 1);
            saved_best_score = line.substr(pos3 + 1);
            best_scores[saved_username] = std::stoul(saved_best_score);
        }
        infile.close();

        // Convert map to vector of pairs for sorting by score
        std::vector<std::pair<std::string, uint32_t>> sorted_scores(best_scores.begin(), best_scores.end());

        // Sort vector by score in descending order
        std::sort(sorted_scores.begin(), sorted_scores.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        return sorted_scores;
    }
}