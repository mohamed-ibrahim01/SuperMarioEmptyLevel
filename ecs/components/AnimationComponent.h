#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

enum row {IDEAL, RUNNING};
struct AnimationComponent
{
    int width;
    int height;
    int animationsNum;
    
    float animationTime = 0.3f;
    bool faceRight = true;
    int row = IDEAL;
    int currentAnimation = 0;
    float totalTime = 0;
};

#endif