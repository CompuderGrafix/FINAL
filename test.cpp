#include <cmath>
#include <iostream>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <string.h>

#include "SpelchkCommon/KinectInator.h"
#include "include/Angel.h"
#include "SpelchkCommon/Camera.h"
#include "SpelchkCommon/Model.h"
#include "SpelchkCommon/SceneGraph.h"


TiemSpelchk:: Lurn2SpielNub virtualWindow;

GLuint program;

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

Model *floorModel;

SceneGraph *boxTowerSceneGraph1;
SceneGraph *boxTowerSceneGraph2;
SceneGraph *stickFigureSceneGraph1;
SceneGraph *sphereSceneGraph;

GLuint projection; // projection matrix uniform shader variable location
GLuint modelView;  // model-view matrix uniform shader variable location
GLuint modelOrientation;  // model-orientation matrix uniform shader variable location

int numberOfLights = 5;
GLuint LightDiffuse;
GLfloat lightPosition[] = {
    0.0, 0.0, 1.0, 1.0,
    -5.65, 3.0, 5.65, 1.0,
    -5.65, 3.0, -5.65, 1.0,
    5.65, 3.0, -5.65, 1.0,
    5.65, 3.0, 5.65, 1.0
};

GLfloat lightDiffuse[] = {
    1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0
};

GLfloat lightSpecular[] = {
    0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 1.0
};

int screenHeight, screenWidth;

bool debug = false;

Camera camera(vec4(0.0, -2.0, -10.0, 0.0));
//Camera camera(vec4(0.0, 0.0, 0.0, 0.0));

//----------------------------------------------------------------------------

void idle(void) {
  glutPostRedisplay();
}

void reshape(int width, int height) {
  camera.setScreenSize(width, height);
  screenHeight = height;
  screenWidth = width;
}

bool light = true;
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 033: // Escape Key
    case 'q':
    case 'Q':
      virtualWindow.Shutdown();
      exit(EXIT_SUCCESS);
      break;
    case 'w': // move up
      camera.moveCamera(0.0, -0.2, 0.0);
      break;
    case 's': // move down
      camera.moveCamera(0.0, 0.2, 0.0);
      break;
    case 'a': // move left
      camera.moveCamera(0.2, 0.0, 0.0);
      break;
    case 'd': // move right
      camera.moveCamera(-0.2, 0.0, 0.0);
      break;
    case 'z': //move back
      camera.moveCamera(0.0,0.0,-0.2);
      break;
    case 'x': //move forward
      camera.moveCamera(0.0,0.0,0.2);
      break;
    case 'l': // move right
      if(light) {
        lightDiffuse[0] = 0.0;
        lightDiffuse[1] = 0.0;
        lightDiffuse[2] = 0.0;
      } else {
        lightDiffuse[0] = 1.0;
        lightDiffuse[1] = 1.0;
        lightDiffuse[2] = 1.0;
      }

      glUniform4fv(LightDiffuse, numberOfLights, lightDiffuse);

      light = !light;
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
  image = SOIL_load_image("crate.jpg", &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
}

void createModels(GLuint vPosition, GLuint vNormal, GLuint vTextureCoords, GLuint uTexture, GLuint uUseTexture, GLuint uModulate, GLuint uTime, GLuint uMaterialAmbient, GLuint uMaterialDiffuse, GLuint uMaterialSpecular, GLuint uMaterialShininess) {

  SceneGraph *tempSceneGraph1;
  SceneGraph *tempSceneGraph2;
  SceneGraph *tempSceneGraph3;

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
  vec4 materialDiffuse(0.3, 0.3, 0.3, 1.0);
  vec4 materialSpecular(0.0, 0.0, 0.0, 1.0);
  float materialShininess = 0.0;

  floorModel = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createFloorModel(floorModel);
  floorModel->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  floorModel->setModulationTime(0);
  floorModel->upload();

  Model *box1 = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createCubeModel(box1);
  box1->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  box1->setTexture(0, textureCoords);
  box1->upload();

  Model *box2 = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createCubeModel(box2);
  box2->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  box2->setTexture(1, textureCoords);
  box2->upload();

  //Create SceneGraph of stacked boxes
  boxTowerSceneGraph1 = new SceneGraph(box1, Translate(0.0, 0.0, 0.0), Translate(0.0, 0.5, 0.0));
  tempSceneGraph1 = new SceneGraph(box2, Translate(0.0, 0.0, 0.0), Translate(0.0, 1.0, 0.0));
  tempSceneGraph2 = new SceneGraph(box1, Translate(0.0, 0.0, 0.0), Translate(0.0, 1.0, 0.0));
  tempSceneGraph3 = new SceneGraph(box2, Translate(0.0, 0.0, 0.0), Translate(0.0, 1.0, 0.0));

  boxTowerSceneGraph1->addSceneGraph(tempSceneGraph1);
  tempSceneGraph1->addSceneGraph(tempSceneGraph2);
  tempSceneGraph2->addSceneGraph(tempSceneGraph3);

  //Stick figure
  materialAmbient = vec4(0.0, 0.0, 0.0, 0.0);
  materialDiffuse = vec4(0.5, 0.2, 0.1, 1.0);
  materialSpecular = vec4(0.0, 0.0, 0.0, 1.0);
  materialShininess = 0.0;

  Model *body = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createLongStickModel(body);
  body->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  body->upload();

  Model *appendage = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createShortStickModel(appendage);
  appendage->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  appendage->upload();

  stickFigureSceneGraph1 = new SceneGraph(body, Translate(0.0, 0.0, 0.0), Translate(0.0, 0.0, 0.0));

  // right leg
  tempSceneGraph1 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(-15.0) * RotateX(-65.0), Translate(0.0, 0.0, 0.0));
  tempSceneGraph2 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(15.0) * RotateX(65.0), Translate(0.0, 0.0, 0.0));
  stickFigureSceneGraph1->addSceneGraph(tempSceneGraph1);
  tempSceneGraph1->addSceneGraph(tempSceneGraph2);

  // left leg
  tempSceneGraph1 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(15.0) * RotateX(45.0), Translate(0.0, 0.0, 0.0));
  tempSceneGraph2 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(-15.0) * RotateX(10.0), Translate(0.0, 0.0, 0.0));
  stickFigureSceneGraph1->addSceneGraph(tempSceneGraph1);
  tempSceneGraph1->addSceneGraph(tempSceneGraph2);

  // right arm
  tempSceneGraph1 = new SceneGraph(appendage, Translate(0.0, 0.3, 0.0) * RotateZ(-20.0) * RotateX(30.0), Translate(0.0, 0.0, 0.0));
  tempSceneGraph2 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(20.0) * RotateX(-110.0), Translate(0.0, 0.0, 0.0));
  stickFigureSceneGraph1->addSceneGraph(tempSceneGraph1);
  tempSceneGraph1->addSceneGraph(tempSceneGraph2);

  // left arm
  tempSceneGraph1 = new SceneGraph(appendage, Translate(0.0, 0.3, 0.0) * RotateZ(20.0) * RotateX(-30.0), Translate(0.0, 0.0, 0.0));
  tempSceneGraph2 = new SceneGraph(appendage, Translate(0.0, -0.8, 0.0) * RotateZ(-20.0) * RotateX(-110.0), Translate(0.0, 0.0, 0.0));
  stickFigureSceneGraph1->addSceneGraph(tempSceneGraph1);
  tempSceneGraph1->addSceneGraph(tempSceneGraph2);

  // head
  Model *head = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  head->load_obj("suzanne.obj");
  head->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  head->upload();

  tempSceneGraph1 = new SceneGraph(head, Translate(0.0, 0.8, 0.0) * Scale(0.4, 0.4, 0.4), Translate(0.0, 0.0, 0.0));
  stickFigureSceneGraph1->addSceneGraph(tempSceneGraph1);

  // second box tower
  boxTowerSceneGraph2 = new SceneGraph(box2, Translate(0.0, 0.0, 0.0), Translate(0.0, 0.5, 0.0));
  tempSceneGraph1 = new SceneGraph(box1, Translate(0.0, 0.5, 0.0), Translate(0.0, 0.5, 0.0));
  tempSceneGraph2 = new SceneGraph(box1, Translate(0.0, 0.5, 0.0), Translate(0.0, 0.5, 0.0));

  boxTowerSceneGraph2->addSceneGraph(tempSceneGraph1);
  boxTowerSceneGraph2->addSceneGraph(tempSceneGraph2);

  // sphere
  materialAmbient = vec4(0.0, 0.0, 0.0, 0.0);
  materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
  materialSpecular = vec4(0.0, 0.0, 0.0, 1.0);
  materialShininess = 5.0;

  Model *sphere = new Model(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, uTime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);
  createTetrahedronModel(sphere, 7);
  //createCubeModel(sphere);
  sphere->setMaterial(materialAmbient, materialDiffuse, materialSpecular, materialShininess);
  sphere->upload();

  sphereSceneGraph = new SceneGraph(sphere, Translate(0.0, 0.0, 0.0), Translate(0.0, 0.0, 0.0));
}

void runningAnimation(float stepsPerSecond, SceneGraph *stickFigure) {
  float stepPercentage = cosf(M_PI * stepsPerSecond) / 2.0 + 0.5;
  float stickFigureYOffset = (cosf(2.0 * M_PI * stepsPerSecond) / 2.0 + 0.5) * 0.2;

  float upperLegRotation = stepPercentage * 110.0;
  float lowerLegRotation = stepPercentage * 55.0;
  float armRotation = stepPercentage * 55.0;

  stickFigure->setModelOffset(Translate(0.0, stickFigureYOffset, 0.0));

  SceneGraph *rightLeg = stickFigure->getChildren()[0];
  SceneGraph *leftLeg = stickFigure->getChildren()[1];
  rightLeg->setApplyRotationMatrix(RotateX(upperLegRotation));
  leftLeg->setApplyRotationMatrix(RotateX(-upperLegRotation));

  rightLeg = rightLeg->getChildren()[0];
  leftLeg = leftLeg->getChildren()[0];
  rightLeg->setApplyRotationMatrix(RotateX(-lowerLegRotation));
  leftLeg->setApplyRotationMatrix(RotateX(lowerLegRotation));

  SceneGraph *rightArm = stickFigure->getChildren()[2];
  SceneGraph *leftArm = stickFigure->getChildren()[3];
  rightArm->setApplyRotationMatrix(RotateX(-armRotation));
  leftArm->setApplyRotationMatrix(RotateX(armRotation));
}

void pivotingBoxes(float elapsedTime) {
  float temp = elapsedTime / 1000.0;
  float location = sinf(M_PI * temp);

  SceneGraph *topBox1 = boxTowerSceneGraph2->getChildren()[0];
  SceneGraph *topBox2 = boxTowerSceneGraph2->getChildren()[1];

  if(((int)(temp + 1.5)) % 4 < 2) {
    // rotate so that when the box is on top of the other box it is upright.
    topBox1->setPrepareRotationMatrix(RotateZ(-90.0) * Translate(-0.5, 0.5, 0.0));
    topBox1->setModelOffset(Translate(-0.5, 0.5, 0.0));
    topBox1->setApplyRotationMatrix(RotateZ(location * 90.0));
  } else {
    topBox1->setPrepareRotationMatrix(RotateZ(90.0) * Translate(0.5, 0.5, 0.0));
    topBox1->setModelOffset(Translate(0.5, 0.5, 0.0));
    topBox1->setApplyRotationMatrix(RotateZ(location * -90.0));
  }

  if(((int)(temp + 0.5)) % 4 < 2) {
    topBox2->setPrepareRotationMatrix(RotateX(90.0) * Translate(0.0, 0.5, -0.5));
    topBox2->setModelOffset(Translate(0.0, 0.5, -0.5));
    topBox2->setApplyRotationMatrix(RotateX(location * 90.0));
  } else {
    topBox2->setPrepareRotationMatrix(RotateX(-90.0) * Translate(0.0, 0.5, 0.5));
    topBox2->setModelOffset(Translate(0.0, 0.5, 0.5));
    topBox2->setApplyRotationMatrix(RotateX(location * -90.0));
  }
}

void rotateBoxTower(float elapsedTime) {
  float rotationsPerMinute = 5.0 * elapsedTime / 36000.0;

  // Rotate boxes
  mat4 rotation = RotateY(rotationsPerMinute * 360.0);
  SceneGraph *tempSceneGraph = boxTowerSceneGraph1;
  tempSceneGraph->setApplyRotationMatrix(rotation);
  tempSceneGraph = tempSceneGraph->getChildren()[0];
  tempSceneGraph->setApplyRotationMatrix(rotation);
  tempSceneGraph = tempSceneGraph->getChildren()[0];
  tempSceneGraph->setApplyRotationMatrix(rotation);
  tempSceneGraph = tempSceneGraph->getChildren()[0];
  tempSceneGraph->setApplyRotationMatrix(rotation);
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

  float elapsedTime = glutGet(GLUT_ELAPSED_TIME);

  camera.setLightMovementTime(elapsedTime);

  mat4 p = camera.getProjectionMatrix();
  glUniformMatrix4fv(projection, 1, GL_TRUE, p);

  mat4 mv = camera.getModelViewMatrix();
  glUniformMatrix4fv(modelView, 1, GL_TRUE, mv);

  mat4 mo = Translate(0.0, 0.0, 0.0);
  glUniformMatrix4fv(modelOrientation, 1, GL_TRUE, mo);

  floorModel->setModulationTime(elapsedTime);

  floorModel->draw();

  rotateBoxTower(elapsedTime);
  drawSceneGraph(boxTowerSceneGraph1, Translate(0.0, 0.0, 0.0));

  float stepsPerSecond = 3.0 * elapsedTime / 1000.0;
  float circlesPerSecond = 0.3 * elapsedTime / 1000.0;
  mat4 stickFigurePositionAndOrientation = RotateY(circlesPerSecond * 360) * Translate(0.0, 2.2, 5.0) * RotateY(90.0);

  runningAnimation(stepsPerSecond, stickFigureSceneGraph1);
  drawSceneGraph(stickFigureSceneGraph1, stickFigurePositionAndOrientation);

  stepsPerSecond = 5.0 * elapsedTime / 1000.0;
  circlesPerSecond = 0.2 * elapsedTime / 1000.0;
  stickFigurePositionAndOrientation = RotateY(circlesPerSecond * -360) * Translate(0.0, 2.2, 7.0) * RotateY(-90.0);

  runningAnimation(stepsPerSecond, stickFigureSceneGraph1);
  drawSceneGraph(stickFigureSceneGraph1, stickFigurePositionAndOrientation);

  pivotingBoxes(elapsedTime);
  mat4 pivotingBoxesDisplacement = Translate(8.0, 0.0, 8.0);
  drawSceneGraph(boxTowerSceneGraph2, pivotingBoxesDisplacement);
  drawSceneGraph(boxTowerSceneGraph2, RotateY(90.0) * pivotingBoxesDisplacement);
  drawSceneGraph(boxTowerSceneGraph2, RotateY(180.0) * pivotingBoxesDisplacement);
  drawSceneGraph(boxTowerSceneGraph2, RotateY(270.0) * pivotingBoxesDisplacement);

  drawSceneGraph(sphereSceneGraph, Translate(2.0, 1.0, 2.0));
  drawSceneGraph(sphereSceneGraph, Translate(2.0, 1.0, -2.0));
  drawSceneGraph(sphereSceneGraph, Translate(-2.0, 1.0, 2.0));
  drawSceneGraph(sphereSceneGraph, Translate(-2.0, 1.0, -2.0));
  glutSwapBuffers();
}

void init(void) {

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vShader.glsl", "fShader.glsl");
  glUseProgram(program);

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  GLuint vNormal = glGetAttribLocation(program, "vNormal");
  GLuint vTextureCoords = glGetAttribLocation(program, "vTextureCoords");

  GLuint uTexture = glGetUniformLocation(program, "mytexture");
  GLuint uUseTexture = glGetUniformLocation(program, "UseTexture");

  GLuint uModulate = glGetUniformLocation(program, "modulate");
  GLuint utime = glGetUniformLocation(program, "time");

  //time for rotation of lights in fragment shader
  GLuint ftime = glGetUniformLocation(program, "fTime");
  camera.setLightMovementRef(ftime);

  // Initialize shader lighting parameters
  color4 lightAmbient(1.0, 1.0, 1.0, 1.0);

  GLuint LightAmbient = glGetUniformLocation(program, "LightAmbient");
  glUniform4fv(LightAmbient, 1, lightAmbient);

  GLuint LightPosition = glGetUniformLocation(program, "LightPosition");
  glUniform4fv(LightPosition, numberOfLights, lightPosition);

  LightDiffuse = glGetUniformLocation(program, "LightDiffuse");
  glUniform4fv(LightDiffuse, numberOfLights, lightDiffuse);

  GLuint LightSpecular = glGetUniformLocation(program, "LightSpecular");
  glUniform4fv(LightSpecular, numberOfLights, lightSpecular);

  projection = glGetUniformLocation(program, "Projection");
  modelView = glGetUniformLocation(program, "ModelView");
  modelOrientation = glGetUniformLocation(program, "ModelOrientation");

  GLuint uMaterialAmbient = glGetUniformLocation(program, "MaterialAmbient");
  GLuint uMaterialDiffuse = glGetUniformLocation(program, "MaterialDiffuse");
  GLuint uMaterialSpecular = glGetUniformLocation(program, "MaterialSpecular");
  GLuint uMaterialShininess = glGetUniformLocation(program, "MaterialShininess");

  createModels(vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uModulate, utime, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);

  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1, 0.1, 0.1, 1.0);
}

void headPositionUpdated(int un, double x, double y, double z)
{
	camera.headMovement(un,x,y,z);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  virtualWindow.setCallback(&headPositionUpdated);

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

  virtualWindow.Start();
  glutMainLoop();

  virtualWindow.Shutdown();
  return 0;
}
