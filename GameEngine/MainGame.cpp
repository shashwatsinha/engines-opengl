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
	_eyeX = 0;
	_maxFPS = 60;
	_mouseVel = 0.2f;
	_moveVel = 0.2f;
	id = -1;
}


MainGame::~MainGame()
{

}

void MainGame::initSystems()
{
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	

	//Open an SDL window
	ptr_window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowWidth, _windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat amb_light[] = { 1, 1, 1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void MainGame::run()
{
	initSystems();
	_obn1 = obj.load("Models/ViolinCase.obj");
    _obn1.setPosition(-2.0f, 0.0f, 5f);
	_obn2 = obj2.load("Models/capsule.obj");
    _obn2.setPosition(5.0f, 0.0f, 0.0f);
	_obn3 = obj3.load("Models/Teapot.obj");
    debug = true;

	gameLoop();
}

void MainGame::gameLoop()
{
	//Will loop until we set _gameState to EXIT
	while (_gameState != GameState::EXIT)
	{
		float _startTicks = SDL_GetTicks();
		processInput();
		draw();
		calculateFPS();
		//print only once every 10 frames
		static int _frameCounter = 0;
		_frameCounter++;
		if (_frameCounter == 10)
		{
		//	cout << _fps << endl;
			_frameCounter = 0;
		}

		float _frameTicks = SDL_GetTicks() - _startTicks;

		//limit FPS to max FPS
		if (1000.0f / _maxFPS > _frameTicks)
		{
			SDL_Delay(1000.0f / _maxFPS - _frameTicks);
		}
	}

	if (_gameState == GameState::EXIT)
	{
		SDL_Quit();
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
			//	std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseIn = true;
			SDL_ShowCursor(SDL_DISABLE);
			break;

		

		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
				//Press F1 to go full screen
			case SDLK_F1:
				SDL_SetWindowFullscreen(ptr_window, SDL_WINDOW_FULLSCREEN);
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
				if (mainCam.camPitch != 90 && mainCam.camPitch != -90)
				{
					mainCam.moveCamera(_moveVel, 0.0f);
				}
				mainCam.moveCameraUp(_moveVel, 0.0f);
				break;

			case SDLK_s:
				if (mainCam.camPitch != 90 && mainCam.camPitch != -90)
				{
					mainCam.moveCamera(_moveVel, 180.0f);
				}
				mainCam.moveCameraUp(_moveVel, 180.0f);
				break;
			case SDLK_p:
				mouseIn = false;
				SDL_ShowCursor(SDL_ENABLE);
				break;
			case SDLK_a:
				mainCam.moveCamera(_moveVel, 90.0f);
				break;

			case SDLK_d:
				mainCam.moveCamera(_moveVel, 270);
				break;
				
            case SDLK_i:
				debugCam.moveCamera(_moveVel, 0.0f);
				break;
			case SDLK_k:
				debugCam.moveCamera(_moveVel, 180.0f);
				break;
			case SDLK_j:
				debugCam.moveCamera(_moveVel, 90.0f);
				break;
			case SDLK_l:
				debugCam.moveCamera(_moveVel, 270.0f);
				break;
			case SDLK_UP:
				debugCam.camPitch += 0.1f;
				break;
			case SDLK_DOWN:
				debugCam.camPitch -= 0.1f;
				break;
			
			}
		}
	}
}



void MainGame::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	mainCam.control(_moveVel, _mouseVel, mouseIn, ptr_window);
	gluLookAt(0, 1,25, 0, 0,0 , 0, 1, 0);
	glTranslatef(mainCam.camX*-1, mainCam.camY*-1, mainCam.camZ*-1);

    if(debug)
	{
		debugCam.drawFrustum();
	}
	glPushMatrix();
	glTranslatef(mainCam.camX*-1, mainCam.camY*-1, mainCam.camZ*-1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(_obn1.getX(), _obn1.getY(), _obn1.getZ());
	glCallList(_obn1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(_obn2.getX(), _obn2.getY(), _obn2.getZ());
	glCallList(_obn2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15, 0, 0);
	glCallList(_obn3);
	glPopMatrix();

	SDL_GL_SwapWindow(ptr_window);

}



//Calculate fps logic
void MainGame::calculateFPS()
{
	static const int _numSamples = 1000;
	static float frameTimes[_numSamples];
	static int _currentFrame = 0;

	static float _prevTicks = SDL_GetTicks();

	float _currentTicks;
	_currentTicks = SDL_GetTicks();

	_frameTime = _currentTicks - _prevTicks;
	frameTimes[_currentFrame%_numSamples] = _frameTime;

	_prevTicks = _currentTicks;

	int _count;

	_currentFrame++;


	if (_currentFrame < _numSamples)
	{
		_count = _currentFrame;
	}

	else
	{
		_count = _numSamples;
	}

	float _frameTimeAverage = 0.0f;
	for (int i = 0; i < _count; i++)
	{
		_frameTimeAverage += frameTimes[i];
	}

	_frameTimeAverage /= _count;

	if (_frameTimeAverage > 0)
	{
		_fps = 1000 / _frameTimeAverage;
	}

	else
	{
		_fps = 60;
	}
}

