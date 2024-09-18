//
// Created by shooting_star_t33 on 9/18/24.
//

#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <iostream>
#include <asio.hpp>
#include <string>


class package {
    public:
        package() = default;
        create_package(const std::string& data) : data_(data) {}
    private:
        std::string data_;


};



#endif //PACKAGE_HPP
