#include "Physic.h"

Object::~Object() {
}
bool Collision(Object &objectA, Object &objectB) {

    if (typeid(objectA) == typeid(Sphere) && typeid(objectB) == typeid(Plane)) {
        // Collision entre Sphere et Plane
        Sphere *sphere = dynamic_cast<Sphere *>(&objectA);
        Plane *plane = dynamic_cast<Plane *>(&objectB);
        if (sphere && plane) {
            return CollisionSpherePlane(*sphere, *plane);
        }
    }
    // Collision entre deux sphères
    else if(typeid(objectA) == typeid(Sphere) && typeid(objectB) == typeid(Sphere)){
        Sphere* sphereA = dynamic_cast<Sphere*>(&objectA);
        Sphere* sphereB = dynamic_cast<Sphere*>(&objectB);
        if (sphereA && sphereB) {
            return CollisionSphereSphere(*sphereA, *sphereB);
        }
    }
        // Collision entre Rectangle et Plane
    else if(typeid(objectA) == typeid(Rectangle) && typeid(objectB) == typeid(Plane)){
        Rectangle* rectangle = dynamic_cast<Rectangle*>(&objectA);
        Plane* plane = dynamic_cast<Plane*>(&objectB);
        if (rectangle && plane) {
            return CollisionRectanglePlane(*rectangle, *plane);
        }
    }
/*
        // Collision entre Sphere et Capsule
    else if(typeid(objectA) == typeid(Sphere) && typeid(objectB) == typeid(Capsule)){
        Sphere* sphere = dynamic_cast<Sphere*>(&objectA);
        Capsule* capsule = dynamic_cast<Capsule*>(&objectB);
        if (sphere && capsule) {
            return CollisionSphereCapsule(sphere, capsule);
        }
    }

        // Collision entre Capsule et Sphere (inverse de Sphere-Capsule)
    else if(typeid(objectA) == typeid(Capsule) && typeid(objectB) == typeid(Sphere)){
        Capsule* capsule = dynamic_cast<Capsule*>(&objectA);
        Sphere* sphere = dynamic_cast<Sphere*>(&objectB);
        if (capsule && sphere) {
            return CollisionSphereCapsule(sphere, capsule);
        }
    }

        // Collision entre Capsule et Plane
    else if(typeid(objectA) == typeid(Capsule) && typeid(objectB) == typeid(Plane)){
        Capsule* capsule = dynamic_cast<Capsule*>(&objectA);
        Plane* plane = dynamic_cast<Plane*>(&objectB);
        if (capsule && plane) {
            return CollisionPlaneCapsule(plane, capsule);
        }
    }

        // Collision entre deux Capsules
    else if(typeid(objectA) == typeid(Capsule) && typeid(objectB) == typeid(Capsule)){
        Capsule* capsuleA = dynamic_cast<Capsule*>(&objectA);
        Capsule* capsuleB = dynamic_cast<Capsule*>(&objectB);
        if (capsuleA && capsuleB) {
            return CollisionCapsuleCapsule(capsuleA, capsuleB);
        }
    }

        // Collision entre Plane et Sphere (inverse de Sphere-Plane)
    else if(typeid(objectA) == typeid(Plane) && typeid(objectB) == typeid(Sphere)){
        Plane* plane = dynamic_cast<Plane*>(&objectA);
        Sphere* sphere = dynamic_cast<Sphere*>(&objectB);
        if (plane && sphere) {
            return CollisionSpherePlane(sphere, plane);
        }
    }

        // Collision entre deux Planes
    else if(typeid(objectA) == typeid(Plane) && typeid(objectB) == typeid(Plane)){
        Plane* planeA = dynamic_cast<Plane*>(&objectA);
        Plane* planeB = dynamic_cast<Plane*>(&objectB);
        if (planeA && planeB) {
            return CollisionPlanPlan(planeA, planeB);
        }
    }

        // Collision entre Plane et Capsule
    else if(typeid(objectA) == typeid(Plane) && typeid(objectB) == typeid(Capsule)){
        Plane* plane = dynamic_cast<Plane*>(&objectA);
        Capsule* capsule = dynamic_cast<Capsule*>(&objectB);
        if (plane && capsule) {
            return CollisionPlaneCapsule(plane, capsule);
        }
    }
*/
    // Aucune collision détectée / types non gérés
    return false;
}

void ObjectManager::updateAllObjects(float deltatime) {
    for (auto &objectA: objects) {
        objectA->accel = glm::vec3(0.0f);

        if (objectA->movable) {

            objectA->lastcontact = objectA->contact;

            objectA->contact = false;


            for (auto &objectB: objects) {
                if (objectA == objectB) continue;


                if (Collision(*objectA, *objectB)) {
                    if(!objectA->contact)
                        objectA->contact = true;

                    glm::vec3 n = glm::normalize(objectB->coord - objectA->coord);
                    float massSum = objectA->mass + objectB->mass;

                        if (objectB->movable) {
                            glm::vec3 vA = objectA->velocity;
                            glm::vec3 vB = objectB->velocity;

                            glm::vec3 vA_new = vA + (2 * objectB->mass / massSum) * glm::dot(vB - vA, n) * n * 0.8f;
                            glm::vec3 vB_new = vB + (2 * objectA->mass / massSum) * glm::dot(vA - vB, n) * n * 0.8f;

                            if (std::abs(n.x) < 0.01f && std::abs(n.z) < 0.01f) {
                                objectA->velocity.y += vA_new.y;
                                objectB->velocity.y += vB_new.y;
                            } else {
                                objectA->velocity += vA_new;
                                objectB->velocity += vB_new;
                            }
                        } else {
                            if (objectA->contact && !objectA->lastcontact) {
                                objectA->velocity.y = -objectA->velocity.y * 0.1f;
                            } else {
                                objectA->velocity = glm::vec3(0.0f);
                            }
                        }
                }
            }
            objectA->updateAccel(deltatime, glm::vec3(0.0f, -9.81f, 0.0f) * objectA->mass);
            objectA->updateVelocity(deltatime);
            objectA->updateCoord(deltatime);


        }
    }

}

bool CollisionSpherePlane(Sphere& sphere, Plane& plane) {
    float distanceSpherePlane = glm::dot(plane.normal, sphere.coord - plane.coord);
    float pen = abs(sphere.radius - distanceSpherePlane);

    glm::vec3 relativePos = sphere.coord - plane.coord;

    glm::mat4 rotationMat = glm::eulerAngleYXZ(glm::radians(plane.rotation.y), glm::radians(plane.rotation.x), glm::radians(plane.rotation.z));
    glm::mat4 inverseRotationMat = glm::transpose(rotationMat);

    glm::vec4 localPos4 = inverseRotationMat * glm::vec4(relativePos, 1.0f);
    glm::vec3 localPos = glm::vec3(localPos4);

    bool withinWidth = std::abs(localPos.x) <= (plane.width / 2.0f);
    bool withinHeight = std::abs(localPos.z) <= (plane.height / 2.0f);

    if (std::abs(distanceSpherePlane) <= sphere.radius) {
        if(withinHeight && withinWidth){
            if(pen>0){
                sphere.coord += pen*plane.normal;
            }
            return true;
        }

    }
    return false;
}
bool CollisionSphereSphere(Sphere &sphereA, Sphere &sphereB){

    float distanceSphereSphere = glm::distance(sphereA.coord, sphereB.coord);

    if (distanceSphereSphere <= (sphereA.radius + sphereB.radius)) {
        float overlap = (sphereA.radius + sphereB.radius) - distanceSphereSphere;
        glm::vec3 collisionDirection = glm::normalize(sphereB.coord - sphereA.coord);

        float totalMass = sphereA.mass + sphereB.mass;
        float moveRatioA = sphereB.mass / totalMass;
        float moveRatioB = sphereA.mass / totalMass;

        sphereA.coord -= collisionDirection * overlap * moveRatioA;
        sphereB.coord += collisionDirection * overlap * moveRatioB;

        return true;
    }

    return false;
}
bool CollisionRectanglePlane(Rectangle &rectangle, Plane &plane) {
    std::vector<glm::vec3> vertices;
    float W = rectangle.width / 2.0f;
    float H = rectangle.height / 2.0f;
    float D = rectangle.depth / 2.0f;

    vertices.push_back(rectangle.coord + glm::vec3(W, H, D));
    vertices.push_back(rectangle.coord + glm::vec3(W, H, -D));
    vertices.push_back(rectangle.coord + glm::vec3(W, -H, D));
    vertices.push_back(rectangle.coord + glm::vec3(W, -H, -D));
    vertices.push_back(rectangle.coord + glm::vec3(-W, H, D));
    vertices.push_back(rectangle.coord + glm::vec3(-W, H, -D));
    vertices.push_back(rectangle.coord + glm::vec3(-W, -H, D));
    vertices.push_back(rectangle.coord + glm::vec3(-W, -H, -D));

    bool positiveSide = false, negativeSide = false;
    float minDistance = FLT_MAX;

    for (const auto& vertex : vertices) {
        float distance = glm::dot(vertex - plane.coord, plane.normal);

        if (distance > 0) {
            positiveSide = true;
        } else {
            negativeSide = true;
            if (distance < minDistance) minDistance = distance;
        }
    }

    if (positiveSide && negativeSide) {
        rectangle.coord -= plane.normal * minDistance;
        return true;
    }

    return false;
}
/*
bool CollisionSphereCapsule(Sphere &sphere, Capsule &capsule){

    return false;
}
bool CollisionPlaneCapsule(Plane &plane, Capsule &capsule){

    return false;
}
bool CollisionPlanPlan(Plane &planeA, Plane &planeB){

    return false;
}
bool CollisionCapsuleCapsule(Capsule &capsuleA, Capsule &capsuleB){

    return false;
}
bool CollisionRectangleSpehre(Rectangle &rectangle, Sphere &sphere){

    return false;
}
bool CollisionRectangleRectangle(Rectangle &rectangle, Rectangle &rectangle){

    return false;
}
*/


