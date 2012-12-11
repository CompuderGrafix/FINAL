#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <string.h>

#include "include/Angel.h"
#include "SpelchkCommon/Camera.h"
#include "SpelchkCommon/Model.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

Model *model;
Model *model2;

GLuint modelView;  // model-view matrix uniform shader variable location
GLuint projection; // projection matrix uniform shader variable location

int screenHeight, screenWidth;

bool debug = false;

Camera camera;

//----------------------------------------------------------------------------

void idle(void) {
  glutPostRedisplay();
}

void reshape(int width, int height) {
  camera.setScreenSize(width, height);
  screenHeight = height;
  screenWidth = width;
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 033: // Escape Key
    case 'q':
    case 'Q':
      exit(EXIT_SUCCESS);
      break;
    case 'w': // move up
      camera.moveCamera(0.0, -0.1, 0.0);
      break;
    case 's': // move down
      camera.moveCamera(0.0, 0.1, 0.0);
      break;
    case 'a': // move left
      camera.moveCamera(0.1, 0.0, 0.0);
      break;
    case 'd': // move right
      camera.moveCamera(-0.1, 0.0, 0.0);
      break;
    case ' ': // reset values to their defaults
      camera.reset();

      if (debug)
        printf("x = %f, y = %f, z = %f\n", camera.getTranslationVector().x, camera.getTranslationVector().y, camera.getTranslationVector().z);

      break;
  }
}

bool leftDown = false;
bool rightDown = false;

int mouseDownX, mouseDownY;

void mouse(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        leftDown = true;
        mouseDownX = x;
        mouseDownY = y;
        break;
      case GLUT_MIDDLE_BUTTON:
        break;
      case GLUT_RIGHT_BUTTON:
        rightDown = true;
        mouseDownX = x;
        mouseDownY = y;
        break;
    }
  } else if (state == GLUT_UP) {
    switch (button) {
      case GLUT_LEFT_BUTTON:
        leftDown = false;
        break;
      case GLUT_MIDDLE_BUTTON:
        break;
      case GLUT_RIGHT_BUTTON:
        rightDown = false;
        break;
    }
  }
}

float rotationFactor = 45.0;
float translationFactor = 5.0;
void motion(int x, int y) {

  float xAngle = 0, yAngle = 0;
  float temp;
  float depth;

  if (leftDown) {
    temp = (x - mouseDownX);
    mouseDownX = x;
    yAngle = (temp * rotationFactor / screenWidth);

    temp = (y - mouseDownY);
    mouseDownY = y;
    xAngle = (temp * rotationFactor / screenHeight);

    camera.rotateCamera(xAngle, yAngle, 0.0);

    if (debug)
      printf("yAngle = %f, xAngle = %f\n", yAngle, xAngle);

  } else if (rightDown) {
    temp = (y - mouseDownY);
    mouseDownY = y;
    depth = (temp * translationFactor / screenHeight);

    camera.moveCamera(0.0, 0.0, depth);

    if (debug)
      printf("x = %f, y = %f, z = %f, yAngle = %f, xAngle = %f\n", camera.getTranslationVector().x, camera.getTranslationVector().y, camera.getTranslationVector().z, yAngle, xAngle);
  }
}

void createModels(GLuint vPosition, GLuint vNormal, GLuint vTextureCoords, GLuint uTexture, GLuint uUseTexture, GLuint uMaterialAmbient, GLuint uMaterialDiffuse, GLuint uMaterialSpecular, GLuint uMaterialShininess) {

  GLfloat cube_texcoords[2*6] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0,
  };

  vec4 materialAmbient(0.0, 0.0, 0.0, 1.0);
  vec4 materialDiffuse(0.8, 0.8, 0.8, 1.0);
  vec4 materialSpecular(0.2, 0.2, 0.2, 1.0);
  float materialShininess = 5.0;

  std::vector<GLfloat> textureCoords;
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 12; j++) {
      textureCoords.push_back(cube_texcoords[j]);
    }
  }

  GLuint texture_id = SOIL_load_OGL_texture(
      "OpenGL_Tutorial_Texture.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, // target
         0,  // level, 0 = base, no minimap,
         GL_RGB, // internalformat
         256,  // width
         256,  // height
         0,  // border, always 0 in OpenGL ES
         GL_RGB,  // format
         GL_UNSIGNED_BYTE, // type
         0);

  printf("texture_id %d\n", texture_id);
  model = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  model->load_obj("suzanne.obj");
  //cube(model);
  model->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  //model->setTexture(texture_id, textureCoords);
  model->upload();

  texture_id = SOIL_load_OGL_texture(
      "OpenGL_Tutorial_Texture2.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y);

  /*
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, // target
         0,  // level, 0 = base, no minimap,
         GL_RGB, // internalformat
         256,  // width
         256,  // height
         0,  // border, always 0 in OpenGL ES
         GL_RGB,  // format
         GL_UNSIGNED_BYTE, // type
         0);
         */
  printf("texture_id %d\n", texture_id);
  //model2 = new Model(vPosition, vNormal, vColor);
  //tetrahedron(model2, 7);
  //model2->upload();
}

void display(void) {
  // clear context variables & draw triangle starting at 0

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 mv = camera.getModelViewMatrix();
  glUniformMatrix4fv(modelView, 1, GL_TRUE, mv);

  mat4 p = camera.getProjectionMatrix();
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  model->draw();

  //mv = mv * Translate(-3.0, 0.0, 0.0);
  //glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
  //model2->draw();

  glutSwapBuffers();
}

void init(void) {

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vProjectionWithMaterialAndLightShader.glsl", "fcolorshader.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  GLuint vNormal = glGetAttribLocation(program, "vNormal");
  GLuint vTextureCoords = glGetAttribLocation(program, "vTextureCoords");

  GLuint uTexture = glGetUniformLocation(program, "mytexture");
  GLuint uUseTexture = glGetUniformLocation(program, "UseTexture");

  // Initialize shader lighting parameters
  point4 lightPosition(0.0, 0.0, 2.0, 1.0);
  color4 lightAmbient(1.0, 1.0, 1.0, 1.0);
  color4 lightDiffuse(1.0, 1.0, 1.0, 1.0);
  color4 lightSpecular(0.0, 0.0, 0.0, 1.0);

  point4 lightPosition2(0.0, -5.0, 0.0, 1.0);
  color4 lightDiffuse2(1.0, 0.0, 0.0, 1.0);
  color4 lightSpecular2(0.0, 0.0, 0.0, 1.0);

  glUniform4fv(glGetUniformLocation(program, "LightAmbient"), 1, lightAmbient);
  glUniform4fv(glGetUniformLocation(program, "LightDiffuse"), 1, lightDiffuse);
  glUniform4fv(glGetUniformLocation(program, "LightSpecular"), 1, lightSpecular);

  glUniform4fv(glGetUniformLocation(program, "LightDiffuse2"), 1, lightDiffuse2);
  glUniform4fv(glGetUniformLocation(program, "LightSpecular2"), 1, lightSpecular2);

  glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, lightPosition);
  glUniform4fv(glGetUniformLocation(program, "LightPosition2"), 1, lightPosition2);

  modelView = glGetUniformLocation(program, "ModelView");
  projection = glGetUniformLocation(program, "Projection");

  GLuint uMaterialAmbient = glGetUniformLocation(program, "MaterialAmbient");
  GLuint uMaterialDiffuse = glGetUniformLocation(program, "MaterialDiffuse");
  GLuint uMaterialSpecular = glGetUniformLocation(program, "MaterialSpecular");
  GLuint uMaterialShininess = glGetUniformLocation(program, "MaterialShininess");

  printf("vPosition %d, vNormal %d, vTextureCoords %d, uTexture %d, uUseTexture %d, uMaterialAmbient %d, uMaterialDiffuse %d, uMaterialSpecular %d, uMaterialShininess %d\n",
      vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createModels(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);

  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1, 0.1, 0.1, 1.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // set rendering context
  glutInitWindowSize(512, 512);
  glutCreateWindow("Project"); // title

  glewInit(); // set OpenGL state and initialize shaders
  init();

  glutDisplayFunc(display); // register callback w/Window System
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();

  return 0;
}
