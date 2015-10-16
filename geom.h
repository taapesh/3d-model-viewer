/*
Name: Arya Mohades
UTEID: am52293
UTCS ID: am611
*/

#ifndef __GEOM_H__
#define __GEOM_H__

#include <limits>
#include <cmath>
#include <stdlib.h>
using namespace std;

#define PI 3.14159

/* Rendering modes */
const int POINT_MODE = 1;
const int WIREFRAME_MODE = 2;
const int SOLID_MODE = 3;
const int SHADED_MODE = 4;

/* Camera control variables */
const float DELTA_ANGLE = 0.2f;
const float DELTA_PAN = 0.005f;
const float DELTA_ZOOM = 0.0015f;

/* Forward declarations */
class Transform;

/* Vector3 class */
class Vector3
{
	public:
		float x, y, z;

		/* Vector constructor */
		Vector3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		float magnitude()
		{
			return sqrt(x*x + y*y + z*z);
		}

		void update(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}

		void convertToUnit()
		{
			float mag = magnitude();
			x = x/mag;
			y = y/mag;
			z = z/mag;
		}
};

/* Point class; represents vertex */
class Point
{
    public:
        float x,y,z;
        Vector3* n;

        /* Point/Vertex constructor */
        Point(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
        {
        	x = _x;
        	y = _y;
        	z = _z;
        	n = new Vector3();
        }

        void updateNormal(Vector3* _n)
        {
        	n->x += (_n->x);
        	n->y += (_n->y);
        	n->z += (_n->z);
        }
};

/* Face class */
class Face
{
	public:
		int u,v,w;
		Vector3* n;

		/* Face constructor */
		Face(int _u, int _v, int _w)
		{
			u = _u;
			v = _v;
			w = _w;
		}
};

/* Model class */
class Model
{
	public:
		const char* filepath;
		char name[256];

		Point* centerPos;
		vector<Point*> vertices;
		vector<Face*> faces;
		float radius;
		float minX, minY, minZ;
		float maxX, maxY, maxZ; 

		int renderMode;
		bool showFaceNormals;
		bool showVertexNormals;

		Transform* transform;

		/* Model constructor */
		Model(const char* _filepath)
		{
			filepath = _filepath;
			minX = minY = minZ = std::numeric_limits<float>::max();
			maxX = maxY = maxZ = std::numeric_limits<float>::min();

			/* Extract object name from filepath */
			int len = strlen(_filepath);
			int nameLength = len - 11;
			strcpy(name, _filepath);
			name[len-4] = 0;
			int currPos = 7;
			for(int i = 0; i < nameLength; ++i)
			{
				name[i] = name[currPos];
				currPos++;
			}
			name[nameLength] = 0;
		}

		/* Initialize model properties */
		void initializeModel()
		{
			setCenterAndRadius();
			computeNormals();
			showFaceNormals = false;
			showVertexNormals = false;
		}

		/* Set rendering mode of model */
		void setRenderMode(int mode)
		{
			renderMode = mode;
		}

		/* Main model display function */
		void display()
		{
			switch(renderMode)
			{
				case POINT_MODE:
					displayPointMode();
					break;
				case WIREFRAME_MODE:
					displayWireframeMode();
					break;
				case SOLID_MODE:
					displaySolidMode();
					break;
				case SHADED_MODE:
					displayShadedMode();
					break;
			}

			glBegin(GL_LINES);

			/* Display face normals if enabled */
			if (showFaceNormals)
			{
				for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
				{
					Face* f = *it;
					float x = (vertices[f->u]->x + vertices[f->v]->x + vertices[f->w]->x)/3.0f;
					float y = (vertices[f->u]->y + vertices[f->v]->y + vertices[f->w]->y)/3.0f;
					float z = (vertices[f->u]->z + vertices[f->v]->z + vertices[f->w]->z)/3.0f;

					glVertex3f(x, y, z);
					glVertex3f(f->n->x + x, f->n->y + y, f->n->z + z);
				}
			}


			/* Display vertex normals if enabled */
			if (showVertexNormals)
			{
				for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
				{
					Point* v = *it;
					glVertex3f(v->x, v->y, v->z);
					glVertex3f(v->n->x + v->x, v->n->y + v->y, v->n->z + v->z);
				}
			}

			glEnd();
		}

		/* Display model in point mode */
		void displayPointMode()
		{
			glBegin(GL_POINTS);

			for(std::vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
			{
				glVertex3f((*it)->x, (*it)->y, (*it)->z);
			}

			glEnd();
		}

		/* Display model in wireframe mode */
		void displayWireframeMode()
		{
			glBegin(GL_LINES);

			for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				Face* f = *it;
				glVertex3f(vertices[f->u]->x, vertices[f->u]->y, vertices[f->u]->z);
				glVertex3f(vertices[f->v]->x, vertices[f->v]->y, vertices[f->v]->z);

				glVertex3f(vertices[f->v]->x, vertices[f->v]->y, vertices[f->v]->z);
				glVertex3f(vertices[f->w]->x, vertices[f->w]->y, vertices[f->w]->z);

				glVertex3f(vertices[f->w]->x, vertices[f->w]->y, vertices[f->w]->z);
				glVertex3f(vertices[f->u]->x, vertices[f->u]->y, vertices[f->u]->z);
			}

			glEnd();
		}

		/* Display model in solid mode */
		void displaySolidMode()
		{
			glBegin(GL_TRIANGLES);

			for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				Face* f = *it;
				glVertex3f(vertices[f->u]->x, vertices[f->u]->y, vertices[f->u]->z);
				glVertex3f(vertices[f->v]->x, vertices[f->v]->y, vertices[f->v]->z);
				glVertex3f(vertices[f->w]->x, vertices[f->w]->y, vertices[f->w]->z);
			}

			glEnd();
		}

		/* Dislpay model in shaded mode */
		void displayShadedMode()
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_COLOR_MATERIAL);

			glBegin(GL_TRIANGLES);
			for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				Face* f = *it;

				// Submit a normal with each vertex
				glNormal3f(vertices[f->u]->n->x, vertices[f->u]->n->y, vertices[f->u]->n->z);
				glVertex3f(vertices[f->u]->x, vertices[f->u]->y, vertices[f->u]->z);

				glNormal3f(vertices[f->v]->n->x, vertices[f->v]->n->y, vertices[f->v]->n->z);
				glVertex3f(vertices[f->v]->x, vertices[f->v]->y, vertices[f->v]->z);

				glNormal3f(vertices[f->w]->n->x, vertices[f->w]->n->y, vertices[f->w]->n->z);
				glVertex3f(vertices[f->w]->x, vertices[f->w]->y, vertices[f->w]->z);
			}

			glEnd();
			glDisable(GL_LIGHTING);
		}

		/* Compute vertex and face normals */
		void computeNormals()
		{
			for(vector<Face*>::iterator it = faces.begin(); it != faces.end(); ++it)
			{
				Face* f = *it;
				Point* v1 = vertices[f->u];
				Point* v2 = vertices[f->v];
				Point* v3 = vertices[f->w];

				Vector3* va = new Vector3(
					v3->x - v1->x,
					v3->y - v1->y,
					v3->z - v1->z
				);

				Vector3* vb = new Vector3(
					v2->x - v1->x,
					v2->y - v1->y,
					v2->z - v1->z
				);

				Vector3* c = new Vector3(
					va->y * vb->z - va->z * vb->y,
					va->z * vb->x - va->x * vb->z,
					va->x * vb->y - va->y * vb->x
				);

				float mag = c->magnitude();
				f->n = new Vector3(c->x/mag, c->y/mag, c->z/mag);

				v1->updateNormal(f->n);
				v2->updateNormal(f->n);
				v3->updateNormal(f->n);
			}

			for(vector<Point*>::iterator it = vertices.begin(); it != vertices.end(); ++it)
			{
				(*it)->n->convertToUnit();
			}
		}

		/* Use min and max x,y,z to compute model center and radius */
		void setCenterAndRadius()
		{
			// Compute center point of the model
			centerPos = new Point(
				(maxX + minX)/2.0f,
				(maxY + minY)/2.0f,
				(maxZ + minZ)/2.0f
			);

			// Compute x, y, and z dimensions of the model
			// Halve values, take the largest to be the model's radius
			float x = (maxX - minX)/2.0f;
			float y = (maxY - minY)/2.0f;
			float z = (maxZ - minZ)/2.0f;

			if (x > y && x > z)
				radius = x;
			if (y > x && y > z)
				radius = y;
			else
				radius = z;

			// Do not allow negative radius
			if (radius < 0.0f)
				radius = -radius;
		}

		/* Add a vertex to vertex list */
		void addVertex(float vertexData[])
		{
			vertices.push_back(new Point(vertexData[0], vertexData[1], vertexData[2]));

			if (vertexData[0] < minX)
				minX = vertexData[0];
			if (vertexData[1] < minY)
				minY = vertexData[1];
			if (vertexData[2] < minZ)
				minZ = vertexData[2];

			if (vertexData[0] > maxX)
				maxX = vertexData[0];
			if (vertexData[1] > maxY)
				maxY = vertexData[1];
			if (vertexData[2] > maxZ)
				maxZ = vertexData[2];
		}

		/* Add a face to face list */
		void addFace(int faceData[])
		{
			faces.push_back(new Face(faceData[0], faceData[1], faceData[2]));
		}
};

/* Transform class */
class Transform
{
	public:
		Vector3* centerPos;
		Vector3* translation;
		Vector3* scale;
		float angle;
		Model* object;

		/* Transform constructor */
		Transform(Model* obj)
		{
			object = obj;
			resetTransform();
			addTranslation(-object->centerPos->x, -object->centerPos->y, -object->centerPos->z);
			object->transform = this;
		}

		/* Reset transform to original state */
		void resetTransform()
		{
			centerPos = new Vector3();
			translation = new Vector3();
			scale = new Vector3(1.0f, 1.0f, 1.0f);
			angle = 0.0f;
		}

		/* Display the model with any transformations added */
		void updateTransform()
		{
			glPushMatrix();
			glTranslatef(translation->x, translation->y, translation->z);
			glScalef(scale->x, scale->y, scale->z);
			glRotatef(angle, centerPos->x, centerPos->y, centerPos->z);

			// Display object with transformations
			object->display();

			glPopMatrix();
		}

		/* Add a translation to the model transform */
		void addTranslation(float transX, float transY, float transZ)
		{
			translation->update(transX, transY, transZ);
		}

		/* Add a scale to the model transform */
		void addScale(float scaleX, float scaleY, float scaleZ)
		{
			scale->update(scaleX, scaleY, scaleZ);
		}

		/* Add a rotation to the model transform */
		void addRotation(float _angle, float axisX, float axisY, float axisZ)
		{
			angle = _angle;
			centerPos->update(axisX, axisY, axisZ);
		}
};

/* Camera class */
class Camera
{
	public:
		Vector3* eyePos;
		Vector3* centerPos;
		Vector3* upVector;
		float zoomAmount;
		float polarAngle;
		float azimuthalAngle;
		float x, y;

		/* Camera constructor */
		Camera(float scale = 1.0f)
		{
			resetCamera(scale);
		}

		/* Update the camera to look at the correct position with correct orientation */
		void updateCamera()
		{
			computeUpVector();

			gluLookAt(
				eyePos->x, eyePos->y, eyePos->z,
				centerPos->x, centerPos->y, centerPos->z,
				upVector->x, upVector->y, upVector->z
			);

			glScalef(zoomAmount, zoomAmount, zoomAmount);
		}

		/* Reset camera to original state */
		void resetCamera(float scale)
		{
			eyePos = new Vector3(0.0f, 0.0f, -1.0f);
			centerPos = new Vector3(0.0f, 0.0f, 0.0f);
			upVector = new Vector3(0.0f, 1.0f, 0.0f);

			polarAngle = 180.0f;
			azimuthalAngle = 0.0f;

			zoomAmount = scale;
		}

		/* Orbit the camera around point of interest */
		void orbitCamera(int _x, int _y)
		{
			x = _x;
			y = _y;

			polarAngle = fmod(polarAngle + x * DELTA_ANGLE, 360.0f);
			azimuthalAngle = fmod(azimuthalAngle + y * DELTA_ANGLE, 180.0f);

			float polarRad = degToRadians(polarAngle);
			float azimuthalRad = degToRadians(azimuthalAngle);

			float x = centerPos->x + cosf(azimuthalRad) * sinf(polarRad);
			float y = centerPos->y + sinf(azimuthalRad) * sinf(polarRad);
			float z = centerPos->z + cosf(polarRad);

			eyePos->update(x, y, z);
		}

		/* Zoom camera in and out of point of interest */
		void zoomCamera(int x)
		{
			zoomAmount -= x * DELTA_ZOOM;

			/* Do not allow negative zoom amount */
			if (zoomAmount <= 0.001f)
				zoomAmount = 0.001f;
		}
		
		/* Pan camera, changing the point of interest */
		void panCamera(int x, int y)
		{
			centerPos->update(
				centerPos->x - x * DELTA_PAN,
				centerPos->y - y * DELTA_PAN,
				centerPos->z
			);
		}

		/* Compute the current up vector */
		void computeUpVector()
		{
			float mag = eyePos->magnitude();
			Vector3* v = new Vector3(eyePos->x/mag, eyePos->y/mag, eyePos->z/mag);
			Vector3* up = new Vector3(0.0f, 1.0f, 0.0f);

			Vector3* s = new Vector3(
				v->y * up->z - v->z * up->y,
				v->z * up->x - v->x * up->z,
				v->x * up->y - v->y * up->x
			);

			upVector->update(
				s->y * v->z - s->z * v->y,
				s->z * v->x - s->x * v->z,
				s->x * v->y - s->y * v->x
			);
		}
		
		/* Convert degrees to radians */
		float degToRadians(float deg)
		{
			return (deg*PI)/180.0f;
		}
};

/* Light class */
class Light
{
	public:
		/* Light constructor */
		Light()
		{
			/* Initialize basic light */
			const GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
			const GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
			const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			const GLfloat light_position[] = { 1.0, -5.0, 1.0, 0.0 };
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		}
};

/* Scene class */
class Scene
{
	public:
		vector<Model*> OBJ_STACK;
		Camera* camera;
		Light* light;
		Model* currentObject;
		
		/* Scene constructor */
		Scene(Camera* c, Model* m, Light* l)
		{
			camera = c;
			light = l;
			currentObject = m;
			OBJ_STACK.push_back(m);
		}

		/* Update the scene camera and object */
		void updateScene()
		{
			camera->updateCamera();
			if (currentObject)
			{
				currentObject->transform->updateTransform();
			}
		}

		/* Add a model to the scene */
		void addModel(Model* m)
		{
			OBJ_STACK.push_back(m);
			currentObject = m;
			camera->resetCamera(0.25f/currentObject->radius);
		}

		/* Delete the current scene model */
		void deleteCurrentObject()
		{
			if (!OBJ_STACK.empty())
			{
				// delete current object
				OBJ_STACK.pop_back();

				if (!OBJ_STACK.empty())
				{
					// set current object to be the previous object
					currentObject = OBJ_STACK.back();
					camera->resetCamera(0.25f/currentObject->radius);
				}
				else
				{
					currentObject = NULL;
				}
			}
		}
};

#endif