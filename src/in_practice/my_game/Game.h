#ifndef GAME_H
#define GAME_H

#include "includes.h"

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
public:
    GameState State;
    GLboolean Key[1024];
    GLuint Width, Height;


    Game(GLuint width, GLuint height);
    ~Game();

    void Init();      
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    
};

#endif
