#include "Camera.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#ifndef M_PI
#define M_PI  3.14159
#endif
Camera::Camera()
{
	camX = 0.0f;
	camY = 0.0f;
	camZ = 0.0f;
    camYaw = 0.0f;
	camPitch = 0.0f;
	bool mouseIn = true;
    nearPlaneDepth = -5.0;
	farPlaneDepth = -100.0;
	viewAngleX = 0.55;
	viewAngleY = 0.4;
}

void Camera::lockCamera()
{
	if (camPitch > 90)
		camPitch = 90;

	if (camPitch < -90)
		camPitch = -90;

	if (camYaw < 0.0f)
		camYaw += 360.0f;

	if (camYaw > 360)
		camYaw -= 360.0f;
}


//moves the camera in w,s,a,d directions, the values are directly passed to gltranslatef
void Camera::moveCamera(float dist,float dir)
{
	float rad = (camYaw + dir) *M_PI / 180.0f;
	camX -= sin(rad)*dist;
	camZ -= cos(rad) * dist;
}


//move the camera using mouse
void Camera::moveCameraUp(float dist, float dir)
{
	float rad = (camPitch + dir) *M_PI / 180.0f;
	camY += sin(rad)*dist;
	
}

void Camera::control(float moveVel, float mouseVel, bool mi, SDL_Window *ptr_window)
{

	if (mi)
	{
		int midX = 320;
		int midY = 240;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpX, tmpY;
		SDL_GetMouseState(&tmpX, &tmpY);
		camYaw += mouseVel*(midX - tmpX);
		camPitch += mouseVel*(midY - tmpY);
		lockCamera();
		SDL_WarpMouseInWindow(ptr_window,midX,midY);
		glRotatef(-camPitch, 1.0, 0.0, 0.0);
		glRotatef(-camYaw, 0.0, 1.0, 0.0);
	}
}

void Camera:: updateCamera()
{
	glTranslatef(-camX, -camY, -camZ);        //move the camera
}
void Camera::drawFrustum()
{
	//Calculate spherical coordinates for the near plane
	float nearMidSphereCoord[3] = {nearPlaneDepth, camYaw * (M_PI / 180), camPitch * (M_PI / 180)};
	float nearTopRightSphereCoord[3] = {nearPlaneDepth, (nearMidSphereCoord[1] + viewAngleY), (nearMidSphereCoord[2] + viewAngleX)};
	float nearBottomRightSphereCoord[3] = {nearPlaneDepth, (nearMidSphereCoord[1] + viewAngleY), (nearMidSphereCoord[2] - viewAngleX)};
	float nearBottomLeftSphereCoord[3] = {nearPlaneDepth, (nearMidSphereCoord[1] - viewAngleY), (nearMidSphereCoord[2] - viewAngleX)};
	float nearTopLeftSphereCoord[3] = {nearPlaneDepth, (nearMidSphereCoord[1] - viewAngleY), (nearMidSphereCoord[2] + viewAngleX)};

	//Convert near plane spherical coordinates to cartesian coordinates
	float* nearTopRightCartCoord = sphericalToCartesianCoordinates(nearTopRightSphereCoord);
	float* nearBottomRightCartCoord = sphericalToCartesianCoordinates(nearBottomRightSphereCoord);
	float* nearBottomLeftCartCoord = sphericalToCartesianCoordinates(nearBottomLeftSphereCoord);
	float* nearTopLeftCartCoord = sphericalToCartesianCoordinates(nearTopLeftSphereCoord);

	//Add camera's position to near plane coordinates
	float* nearTopRight = addCamPosToCoord(nearTopRightCartCoord);
	float* nearBottomRight = addCamPosToCoord(nearBottomRightCartCoord);
	float* nearBottomLeft = addCamPosToCoord(nearBottomLeftCartCoord);
	float* nearTopLeft = addCamPosToCoord(nearTopLeftCartCoord);

	//Calculate spherical coordinates for the far plane
	float farMidSphereCoord[3] = {farPlaneDepth, camYaw, camPitch};
	float farTopRightSphereCoord[3] = {farPlaneDepth, (farMidSphereCoord[1] + viewAngleY), (farMidSphereCoord[2] + viewAngleX)};
	float farBottomRightSphereCoord[3] = {farPlaneDepth, (farMidSphereCoord[1] + viewAngleY), (farMidSphereCoord[2] - viewAngleX)};
	float farBottomLeftSphereCoord[3] = {farPlaneDepth, (farMidSphereCoord[1] - viewAngleY), (farMidSphereCoord[2] - viewAngleX)};
	float farTopLeftSphereCoord[3] = {farPlaneDepth, (farMidSphereCoord[1] - viewAngleY), (farMidSphereCoord[2] + viewAngleX)};

	//Convert far plane spherical coordinates to cartesian coordinates
	float* farTopRightCartCoord = sphericalToCartesianCoordinates(farTopRightSphereCoord);
	float* farBottomRightCartCoord = sphericalToCartesianCoordinates(farBottomRightSphereCoord);
	float* farBottomLeftCartCoord = sphericalToCartesianCoordinates(farBottomLeftSphereCoord);
	float* farTopLeftCartCoord = sphericalToCartesianCoordinates(farTopLeftSphereCoord);

	//Add camera's position to far plane coordinates
	float* farTopRight = addCamPosToCoord(farTopRightCartCoord);
	float* farBottomRight = addCamPosToCoord(farBottomRightCartCoord);
	float* farBottomLeft = addCamPosToCoord(farBottomLeftCartCoord);
	float* farTopLeft = addCamPosToCoord(farTopLeftCartCoord);

	//Begin OpenGL drawing and change drawing color to red
	glLineWidth(1.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);

	//Draw lines outlining the near and far planes
	glVertex3f(nearTopRight[0], nearTopRight[1], nearTopRight[2]);
	glVertex3f(farTopRight[0], farTopRight[1], farTopRight[2]);
	glVertex3f(farBottomLeft[0], farBottomLeft[1], farBottomLeft[2]);
	glVertex3f(nearBottomLeft[0], nearBottomLeft[1], nearBottomLeft[2]);
	glVertex3f(nearTopRight[0], nearTopRight[1], nearTopRight[2]);
	glVertex3f(nearBottomRight[0], nearBottomRight[1], nearBottomRight[2]);
	glVertex3f(farBottomRight[0], farBottomRight[1], farBottomRight[2]);
	glVertex3f(farTopRight[0], farTopRight[1], farTopRight[2]);
	glVertex3f(farTopLeft[0], farTopLeft[1], farTopLeft[2]);
	glVertex3f(farBottomLeft[0], farBottomLeft[1], farBottomLeft[2]);
	glVertex3f(farBottomRight[0], farBottomRight[1], farBottomRight[2]);
	glVertex3f(farTopLeft[0], farTopLeft[1], farTopLeft[2]);
	glVertex3f(nearTopLeft[0], nearTopLeft[1], nearTopLeft[2]);
	glVertex3f(nearBottomRight[0], nearBottomRight[1], nearBottomRight[2]);
	glVertex3f(nearBottomLeft[0], nearBottomLeft[1], nearBottomLeft[2]);
	glVertex3f(nearTopLeft[0], nearTopLeft[1], nearTopLeft[2]);
	glVertex3f(nearTopRight[0], nearTopRight[1], nearTopRight[2]);
	glVertex3f(nearBottomRight[0], nearBottomRight[1], nearBottomRight[2]);

	//End OpenGL drawing and change draw color back to white
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}

float* Camera::sphericalToCartesianCoordinates(float* toConvert)
{
	toConvert[2] = (M_PI / 2) - toConvert[2];

	float toReturn[3];

	toReturn[0] = toConvert[0] * cos(toConvert[1]) * sin(toConvert[2]);
	toReturn[1] = toConvert[0] * sin(toConvert[1]) * sin(toConvert[2]);
	toReturn[2] = toConvert[0] * cos(toConvert[2]);

	return toReturn;
}

float* Camera::addCamPosToCoord(float* toAddTo)
{
	float toReturn[3];

	toReturn[0] = toAddTo[0] + camX;
	toReturn[1] = toAddTo[1] + camY;
	toReturn[2] = toAddTo[2] + camZ;

	return toReturn;
}