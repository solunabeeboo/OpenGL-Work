#include <stdio.h>
#include <math.h>
using namespace std;

#include <rafa/shader.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
float vertices[21] = {
	//Position (XYZ) Color(RGBA)
	-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0, // BOTTOM LEFT
	0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.5, 1.0, // BOTTOM RIGHT
	0.0f, 0.5f, 0.0f, 0.5, 0.25, 1.00, 1.0, //TOP CENTER
};

int main() {

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	//Position Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//  Color Attributes
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(sizeof(float) * 3)); //think of it, stride is how many, and the (void*)0 is where to start!
	glEnableVertexAttribArray(1);

	//do ALL the above in Shader class ..ew/rafa/shader.h
	Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int shaderProgram = ourShader.use();


	glPointSize(8);

	//use triangle mesh
	glBindVertexArray(VAO);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//seconds elapsed from window start, in seconds!
		float time = glfwGetTime();

		//Clear framebuffer
		//98, 123, 156
		glClearColor(0.384f, 0.482f, 0.612f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//takes program, and var name, and gets the memory address for it! could do it before the loop but wtv
		int loc = glGetUniformLocation(shaderProgram, "uTime");
		glUniform1f(loc, time); //and this actually sets the Uniform!

		//THE ALMIGHTY DRAW CALL
		//	Could use glTriangles... but dont have a lot of points yet! So lets just use point mode
		//	Kicks off our pipeline!
		//  glDrawArrays(GL_POINTS, 0, 1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

}

	//NOTES NOTES NOTES
	//now the stride is correct so the position is fine... it knows where to read position!
	//your telling it where in the array to read the next four floats for vector pos
	//so with our color stuff its every SEVEN points


	//could just make it an unsigned vertexShader, but using GLuint is better practice
	//its an ID for a specific gpu resource. Every shader and ID needs to be in GPU memory. Its a POINTER
	//GLuint vertexShader;
	//vertexShader = glCreateShader(GL_VERTEX_SHADER);
	////NULL bc it will auto find out our shader length.
	////all this says is THIS is the source code for this shader
	//glShaderSource(vertexShader, 1, &ourShader, NULL);
	//glCompileShader(vertexShader);
	////so lets get some info onto if this actuall compiles...
	////so what is this? Were getting an integer variable form our shader program. So "iv" = "integer variable"
	//int success;
	////doesnt return anyrthing, so lets just have it set success to something to get feedback!
	//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	////will return a myriad of values based on its status, but 0 is failed so lets check for that!
	//if (success == 0)
	//{
	//	//arbitrary allocation of memory to store the info log since we don'tknow what its size will be...
	//	char infoLog[512];
	//	//retrieves the message with the GPU
	//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	//	cout << "\nVertex Shader Compilation Error: " << infoLog << "---END OF LOG: 512char---" << endl;
	//}

	//GLuint fragmentShader;
	////so besides changing the names, we need to change the num to GL_FRAGMENT_SHADER bc were doing a different enum here!
	//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	////also need to use the fragment shader source instead...
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);
	////deleted success to share the variable with the above shader
	//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	//if (success == 0)
	//{
	//	char infoLog[512];
	//	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	//	cout << "\nFragment Shader Compilation Error: " << infoLog << "---END OF LOG: 512char---" << endl;
	//}

	////this creats a FULL executable program for us for whenever we want to do out drawing!
	//GLuint shaderProgram;
	//shaderProgram = glCreateProgram();
	////soo now we need to actually use the shaders!
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);

	////and... now we need to link them together! Like you would scripts into a project, and maybe there will be linking errors
	//glLinkProgram(shaderProgram);

	////soo same errors process but gets it from program not shader!
	////notice its all the same stuff just diff specifics
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	//if (success == 0)
	//{
	//	char infoLog[512];
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	//	cout << "\nShader Linker Error: " << infoLog << "---END OF LOG: 512char---" << endl;
	//}

	//and now just tell OpenGL what to use!
	//  glUseProgram(shaderProgram);
	//simply makes points bigger!
	//  glPointSize(24); //24x24p point

	//Now if you have a bunch of diff shaders like in a game, drawing a super fancy square and a simple textured one like in a game: you'd just use one program to draw one,
	//and another to draw the other!
	// 
	//glUseProgram(shaderProgram);

	//...the render loop would be here


	//THESE WOULD BE ABOVE MAIN
//x,y,z,w. Where do we want the vertex?
//THIS is a vlid vertex shader. the R"()" is just a fancy new way to write a string w/o a bunch of \\n lines
//POSITION

//were sending data out the shader, through the rasterizer, to the fragment shader
//just need some name matchint out/in varialbes + types!
//a _uniform_ means its NOT changing across the rendering pass, and will stay the same for each frame, unlike Color
//it can also be shared namewise across shaders
//const char* vertexShaderSource = R"(
//	#version 330
//	layout(location = 0) in vec3 aPos;
//	layout(location = 1) in vec4 aColor;
//	
//	out vec4 Color;
//
//	uniform float uTime;
//	void main()
//	{
//		Color = aColor;
//		vec3 pos = aPos;
//		pos.y += sin((2 * uTime) + pos.x) * 0.3;
//		pos.x += cos((2 * uTime )+ pos.y) * 0.3;
//		gl_Position = vec4(pos, 1.0);
//	}
//)";

//we want to run it once per pixel/fragment
//"out" means this will be an output form the shader! will b emore used later
//using an out bc openGL will assume thats the output vector could be called anything
//wil set, for all fragments/pixels of this shader, set the colors of the pixels to RED
//COLOR
// 
//were takign in an out from vertex, during rasterization it will interpolate the colors with "Barycentric Interpolation"
//a method to mix values across a triangle, a weighted blend of its vertex attributes
//in this case vec4 Color is a varying varible, because it varies per vertex!
//const char* fragmentShaderSource = R"(
//	#version 330
//	out vec4 FragColor;
//	in vec4 Color;
//
//	uniform float uTime;
//	void main()
//	{
//		vec4 modifiedColor = Color;
//
//		float sinTime = sin(uTime);
//		if (sinTime < 0)
//			sinTime *= -1;
//
//		modifiedColor *= sinTime + 0.5;
//		FragColor = modifiedColor;
//	}
//
//)";

