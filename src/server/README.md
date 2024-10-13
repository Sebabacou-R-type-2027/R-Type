# UDP Game Server

## Overview

This project implements a UDP-based server using the ASIO library. The server manages client connections, lobby creation, and host selection for multiplayer games based on client latency. It can handle multiple clients, allowing them to create or join lobbies and start games, with the server selecting the host dynamically.

## Features

- **Asynchronous UDP communication**: Utilizes ASIO for efficient, non-blocking network operations.
- **Lobby system**: Clients can create, join, and leave lobbies.
- **Host selection**: The server automatically selects the client with the lowest latency to act as the game host.
- **Client management**: Handles login, logout, and disconnect events from clients.

##  Structure

The project consists of several key components:

- **`UdpServer`**: Main class that handles UDP communication, lobby management, and client interactions.
- **`Lobby`**: Class that manages the state of individual game lobbies, including client membership and host designation.
- **Main entry point**: The `main.cpp` file initializes and runs the server.

### File Descriptions

- **`UdpServer.cpp`**: Implementation of the `UdpServer` class, which handles all client communication, lobby operations, and latency-based host selection.
- **`UdpServer.hpp`**: Header file for the `UdpServer` class.
- **`Lobby.cpp`**: Implementation of the `Lobby` class, which manages individual game lobbies.
- **`Lobby.hpp`**: Header file for the `Lobby` class.
- **`main.cpp`**: Main file that initializes the server and starts the main event loop.

## Requirements

- **C++17 or later**: The project makes use of modern C++ features.
- **ASIO**: A library for network and low-level I/O programming. Make sure to link with the appropriate ASIO library (or use Boost ASIO).
- **CMake**: For building and managing the project.

## Building and Running

### Prerequisites

- Install the required dependencies:
    - ASIO (or Boost ASIO if you're using Boost)
    - CMake

## Usage

### Starting the Server

To start the server, simply run the compiled `UdpServer` binary. The server will start listening for incoming client connections on the specified port.

### Client Interaction

Clients can:
- **Create a lobby**: Clients can send a message to create a new lobby, and the server will assign the client as the lobby host.
- **Join a lobby**: Clients can join existing lobbies by providing the correct lobby ID.
- **Leave a lobby**: Clients can leave a lobby, and if they are the host, the lobby will be deleted.
- **Start a game**: The server will select the client with the lowest latency as the game host by pinging all clients in the lobby.
