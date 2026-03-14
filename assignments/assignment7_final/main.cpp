#include <rafa/header.h>
#include <rafa/shader.h>
#include <rafa/texture.h>
#include <rafa/camera.h>
#include <rafa/vertices.h>
#include <rafa/grassObject.h>
using namespace std;
using namespace vert;

//Scott: Added the draw call for the cylinder and the sphere
//Also added the different shading feature
//Rafa: provided the base project to work from, added calls for plane and box   
//also revamped the ImGUI to have dropdowns and collapsibles

const glm::vec4 CLEAR_COLOR = glm::vec4(0.2, 0.2, 0.2, 1.0);
int SCREEN_WIDTH = 1260;
int SCREEN_HEIGHT = 720;
const float SIZE = 1;

glm::vec3 lightPosition = glm::vec3(14.8, 20, -5);
glm::vec3 lightColor = glm::vec3(1, 1, 1);
float ambientK = 0.220;
float diffuseK = 0.283;
float specularK = 0.103;
float shininess = 36;

//water shader
float speed = 1.310f;
float amplitude = 0.50f;
float frequency = 0.010f;
float wfloor = 1.0;
//frag
float waterAlpha = 0.850f;
float darkness = 2.490f;
float artificialDepth = 1.530f;
float foamSpeed = 7.610f;
float waveSpeed = 0.020f;
float frothedScale = 4.440f;
float frothBrightness = 4.340f;


bool wireframe = false;
bool point = false;
bool culling = true;

int shading = 2;

// Grass Data
bool pushChanges = true;
float density = 0.2; // 0.0 - 1.0

float spread = 97; // 0.0 - Infinity
int shapeNumber = 1; // CIRCLE or SQUARE
grass::grassShapes shape = grass::SQUARE;
float height = 0.568; // 0.0 - Infinity
float baseSize = 0.050; // 0.0 - spread
float heightVariation = 0.7; // 0.0 - height

vec3 color = vec3(0.7, 0.6, 0.5); // All values 0.0 - 1.0
float colorVariationAmount = 0.2; // 0.0 - 1.0
float brightnessVariationAmount = 0.1; // 0.0 - 1.0
// Setting this to the max makes it look like dappled light

float windSpeed = 1.385; // 0.0 - Infinity
float windStrength = 0.03; // 0.0 - Infinity
float windDirection = 0; // 0.0 - 360.0

vec3 position = vec3(0, 0, 3);

bool fadeOff = false;
float upNormal = 0.25;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

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

	//register frame callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//capture cursor and further initialize camera class
	Camera camera;
	camera.setInputMode(window, SCREEN_WIDTH, SCREEN_HEIGHT);

	//IMGUI Initialization (PUT AFTER CAMERA)
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//Initialization goes here!
		//Shaders
	Shader unlitShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	Shader litShader("assets/vertexShader.vert", "assets/lit_fragmentShader.frag");
	Shader waterShader("assets/oceanVertex.vert", "assets/oceanFragment.frag");
	Shader grassShader("assets/grassVertexShader.vert", "assets/grassFragmentShader.frag");
	Shader skyboxShader("assets/skyboxVertex.vert", "assets/skyboxFragment.frag");

	//Textures
	Texture woodTexture("assets/woodfloor.jpg");
	unsigned int wdTex = woodTexture.generateTexture();

	Texture waterTexture("assets/WATER.jpg");
	unsigned waveTex = waterTexture.generateTexture();

	Texture frothTexture("assets/ParticleCloudWhite.png");
	unsigned frothTex = frothTexture.generateTexture();

	Texture grassTexture("assets/grass.png");
	unsigned int gsTex = grassTexture.generateTexture();
	std::vector<std::string> skyboxFaces = {
	"assets/skybox/right.png",
	"assets/skybox/left.png",
	"assets/skybox/top.png",
	"assets/skybox/bottom.png",
	"assets/skybox/front.png",
	"assets/skybox/back.png"
	};
	CubemapTexture skyboxTexture(skyboxFaces);

	//Geometry
	int planeSeg = 1000;
	MeshData planeData = vert::createPlane(1000, 1000, planeSeg);
	Mesh planeMesh(planeData);

	float boxSeg = 2;
	MeshData boxData = vert::createBox(1, 1, 1, boxSeg);
	Mesh boxMesh(boxData);

	float box2Seg = 40;
	MeshData boxData2 = vert::createBox(10, 20, 10, box2Seg);
	Mesh bigMesh(boxData2);

	float sphereSeg = 32;
	MeshData sphereData = vert::createSphere(2.0f, sphereSeg);
	Mesh sphereMesh(sphereData);

	float cylinderSeg = 32;
	MeshData cylinderData = vert::createCylinder(3.0f, 1.0f, cylinderSeg);
	Mesh cylinderMesh(cylinderData);

	float torusSeg[] = { 15, 15 }; // 0 = # of rings, 1 = segments in rings
	MeshData torusData = vert::createTorus(1, 5, torusSeg[0], torusSeg[1]);
	Mesh torusMesh(torusData);

	float terrainWidth = 100.0f; 	float editedterrainWidth = terrainWidth;
	float terrainDepth = 100.0f;    float editedterrainDepth = terrainDepth;
	float terrainHeightScale = 3.8f; float editedterrainHeightScale = terrainHeightScale;
	int terrainSegments = 100;
	MeshData terrainData = vert::createTerrain(terrainWidth, terrainDepth, terrainSegments, terrainHeightScale);
	Mesh terrainMesh(terrainData);

	MeshData skyboxData = vert::createSphere(90.0f, 64);
	Mesh skyboxMesh(skyboxData);

	Model backpackModel("assets/backpack/backpack.obj");
	//Model ironmanModel("assets/ironman/IronMan.obj");
	//Model mustangModel("assets/mustang/Mustang.obj");

	grass::grassField myGrass;
	myGrass.setDensity(density);
	myGrass.setColor(color, colorVariationAmount, brightnessVariationAmount, upNormal);
	myGrass.setPosition(vec3(0, 0, 0));
	myGrass.setSize(spread, shape, height, baseSize, heightVariation);
	myGrass.setWind(windSpeed, windStrength, windDirection);
	myGrass.setFade(fadeOff);
	myGrass.setTerrainData(&terrainData, terrainWidth, terrainDepth, 0.0f);
	myGrass.setConformToTerrain(true); // Start with it off
	myGrass.regenerateGrass();
	myGrass.use();

	//Set Textures
	litShader.use();
	litShader.setInt("uTexture", 0);

	waterShader.use();
	waterShader.setInt("uTexture", 0);
	waterShader.setInt("uFoamTex", 1);

	grassShader.use();
	grassShader.setInt("uTexture", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	glPointSize(2);
	camera.cameraPosition.y = 10;

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Camera Events
		camera.calculateDeltaTime();
		camera.processInput(window);
		//camera.movePredefinedCircle();
		//ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		//seconds elapsed from window start, in seconds!
		float time = glfwGetTime();
		//Clear framebuffer
		glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, CLEAR_COLOR.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

		//WIREFRAME CULLING
		if (wireframe)
			glPolygonMode(GL_FRONT, GL_LINE); //Enable wireframe mode
		else
			glPolygonMode(GL_FRONT, GL_FILL); //Disable wireframe mode
		if (culling)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);  // This should be outside the if/else
		//END WIREFRAME CULLING

		//SET SHADER VARS
		litShader.use();

		litShader.setFloat("uTime", time);
		litShader.setFloat("uSize", SIZE);
		litShader.setVector("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);

		litShader.setVector("uLightColor", lightColor.x, lightColor.y, lightColor.z);
		litShader.setFloat("uAmbient", ambientK);
		litShader.setFloat("uDiffuse", diffuseK);
		litShader.setVector("uLightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		litShader.setVector("uViewPos", camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z);
		litShader.setFloat("uSpecularStrength", specularK);
		litShader.setFloat("uShininess", shininess);
		litShader.setInt("uShading", shading);

		//SET SHADER VARS
		waterShader.use();
		//vertex
		waterShader.setFloat("uTime", time);
		waterShader.setFloat("uSpeed", speed);
		waterShader.setFloat("uFrequency", frequency);
		waterShader.setFloat("uAmplitude", amplitude);
		waterShader.setFloat("uFloor", wfloor);

		//fragment
		waterShader.setFloat("uSize", SIZE);
		waterShader.setVector("uResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		waterShader.setVector("uLightColor", lightColor.x, lightColor.y, lightColor.z);
		waterShader.setFloat("uAmbient", ambientK);
		waterShader.setFloat("uDiffuse", diffuseK);
		waterShader.setVector("uLightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		waterShader.setVector("uViewPos", camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z);
		waterShader.setFloat("uSpecularStrength", specularK);
		waterShader.setFloat("uShininess", shininess);
		waterShader.setInt("uShading", shading);

		waterShader.setFloat("waterAlpha", waterAlpha);
		waterShader.setFloat("darkness", darkness);
		waterShader.setFloat("artificialDepth", artificialDepth);
		waterShader.setFloat("foamSpeed", foamSpeed);
		waterShader.setFloat("waveSpeed", waveSpeed);
		waterShader.setFloat("frothedScale", frothedScale);
		waterShader.setFloat("frothBrightness", frothBrightness);

		//SET SHADER VARS
		grassShader.use();

		grassShader.setFloat("uTime", time);
		grassShader.setVector("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
		grassShader.setVector("ambientLightColor", lightColor.x, lightColor.y, lightColor.z);
		grassShader.setVector("diffuseLightColor", lightColor.x, lightColor.y, lightColor.z);
		grassShader.setVector("specularLightColor", lightColor.x, lightColor.y, lightColor.z);
		grassShader.setVector("viewPos", camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z);
		grassShader.setFloat("ambientStrength", ambientK);
		grassShader.setFloat("diffuseStrength", diffuseK);
		grassShader.setFloat("specularStrength", specularK);
		grassShader.setFloat("shininess", shininess);

		grassShader.setFloat("height", height);
		grassShader.setFloat("windSpeed", windSpeed);
		grassShader.setFloat("windStrength", windStrength);
		grassShader.setFloat("windDirection", windDirection);
		grassShader.setVector("baseColor", color.x, color.y, color.z);
		grassShader.setFloat("colorStrength", colorVariationAmount);
		grassShader.setFloat("brightnessStrength", brightnessVariationAmount);

		grassShader.setInt("uShading", shading);

		//VIEW AND PROJECT 
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		//providing options for perspective AND orthographic so can switch realtime
		projection = camera.getProjection(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.1f, 100.0f,
			-100.0f, 100.0f, -56.25f, 56.25f);
		view = camera.GetViewMatrix();
		//END VIEW AND PROJECT

		//set view project
		unlitShader.use();
		unlitShader.setMatrix4fv("projection", glm::value_ptr(projection));
		unlitShader.setMatrix4fv("view", glm::value_ptr(view));

		litShader.use();
		litShader.setMatrix4fv("projection", glm::value_ptr(projection));
		litShader.setMatrix4fv("view", glm::value_ptr(view));

		waterShader.use();
		waterShader.setMatrix4fv("projection", glm::value_ptr(projection));
		waterShader.setMatrix4fv("view", glm::value_ptr(view));

		grassShader.use();
		grassShader.setMatrix4fv("projection", glm::value_ptr(projection));
		grassShader.setMatrix4fv("view", glm::value_ptr(view));

		//SKYBOX (Draw FIRST)
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_CULL_FACE);  // Disable culling for skybox

		skyboxShader.use();
		skyboxShader.setMatrix4fv("view", glm::value_ptr(view));
		skyboxShader.setMatrix4fv("projection", glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		skyboxTexture.bind();
		skyboxMesh.draw(false);

		glDepthFunc(GL_LESS);
		if (culling)  // Re-enable culling if it was on
			glEnable(GL_CULL_FACE);

		//GroundShader - Switch back to lit shader
		litShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, wdTex);

		/*//big box
		model = glm::mat4(1.0f);  // RESET model matrix
		model = glm::translate(model, glm::vec3(-20.0, 4.0, -5.0));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		bigMesh.draw(point);

		//Drawing Sphere
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0, 4.0, -5.0));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		sphereMesh.draw(point);

		//Drawing Cylinder
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0, 4.0, -5.0));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		cylinderMesh.draw(point);

		//Drawing Torus
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 5.0, -8.0));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		torusMesh.draw(point);*/

		//Drawing Terrain
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		terrainMesh.draw(point);

		//Drawing Backpack
		litShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 3.5, 0.0));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-110.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(.25, .25, .25));
		litShader.setMatrix4fv("model", glm::value_ptr(model));
		backpackModel.draw(litShader);

		//UNLIT SHADER
		//unlit box
		unlitShader.use();
		unlitShader.setVector("uLightColor", lightColor.x, lightColor.y, lightColor.z);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		unlitShader.setMatrix4fv("model", glm::value_ptr(model));
		boxMesh.draw(point);

		//GRASS SHADER
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gsTex);
		grassShader.use();

		if (pushChanges)
		{
			pushChanges = false;

			if (myGrass.getConformToTerrain())
			{
				myGrass.setPosition(vec3(0, 0, 0));
			}
			else
			{
				myGrass.setPosition(vec3(0, 0, 0));
			}
			myGrass.setTerrainData(&terrainData, terrainWidth, terrainDepth, 0.0f);

			myGrass.setDensity(density);
			myGrass.setColor(color, colorVariationAmount, brightnessVariationAmount, upNormal);
			myGrass.setSize(spread, shape, height, baseSize, heightVariation);
			myGrass.setWind(windSpeed, windStrength, windDirection);
			myGrass.setFade(fadeOff);
			myGrass.regenerateGrass();
			myGrass.updateBuffers();
		}
		myGrass.draw(grassShader);

		//WATER SHADER (KEEP LAST)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthMask(GL_FALSE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waveTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, frothTex);

		//water plane
		waterShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-500, -3.0, 500));
		waterShader.setMatrix4fv("model", glm::value_ptr(model));
		planeMesh.draw(point);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		//CURSOR STUFF
		camera.dynamicCursorLocking(window, GLFW_MOUSE_BUTTON_2);

		//IMGUI Loop
		//Create the Settings Window
		//REMEMBER: ImGui follows python coding
		ImGui::Begin("Settings");

		if (ImGui::CollapsingHeader("Lighting Controls!"))
		{
			ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
			ImGui::ColorEdit3("Light Color", &lightColor.r);
			ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);
		}

		if (ImGui::CollapsingHeader("Terrain Controls"))
		{
			ImGui::DragFloat("Terrain Width", &editedterrainWidth, 1.0f, 10.0f, 500.0f);
			ImGui::DragFloat("Terrain Depth", &editedterrainDepth, 1.0f, 10.0f, 500.0f);
			ImGui::DragFloat("Height Scale", &editedterrainHeightScale, 0.1f, 0.0f, 20.0f);
			ImGui::DragInt("Terrain Segments", &terrainSegments, 1, 1, 1000);

			// Reconstruct terrain if parameters changed
			if (terrainData.segments != terrainSegments || editedterrainWidth != terrainWidth ||
				editedterrainHeightScale != terrainHeightScale || editedterrainDepth != terrainDepth)
			{
				terrainWidth = editedterrainWidth;
				terrainHeightScale = editedterrainHeightScale;
				terrainDepth = editedterrainDepth;
				terrainData = vert::createTerrain(terrainWidth, terrainDepth, terrainSegments, terrainHeightScale);
				terrainMesh.recon(terrainData);

				// Update grass with new terrain data
				myGrass.setTerrainData(&terrainData, terrainWidth, terrainDepth, 0.0f);
				if (myGrass.getConformToTerrain())
				{
					myGrass.regenerateGrass();
					myGrass.updateBuffers();
				}
			}
		}

		if (ImGui::CollapsingHeader("Water Controls"))
		{
			ImGui::DragFloat("Water Speed", &speed, 0.1f, 0.01f, 100.0f);
			ImGui::DragFloat("Wave Frequency", &frequency, 0.0001f, 0.000001f, 500.0f);
			ImGui::DragFloat("Wave Amplitude", &amplitude, 0.1f, 0.0f, 500.0f);
			ImGui::DragFloat("Wave Floor", &wfloor, 0.1f, 0.0f, 100.0f);

			ImGui::DragFloat("Water Alpha", &waterAlpha, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Water Darkness", &darkness, 0.01f, 0.0f, 4.0f);
			ImGui::DragFloat("Artificial Depth", &artificialDepth, 0.01f, 0.0f, 4.0f);
			ImGui::DragFloat("Foam Speed", &foamSpeed, 0.01f, 0.0f, 50.0f);
			ImGui::DragFloat("Wave Speed", &waveSpeed, 0.01f, 0.0f, 50.0f);
			ImGui::DragFloat("Froth Scale", &frothedScale, 0.01f, 0.0f, 10.0f);
			ImGui::DragFloat("Froth Brightness", &frothBrightness, 0.01f, 0.0f, 10.0f);

			ImGui::DragInt("Segments", &planeSeg);
			if (planeData.segments != planeSeg)
			{
				planeData = vert::createPlane(1000, 1000, planeSeg);
				planeMesh.recon(planeData);
			}
		}

		if (ImGui::CollapsingHeader("Grass"))
		{
			if (ImGui::CollapsingHeader("Object"))
			{
				ImGui::SliderFloat("Density", &density, 0.0f, 1.0f);
				const char* shapeNames[3] =
				{
					"Cirle",
					"Square"
				};
				ImGui::Combo("Style", &shapeNumber, shapeNames, 2);
				ImGui::SliderFloat("Spread", &spread, 0.1f, 500.0f);
				ImGui::SliderFloat("Height", &height, 0.1f, 50.0f);
				ImGui::SliderFloat("Blade Size", &baseSize, 0.01f, spread / 4);
				ImGui::SliderFloat("Height Variation", &heightVariation, 0.0f, height);
				ImGui::Checkbox("Fade Off", &fadeOff);

				if (shapeNumber == 0) shape = grass::CIRCLE;
				else shape = grass::SQUARE;
			}
			if (ImGui::CollapsingHeader("Color"))
			{
				ImGui::ColorEdit3("Base Color", &color.x);
				ImGui::SliderFloat("Color Variation", &colorVariationAmount, 0.0f, 10.0f);
				ImGui::SliderFloat("Brightness Variation", &brightnessVariationAmount, 0.0f, 10.0f);
				ImGui::SliderFloat("Y Normals", &upNormal, -1.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("Wind"))
			{
				ImGui::SliderFloat("Wind Speed", &windSpeed, 0.0f, 25.0f);
				ImGui::SliderFloat("Wind Strength", &windStrength, 0.0f, 25.0f);
				ImGui::SliderFloat("Wind Direction", &windDirection, 0.0f, 2 * ew::PI);
			}
			/*if (ImGui::CollapsingHeader("Position"))
			{
				ImGui::DragFloat3("Object Position", &position.x, 0.1f, 100.0f);
			}*/

			pushChanges = ImGui::Button("Regenerate Grass");

			if (ImGui::Button("Debug Log"))
			{
				myGrass.DebugLog();
			}

			if (ImGui::Button("Preset: Forest"))
			{
				pushChanges = true;
				density = 0.5;
				baseSize = 0.1;
				heightVariation = 0.5;
				color = vec3(0.2, 0.5, 0.2);
				colorVariationAmount = 0.8;
				brightnessVariationAmount = 0.8;
				windSpeed = 2.8;
				windStrength = 0.7;
			}

			if (ImGui::Button("Preset: Beach"))
			{
				pushChanges = true;
				density = 0.2;
				baseSize = 0.05;
				heightVariation = 0.7;
				color = vec3(0.7, 0.6, 0.5);
				colorVariationAmount = 0.2;
				brightnessVariationAmount = 0.1;
				windSpeed = 1.4;
				windStrength = 1.4;
			}

			if (ImGui::Button("Preset: Rainbow"))
			{
				pushChanges = true;
				density = 0.5;
				baseSize = 0.1;
				heightVariation = 0.1;
				color = vec3(1, 1, 1);
				colorVariationAmount = 5;
				brightnessVariationAmount = 0.1;
				windSpeed = 0.8;
				windStrength = 0.8;
			}

			bool grassConform = myGrass.getConformToTerrain();
			if (ImGui::Checkbox("Conform to Terrain", &grassConform))
			{
				myGrass.setConformToTerrain(grassConform);
				myGrass.setTerrainData(&terrainData, terrainWidth, terrainDepth, 0.0f);

				if (grassConform)
				{
					myGrass.setPosition(vec3(0, 0, 0));
				}
				else
				{
					myGrass.setPosition(vec3(0, 0, 0));
				}

				myGrass.regenerateGrass();
				myGrass.updateBuffers();
			}
		}

		if (ImGui::CollapsingHeader("Geometry Controls"))
		{
			ImGui::Checkbox("Wireframe", &wireframe);
			ImGui::Checkbox("Point Drawing", &point);
			ImGui::Checkbox("Backface Culling", &culling);
		}

		if (ImGui::CollapsingHeader("Shading Controls"))
		{
			const char* items[] = { "UVs", "Normals", "Shaded" };
			ImGui::Combo("SMode", &shading, items, IM_ARRAYSIZE(items));
		}

		if (ImGui::CollapsingHeader("Segment Controls"))
		{
			ImGui::DragFloat("Big Box", &box2Seg);
			if (boxData2.segments != box2Seg)
			{
				boxData2 = vert::createBox(10, 20, 10, box2Seg);
				bigMesh.recon(boxData2);
			}
			ImGui::DragFloat("Sphere", &sphereSeg);
			if (sphereData.segments != sphereSeg)
			{
				sphereData = vert::createSphere(2.0f, sphereSeg);
				sphereMesh.recon(sphereData);
			}
			ImGui::DragFloat("Cylinder", &cylinderSeg);
			if (cylinderData.segments != cylinderSeg)
			{
				cylinderData = vert::createCylinder(3.0f, 1.0f, cylinderSeg);
				cylinderMesh.recon(cylinderData);
			}
			ImGui::DragFloat2("Torus", torusSeg);
			//PARSING the crappy use of float as a "pair" of ints. first half (pre '.') is segments in rings
			//second half (post '.') is # of rings in torus. If either is different then reconstruct
			if (stof(to_string(torusData.segments).substr(0, to_string(torusData.segments).find('.'))) != torusSeg[1]
				|| stof(to_string(torusData.segments).substr(to_string(torusData.segments).find('.'))) != torusSeg[0])
			{
				torusData = vert::createTorus(1, 5, torusSeg[0], torusSeg[1]);
				torusMesh.recon(torusData);
			}
		}

		ImGui::End();

		//Actually render the Imgui!
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//Drawing happens here!
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	printf("Shutting down...");

}