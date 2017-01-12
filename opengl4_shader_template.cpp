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
	const int screenWidth = 1920;
	const int screenHeight = 1080;
	unsigned long long performanceFrequency;		//the frequency of the performance counter in counts per seonds
	bool running = true;
	bool fullscreen = false;


	/* the vertex shader positions each vertex point */
	const char *vertexShaderSource =	"#version 430 core														\n"
										"layout(location = 0) in vec4 in_position;								\n"
										"void main(void)														\n"
										"{																		\n"
										"    gl_Position = in_position;											\n"
										"}																		\n";
	
	/* the fragment shader colours each fragment (pixel-sized area of the triangle) */
	const char *fragmentShaderSource =	"#version 430 core														\n"
										"																		\n"
										"out vec4 color;														\n"
										"																		\n"
										"void main(void)														\n"
										"{																		\n"
										"    color = vec4(0.0, 0.0, 1.0, 1.0);									\n"
										"}																		\n";


	char *readFile(const char *filename)
	{
		FILE* fp = fopen(filename, "r");
		//get file length
		fseek(fp, 0, SEEK_END);
		long fileLength = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* contents = (char*)malloc(fileLength + 1);

		//clear memory
		for (int i = 0; i < fileLength + 1; i++) {
			contents[i] = 0;
		}

		fread(contents, 1, fileLength, fp);
		contents[fileLength + 1] = '\0';
		fclose(fp);
		return contents;
	}

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

	/* build and compile shader program */
	const char *vertexShaderSource = readFile("data/shaders/shader05.vert");
	const char *fragmentShaderSource = readFile("data/shaders/shader05.frag");

	//vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for compilation errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//printf("Vertex shader error:\n%s\n", infoLog);
		
	}

	//fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Fragment shader error:\n%s\n", infoLog);
	}

	//link shaders to make the shaderprogram
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Error when linking shaders:\n%s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float iResolutionUniform = glGetUniformLocation(shaderProgram, "iResolution");
	float iGlobalTimeUniform = glGetUniformLocation(shaderProgram, "iGlobalTime");

	GLfloat vertices[] =
	{
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	GLuint vertexBufferObject;
	//generate 1 vertex buffer
	glGenBuffers(1, &vertexBufferObject);
	//bind the buffer to the GL_ARRAY_BUFFER
	//buffer calls to the GL_ARRAY_BUFFER targets the currently bound buffer (vbo)
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	//copies the vertex data into the buffer that is bound to the GL_ARRAY_BUFFER
	//GL_STREAM_DRAW - the data will change every time it is drawn
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//enable the position attribute - enable the data that was copied to the buffer
	//to go to through the graphics processing pipeline 
	glEnableVertexAttribArray(0);
	//tells opengl how our data buffer is structured - point it to the start of the buffer
	glVertexAttribPointer(0,	//index of vertex attribute to modify
		2,						//size of the vertex attribute, number of components. The vertex attribute is a vec2 so it is composed of 2 values.
		GL_FLOAT,				//data type of each component in the array
		GL_FALSE,				//normalized = false
		0,						//stride, space between consecutive vertex attribute sets
		0);						//offset, where to start read in the buffer

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

		//use shader program for rendering
		glUseProgram(shaderProgram);
		glUniform1f(iGlobalTimeUniform, float(SDL_GetTicks()));
		glUniform2f(iResolutionUniform, float(screenWidth), float(screenHeight));

		//set wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//update the input attributes
		//glVertexAttrib4fv(0, position);
		//glVertexAttrib4fv(1, col);

		glBindVertexArray(vertexBufferObject);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

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
	
	glDeleteProgram(shaderProgram);
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
