#ifndef __BIOMES_H__
#define __BIOMES_H__

#include <math.h>

#define DISTRIBUTION_CONSTANT 2.77

// Biome class dictates biome generation from the perlin noise heightmap
enum class Biome {
    BIOME_WATER = 0, // The number in the enum corresponds to the textures position within the sprite sheet
    BIOME_SAND = 1,
    BIOME_SHORT_GRASS = 2,
    BIOME_TALL_GRASS = 3,
    BIOME_SOFT_DIRT = 4,
    BIOME_HARD_DIRT = 5,
    BIOME_SNOW = 6,
    BIOME_NULL = 7
};


// Redistribution function makes perlin noise values a bit more usable: according to: http://digitalfreepen.com/2017/06/20/range-perlin-noise.html the range of 2d perlin noise is: [-sqrt{1/2}, sqrt{1/2}]
float redistribution(float val) {
    return pow((sqrtf(0.5) + val), DISTRIBUTION_CONSTANT);
}
 


// Returns the biome that a specific value falls into
Biome determineBiome(float val) {
    float e = redistribution(val);
    
    // Start dumping em biomes
    if (e < 0.6) return Biome::BIOME_WATER;
    if (e < 0.9) return Biome::BIOME_SAND;
    if (e < 1.3) return Biome::BIOME_SHORT_GRASS;
    if (e < 1.6) return Biome::BIOME_TALL_GRASS;
    if (e < 2.0) return Biome::BIOME_SOFT_DIRT;
    if (e < 2.5) return Biome::BIOME_HARD_DIRT;
    if (e >= 2.5) return Biome::BIOME_SNOW;

    return Biome::BIOME_SAND;
}





#endif