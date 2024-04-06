#ifndef LEARNINGPROJECT_PHYSIC_H
#define LEARNINGPROJECT_PHYSIC_H

#include "glm/geometric.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"


#include <vector>
#include <iostream>
#include <memory>


class Object{
public:
    glm::vec3 coord;
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 accel = glm::vec3(0.0f);

    float mass;
    bool movable;
    bool contact;
    bool lastcontact;

    Object(glm::vec3 coord, float mass, bool movable)
           : coord(coord), mass(mass), movable(movable){};

    virtual ~Object();

    void updateCoord(float deltatime){
        this->coord += velocity*deltatime;
    }
    void updateVelocity(float deltatime){
        this->velocity += accel*deltatime;
    }
    void updateAccel(float deltatime, glm::vec3 force){
        this->accel += force/mass;
    }
};

bool Collision(Object &objectA, Object &objectB);

class ObjectManager {
private:
    std::vector<std::unique_ptr<Object>> objects;
public:
    static ObjectManager& getInstance() {
        static ObjectManager instance;
        return instance;
    }

    template <typename T, typename... Args>
    T& createObject(Args&&... args) {
        T* obj = new T(std::forward<Args>(args)...);
        objects.push_back(std::unique_ptr<Object>(obj));
        return *obj;
    }

    void updateAllObjects(float deltatime);
    void reactionCollision(Object &objectA, Object &objectB);

        ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;

protected:
    ObjectManager() {}
};


class Sphere : public Object{
public:
    float radius;

    Sphere(glm::vec3 coord, float mass, bool movable, float radius)
    : Object(coord, mass, movable), radius(radius) {};


};

class Plane : public Object{
public:
    float width;
    float height;
    glm::vec3 normal;
    glm::vec3 rotation;

    Plane(glm::vec3 coord, float mass, bool movable, float width, float height, glm::vec3 normal)
    : Object(coord, mass, movable), width(width), height(height), normal(normal) {};
};

class Capsule : public Object{
public:
    float radius;
    float height;

    Capsule(glm::vec3 coord, float mass, bool movable, float radius, float height)
            : Object(coord, mass, movable), radius(radius), height(height) {};

};
class Cylinder : public Object{
public:
    float radius;
    float height;

    Cylinder(glm::vec3 coord, float mass, bool movable, float radius, float height)
            : Object(coord, mass, movable), radius(radius), height(height) {};

};
class Rectangle : public Object{
public:
    float width;
    float height;
    float depth;
    
    Rectangle(glm::vec3 coord, float mass, bool movable, float width, float height, float depth)
            : Object(coord, mass, movable), width(width), height(height), depth(depth){};

};
void updateAllObjects(std::vector<Object*> &listobject, float deltatime, bool update);
bool CollisionSpherePlane(Sphere& sphere, Plane& plane);
bool CollisionSphereSphere(Sphere &sphereA, Sphere &sphereB);
bool CollisionRectanglePlane(Rectangle &rectangle, Plane &plane);
/*
bool CollisionSphereCapsule(Sphere *sphere, Capsule *capsule);
bool CollisionPlaneCapsule(Plane *plane, Capsule *capsule);
bool CollisionPlanPlan(Plane *planeA, Plane *planeB);
bool CollisionCapsuleCapsule(Capsule *capsuleA, Capsule *capsuleB);
 */
bool Collision(Object &objectA, Object &objectB);

#endif //LEARNINGPROJECT_PHYSIC_H
