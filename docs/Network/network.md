# R-Type

## General Introduction

The R-Type project is a multiplayer network game inspired by the classic shoot'em up genre. The main objective of this documentation is to detail the design and implementation of the network system used in the project, explaining the technological choices, communication mechanisms, and data structure exchanges. This documentation aims to provide a comprehensive overview of how the network operates, from packet creation to transmission, while exploring the challenges and solutions implemented to ensure a smooth and responsive gaming experience.

### Network Objectives

The network must meet the following requirements:
- **Low Latency**: Ensure fast data exchanges to maintain game responsiveness.
- **Reliability**: Effectively manage packet loss and guarantee session continuity.
- **Scalability**: Allow multiple players to connect simultaneously while maintaining performance.

This documentation is intended as a comprehensive resource for developers who want to understand the internal workings of the network, the rationale behind the technological choices, and the best practices adopted for data transmission in the R-Type project.

## Inspiration

The development of the R-Type network was heavily inspired by the principles established by successful multiplayer games, particularly **Age of Empires**, known for its clever **peer-to-peer** based solution. The network architecture of Age of Empires was designed to minimize latency and optimize player synchronization, two critical aspects in real-time multiplayer games.

### Why Draw Inspiration from the Peer-to-Peer Model?

Unlike the traditional client-server model, where a central server handles most of the computation and data exchange, the **peer-to-peer** model offers several advantages:
- **Reduced Latency**: Direct exchanges between peers reduce latency, enhancing game responsiveness.
- **Load Distribution**: Each player contributes to data transmission and processing, reducing the load on a single server and enabling better scalability.
- **Resilience to Failure**: The network can be designed to tolerate the disconnection of a peer without disrupting the session for other players, making the experience more robust.

### Influence of Age of Empires

The **Age of Empires** network relies on a deterministic synchronization where each peer executes the same actions simultaneously, ensuring all participants remain synchronized. This concept is often called **lockstep simulation**. In the R-Type project, similar mechanisms were considered to ensure that each player has a consistent view of the game state while minimizing perceived latency.

### Implementation of Inspired Ideas

For the R-Type network, the implementation relies on:
- **Peer-to-Peer Communication Protocol**: Adapting packet distribution techniques between peers to synchronize game states.
- **Synchronization and Validation**: Each action initiated by a player is validated and propagated to other peers to ensure game consistency.
- **Failure Tolerance Management**: Mechanisms for network recovery and reorganization in case a peer disconnects, allowing the game to continue uninterrupted.

These choices inspired by **Age of Empires** enable an optimized multiplayer experience, meeting the demands of fluidity and reliability required for a real-time game like R-Type.

## Technological Choices

The integration of **Asio** and **Lz4** was motivated by in-depth comparative studies. You can review the details of these studies below:

- [Comparative Study on Asio](./Etudes/ComparativeStudyAsio.pdf): Analysis of alternatives for network communication management.
- [Comparative Study on Lz4](./Etudes/ComparativeStudyCompressionLib.pdf): Comparison of compression algorithms and their impact on network performance.

### Conclusion on Technological Choices

The integration of **Asio** and **Lz4** in the R-Type project addresses the critical needs of the network, namely speed, responsiveness, and efficiency. **Asio** provides a flexible and robust solution for managing asynchronous network communication, while **Lz4** ensures that compressed data can be transmitted quickly without compromising overall system performance. These choices create a performant and resilient network capable of meeting the demands of a competitive and immersive multiplayer game.

## Network Architecture

### 1. Communication Model

The R-Type project uses a hybrid approach where the **main server** handles the initial coordination of lobbies and connections, while **peer-to-peer** communication elements can be integrated for optimized client-to-client exchanges. This allows the project to benefit from the robustness of a centralized server and the speed of direct peer interactions.

#### Example of Game Launch
![Game Launch](./images/img_3.png)

When a game is launched:
- Each client sends a ping request to the main server to assess latency.
- The server chooses the client with the best ping to host the game, ensuring minimal latency for most participants.
- `GAME_LAUNCH` requests are sent to other clients to synchronize the game start.

### 2. Lobby Management Protocol

The lobby management process consists of three main steps:
1. **Creating a Lobby**: A client sends a request to the main server to create a lobby. The server generates a unique `LOBBY_ID`, which is returned to the initiating client.
2. **Joining a Lobby**: Other clients use the `LOBBY_ID` to send a `join_lobby` request to the server and join the same instance.
3. **Player Synchronization**: Once all players are connected, the lobby is synchronized and ready for game launch.

![Lobby Protocol](./images/img_2.png)

### 3. Basic Client-Server Protocol

The basic protocol handles authentication and standard interactions such as lobby creation and player state updates. Each interaction between the client and server is confirmed with an acknowledgment (`ACK`).

![Client-Server Protocol](./images/img.png)

### 4. Chat Box Protocol

The game includes a chat protocol that allows players to communicate during matches. Messages are compressed to optimize bandwidth and sent to the main server, which then distributes them to all clients in the lobby. Each client decompresses and displays the received messages.

![Chat Protocol](./images/img_1.png)

#### Chat Protocol Steps:
1. The client sends a compressed message to the main server.
2. The server adds the message to the compressed chat history.
3. The server distributes the compressed message to all clients in the lobby.
4. Each client decompresses the message and displays it in the user interface.

### Conclusion

This network architecture, which combines client-server and peer-to-peer aspects, ensures a smooth and responsive gaming experience. Choosing to use a main server for initial coordination and lobby management while allowing direct peer communications when possible ensures reduced latency and optimal player synchronization.
