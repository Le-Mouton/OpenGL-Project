#include "Player.h"

Player::Player() {
    hitBox = &(ObjectManager::getInstance().createObject<Rectangle>(Position, 1.0f, true, 1.0f, 4.0f, 1.0f));
}

void Player::PlayerUpdate() {
    
    Position.x = hitBox->coord.x;
    Position.z = hitBox->coord.z;

    Position.y = hitBox->coord.y;
        
}

void Player::ProcessKeyboard(Player_movement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD){
        if(hitBox->contact){
            hitBox->coord += glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        }else{
            hitBox->coord += glm::vec3(Front.x, 0.0f, Front.z) * 0.1f * velocity;
        }
    }
    if (direction == BACKWARD) {
        if (hitBox->contact) {
            hitBox->coord -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        } else {
            hitBox->coord -= glm::vec3(Front.x, 0.0f, Front.z) * 0.1f * velocity;
        }
    }
    if (direction == LEFT){
        if(hitBox->contact){
            hitBox->coord -= Right * velocity;
        }else{
            hitBox->coord -= Right * 0.1f * velocity;
        }
    }
    if (direction == RIGHT) {
        if (hitBox->contact) {
            hitBox->coord += Right * velocity;
        } else {
            hitBox->coord += Right * 0.1f * velocity;
        }
    }
    if (direction == DOWN && hitBox->contact){

        hitBox->coord += 0.1f * Up;
        hitBox->velocity += glm::vec3(0.0f, 7.5f, 0.0f) * Up;
    }
    if (direction == UP && hitBox->contact) {
        hitBox->velocity -= glm::vec3(0.0f, 7.5f, 0.0f) * Up;
    }
}
