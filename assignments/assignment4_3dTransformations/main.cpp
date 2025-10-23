#include <rafa/header.h>
#include <rafa/shader.h>
#include <rafa/texture.h>
#include <rafa/camera.h>
using namespace std;



const int SCREEN_WIDTH = 1260;
const int SCREEN_HEIGHT = 720;
const float SIZE = 1.0;

//float vertices[40] = {
//	//Position (XYZ) Color(RGBA) UV(xy)
//	-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, // BOTTOM LEFT (y0)
//	0.5f, -0.5f, 0.0f, 0.0, 1.0, 0.5, 1.0, 1.0, 0.0,  // BOTTOM RIGHT (y1)
//	0.5f, 0.5f, 0.0f, 0.5, 0.25, 1.00, 1.0, 1.0, 1.0, //TOP RIGHT (y2)
//	-0.5f, 0.5f, 0.0f, 0.5, 0.25, 1.00, 1.0, 0.0, 1.0,//TOP LEFT (y3)
//};
//unsigned int indices[6] = { //going counter clock represents front facing trianglein OpenGL by convention. Otherway is backface!
//	0, 1, 2, //bottom right triangle
//	2, 3, 0 //top left triangle!
//}; //instead of 6 vertices for 2 triangles, we share by referencing w/ indices! only duplicate is unsigned int, big data saves!

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

	Camera camera;

	//capture cursor and further initialize camera class
	camera.setInputMode(window, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

	//Initialization goes here!
	Shader ourShader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	Texture woodTexture("assets/woodfloor.png");
	unsigned int wdTex = woodTexture.generateTexture();

	float vertices[] = {
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// world space positions of our cubes
	float posXMin = -8.0f;
	float posXMax = 8.0f;
	float posYMin = -5.0f;
	float posYMax = 5.0f;
	glm::vec3 cubePositions[] = {
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f)),
		glm::vec3(ew::RandomRange(posXMin, posXMax), ew::RandomRange(posYMin, posYMax), ew::RandomRange(-20.0f, -2.0f))
	};

	glm::vec3 cubeRotations[] = {
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f)),
		glm::vec3(ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f), ew::RandomRange(0.0f, 360.0f))
	};

	glm::vec3 cubeScales[] = {
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f)),
		glm::vec3(ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f), ew::RandomRange(0.5f, 2.5f))
	};

	GLuint VAO; //this VAO binds to the VBO, EBO and whatever else to keep it organized!
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//GLuint EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//  Color Attributes
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3)); //think of it, stride is how many, and the (void*)0 is where to start!
	//glEnableVertexAttribArray(1);
	//  UV Attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3)); //think of it, stride is how many, and the (void*)0 is where to start!
	glEnableVertexAttribArray(1);





	//do ALL the above in Shader class ..ew/rafa/shader.h
	unsigned int shaderProgram = ourShader.use();
	glPointSize(8);
	//ourShader.setInt("texture1", 0);
	//ourShader.setInt("texture2", 1);

	//Orthographic Projection Matrix!
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	//now... we need to make a _Transformation Matrix_ to utilize the projmatrix!
	ourShader.setInt("uTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wdTex);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Camera Events
		camera.calculateDeltaTime();
		camera.processInput(window);
		//camera.movePredefinedCircle();

		//seconds elapsed from window start, in seconds!
		float time = glfwGetTime();

		//Clear framebuffer
		//98, 123, 156
		glClearColor(0.384f, 0.482f, 0.612f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		//takes program, and var name, and gets the memory address for it! could do it before the loop but wtv
		ourShader.setFloat("uTime", time);
		ourShader.setVector("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		ourShader.setFloat("uSize", SIZE);

		//Models, model matrix!
		glm::mat4 view = glm::mat4(1.0f);
		//providing options for perspective AND orthographic so can switch realtime
		glm::mat4 projection = camera.getProjection(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.1f, 100.0f,
			-10.0f, 10.0f, -10.0f, 10.0f);

		ourShader.setMatrix4fv("projection", glm::value_ptr(projection));
		view = camera.GetViewMatrix();
		ourShader.setMatrix4fv("view", glm::value_ptr(view));

		//use triangle mesh
		glBindVertexArray(VAO);

		//model loop!
		for (unsigned int i = 0; i < 20; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(cubeRotations[i].x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(cubeRotations[i].y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(cubeRotations[i].z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, cubeScales[i]);

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMatrix4fv("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// note that we're translating the scene in the reverse direction of where we want to move
		//for multiple cubes...
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//THE ALMIGHTY DRAW CALL
		//so this is a little diff from the others... asks how to draw, asks how MANY indices, how theyre stored, and then pos>NULL
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Drawing happens here!
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	printf("Shutting down...");

}

//Our vertex coordinates first start in local space as local coordinates and are then further processed to world coordinates, 
// view coordinates, clip coordinates and eventually end up as screen coordinates.