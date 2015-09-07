#include "MainGame.h"
#include "Errors.h"
#include "iostream"
#include "ObjLoader.h"
using namespace std;

MainGame::MainGame()
{
	ptr_window = nullptr;
	_windowWidth = 640;
	_windowHeight = 480;
	_xDist = 0;
	_yDist = 0;
	_zDist = 0;
	_yRot = 0;
	_zRot = 0;
	_xRot = 0;
}


MainGame::~MainGame()
{

}

void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Open an SDL window
	ptr_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
	if (ptr_window == nullptr) 
	{
		fatalError("SDL Window could not be created!");
	}

	//Set up our OpenGL context
	SDL_GLContext glContext = SDL_GL_CreateContext(ptr_window);
	if (glContext == nullptr) 
	{
		fatalError("SDL_GL context could not be created!");
	}

	//Set up glew 
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("Could not initialize glew!");
	}

	//Tell SDL that we want a double buffered window so we dont get
	//any flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, _windowWidth, _windowHeight);
	GLfloat aspect = (GLfloat)_windowWidth / _windowHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, aspect, 1, 500);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.1f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
}

void MainGame::run()
{
	initSystems();
	obj.load("Models/Teapot.obj");
	gameLoop();
}

void MainGame::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT) {
		processInput();
		draw();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type) 
		{
		case SDL_WINDOWEVENT:
			switch (evnt.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				//any flickering
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

				//Set the background color to blue
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				//Set the base depth to 1.0
				glClearDepth(1.0);
				//Clear the color and depth buffer
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				break;
			}
			break;

		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;

		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
				//Press F1 to go full screen
			case SDLK_F1 :
				SDL_SetWindowFullscreen(ptr_window,SDL_WINDOW_FULLSCREEN);
				break;
				//Press F2 to come back from fullscreen
				//Use function SDL_SetWindowSize() for custom size window
			case SDLK_F2:
				SDL_SetWindowFullscreen(ptr_window, 0);
				break;
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT;
				break;
			case SDLK_w:
				_yDist+=0.1f;
				break;

			case SDLK_s:
				_yDist -= 0.1f;
				break;

			case SDLK_a:
				_xDist -= 0.1f;
				break;

			case SDLK_d:
				_xDist += 0.1f;
				break;

			case SDLK_q:
				_yRot += 1.0f;
				break;

			case SDLK_e:
				_yRot -= 1.0f;
				break;

			case SDLK_r:
				_zRot += 1.0f;
				break;

			case SDLK_t:
				_zRot -= 1.0f;
				break;

			case SDLK_y:
				_xRot += 1.0f;
				break;

			case SDLK_u:
				_xRot -= 1.0f;
				break;

			case SDLK_z:
				_zDist += 1.0f;
				break;

			case SDLK_x:
				_zDist -= 1.0f;
				break;
			}

		
		}
	}
}



void MainGame::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 1, 20, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	
	glTranslatef(_xDist, _yDist, _zDist);
	glRotatef(_yRot,0.0f, 1.0f, 0.0f);
	glRotatef(_xRot, 1.0f, 0.0, 0.0f);
	glRotatef(_zRot, 0.0f, 0.0, 1.0f);
	obj.Draw();
	glPopMatrix();
	SDL_GL_SwapWindow(ptr_window);

}



