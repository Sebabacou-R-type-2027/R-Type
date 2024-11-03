import std;
import game;
import Client;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }
    asio::io_context io_context;
    auto client = std::make_shared<Client>(io_context);
    game::game game(client);

    try {
        std::thread network_thread(&Client::main_loop, client);
        client->connect(argv[1], std::stoi(argv[2]));
        game.begin_scene(std::make_unique<game::scenes::menu>(game)); // TODO : pass the Client object
        game.run();
        network_thread.join();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
