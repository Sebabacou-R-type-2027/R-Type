import std;
import game;

int main()
{
    game::game game;
    game.begin_scene(std::make_unique<game::scenes::menu>(game));

    game.run();

    return 0;
}
