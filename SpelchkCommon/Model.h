/*
 * Model.h
 *
 *  Created on: Dec 2, 2012
 *      Author: Hoanh Nguyen
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "../include/Angel.h"
#include <vector>
#include <SOIL/SOIL.h>

#include <iostream>
#include <fstream>
#include <sstream>

class Model {
  private:
    GLuint vPosition, vNormal, vTextureCoords;
    GLuint uTexture, uUseTexture;
    GLuint uModulate, utime;
    GLuint uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess;
    GLuint vboVertices, vboNormals, iboElements, vboTextureCoords;

    vec4 materialAmbient, materialDiffuse, materialSpecular;
    float materialShininess;

    std::vector<vec4> vertices;
    std::vector<vec3> normals;
    std::vector<GLushort> elements;
    std::vector<GLfloat> textureCoords;

    bool useTexture;
    GLint textureId;

    bool modulate;
    float elapsedTime;

  public:
    Model(GLuint vPosition, GLuint vNormal, GLuint vTextureCoords,
        GLuint uTexture, GLuint uUseTexture, GLuint uModulate, GLuint uTime,
        GLuint uMaterialAmbient, GLuint uMaterialDiffuse, GLuint uMaterialSpecular, GLuint uMaterialShininess);
    virtual ~Model();

    void load_obj(const char* filename);

    void addPoint(vec4 vertex);
    void addElements(GLshort a, GLshort b, GLshort c);
    void calculateNormals();

    void setMaterial(vec4 materialAmbient, vec4 materialDiffuse, vec4 materialSpecular, float materialShininess);
    void setTexture(GLint textureId, std::vector<GLfloat> textureCoords);
    void setModulationTime(float t);

    void upload();
    void draw();
};

void triangle(Model *model, const vec4& a, const vec4& b, const vec4& c);
void createTetrahedronModel(Model *model, int count);
void createCubeModel(Model *model);
void createFloorModel(Model *model);
void createLongStickModel(Model *model);
void createShortStickModel(Model *model);

#endif /* MODEL_H_ */
