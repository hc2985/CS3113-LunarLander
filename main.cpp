/**
* Author: Hyeonung Cho
* Assignment: Lunar Lander
* Date due: 2025-27-13, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

// Global Constants
constexpr float  LANDER_WIDTH = 50.0f; 
constexpr float LANDER_HEIGHT = 100.0f;
constexpr float MESSAGE_WIDTH = 800.0f;
constexpr float MESSAGE_HEIGHT = 200.0f;
constexpr float FUEL_WIDTH = 300.0f;
constexpr float FUEL_HEIGHT = 120.0f;
constexpr int OFFSET = 20;

std::vector<const char*> landers = {
    "assets/base.png",
    "assets/left.png",
    "assets/right.png",
    "assets/left-right.png",
    "assets/up.png",
    "assets/up-left.png",
    "assets/up-right.png",
    "assets/up-right-left.png",
    "assets/boom.png"
};

std::vector<const char*> spaces = {
    "assets/space.jpg"
};

std::vector<const char*> moons = {
    "assets/moon.jpg"
};

std::vector<const char*> asteroids = {
    "assets/asteroid.png"
};

std::vector<const char*> wins = {
    "assets/win.png"
};

std::vector<const char*> fails = {
    "assets/fail.png"
};

std::vector<const char*> fuels = {
    "assets/full.png",
    "assets/half.png",
    "assets/empty.png"
};

constexpr char    BG_COLOUR[] = "#202123a5";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
constexpr Vector2 MOON_POS    = { SCREEN_WIDTH/2, SCREEN_HEIGHT+100.0f};
constexpr Vector2 ASTEROID_POS    = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100.0f};
constexpr Vector2 LANDER_POS = {SCREEN_WIDTH / 2, 0};
constexpr Vector2 FUEL_POS = {SCREEN_WIDTH-400.0f, SCREEN_HEIGHT-200.0f};

// Global Variables 
AppStatus gAppStatus = RUNNING;
bool gGameMode = true; //true = multi
float gPreviousTicks = 0.0f;
bool animation_set = true;
bool gameover_status = false;

bool up = false;
bool left = false;
bool right = false;

bool gamewin = false;
bool gameover = false;

Entity* lander = nullptr;
Entity* background = nullptr;
Entity* moon = nullptr;
Entity* asteroid = nullptr;
Entity* win = nullptr;
Entity* fail = nullptr;
Entity* fuel = nullptr;

// Function Declarations
void initialise();
void processInput();
void update();
void render(); 
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");  

    lander = new Entity(
        LANDER_POS,
        {LANDER_WIDTH, LANDER_HEIGHT},
        &landers,
        landers.size(),
        false    
    );

    background = new Entity(
        ORIGIN,
        {SCREEN_WIDTH*3, SCREEN_HEIGHT*3},
        &spaces,
        spaces.size(),
        false
    );

    moon = new Entity(
        MOON_POS,
        {SCREEN_WIDTH, SCREEN_HEIGHT/2},
        &moons,
        moons.size(),
        false
    );

    asteroid = new Entity(
        ASTEROID_POS,
        {LANDER_WIDTH, LANDER_WIDTH},
        &asteroids,
        asteroids.size(),
        true
    );

    win = new Entity(
        ORIGIN,
        {MESSAGE_WIDTH, MESSAGE_HEIGHT},
        &wins,
        wins.size(),
        false
    );

    fail = new Entity(
        ORIGIN,
        {MESSAGE_WIDTH, MESSAGE_HEIGHT},
        &fails,
        fails.size(),
        false
    );

    fuel = new Entity(
        FUEL_POS,
        {FUEL_WIDTH, FUEL_HEIGHT},
        &fuels,
        fuels.size(),
        false
    );

    asteroid->setVelocity({150.0f, 0.0f});


    SetTargetFPS(FPS);
}



void processInput() 
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    up = IsKeyDown(KEY_W);
    left = IsKeyDown(KEY_A);
    right = IsKeyDown(KEY_D);
}

void update() 
{
    // Delta time
    if (!gameover && !gamewin){
        float ticks = (float) GetTime();
        float deltaTime = ticks - gPreviousTicks;
        gPreviousTicks  = ticks;

        lander->setThrust(up, left, right);
        std::vector<Entity*> collisonchecker = {moon, asteroid};
        lander->update(deltaTime, collisonchecker);
        asteroid->update(deltaTime, {});
        lander->markFuel(fuel);
    }
    if (lander->getWin()){
        gamewin = true;
    }
    if (lander->getLose()){
        gameover = true;
    }
    
}   

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    background->render();
    moon->render();
    asteroid->render();
    lander->render();
    fuel->render();

    if(gamewin){
        win->render();
    }else if(gameover){
        fail->render();
    }

    EndDrawing();
}

void shutdown() 
{ 
    CloseWindow();  
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}