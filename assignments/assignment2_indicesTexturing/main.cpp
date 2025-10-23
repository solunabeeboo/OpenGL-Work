#include <stdio.h>
#include <math.h>
using namespace std;

#include <rafa/shader.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float BASE_COLOR[3] = {100, 50, 20};
const float ZOOM = 22;
const float OFFSET = 1.1;

float vertices[40] = {
	//Position (XYZ) Color(RGBA) UV(xy)
	-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, // BOTTOM LEFT (y0)
	0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.5, 1.0, 1.0, 0.0,  // BOTTOM RIGHT (y1)
	0.5f, 0.5f, 0.0f, 0.5, 0.25, 1.00, 1.0, 1.0, 1.0, //TOP RIGHT (y2)
	-0.5f, 0.5f, 0.0f, 0.5, 0.25, 1.00, 1.0, 0.0, 1.0,//TOP LEFT (y3)
};
unsigned int indices[6] = { //going counter clock represents front facing trianglein OpenGL by convention. Otherway is backface!
	0, 1, 2, //bottom right triangle
	2, 3, 0 //top left triangle!
}; //instead of 6 vertices for 2 triangles, we share by referencing w/ indices! only duplicate is unsigned int, big data saves!

int main() {

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Texture", NULL, NULL);
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

	GLuint VAO; //this VAO binds to the VBO, EBO and whatever else to keep it organized!
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//  Color Attributes
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3)); //think of it, stride is how many, and the (void*)0 is where to start!
	glEnableVertexAttribArray(1);
	//  UV Attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 7)); //think of it, stride is how many, and the (void*)0 is where to start!
	glEnableVertexAttribArray(2);

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//takes program, and var name, and gets the memory address for it! could do it before the loop but wtv
		ourShader.setFloat("uTime", time);
		ourShader.setFloat("uOffset", OFFSET);
		ourShader.setVector("uBaseColor", BASE_COLOR[0], BASE_COLOR[1], BASE_COLOR[2]);
		ourShader.setFloat("uSize", ZOOM);

		//THE ALMIGHTY DRAW CALL
		//so this is a little diff from the others... asks how to draw, asks how MANY indices, how theyre stored, and then pos>NULL
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Drawing happens here!
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

}