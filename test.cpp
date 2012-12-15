#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <string.h>

#include "include/Angel.h"
#include "SpelchkCommon/Camera.h"
#include "SpelchkCommon/Model.h"
#include "SpelchkCommon/KinectInator.h"
#include "SpelchkCommon/SceneGraph.h"

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

Model *floorModel;

Model *model;
Model *model2;

SceneGraph *sceneGraph1;
SceneGraph *sceneGraph2;
SceneGraph *sceneGraph3;
SceneGraph *sceneGraph4;
SceneGraph *sceneGraph5;

GLuint projection; // projection matrix uniform shader variable location
GLuint modelView;  // model-view matrix uniform shader variable location
GLuint modelOrientation;  // model-orientation matrix uniform shader variable location

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

  } else if (rightDown) {
    temp = (y - mouseDownY);
    mouseDownY = y;
    depth = (temp * translationFactor / screenHeight);

    camera.moveCamera(0.0, 0.0, depth);
  }
}

void loadTextures() {
  GLuint textures[2];
  glGenTextures(2, textures);

  int width, height;
  unsigned char* image;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  image = SOIL_load_image("OpenGL_Tutorial_Texture.jpg", &width, &height, 0, SOIL_LOAD_RGB );
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
  SOIL_free_image_data(image);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  image = SOIL_load_image("OpenGL_Tutorial_Texture2.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
}

void createModels(GLuint vPosition, GLuint vNormal, GLuint vTextureCoords, GLuint uTexture, GLuint uUseTexture, GLuint uMaterialAmbient, GLuint uMaterialDiffuse, GLuint uMaterialSpecular, GLuint uMaterialShininess) {

  loadTextures();
  GLfloat squareTexureCoords[2*6] = {
    0.0, 0.0,
    0.0, 1.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    1.0, 0.0,
  };

  std::vector<GLfloat> textureCoords;
  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 12; j++) {
      textureCoords.push_back(squareTexureCoords[j]);
    }
  }

  vec4 materialAmbient(0.0, 0.0, 0.0, 1.0);
  vec4 materialDiffuse(0.0, 0.8, 0.0, 1.0);
  vec4 materialSpecular(0.2, 0.2, 0.2, 1.0);
  float materialShininess = 5.0;

  vec4 floorPoints[4] = {
      vec4(-10.0, 0.0, -10.0, 1.0),
      vec4(-10.0, 0.0, 10.0, 1.0),
      vec4(10.0, 0.0, 10.0, 1.0),
      vec4(10.0, 0.0, -10.0, 1.0)
  };
  floorModel = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  floorModel->addPoint(floorPoints[0]);
  floorModel->addPoint(floorPoints[1]);
  floorModel->addPoint(floorPoints[2]);
  floorModel->addPoint(floorPoints[0]);
  floorModel->addPoint(floorPoints[2]);
  floorModel->addPoint(floorPoints[3]);
  floorModel->calculateNormals();
  floorModel->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  floorModel->upload();

  materialAmbient = vec4(0.0, 0.0, 0.0, 1.0);
  materialDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
  materialSpecular = vec4(0.2, 0.2, 0.2, 1.0);
  materialShininess = 5.0;

  model = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  //model->load_obj("suzanne.obj");
  cube(model);
  model->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  model->setTexture(0, textureCoords);
  model->upload();

  model2 = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  cube(model2);
  //tetrahedron(model2, 7);
  model2->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  model2->setTexture(1, textureCoords);
  model2->upload();

  sceneGraph1 = new SceneGraph(model, Translate(0.0, 0.0, 0.0), Translate(0.0, 0.5, 0.0));
  sceneGraph2 = new SceneGraph(model2, Translate(0.5, 0.5, 0.0), Translate(0.5, 0.5, 0.0));
  sceneGraph3 = new SceneGraph(model2, Translate(-0.5, 0.5, 0.0), Translate(-0.5, 0.5, 0.0));

  sceneGraph4 = new SceneGraph(model, Translate(0.5, 0.5, 0.0), Translate(-0.5, 0.5, 0.0));
  sceneGraph5 = new SceneGraph(model, Translate(-0.5, 0.5, 0.0), Translate(0.5, 0.5, 0.0));

  sceneGraph1->addSceneGraph(sceneGraph2);
  sceneGraph1->addSceneGraph(sceneGraph3);

  sceneGraph2->addSceneGraph(sceneGraph4);
  sceneGraph3->addSceneGraph(sceneGraph5);
}

void applyAnimation() {
  float time = glutGet(GLUT_ELAPSED_TIME);
  float location = sinf(time / 4000.0);
  mat4 rotation = RotateZ(location * 90.0);

  sceneGraph2->setApplyRotationMatrix(rotation);
  sceneGraph3->setApplyRotationMatrix(rotation);

  if(location >=0) {
    sceneGraph4->setPrepareRotationMatrix(Translate(0.5, 0.5, 0.0));
    sceneGraph4->setModelOffset(Translate(-0.5, 0.5, 0.0));

    sceneGraph5->setPrepareRotationMatrix(Translate(-0.5, 0.5, 0.0));
    sceneGraph5->setModelOffset(Translate(0.5, 0.5, 0.0));
  } else {
    sceneGraph4->setPrepareRotationMatrix(Translate(-0.5, 0.5, 0.0));
    sceneGraph4->setModelOffset(Translate(0.5, 0.5, 0.0));

    sceneGraph5->setPrepareRotationMatrix(Translate(0.5, 0.5, 0.0));
    sceneGraph5->setModelOffset(Translate(-0.5, 0.5, 0.0));
  }

  rotation = RotateZ(location * -90.0);
  sceneGraph4->setApplyRotationMatrix(rotation);
  sceneGraph5->setApplyRotationMatrix(rotation);
}

void drawSceneGraph(SceneGraph *sceneGraph, mat4 mo) {
  mo = mo * sceneGraph->getPrepareRotationMatrix() * sceneGraph->getApplyRotationMatrix() * sceneGraph->getModelOffset();
  glUniformMatrix4fv(modelOrientation, 1, GL_TRUE, mo);
  sceneGraph->getModel()->draw();

  for(unsigned int i = 0; i < sceneGraph->getChildren().size(); i++) {
    drawSceneGraph(sceneGraph->getChildren()[i], mo);
  }
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 p = camera.getProjectionMatrix();
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  mat4 mv = camera.getModelViewMatrix();
  glUniformMatrix4fv(modelView, 1, GL_TRUE, mv);

  mat4 mo = Translate(0.0, 0.0, 0.0);
  glUniformMatrix4fv(modelOrientation, 1, GL_TRUE, mo);

  floorModel->draw();

  applyAnimation();
  drawSceneGraph(sceneGraph1, Translate(0.0, 0.0, 0.0));

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

  point4 lightPosition2(0.0, 5.0, 0.0, 1.0);
  color4 lightDiffuse2(0.0, 0.0, 1.0, 1.0);
  color4 lightSpecular2(0.0, 0.0, 0.0, 1.0);

  glUniform4fv(glGetUniformLocation(program, "LightAmbient"), 1, lightAmbient);
  glUniform4fv(glGetUniformLocation(program, "LightDiffuse"), 1, lightDiffuse);
  glUniform4fv(glGetUniformLocation(program, "LightSpecular"), 1, lightSpecular);

  glUniform4fv(glGetUniformLocation(program, "LightDiffuse2"), 1, lightDiffuse2);
  glUniform4fv(glGetUniformLocation(program, "LightSpecular2"), 1, lightSpecular2);

  glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, lightPosition);
  glUniform4fv(glGetUniformLocation(program, "LightPosition2"), 1, lightPosition2);


  projection = glGetUniformLocation(program, "Projection");
  modelView = glGetUniformLocation(program, "ModelView");
  modelOrientation = glGetUniformLocation(program, "ModelOrientation");

  GLuint uMaterialAmbient = glGetUniformLocation(program, "MaterialAmbient");
  GLuint uMaterialDiffuse = glGetUniformLocation(program, "MaterialDiffuse");
  GLuint uMaterialSpecular = glGetUniformLocation(program, "MaterialSpecular");
  GLuint uMaterialShininess = glGetUniformLocation(program, "MaterialShininess");

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
