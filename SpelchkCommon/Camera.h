/*
 * Camera.h
 *
 *  Created on: Dec 1, 2012
 *      Author: Hoanh Nguyen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Angel.h"

class Camera {
  private:
    int projectionType;

    // Projection transformation parameters
    GLfloat fovy;  // Field-of-view in Y direction angle (in degrees)
    GLfloat aspect;       // Viewport aspect ratio

    GLfloat left, right;
    GLfloat bottom, top;
    GLfloat zNear, zFar;

    int screenWidth, screenHeight;

    GLfloat xDepth, yDepth, zDepth;
    GLfloat xAngle, yAngle, zAngle;

    vec4 initialTranslationVector;
    vec4 translationVector;
    vec4 oldTranslationVector;

    mat4 modelViewMatrix;

    void calculateTranslationVector();

  public:
    Camera(vec4 initialTranslationVector);
    virtual ~Camera();

    mat4 getProjectionMatrix();
    mat4 getModelViewMatrix();
    vec4 getTranslationVector();

    // Displaces camera from its current location
    void moveCamera(float xDepth, float yDepth, float zDepth);

    // Rotate camera from its current orientation
    void rotateCamera(float xAngle, float yAngle, float zAngle);

    void setScreenSize(int width, int height);
    void setProjection(int projectionType);

    void reset();
};

#endif /* CAMERA_H_ */
