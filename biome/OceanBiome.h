#ifndef OCEANBIOME_H_INCLUDED
#define OCEANBIOME_H_INCLUDED

#include "Biome.h"

class OceanBiome : public Biome {
  public:
    OceanBiome(int seed);

    Block getPlant(Rand &rand) const override;
    Block getTopBlock(Rand &rand) const override;
    Block getUnderWaterBlock(Rand &rand) const override;
    void makeTree(Rand &rand, Chunk &chunk, int x, int y, int z) const override;

  private:
    NoiseParameters getNoiseParameters() override;
};

#endif // OCEANBIOME_H_INCLUDED