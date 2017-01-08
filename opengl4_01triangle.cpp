	/* Linking */
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "SDL2main.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glew32.lib")

	/* Includes and Defines */
	#include <Sdl2/SDL.h>
	#include <GL/glew.h>
	#include <stdio.h>
	#define PI32 3.14159265359f

	/* Globals */
	SDL_Window *window = 0;
	SDL_Event event; 
	const int screenWidth = 960;
	const int screenHeight = 540;
	unsigned long long performanceFrequency;		//the frequency of the performance counter in counts per seonds
	bool running = true;
	bool fullscreen = false;

int main(int argc, char **argv)
{
	performanceFrequency = SDL_GetPerformanceFrequency();
	SDL_Init(SDL_INIT_VIDEO);
	
	window = SDL_CreateWindow("openGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
				screenWidth, screenHeight, SDL_WINDOW_OPENGL/* | SDL_WINDOW_FULLSCREEN_DESKTOP*/);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	//use doublebuffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//use vsync
	SDL_GL_SetSwapInterval(1);

	//initialize GLEW to setup the OpenGL function pointers
	glewInit();

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));

	if (!glewIsSupported("GL_VERSION_4_5"))
	{
		printf("OpenGL 4.5 is not available!\n");
	}

	//define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	GLfloat vertices[] =
	{
		0.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
	};

	GLuint bufferID;
	//generate 1 vertex buffer
	glGenBuffers(1, &bufferID);
	//bind the buffer to the GL_ARRAY_BUFFER binding point 
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//upload the buffer data to the GPU - send the data to the buffer that is bound to the GL_ARRAY_BUFFER binding point
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//enable the data that was copied to the buffer to go to through the graphics processing pipeline 
	//enable the position attribute
	glEnableVertexAttribArray(0);
	//tells opengl how our data buffer is structured - point it to the start of the buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	unsigned long long lastCounter = SDL_GetPerformanceCounter();
	unsigned int currentTick = SDL_GetTicks();

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_LEFT:
						case SDLK_RIGHT:
						case SDLK_UP:
						case SDLK_DOWN:
						case SDLK_n:
						case SDLK_RETURN:
						{
							if (fullscreen)
							{
								SDL_SetWindowFullscreen(window, SDL_FALSE);
								fullscreen = false;
							}
							else
							{
								SDL_SetWindowFullscreen(window, SDL_TRUE);
								fullscreen = true;
							}
						} break;
					}
				} break;

				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
						running = false;
				} break;

				case SDL_QUIT:
				{
					running = false;
				}	break;
			}
		}
		
		/* render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* update screen */
		SDL_GL_SwapWindow(window);

		unsigned long long endCounter = SDL_GetPerformanceCounter();
		unsigned long long counterElapsed = endCounter - lastCounter;
		double msPerFrame = (((1000.0f * (double)counterElapsed) / (double)performanceFrequency));
		double fps = (double)performanceFrequency / (double)counterElapsed;
		lastCounter = endCounter;
		currentTick = SDL_GetTicks();

		char message[256];
		sprintf_s(message, "%.03fms, %.03fFPS\0", msPerFrame, fps);
		SDL_SetWindowTitle(window, message);
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
