/*
 * Camera.cpp
 *
 *  Created on: Dec 1, 2012
 *      Author: Hoanh Nguyen
 */

#include "Camera.h"

Camera::Camera(vec4 _initialTranslationVector) {
  timeRef = 0;
  initialTranslationVector = _initialTranslationVector;
  reset();
}

Camera::~Camera() {
}

void Camera::reset() {
  projectionType = 0;
  fovy = 45;

  left = -1.0;
  right = 1.0;
  bottom = -1.0;
  top = 1.0;
  zNear = 0.1;
  zFar = 20.0;

  xDepth = 0.0;
  yDepth = 0.0;
  zDepth = 0.0;

  xAngle = 0.0;
  yAngle = 0.0;
  zAngle = 0.0;

  xHead = 0.0;
  yHead = 0.0;
  zHead = 0.0;

  xHeadAngle = 0.0;
  yHeadAngle = 0.0;
  zHeadAngle = 0.0;

  oldTranslationVector = initialTranslationVector;
  translationVector = oldTranslationVector;
  calculateTranslationVector();
}

mat4 Camera::getProjectionMatrix() {
  switch (projectionType) {
    case 1:
      return Ortho(left, right, bottom, top, zNear, zFar);
    case 2:
      return Frustum(left, right, bottom, top, zNear, zFar);
    default:
      return Perspective(fovy, aspect, zNear, zFar);
  }
}

mat4 Camera::getModelViewMatrix() {
  modelViewMatrix = RotateX(xAngle) * RotateY(yAngle) * RotateZ(zAngle) * * RotateX(xHeadAngle) * RotateY(yHeadAngle) * Translate(translationVector) * RotateX(-xHeadAngle) * RotateY(-yHeadAngle);
  return modelViewMatrix;
}

vec4 Camera::getTranslationVector() {
  return translationVector;
}

void Camera::calculateTranslationVector() {
  // calculate displacement based on current angles (note rotations done in reverse order and negative to move model in opposite direction)

  vec4 calculateDisplacement =  RotateZ(-zAngle) * RotateY(-yAngle) * RotateX(-xAngle) * vec4(xDepth, yDepth, -zDepth, 0.0);
  translationVector = (oldTranslationVector + calculateDisplacement);
}

void Camera::moveCamera(float _xDepth, float _yDepth, float _zDepth) {
  oldTranslationVector = translationVector;

  xDepth = _xDepth;
  yDepth = _yDepth;
  zDepth = _zDepth;

  calculateTranslationVector();
}

void Camera::rotateCamera(float _xAngle, float _yAngle, float _zAngle) {
  xAngle += _xAngle;
  yAngle += _yAngle;
  zAngle += _zAngle;

  // Keep camera from flipping over
  if(xAngle > 90.0) {
    xAngle = 90.0;
  } else if(xAngle < -90) {
    xAngle = -90;
  }

  calculateTranslationVector();
}

void Camera::setScreenSize(int width, int height) {
  glViewport(0, 0, width, height);
  screenWidth = width;
  screenHeight = height;
  aspect = GLfloat(width) / height;
}

void Camera::setProjection(int _projectionType) {
  projectionType = _projectionType;
}

void Camera::setLightMovementRef(GLuint ref)
{
	timeRef = ref;

}

void Camera::setLightMovementTime(float elapsed)
{
	if (timeRef != 0)
	  glUniform1f(timeRef, elapsed);
}
void Camera::headMovement(int usernum, double x, double y, double z)
{
	//mm to meters and cast to float

	calculateTranslationVector();
	getModelViewMatrix();

	vec4 originCentric = modelViewMatrix * vec4(x/1000.0,y/1000.0,z/1000.0,1.0);
	if (originCentric.z != 0)
	{
		float ysin = originCentric.x/originCentric.z;
		float xcos = originCentric.y/originCentric.z;
		if (ysin > 1)
			ysin = 1;
		else if (ysin < -1)
			ysin = -1;
		if (xcos > 1)
			xcos = 1;
		else if (xcos < -1)
			xcos = -1;
		yHeadAngle = -atan(ysin);
		xHeadAngle = -atan(xcos);
	}

	moveCamera(xHead-(float)(x/1000.0), yHead -(float)(y/1000.0), zHead +(float)(z/1000.0));
	xHead = (float)(x/1000.0);
	yHead = (float)(y/1000.0);
	zHead = -(float)(z/1000.0);

	printf("%d - (%6.2f, %6.2f, %6.2f) ==> (%6.2f,%6.2f,%6.2f), yaw=%f, pitch=%f\n", usernum,x,y,z,xHead, yHead, zHead, xHeadAngle, yHeadAngle);
}
