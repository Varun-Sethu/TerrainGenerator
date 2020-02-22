#ifndef __MAP_H__
#define __MAP_H__

#include <SFML/Graphics.hpp>
#include <string>
#include "biomes.h"

class Map : public sf::Drawable, public sf::Transformable {

public:


    // Loads a specific texture set and generates a map
    bool load(std::string tilesetLocation, sf::Vector2u tileSize, sf::Vector2u mapDimensions, const float* data) {
        // Load the tileset
        if (!__tilest.loadFromFile(tilesetLocation)) return false;

        // Resize the vertex array
        __verticies.setPrimitiveType(sf::Quads);
        __verticies.resize(mapDimensions.x * mapDimensions.y * 4);


        // Populate the verticies
        for (int i = 0; i < mapDimensions.x; ++i) {
            for (int j = 0; j < mapDimensions.y; ++j) {
                // Determine the provided texture value of the tile we are computing
                float texture = (float) determineBiome(data[i + j*mapDimensions.x]);
                sf::Vertex* cell = &__verticies[(i + j*mapDimensions.x) * 4];

                // Set the relative positions of this vertex
                cell[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                cell[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                cell[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                cell[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
                // The function assumes that when texturing, the height of a tile = the width of a file and thus the height of the tileset
                int textureDim = __tilest.getSize().y;

                // Set the textures
                cell[0].texCoords = sf::Vector2f(texture * textureDim, 0);
                cell[1].texCoords = sf::Vector2f((texture + 1) * textureDim, 0);
                cell[2].texCoords = sf::Vector2f((texture + 1) * textureDim, textureDim);
                cell[3].texCoords = sf::Vector2f(texture * textureDim, textureDim);
            }
        }
        return true;
    }


private:

    // Function that mus be overriden for drawing
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &__tilest;
        target.draw(__verticies, states);
    }

    sf::VertexArray __verticies;
    sf::Texture __tilest;
};



#endif