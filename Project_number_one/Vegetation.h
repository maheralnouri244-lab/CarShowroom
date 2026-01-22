#include "Model.h"
#include "City.h"

class Vegetation : public City{
public:
    Vegetation();
    void setTextures(unsigned int bark, unsigned int leaf);

    void drawTree(float x, float y, float z);
    void drawFlowerBed(float x, float y, float z);
    void drawRock(float x, float y, float z, float size);
    void drawReed(float x, float y, float z);

private:
    Model treeModel;
};