#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "ObjLoader.h"
#include "Camera.h"
#include <SDL\SDL_net.h>
#include <cstring>

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();
	void initSystems();
	void gameLoop();
	void draw();
	void connection();
	void run();
	void processInput();
	void mouseLook(int, int);
	GameState _gameState;
	ObjLoader obj;
	ObjLoader obj2;
	ObjLoader obj3;
	int _obn1;
	int _obn2;
	int _obn3;
	Camera mainCam;
	float _xDist;
	float _yDist;
	float _zDist;
	float _xRot;
	float _yRot;
	float _zRot;
	float _eyeX;
	int _windowHeight;
	int _windowWidth;
	void calculateFPS();
	int id;
	void setID(int i);
	int getID();

private:
	SDL_Window *ptr_window;
	bool mouseIn;
	float _fps;
	float _frameTime;
	float _maxFPS;
	float _moveVel;
	float _mouseVel;
};

struct data
{
	TCPsocket socket;
	Uint32 timeout;
	int id;
	data(TCPsocket sock, Uint32 t, int i)
	{
		socket = sock;
		timeout = t;
		id = i;
	}
};

