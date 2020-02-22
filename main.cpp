#include <SFML/Graphics.hpp>
#include <math.h>
#include "map.h"
#include "perlin.h"






int main() {
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "Terrain Generator");
    window.setVerticalSyncEnabled(true);

    // Generate the perlin noise map
    float level[256*256] = {};
    Perlin::perlinOctave(256, 256, 64, 4, 0.75f, level);


    // Build and display the map
    Map generatedMap;
    if (!generatedMap.load("spritesheet.png", sf::Vector2u(4, 4), sf::Vector2u(256, 256), level)) {
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