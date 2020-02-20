#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <cstdlib>


// generating file, generates a simple height map using perlin's noise function
class PerlinGenerator {
public:
    // Constructor to build the gradient array for the generator
    PerlinGenerator(uint cellSize, uint pageHeight, uint pageWidth): __cellSize(cellSize), __pageHeight(pageHeight), __pageWidth(pageWidth) {
        // Generate the gradients first
        __gH = ceil(pageHeight / cellSize) + 1; 
        __gW = ceil(pageWidth / cellSize) + 2;
        __scalingX = __pageWidth / cellSize;
        __scalingY =  __pageHeight / cellSize;

        // Used for generating random floating point numbers (used in unit vector generation)
        std::random_device rand_dev;
        std::mt19937 et(rand_dev());
        std::uniform_real_distribution<float> dist(-(3.141592), (3.141592));

        // Allocate the array of gradients on the heap
        __gradients = new sf::Vector2f*[__gW];
        for (int i = 0; i < __gW; i++) {            
            __gradients[i] = new sf::Vector2f[__gH];

            for (int j = 0; j < __gH; j++) {
                // Populate the newly allocated array with random unit vectors that correspond to gradient vectors
                float phi = dist(et);
                __gradients[i][j] = sf::Vector2f(cos(phi), sin(phi));
            }
        }
    }
    
    // Destructor to deallocate the gradient array
    ~PerlinGenerator() {
        for (int i = 0; i < __gW; i++) {
            delete[] __gradients[i];
        }
        delete[] __gradients;
    }



    // Function that returns the noise of a specific pixel
    float noise(uint px, uint py) {
        // Determine what cell this point falls under and get the list of gradients that must be dotted with the position vector
        uint boxX = floor((float) px / (float) __cellSize); 
        uint boxY = floor((float) py / (float) __cellSize);
        sf::Vector2f pixelRelativePosition = sf::Vector2f(
            ((px + 0.5) - boxX * __cellSize) / __cellSize, 
            ((py + 0.5) - boxY * __cellSize) / __cellSize
        );

        // Attain the graidnets
        sf::Vector2f gradients[4] = {
            __gradients[boxX][boxY], __gradients[boxX + 1][boxY], __gradients[boxX][boxY + 1], __gradients[boxX + 1][boxY + 1]
        };
        // Compute the appropriate distance vectors
        sf::Vector2f distances[4] = {
           sf::Vector2f(pixelRelativePosition.x, pixelRelativePosition.y), sf::Vector2f(pixelRelativePosition.x - 1, pixelRelativePosition.y), 
           sf::Vector2f(pixelRelativePosition.x, pixelRelativePosition.y - 1), sf::Vector2f(pixelRelativePosition.x - 1, pixelRelativePosition.y - 1)
        };
        // Now here comes the actual noise function, compute the dot product between every distance vector and the gradient vector and interpolate between these values
        float dotProducts[4] = {dotProduct(gradients[0], distances[0]), dotProduct(gradients[1], distances[1]), dotProduct(gradients[2], distances[2]), dotProduct(gradients[3], distances[3])};


        // Compute the weightings of each of the dot products
        float p0p1 = lerp(dotProducts[0], dotProducts[1], fade(pixelRelativePosition.x));
        float p2p3 = lerp(dotProducts[2], dotProducts[3], fade(pixelRelativePosition.x));

        // Final noise value is the interpolation of the first two interpolations
        float res = lerp(p0p1, p2p3, fade(pixelRelativePosition.y));
        return res;
    }  





private:
    sf::Vector2f** __gradients;
    uint __gH, __gW;
    uint __pageHeight, __pageWidth;
    float __scalingX, __scalingY; // The number of pixels per cell in the x and y direction
    uint __cellSize;

    // Compute the dot product between two vectors (for some reason SFML does not have this natively????) 
    inline float dotProduct(sf::Vector2f a, sf::Vector2f b) {
        return (a.x * b.x) + (a.y * b.y);
    }
    // Interpolation function
    inline float lerp(float x0, float x1, float w) {
        return (1.0 - w) * x0 + w * x1;
    }
    // Fade function smoothens out the function and prevents 0 values and corners near the box definitions
    inline float fade(float t) {
        return t*t*t*(t*(t*6.0 - 15.0) + 10.0);
    }
};


#endif