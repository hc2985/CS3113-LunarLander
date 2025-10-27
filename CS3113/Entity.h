#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction  { LEFT, UP, RIGHT, DOWN }; // For walking
enum PongState { IDLE, HIT     };

constexpr int SCREEN_WIDTH  = 1600 ,
              SCREEN_HEIGHT = 900 ,
              FPS           = 120;              

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Texture2D mTexture;
    std::vector<Texture2D> mTextures; 

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    Direction mDirection;
    
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    float mFuel = 50.0f;
    float mFuelrate = 10.0f;

    float mAngle;

    bool mThrustUp = false;
    bool mThrustLeft = false;
    bool mThrustRight = false;
    bool mAsteroid = false;

    bool mGameover = false;
    bool mGamewin = false;

    bool mFuelhalf = false;
    bool mFuelempty = false;

    int mSize;
    
    bool isColliding(Entity *other);
    void animate(float deltaTime);
    

public:
    static const int DEFAULT_SIZE        = 250;
    static const int DEFAULT_SPEED       = 150;
    static const int DEFAULT_FRAME_SPEED = 8;

    Entity();
    Entity(Vector2 position, Vector2 scale, std::vector<const char*>* filepaths, int size, bool asteroid);

    void update(float deltaTime, std::vector<Entity*> others);
    void render();
    void checkCollisions(std::vector<Entity*> others);

    void markFuel(Entity *other){
        other->setFuel(mFuel); 
    }

    void setThrust(bool up, bool left, bool right) {
        mThrustUp = up;
        mThrustLeft = left;
        mThrustRight = right;
    }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    Vector2 getVelocity()                  const { return mVelocity; }
    Vector2 getAcceleration()              const { return mAcceleration; }
    // Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getAngle()                 const { return mAngle;                 }
    bool        getWin()                   const { return mGamewin;               }
    bool        getLose()                  const { return mGameover;              } 

    



    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                }
    void setScale(Vector2 newScale)
        { mScale = newScale;                      }
    void setColliderDimensions(Vector2 newDimensions) 
        { mColliderDimensions = newDimensions;    }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                 }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                      }
    void setAcceleration(Vector2 a) 
        { mAcceleration = a;                      }
    void setVelocity(Vector2 v) 
        { mVelocity = v;                          }
    void setFuel(float fuel){
        mFuel = fuel;
        if(mFuel < 25.0f){
            mFuelhalf = true;
        }
        if(mFuel <= 0.0f){
            mFuelempty = true;
        }  
    }
    

};

#endif // ENTITY_CPP