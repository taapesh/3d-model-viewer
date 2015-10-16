/*
Name: Arya Mohades
UTEID: am52293
UTCS ID: am611
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "loader.h"
using namespace std;

/* Camera controls */
bool ZOOM_ENABLED = false;
bool ORBIT_ENABLED = false;
bool PAN_ENABLED = false;

/* Translate object controls */
bool MOVE_LEFT = false;
bool MOVE_RIGHT = false;
bool MOVE_UP = false;
bool MOVE_DOWN = false;

/* Mouse position */
int mouseX, mouseY;

Scene* scene;
Camera* camera;
Transform* transform;
Model* model;
Light* light;


void onDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* display stuff */
	scene->updateScene();

	glutSwapBuffers();
}

// MODIFY //
void onReshape(int width, int height)
{
	GLfloat aspectRatio = (GLfloat) width / (GLfloat) height;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
}

void onMouse(int button, int state, int x, int y)
{
	ORBIT_ENABLED = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
	ZOOM_ENABLED = (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);
	PAN_ENABLED = (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN);

	mouseX = x;
	mouseY = y;
}

void processCommand(char* command)
{
	char *p = strtok(command, " ");
	if (strcmp(p, "L") == 0) {
		// Load new object and add to stack
		p = strtok(NULL, " ");
		int len = strlen(p);
		char filepath[len + 32];
		const char* ext = ".obj";
		const char* root = "models/";
		strcpy(filepath, root);

		strcat(filepath, p);
		if (strstr(p, ext) == NULL)
			strcat(filepath, ext);

		
		/* Load new model*/
		Model* model = new Model(filepath);
		TrimeshLoader loader;
		loader.loadOBJ(model);
		model->initializeModel();
		model->setRenderMode(SHADED_MODE);
		transform = new Transform(model);

		scene->addModel(model);
		cout << "Loaded " << model->name << " model." << endl;
		glutPostRedisplay();
	}
	else if (strcmp(p, "D") == 0)
	{
		// Delete current object and remove from stack
		if (scene->currentObject != NULL)
			cout << "Deleted " << scene->currentObject->name << " model." << endl;
		scene->deleteCurrentObject();
		glutPostRedisplay();
	}
	else if (strcmp(p, "I") == 0)
	{
		// Neutralize effects of any transformations
		if (scene->currentObject != NULL)
		{
			scene->currentObject->transform->resetTransform();
			scene->camera->resetCamera(0.25f/scene->currentObject->radius);
			cout << "Reset model transform." << endl;
			glutPostRedisplay();
		}
	}
	else if (strcmp(p, "T") == 0)
	{
		if (scene->currentObject != NULL)
		{
			// Add a translation to the transform; T <tx> <ty> <tz>:
			p = strtok(NULL, " ");
			float transX = atof(p);

			p = strtok(NULL, " ");
			float transY = atof(p);

			p = strtok(NULL, " ");
			float transZ = atof(p);

			scene->currentObject->transform->addTranslation(transX, transY, transZ);
			cout << "Added translation to model transform." << endl;
			glutPostRedisplay();
		}
	}
	else if (strcmp(p, "S") == 0)
	{
		if (scene->currentObject != NULL)
		{
			// Add a scale to the transform; S <sx> <sy> <sz>:
			p = strtok(NULL, " ");
			float scaleX = atof(p);

			p = strtok(NULL, " ");
			float scaleY = atof(p);

			p = strtok(NULL, " ");
			float scaleZ = atof(p);

			scene->currentObject->transform->addScale(scaleX, scaleY, scaleZ);
			cout << "Added scale to model transform." << endl;
			glutPostRedisplay();
		}
	}
	else if (strcmp(p, "R") == 0)
	{
		if (scene->currentObject != NULL)
		{
			// Add a rotation to the transform; R <theta> <ax> <ay> <az>:
			p = strtok(NULL, " ");
			float angle = atof(p);

			p = strtok(NULL, " ");
			float axisX = atof(p);

			p = strtok(NULL, " ");
			float axisY = atof(p);

			p = strtok(NULL, " ");
			float axisZ = atof(p);

			scene->currentObject->transform->addRotation(angle, axisX, axisY, axisZ);
			cout << "Added rotation to model transform." << endl;
			glutPostRedisplay();
		}
	}
	else if (strcmp(p, "V") == 0)
	{
		// transforms interpreted as being in viewer's local coordinate system
		cout << "Transformations now interpreted as being in viewer's local coordinate system." << endl;

	}
	else if (strcmp(p, "W") == 0)
	{
		// transforms interpreted as being in the world coordinate system
		cout << "Transformations now interpreted as being in the world coordinate system." << endl;
	}
}

void onKeyboardUp(unsigned char k, int x, int y)
{
	switch(k)
	{
		case 'w':
			MOVE_UP = false;
			break;
		case 'a':
			MOVE_LEFT = false;
			break;
		case 's':
			MOVE_DOWN = false;
			break;
		case 'd':
			MOVE_RIGHT = false;
			break;
	}
}

/* Use to implement any keyboard toggles/commands */
void onKeyboard(unsigned char k, int x, int y)
{
	switch(k)
	{
		case 'n':
			// Toggle face normals
			scene->currentObject->showFaceNormals = !scene->currentObject->showFaceNormals;

			if (scene->currentObject->showFaceNormals)
				cout << "Now displaying face normals." << endl;
			else
				cout << "Now hiding face normals." << endl;

			glutPostRedisplay();
			break;

		case 'm':
			// Toggle vertex normals
			scene->currentObject->showVertexNormals = !scene->currentObject->showVertexNormals;

			if (scene->currentObject->showVertexNormals)
				cout << "Now displaying vertex normals." << endl;
			else
				cout << "Now hiding vertex normals." << endl;

			glutPostRedisplay();
			break;

		case '1':
			// Set render mode to POINT_MODE
			scene->currentObject->setRenderMode(POINT_MODE);

			cout << "Now rendering object in point mode." << endl;

			glutPostRedisplay();
			break;

		case '2':
			// Set render mode to WIREFRAME_MODE
			scene->currentObject->setRenderMode(WIREFRAME_MODE);

			cout << "Now rendering object in wireframe mode." << endl;

			glutPostRedisplay();
			break;

		case '3':
			// Set render mode to SOLID_MODE
			scene->currentObject->setRenderMode(SOLID_MODE);

			cout << "Now rendering object in solid mode." << endl;

			glutPostRedisplay();
			break;

		case '4':
			// Set render mode to SHADED_MODE
			scene->currentObject->setRenderMode(SHADED_MODE);

			cout << "Now rendering object in shaded mode." << endl;

			glutPostRedisplay();
			break;

		case 'i':
			{
				char command[256];
	  			cout << "Command: ";
	  			std::cin.getline(command, 256);
	  			processCommand(command);
  			}
			break;

		case 'w':
			MOVE_UP = true;
			break;
		case 'a':
			MOVE_LEFT = true;
			break;
		case 's':
			MOVE_DOWN = true;
			break;
		case 'd':
			MOVE_RIGHT = true;
			break;

		default:
			break;
	}
}

void onMouseMotion(int x, int y)
{
	if (ORBIT_ENABLED)
	{
		// orbit camera
		camera->orbitCamera(mouseX - x, mouseY - y);
	}
	if (ZOOM_ENABLED)
	{
		// zoom camera
		camera->zoomCamera(y - mouseY);
	}
	if (PAN_ENABLED)
	{
		// pan camera
		camera->panCamera(mouseX - x, mouseY - y);
	}

	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

void onIdle()
{
	if (MOVE_RIGHT && !MOVE_LEFT)
	{
		scene->currentObject->transform->translation->x += 0.1f;
		glutPostRedisplay();
	}
	if (MOVE_LEFT && !MOVE_RIGHT)
	{
		scene->currentObject->transform->translation->x -= 0.1f;
		glutPostRedisplay();
	}
	if (MOVE_UP && !MOVE_DOWN)
	{
		scene->currentObject->transform->translation->y += 0.1f;
		glutPostRedisplay();
	}
	if (MOVE_DOWN && !MOVE_UP)
	{
		scene->currentObject->transform->translation->y -= 0.1f;
		glutPostRedisplay();
	}
}
void initializeScene(char* filepath)
{
	/* Initialize model*/
	model = new Model(filepath);
	TrimeshLoader loader;
	loader.loadOBJ(model);
	model->initializeModel();
	model->setRenderMode(SHADED_MODE);
	transform = new Transform(model);

	/* Initialize light */
	light = new Light();

	/* Initialize camera */
	camera = new Camera(0.20f/model->radius);

	/* Initialize scene */
	scene = new Scene(camera, model, light);

	cout << "Loaded scene and " << model->name << " model." << endl;
}

int main(int argc, char **argv)
{
	char* commandFile;
	char* pathToModelFile;

	if (argc == 1)
	{
		cout << "Invalid arguments. Include path to .obj file." << endl;
		return 0;
	}

	if (argc == 2)
	{
		pathToModelFile = argv[1];
	}
	else if (argc != 4)
	{
		cout << "Invalid number of arguments." << endl;
		return 0;
	}
	else
	{
		if (strcmp(argv[1], "-f") == 0)
		{
			commandFile = argv[2];
			pathToModelFile = argv[3];
		}
		else if (strcmp(argv[2], "-f") == 0)
		{
			commandFile = argv[3];
			pathToModelFile = argv[1];
		}
		else
		{
			cout << "Invalid arguments." << endl;
			return 0;
		}
	}

	/* Initialize window */ 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("File Loader/Viewer");

	/* Bind callbacks */
	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutMouseFunc(onMouse);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);
	glutIdleFunc(onIdle);

	initializeScene(pathToModelFile);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    if (commandFile != NULL)
    {
		std::string line;
   		std::ifstream infile(commandFile);

   		if (infile)
   		{
   			cout << "Loaded command file." << endl;
      		while (getline(infile, line))
      		{
				char *cstr = new char[line.length() + 1];
				strcpy(cstr, line.c_str());
				cout << "Processing command: " << line << endl;
      			processCommand(cstr);
      		}
   		}
		infile.close();
    }
    
	glutMainLoop();
}