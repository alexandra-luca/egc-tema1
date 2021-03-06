#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

#include <math.h>

using namespace std;

#define MAX_BALOANE 12
#define MAX_SHURIKEN 10

bool isPressedW = false;
bool isPressedS = false;
float playerX = 20;
float playerY = 300;
float balonX[MAX_BALOANE];
float balonY[MAX_BALOANE];
int tip[MAX_BALOANE];
float shurikenX[MAX_SHURIKEN];
float shurikenY[MAX_SHURIKEN];
float sageataX[100];
float sageataY[100];
bool sageataEnabled[100];
float sageataScaleX = 2.5;
float sageataScaleY = 0.02;
float squareSide = 100;
float sageata_length;
float varfX, varfY;
float centruX, centruY;
float distanta;
int scor = 0;
float centruPlayerX, centruPlayerY;
float centruShurikenX, centruShurikenY;
int vieti = 3;
float speed[100];
float cateta_opusa, cateta_alaturata, ipotenuza;
float sin_unghi[100], cos_unghi[100];
float contor = 0;
bool isPressedMouse = false;
float dreptunghiX = 2.5;
float dreptunghiY = 0.1;
int index = 0;
float rotateShuriken = 0;
float sin_arc = 0;
float playerSize = 150;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;

	// player
	{
		glm::vec3 color = glm::vec3(0.7, 0, 0.3);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(10, 5, 0), color),
			VertexFormat(glm::vec3(15, 15, 0), color),
			VertexFormat(glm::vec3(15, 25, 0), color),
			VertexFormat(glm::vec3(10, 35, 0), color),
			VertexFormat(glm::vec3(0, 40, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3, 4, 5
		};

		meshes["player"] = new Mesh("generated player");
		meshes["player"]->InitFromData(vertices, indices);
		meshes["player"]->SetDrawMode(GL_LINE_LOOP);

		Mesh* player = meshes["player"];
		AddMeshToList(player);
	}

	// baloane
	for (int i = 0; i < MAX_BALOANE; i++) {
		balonY[i] = -100 - rand() % 600;
		balonX[i] = rand() % 1000 + 280;
	}

	for (int i = 0; i < 5; i++) {
		tip[i] = 0;
	}
	for (int i = 5; i < 10; i++) {
		tip[i] = 1;
	}
	tip[10] = 2;
	tip[11] = 3;

	// balon rosu
	{
		glm::vec3 color = glm::vec3(1, 0, 0);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(50, 50, 0), color),
			VertexFormat(glm::vec3(50, 0, 0), color),
			VertexFormat(glm::vec3(100, 20, 0), color),
			VertexFormat(glm::vec3(100, 80, 0), color),
			VertexFormat(glm::vec3(50, 100, 0), color),
			VertexFormat(glm::vec3(0, 80, 0), color),
			VertexFormat(glm::vec3(0, 20, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3, 4, 5, 6, 1
		};

		meshes["balon_rosu"] = new Mesh("generated balon");
		meshes["balon_rosu"]->InitFromData(vertices, indices);
		meshes["balon_rosu"]->SetDrawMode(GL_TRIANGLE_FAN);

		Mesh* balon_rosu = meshes["balon_rosu"];
		AddMeshToList(balon_rosu);
	}

	// balon galben
	{
		glm::vec3 color = glm::vec3(0.8, 0.7, 0);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(50, 50, 0), color),
			VertexFormat(glm::vec3(50, 0, 0), color),
			VertexFormat(glm::vec3(100, 20, 0), color),
			VertexFormat(glm::vec3(100, 80, 0), color),
			VertexFormat(glm::vec3(50, 100, 0), color),
			VertexFormat(glm::vec3(0, 80, 0), color),
			VertexFormat(glm::vec3(0, 20, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3, 4, 5, 6, 1
		};

		meshes["balon_galben"] = new Mesh("generated balon");
		meshes["balon_galben"]->InitFromData(vertices, indices);
		meshes["balon_galben"]->SetDrawMode(GL_TRIANGLE_FAN);

		Mesh* balon_galben = meshes["balon_galben"];
		AddMeshToList(balon_galben);
	}

	// balon verde - BONUS
	// cand jucatorul il nimereste, primeste o viata
	{
		glm::vec3 color = glm::vec3(0, 0.5, 0);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(50, 50, 0), color),
			VertexFormat(glm::vec3(50, 0, 0), color),
			VertexFormat(glm::vec3(100, 20, 0), color),
			VertexFormat(glm::vec3(100, 80, 0), color),
			VertexFormat(glm::vec3(50, 100, 0), color),
			VertexFormat(glm::vec3(0, 80, 0), color),
			VertexFormat(glm::vec3(0, 20, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3, 4, 5, 6, 1
		};

		meshes["balon_verde"] = new Mesh("generated balon");
		meshes["balon_verde"]->InitFromData(vertices, indices);
		meshes["balon_verde"]->SetDrawMode(GL_TRIANGLE_FAN);

		Mesh* balon_verde = meshes["balon_verde"];
		AddMeshToList(balon_verde);
	}

	// balon mov - BONUS
	// cand jucatorul il nimereste, moare
	{
		glm::vec3 color = glm::vec3(0.4, 0, 0.5);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(50, 50, 0), color),
			VertexFormat(glm::vec3(50, 0, 0), color),
			VertexFormat(glm::vec3(100, 20, 0), color),
			VertexFormat(glm::vec3(100, 80, 0), color),
			VertexFormat(glm::vec3(50, 100, 0), color),
			VertexFormat(glm::vec3(0, 80, 0), color),
			VertexFormat(glm::vec3(0, 20, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3, 4, 5, 6, 1
		};

		meshes["balon_mov"] = new Mesh("generated balon");
		meshes["balon_mov"]->InitFromData(vertices, indices);
		meshes["balon_mov"]->SetDrawMode(GL_TRIANGLE_FAN);

		Mesh* balon_mov = meshes["balon_mov"];
		AddMeshToList(balon_mov);
	}

	// shuriken
	{
		glm::vec3 color = glm::vec3(0.2, 0.2, 1);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(50, 0, 0), color),
			VertexFormat(glm::vec3(100, 0, 0), color),
			VertexFormat(glm::vec3(0, 50, 0), color),
			VertexFormat(glm::vec3(50, 50, 0), color),
			VertexFormat(glm::vec3(100, 50, 0), color),
			VertexFormat(glm::vec3(0, 100, 0), color),
			VertexFormat(glm::vec3(50, 100, 0), color),
			VertexFormat(glm::vec3(100, 100, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 4, 3, 
			1, 2, 4,
			4, 5, 8,
			4, 7, 6
		};

		meshes["shuriken"] = new Mesh("generated shuriken");
		meshes["shuriken"]->InitFromData(vertices, indices);
		meshes["shuriken"]->SetDrawMode(GL_TRIANGLES);

		Mesh* shuriken = meshes["shuriken"];
		AddMeshToList(shuriken);
	}

	for (int i = 0; i < MAX_SHURIKEN; i++) {
		shurikenY[i] = rand() % 720;
		shurikenX[i] = 1280 + rand() % 2000;
	}

	// sageata
	Mesh* sageata = Object2D::CreateSquare("sageata", corner, squareSide, glm::vec3(0.7, 0, 0.3), true);
	AddMeshToList(sageata);

	// dreptunghi
	Mesh* dreptunghi = Object2D::CreateSquare("dreptunghi", corner, squareSide, glm::vec3(0.7, 0, 0), true);
	AddMeshToList(dreptunghi);

	// triunghi
	{
		glm::vec3 color = glm::vec3(0.7, 0, 0.3);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, -10, 0), color),
			VertexFormat(glm::vec3(20, 0, 0), color),
			VertexFormat(glm::vec3(0, 10, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2
		};

		meshes["triunghi"] = new Mesh("generated trinunghi");
		meshes["triunghi"]->InitFromData(vertices, indices);
		meshes["triunghi"]->SetDrawMode(GL_TRIANGLES);

		Mesh* triunghi = meshes["triunghi"];
		AddMeshToList(triunghi);
	}

	// linie in zig-zag
	{
		glm::vec3 color = glm::vec3(1, 1, 1);

		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(0, 0, 0), color),
			VertexFormat(glm::vec3(10, 20, 0), color),
			VertexFormat(glm::vec3(0, 40, 0), color),
			VertexFormat(glm::vec3(10, 60, 0), color),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2, 3
		};

		meshes["zigzag"] = new Mesh("generated zigzag");
		meshes["zigzag"]->InitFromData(vertices, indices);
		meshes["zigzag"]->SetDrawMode(GL_LINE_STRIP);

		Mesh* zigzag = meshes["zigzag"];
		AddMeshToList(zigzag);
	}
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{
	// player
	float playerMeshWidth = 15;
	float playerMeshHeight = 40;

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(playerX, playerY);
	modelMatrix *= Transform2D::Scale(3,3);
	modelMatrix *= Transform2D::Translate(playerMeshWidth/2, playerMeshHeight/2);
	modelMatrix *= Transform2D::Rotate(-asin(sin_arc));
	modelMatrix *= Transform2D::Translate(-playerMeshWidth/2, -playerMeshHeight/2);
	RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);

	// baloane
	for (int b = 0; b < MAX_BALOANE; b++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(balonX[b], balonY[b]);
		if (tip[b] == 0) {
			RenderMesh2D(meshes["balon_rosu"], shaders["VertexColor"], modelMatrix);
		}
		else if (tip[b] == 1) {
			RenderMesh2D(meshes["balon_galben"], shaders["VertexColor"], modelMatrix);
		}
		else if (tip[b] == 2) {
			RenderMesh2D(meshes["balon_verde"], shaders["VertexColor"], modelMatrix);
		}
		else if (tip[b] == 3) {
			RenderMesh2D(meshes["balon_mov"], shaders["VertexColor"], modelMatrix);
		}
		modelMatrix *= Transform2D::Translate(50, -60);
		RenderMesh2D(meshes["zigzag"], shaders["VertexColor"], modelMatrix);

		balonY[b] += 100 * deltaTimeSeconds;

		if (balonY[b] > 800) {
			balonY[b] = -100 - rand() % 200;
			balonX[b] = rand() % 1000 + 280;
		}
	}

	rotateShuriken += 2 * deltaTimeSeconds;

	// shuriken
	for (int s = 0; s < MAX_SHURIKEN; s++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(shurikenX[s], shurikenY[s]);
		modelMatrix *= Transform2D::Translate(squareSide/2, squareSide/2);
		modelMatrix *= Transform2D::Rotate(rotateShuriken);
		modelMatrix *= Transform2D::Translate(-squareSide/2, -squareSide/2);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);

		shurikenX[s] -= 200 * deltaTimeSeconds;

		if (shurikenX[s] < -200) {
			shurikenX[s] = 1500;
			shurikenY[s] = rand() % 500;
		}
	}

	// sageata
	for (int i = 0; i < 100; i++) {
		if (sageataEnabled[i]) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(sageataX[i], sageataY[i]);
			modelMatrix *= Transform2D::Rotate(asin(sin_unghi[i]));
			modelMatrix *= Transform2D::Scale(sageataScaleX, sageataScaleY);
			RenderMesh2D(meshes["sageata"], shaders["VertexColor"], modelMatrix);

			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(sageataX[i], sageataY[i]);
			modelMatrix *= Transform2D::Rotate(asin(sin_unghi[i]));
			modelMatrix *= Transform2D::Translate(250, 0);
			RenderMesh2D(meshes["triunghi"], shaders["VertexColor"], modelMatrix);

			sageataX[i] += speed[i] * cos_unghi[i] * deltaTimeSeconds;
			sageataY[i] += speed[i] * sin_unghi[i] * deltaTimeSeconds;
		}
	}

	for (int i = 0; i < 100; i++) {
		for (int b = 0; b < MAX_BALOANE; b++) {
			// coliziune sageata-balon
			sageata_length = squareSide * sageataScaleX;

			varfX = sageataX[i] + sageata_length * cos_unghi[i];
			varfY = sageataY[i] + sageata_length * sin_unghi[i];
			centruX = squareSide / 2 + balonX[b];
			centruY = squareSide / 2 + balonY[b];

			distanta = sqrt(pow(centruY - varfY, 2) + pow(centruX - varfX, 2));

			if (distanta < squareSide / 2) {
				balonY[b] = -100 - rand() % 200;
				balonX[b] = rand() % 1000 + 280;

				if (tip[b] == 0) {
					// daca balonul e rosu, scorul creste
					scor++;
					cout << "Scor: " << scor << endl;
				}
				else if (tip[b] == 1) {
					// daca balonul e galben, scorul scade
					scor--;
					cout << "Scor: " << scor << endl;
				}
				else if (tip[b] == 2) {
					// daca balonul e verde, jucatorul primeste o viata
					vieti++;
					cout << "Ai primit o viata!" << endl;
					cout << "Vieti: " << vieti << endl;
				}
				else if (tip[b] == 3) {
					// daca nimereste balonul mov, se termina jocul
					cout << "Ouch, ai nimerit balonul mov!" << endl;
					cout << "Game over" << endl;
					exit(0);
				}
			}
		}
	}

	for (int i = 0; i < 100; i++) {
		for (int s = 0; s < MAX_SHURIKEN; s++) {
			// coliziune sageata-shuriken
			centruX = squareSide / 2 + shurikenX[s];
			centruY = squareSide / 2 + shurikenY[s];

			varfX = sageataX[i] + sageata_length * cos_unghi[i];
			varfY = sageataY[i] + sageata_length * sin_unghi[i];

			distanta = sqrt(pow(centruY - varfY, 2) + pow(centruX - varfX, 2));

			if (distanta < squareSide / 2) {
				shurikenX[s] = 1500;
				shurikenY[s] = rand() % 500;

				// scorul creste
				scor++;
				cout << "Scor: " << scor << endl;
			}
		}
	}

	// coliziune shuriken-player
	for (int s = 0; s < MAX_SHURIKEN; s++) {
		centruPlayerX = playerSize / 2 + playerX - 50;
		centruPlayerY = playerSize / 2 + playerY;

		centruShurikenX = squareSide / 2 + shurikenX[s];
		centruShurikenY = squareSide / 2 + shurikenY[s];

		distanta = sqrt(pow(centruPlayerY - centruShurikenY, 2) + pow(centruPlayerX - centruShurikenX, 2));

		if (distanta < squareSide/2 + playerSize/2) {
			shurikenX[s] = 1500;
			shurikenY[s] = rand() % 500;

			// jucatorul pierde o viata
			vieti--;
			cout << "Vieti ramase: " << vieti << endl;

			if (vieti == 0) {
				cout << "Game over" << endl;
				exit(0);
			}
		}
	}

	// dreptunghi
	if (isPressedMouse) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Scale(dreptunghiX, dreptunghiY);
		RenderMesh2D(meshes["dreptunghi"], shaders["VertexColor"], modelMatrix);
	}
}

void Laborator3::FrameEnd()
{

}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
	if (isPressedW)
		playerY += 300 * deltaTime;
	if (isPressedS)
		playerY -= 300 * deltaTime;
	if (isPressedMouse) {
		contor += deltaTime;

		dreptunghiX = 5*contor;
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_W)
		isPressedW = true;
	if (key == GLFW_KEY_S)
		isPressedS = true;
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_W)
		isPressedW = false;
	if (key == GLFW_KEY_S)
		isPressedS = false;
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	mouseY = 720 - mouseY;
	// add mouse move event
	cateta_opusa = mouseY - (playerY + squareSide/2);
	cateta_alaturata = mouseX - playerX;
	ipotenuza = sqrt(pow(cateta_opusa, 2) + pow(cateta_alaturata, 2));

	sin_arc = - cateta_opusa / ipotenuza;
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (button == GLFW_MOUSE_BUTTON_2)
		isPressedMouse = true;
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (button == GLFW_MOUSE_BUTTON_2) {
		isPressedMouse = false;

		sageataEnabled[index] = true;
		sageataX[index] = playerX;
		sageataY[index] = playerY + squareSide / 2;

		// window = 1280 x 720
		mouseY = 720 - mouseY;

		cateta_opusa = mouseY - sageataY[index];
		cateta_alaturata = mouseX - sageataX[index];
		ipotenuza = sqrt(pow(cateta_opusa, 2) + pow(cateta_alaturata, 2));

		sin_unghi[index] = cateta_opusa / ipotenuza;
		cos_unghi[index] = cateta_alaturata / ipotenuza;

		speed[index] = 500 * (contor+1);
		contor = 0;

		index++;
		if (index == 100) {
			index = 0;
		}
	}
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
