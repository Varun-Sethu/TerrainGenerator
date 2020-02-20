#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <SFML/Graphics.hpp>
#include <ctime>
#include <math.h>
#include <random>
#include <cstdlib>


// generating file, generates a simple height map using perlin's noise function
class PerlinGenerator {
public:
    // Constructor to build the gradient array for the generator
    PerlinGenerator(uint cellSize, uint pageHeight, uint pageWidth): __cellSize(cellSize), __pageHeight(pageHeight), __pageWidth(pageWidth) {
        // Generate the gradients first
        __gH, __gW = ceil(pageHeight / cellSize) + 1, ceil(pageWidth / cellSize) + 2;
        __scalingX, __scalingY = __pageWidth / cellSize, __pageHeight / cellSize;

        // Used for generating random floating point numbers (used in unit vector generation)
        std::random_device rand_dev;
        std::mt19937 et(rand_dev());
        std::uniform_real_distribution<float> dist(-2000, 2000);

        // Allocate the array of gradients on the heap
        __gradients = new sf::Vector2f*[__gW];
        for (int i = 0; i < __gW; i++) {            
            __gradients[i] = new sf::Vector2f[__gH];

            for (int j = 0; j < __gH; j++) {
                // Populate the newly allocated array with random unit vectors that correspond to gradient vectors
                auto gradVec = sf::Vector2f(dist(et), dist(et));
                gradVec /= sqrtf32(pow(gradVec.x, 2) + pow(gradVec.y, 2));
                __gradients[i][j] = gradVec;
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
        uint boxX, boxY = floor(px / __scalingX), floor(py / __scalingY);
        sf::Vector2f pixelCenter = sf::Vector2f(px + 0.5, py + 0.5);
        sf::Vector2f gradients[4] = {
            __gradients[boxX][boxY], __gradients[boxX + 1][boxY], __gradients[boxX][boxY + 1], __gradients[boxX + 1][boxY + 1]
        };
        // Compute the appropriate distance vectors
        sf::Vector2f distances[4] = {
            pixelCenter - sf::Vector2f(boxX * __scalingX, boxY * __scalingY), pixelCenter - sf::Vector2f((boxX + 1) * __scalingX, boxY * __scalingY),
            pixelCenter - sf::Vector2f(boxX * __scalingX, (boxY + 1) * __scalingY), pixelCenter - sf::Vector2f((boxX + 1) * __scalingX, (boxY + 1) * __scalingY)
        };

        // Now here comes the actual noise function, compute the dot product between every distance vector and the gradient vector and interpolate between these values
        float dotProducts[4] = {dotProduct(gradients[0], distances[0]), dotProduct(gradients[1], distances[1]), dotProduct(gradients[2], distances[2]), dotProduct(gradients[3], distances[3])};
        // Compute the weightings of each of the dot products
        float p0p1 = lerp(dotProducts[0], dotProducts[1], fade(pixelCenter.x - boxX));
        float p2p3 = lerp(dotProducts[2], dotProducts[3], fade(pixelCenter.y - boxY));

        // Final noise value is the interpolation of the first two interpolations
        return lerp(p0p1, p2p3, fade(pixelCenter.y - boxY));
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
        return (1.0 - w)*x0 + w*x1;
    }
    // Fade function smoothens out the function and prevents 0 values and corners near the box definitions
    inline float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }
};


#endif