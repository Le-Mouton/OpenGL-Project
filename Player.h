#ifndef LEARNINGPROJECT_PLAYER_H
#define LEARNINGPROJECT_PLAYER_H

#include "Camera.h"
#include "Physic.h"
#include "Mesh.h"

enum Player_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Player : public Camera{
public:
    Player();

    void ProcessKeyboard(Player_movement direction, float deltaTime);

    void PlayerUpdate();
private:
    Rectangle* hitBox;
};


#endif //LEARNINGPROJECT_PLAYER_H
