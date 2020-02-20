#include <SFML/Graphics.hpp>
#include "map.h"




int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 512), "Terrain Generator");
    window.setVerticalSyncEnabled(true);

    const int level[] = {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

    Map generatedMap;
    if (!generatedMap.load("spritesheet.png", sf::Vector2u(4, 4), sf::Vector2u(16, 8), level)) {
        return -1;
    }

    while(window.isOpen()) {
        sf::Event event;
        
        // Keep the window open until the close event is called
        while(window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        window.draw(generatedMap);
        window.display();
    }



    return 0;
}