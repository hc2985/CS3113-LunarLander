#include "Entity.h"

Entity::Entity()
    : mPosition{0.0f, 0.0f},
      mScale{DEFAULT_SIZE, DEFAULT_SIZE},
      mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
      mDirection{UP}, mSize{1},
      mFrameSpeed{0}, mAngle(0.0f),
      mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f}
{
}

Entity::Entity(Vector2 position, Vector2 scale, std::vector<const char*>* filepaths, int size, bool asteroid)
    : mPosition(position),
      mScale(scale),
      mColliderDimensions(scale),
      mDirection(UP),
      mSize(size),
      mFrameSpeed(DEFAULT_FRAME_SPEED),
      mAngle(0.0f),
      mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f},
      mAsteroid(asteroid)
{
    for (int i = 0; i < size; i++) {
        mTextures.push_back(LoadTexture(filepaths->at(i)));
    }
    mTexture = mTextures[0];
}

bool Entity::isColliding(Entity* other)
{
    if (!other) return false;

    //lander edges
    float self_l = mPosition.x - mColliderDimensions.x * 0.3f;
    float self_r = mPosition.x + mColliderDimensions.x * 0.3f;
    float self_t = mPosition.y - mColliderDimensions.y * 0.25f;
    float self_b = mPosition.y + mColliderDimensions.y * 0.4f;

    //other edges
    float other_l = other->getPosition().x - other->getColliderDimensions().x * 0.5f;
    float other_r = other->getPosition().x + other->getColliderDimensions().x * 0.5f;
    float other_t = other->getPosition().y - other->getColliderDimensions().y * 0.5f;
    float other_b = other->getPosition().y + other->getColliderDimensions().y * 0.5f;

    bool overlap = (self_l < other_r && self_r > other_l &&
                    self_t < other_b && self_b > other_t);

    if (!overlap) return false;

    float overlapX = std::min(self_r - other_l, other_r - self_l);
    float overlapY = std::min(self_b - other_t, other_b - self_t);

    if (overlapX < overlapY) {
        mGameover = true;
        //horizontal collision
        if (mPosition.x < other->getPosition().x){
            mPosition.x -= overlapX;
        }else{
            mPosition.x += overlapX;
        }           
        mVelocity.x = -0.3*mVelocity.x;
    } else {
        //vertical collision
        if (mPosition.y < other->getPosition().y){
            mPosition.y -= overlapY;
            if((abs(mVelocity.x) < 30 && abs(mVelocity.y) < 30) || (abs(mVelocity.x - other->getVelocity().x) < 30 && abs(mVelocity.y - other->getVelocity().y) < 30 )){
                mGamewin = true; 
            } else if ((abs(mVelocity.x - other->getVelocity().x) > 100 || abs(mVelocity.y - other->getVelocity().y) > 100 )) {
                mGameover = true;
            }
        }else{
            mGameover = true;
            mPosition.y += overlapY;
        }
        mVelocity.y = -0.3*mVelocity.y;
        mVelocity.x = other->getVelocity().x;
    }

    return true;
}


void Entity::update(float deltaTime, std::vector<Entity*> others)
{
    if (mAsteroid){
        mVelocity.y = 0.0f;

        mPosition.x += mVelocity.x * deltaTime;

        if (mPosition.x <= 0.0f){
            mVelocity = {150.0f, 0.0f};
        }else if (mPosition.x >= SCREEN_WIDTH){
            mVelocity = {-150.0f, 0.0f};
        }
        return;
    }

    if (mPosition.x < -50.0f || mPosition.x >= SCREEN_WIDTH || mPosition.y <= -100.0f){
        mGameover = true;
    }

    //gravity
    mAcceleration = {0.0f, 150.0f};

    //thrusts
    if (mThrustUp && mFuel > 0){
        mAcceleration.y -= 400.0f;
        mFuel -= deltaTime*mFuelrate;  
    }    
    if (mThrustLeft && mFuel > 0){
        mAcceleration.x += 500.0f;   
        mFuel -= deltaTime*mFuelrate/3;   
    }
        
    if (mThrustRight && mFuel > 0){
        mAcceleration.x -= 500.0f;
        mFuel  -= deltaTime*mFuelrate/3;
    } 

    if(mFuel < 0){
        mFuel = 0;
    } 

    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    //damping to horizontal movement
    mVelocity.x *= 0.98f;

    if (!others.empty())
        checkCollisions(others);

    mPosition.x += mVelocity.x * deltaTime;
    mPosition.y += mVelocity.y * deltaTime;


}


void Entity::checkCollisions(std::vector<Entity*> others)
{
    for (Entity* e : others)
        if (e) isColliding(e);
}

void Entity::render()
{
    if (mTextures.size() == 3){
        if (mFuelempty){
            mTexture = mTextures[2];
        } else if (mFuelhalf) {
            mTexture = mTextures[1];
        } else {
            mTexture = mTextures[0];
        }
        Rectangle src = {0.0f, 0.0f, (float)mTexture.width, (float)mTexture.height};
        Rectangle dst = {mPosition.x, mPosition.y, mScale.x, mScale.y};
        Vector2 origin = {mScale.x / 2, mScale.y / 2};

        DrawTexturePro(mTexture, src, dst, origin, mAngle, WHITE);
        return;
    }

    if (mGameover){
        mTexture = mTextures[8];
    } else if (mGamewin || mFuel <= 0){
        mTexture = mTextures[0];
    } else if (mThrustUp && mThrustLeft && mThrustRight) {
        mTexture = mTextures[7];
    } else if (mThrustUp && mThrustRight) {
        mTexture = mTextures[6];
    } else if (mThrustUp && mThrustLeft) {
        mTexture = mTextures[5];
    } else if (mThrustLeft && mThrustRight) {
        mTexture = mTextures[3];
    } else if (mThrustUp) {
        mTexture = mTextures[4];
    } else if (mThrustLeft) {
        mTexture = mTextures[1];
    } else if (mThrustRight) {
        mTexture = mTextures[2];
    } else {
        mTexture = mTextures[0];
    }

    Rectangle src = {0.0f, 0.0f, (float)mTexture.width, (float)mTexture.height};
    Rectangle dst = {mPosition.x, mPosition.y, mScale.x, mScale.y};
    Vector2 origin = {mScale.x / 2, mScale.y / 2};

    DrawTexturePro(mTexture, src, dst, origin, mAngle, WHITE);
}
