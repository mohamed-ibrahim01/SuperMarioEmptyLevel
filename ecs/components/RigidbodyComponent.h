#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <SFML/Graphics.hpp>

struct RigidbodyComponent
{
    float speed;
    float jumpHeight;
    bool canJump = false;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
};

#endif

