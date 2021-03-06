#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

bool isPressedP;
int isPressedC;
float x, y, z;

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	
	{
		Mesh* mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	x = 1;
	y = 0.5;
	z = 0;
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(0, 0, 0, 1);
	if (isPressedP == true) {
		glClearColor(0, 1, 1, 1);
	}

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(x, y, z), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

	// render the teapot
	// teapot -> box -> sphere
	if (isPressedC == 0) {
		RenderMesh(meshes["teapot"], glm::vec3(2, 0.5f, 0));
	} else if (isPressedC == 1) {
			RenderMesh(meshes["box"], glm::vec3(2, 0.5f, 0));
	} else if (isPressedC == 2) {
		RenderMesh(meshes["sphere"], glm::vec3(2, 0.5f, 0));
	}
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_P) {
		// do something
		isPressedP = true;
	}

	if (key == GLFW_KEY_C) {
		isPressedC++;
		cout << isPressedC << endl;
	}

	if (isPressedC == 3)
		isPressedC = 0;

	if (key == GLFW_KEY_W) {
		y += 0.2;
	}

	if (key == GLFW_KEY_A) {
		x -= 0.2;
	}

	if (key == GLFW_KEY_S) {
		y -= 0.2;
	}

	if (key == GLFW_KEY_D) {
		x += 0.2;
	}

	if (key == GLFW_KEY_E) {
		z += 0.2;
	}

	if (key == GLFW_KEY_Q) {
		z -= 0.2;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
