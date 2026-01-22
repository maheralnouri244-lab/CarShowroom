#define ABRARCODE_H

#include <GL/glut.h>
#include <vector>
#include "BeetleCar.h"
#include "Pillar.h"
#include "Hpillar.h"
#include "Showroomside.h"
#include "NeonTube.h"
#include "SteveModel.h"
#include "Furniture.h"
#include "AccessoriesSection.h"

enum InteractionState { STATE_WALKING, STATE_ENTERING, STATE_DRIVING, STATE_ELEVATOR };
enum ElevatorState { ELEV_IDLE, ELEV_MOVING_UP, ELEV_MOVING_DOWN, ELEV_DOOR_OPENING, ELEV_DOOR_CLOSING };

struct CarData {
    float x, y, z, scale, rotY;
    int colorIdx;
    float currentDoors[4];
    float targetDoors[4];
    float currentSteer;
};

struct BoundingBox {
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

class AbrarCode {
public:
    AbrarCode();

    void drawCars();
    void drawSecondFloor();
    void drawGroundFloorElevator();

    void update(float& camX, float& camY, float& camZ, float& yaw, float& pitch);
    void handleInput(unsigned char key, float& camX, float& camY, float& camZ, const std::vector<BoundingBox>& collidableObjects);
    InteractionState getState() {
        return currentState;
    }

private:
    BeetleCar myCar;
    std::vector<CarData> cars;
    InteractionState currentState;
    int activeCarIndex;
    long lastCollisionSoundTime; 
    bool carWasInsideShowroom;
    float animT;
    float startX, startY, startZ, startYaw, startPitch;
    float targetX, targetY, targetZ, targetYaw, targetPitch;

    float elevatorY;
    float targetElevatorY;
    ElevatorState elevState;
    float doorGapGround;
    float doorGapSecond;
    float doorGapCabin;
    bool playerInElevator;
    bool elevatorDoorsOpen;
    float elevatorDoorOffset;

    void attemptEnterCar(float camX, float camZ);
    void driveActiveCar(float speed, float turn, const std::vector<BoundingBox>& collidableObjects);
    float dist(float x1, float z1, float x2, float z2);

    void drawElevatorShaft(float x, float z, float h);
    void drawElevatorCabin(float x, float y, float z);
    void drawElevatorDoor(float x, float y, float z, float gap);

    void drawRoom(float x, float y, float z, float w, float h, float d);
    void drawMegaAccessoriesShop(float x, float y, float z);
    void drawProduct(int type);
    void drawCheckoutCounter(float x, float y, float z);
    void drawQueue(float x, float y, float z);
    void drawVideoWall(float x, float y, float z, float w, float h);
    void drawSecretariatOffice(float x, float y, float z);
    void drawLuxurySofa(float x, float y, float z, float rotY);
    void drawChandelier(float x, float y, float z);
    void drawFountain(float x, float y, float z);
    void drawPeopleOnFloor(float floorY);
    void drawMassiveShelf(float x, float y, float z, int rows, int cols, int type);
    void drawHologram(float x, float y, float z);
    void drawTV(float x, float y, float z, float r);
    void drawTechStation(float x, float y, float z);
    void drawInfoKiosk(float x, float y, float z);
    bool checkCarCollision(float nextX, float nextZ, const std::vector<BoundingBox>& collidableObjects);
    bool isCarOnAllowedSurface(float x, float z);
    Furniture furniture;
    AccessoriesSection accessories;
};

