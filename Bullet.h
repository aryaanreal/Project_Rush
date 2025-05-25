#pragma once //makes sure this file is only included once during compiliation
#include "Entity.h" //include the base class that Bullet inherits from

//Bullet class inherits from Entity
class Bullet: public Entity {
    SDL_Texture*texture; //This is the image used to draw the bullet
public:
    //Consturctor:sets the bullet's position,speed, and texture
    Bullet(float x, float y, float speed = 10.0f, SDL_Texture*tex);

    //moves the bullet(e.g:uward or downward)
    void move() override;

    //Draws the bullet on the screen
    void draw(SDL_Renderer*renderer) override;
};