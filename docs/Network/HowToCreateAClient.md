# How To Create A Client With Any Language 

This document provides a step-by-step guide on how to create a client for the R-Type project. The client is responsible for connecting to the main server, joining lobbies, and interacting with other players in real-time. By following these instructions, you will be able to create a functional client that can participate in multiplayer matches.

## Prerequisites

Before creating a client for the R-Type project, make sure you have the following prerequisites installed on your system:

- **CMake**: A cross-platform build system that generates native build files.
- **Clang++ Compiler**: A compiler that supports C++17 or higher.
- **Asio Library**: A cross-platform C++ library for network and low-level I/O programming.
- **Lz4 Library**: A fast compression algorithm library that optimizes data transmission over the network.

## Steps to Create a Client

Follow these steps to create a client for the R-Type project:

### Step 1: Connect to the Main Server

The first step is to establish a connection with the main server. Use a network library like Asio to create a UDP connection to the server's IP address and port number.

```cpp
#include <asio.hpp>

int main() {
    asio::io_context io_context;
    asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
    asio::ip::udp::endpoint server_endpoint(asio::ip::address::from_string("127.0.0.1"), 8080);

    socket.connect(server_endpoint);
    return 0;
}
```

```python
import socket

def main():
    server_address = ('127.0.0.1', 8080)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.connect(server_address)

if __name__ == '__main__':
    main()
```

### Step 2: Login and Authentication

Once connected to the server, you need to implement a login mechanism to authenticate the client. Send a login request to the server with the client's credentials and wait for a response.
I will show how the create a packet and send it to the server.

### How does the login work ?

You need to send a packet to the server that begin with a header that contains the size of the packet and the type of the packet. The server will read the header and then the data of the packet. The server will then check the type of the packet and will execute the corresponding function.

i will show you how to create a packet and send it to the server.
    
If you are using our Packet class, you can create a packet like this:

```cpp
#include "Packet.hpp"
#include "PacketFactory.hpp"
#include "PacketACK.hpp"
#include "PacketCMD.hpp"

int main() {
    
    //Connect to the server
    
    {...}

    std::string message = "login user password";
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
    
    dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
    packet->send_packet(remote_endpoint_);
    return 0;
}
```

if you are not using our Packet class, you can create a packet by setting the size of the packet and the type of the packet and then send it to the server.

So to login the header will look like this:

size = 1 - 3 bytes | id = 1 - 2 bytes | type = 1 bytes

#### in this casse `type` will be a two bytes equal to 2, `id` will be a two bytes equal to 1 and `size` will be a three bytes equal to the size of the data.

The data will look like this: `login user password`

Then just send the packet to the server.

```python
import socket

def main():
    server_address = ('127.0.0.1', 8080)
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.connect(server_address)
    
    message = "login user password"
    message = message.encode()
    
    size = len(message).to_bytes(3, byteorder='big')
    id = 1.to_bytes(2, byteorder='big')
    type = 2.to_bytes(1, byteorder='big')
    
    packet = size + id + type + message
    client_socket.send(packet)
    
if __name__ == '__main__':
    main()
```

### Step 3: Join a Lobby

After successful authentication, the client can join a lobby by sending a join request to the server. The server will assign a unique lobby ID to the client, allowing them to interact with other players in the same lobby.

### How does the join / create lobby work ?

You need to send a packet to the server that begin with a header that contains the size of the packet and the type of the packet. The server will read the header and then the data of the packet. The server will then check the type of the packet and will execute the corresponding function.

i will show you how to create a packet and send it to the server.

If you are using our Packet class, you can create a packet like this:

```cpp
#include "Packet.hpp"
#include "PacketFactory.hpp"
#include "PacketACK.hpp"
#include "PacketCMD.hpp"

int main() {
    
    //Connect to the server
    
    {...}

    std::string message = "join_lobby lobby_id";
    //std::string message = "create_lobby"; if you want to create a lobby
    
    auto packet = PacketFactory::create_packet(PacketFactory::TypePacket::CMD, socket_);
        
    dynamic_cast<PacketCMD*>(packet.get())->format_data(message);
    packet->send_packet(remote_endpoint_);
    return 0;
}
```

if you are not using our Packet class, you can create a packet by setting the size of the packet and the type of the packet and then send it to the server.

So to join a lobby the header will look like this:

size = 1 - 3 bytes | id = 1 - 2 bytes | type = 1 bytes

#### in this casse `type` will be a two bytes equal to 2, `id` will be a two bytes equal to 1 and `size` will be a three bytes equal to the size of the data.

The data will look like this: `join_lobby lobby_id` or `create_lobby`

Then just send the packet to the server.

no need to show the python code, it's the same as the login.

### Step 4: Interact with Other Players

Once in a lobby, the client can interact with other players by sending messages, game commands, or other data. Implement the necessary communication protocols to exchange information with other clients in real-time.

### Conclusion

By following these steps, you can create a client for the R-Type project that can connect to the main server, join lobbies, and interact with other players. Make sure to handle network errors and edge cases to ensure a smooth multiplayer experience for all participants.


