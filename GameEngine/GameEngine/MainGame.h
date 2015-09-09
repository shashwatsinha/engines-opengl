#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "ObjLoader.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();
	void initSystems();
	void gameLoop();
	void draw();
	void run();
	void processInput();
	GameState _gameState;
	ObjLoader obj;
	float _xDist;
	float _yDist;
	float _zDist;
	float _xRot;
	float _yRot;
	float _zRot;

	void calculateFPS();

private:
	SDL_Window *ptr_window;
	int _windowHeight;
	int _windowWidth;
	float _fps;
	float _frameTime;
	float _maxFPS;
};

