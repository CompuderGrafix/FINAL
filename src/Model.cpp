/*
 * Model.cpp
 *
 * Used http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ as a reference
 */

#include "Model.h"

Model::Model(GLuint _vPosition, GLuint _vNormal, GLuint _vTextureCoords,
    GLuint _uTexture, GLuint _uUseTexture,
    GLuint _uMaterialAmbient, GLuint _uMaterialDiffuse, GLuint _uMaterialSpecular, GLuint _uMaterialShininess) {

  vPosition = _vPosition;
  vNormal = _vNormal;
  vTextureCoords = _vTextureCoords;

  uTexture = _uTexture;
  uUseTexture = _uUseTexture;
  texture_id = 0;

  uMaterialAmbient = _uMaterialAmbient;
  uMaterialDiffuse = _uMaterialDiffuse;
  uMaterialSpecular = _uMaterialSpecular;
  uMaterialShininess = _uMaterialShininess;

  printf("vPosition %d, vNormal %d, vTextureCoords %d, uTexture %d, uUseTexture %d, uMaterialAmbient %d, uMaterialDiffuse %d, uMaterialSpecular %d, uMaterialShininess %d\n",
      vPosition, vNormal, vTextureCoords, uTexture, uUseTexture, uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess);

  vboVertices = 0;
  vboNormals = 0;
  iboElements = 0;
  vboTextureCoords = 0;
  materialShininess = 0;

  useTexture = false;
}

Model::~Model() {
  if (vboVertices != 0)
    glDeleteBuffers(1, &vboVertices);
  if (vboNormals != 0)
    glDeleteBuffers(1, &vboNormals);
  if (iboElements != 0)
    glDeleteBuffers(1, &iboElements);
  if (vTextureCoords != 0)
    glDeleteBuffers(1, &vTextureCoords);
}

void Model::addPoint(vec4 vertex) {
  vertices.push_back(vertex);
}

void Model::addElements(GLshort a, GLshort b, GLshort c) {
  elements.push_back(a);
  elements.push_back(b);
  elements.push_back(c);
}

void Model::calculateNormals() {

  if(elements.size() > 0) {
    std::vector<int> nb_seen;

    normals.resize(vertices.size(), vec3(0.0, 0.0, 0.0));
    nb_seen.resize(vertices.size(), 0);
    for (unsigned int i = 0; i < elements.size(); i += 3) {
      GLushort ia = elements[i];
      GLushort ib = elements[i + 1];
      GLushort ic = elements[i + 2];
      vec3 normal = normalize(cross(vertices[ib] - vertices[ia], vertices[ic] - vertices[ia]));

      int v[3];
      v[0] = ia;
      v[1] = ib;
      v[2] = ic;
      for (int j = 0; j < 3; j++) {
        GLushort cur_v = v[j];
        nb_seen[cur_v]++;
        if (nb_seen[cur_v] == 1) {
          normals[cur_v] = normal;
        } else {
          // average
          normals[cur_v].x = normals[cur_v].x * (1.0 - 1.0 / nb_seen[cur_v]) + normal.x * 1.0 / nb_seen[cur_v];
          normals[cur_v].y = normals[cur_v].y * (1.0 - 1.0 / nb_seen[cur_v]) + normal.y * 1.0 / nb_seen[cur_v];
          normals[cur_v].z = normals[cur_v].z * (1.0 - 1.0 / nb_seen[cur_v]) + normal.z * 1.0 / nb_seen[cur_v];
          normals[cur_v] = normalize(normals[cur_v]);
        }
      }
    }
  } else {
    for (unsigned int i = 0; i < vertices.size(); i += 3) {
      vec3 normal = normalize(cross(vertices[i + 1] - vertices[i], vertices[i + 2] - vertices[i + 1]));
      normals.push_back(normal);
      normals.push_back(normal);
      normals.push_back(normal);
    }
  }
}

void Model::setMaterial(vec4 _materialAmbient, vec4 _materialDiffuse, vec4 _materialSpecular, float _materialShininess) {
  materialAmbient = _materialAmbient;
  materialDiffuse = _materialDiffuse;
  materialSpecular = _materialSpecular;
  materialShininess = _materialShininess;
}

void Model::setTexture(GLuint _texture_id, std::vector<GLfloat> _textureCoords) {
  texture_id = _texture_id;
  useTexture = true;
  textureCoords = _textureCoords;
}

void Model::upload() {
  if (vertices.size() > 0) {
    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
  }

  if (normals.size() > 0) {
    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);
  }

  if (elements.size() > 0) {
    glGenBuffers(1, &iboElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(elements[0]), elements.data(), GL_STATIC_DRAW);
  }

  if (textureCoords.size() > 0) {
    glGenBuffers(1, &vboTextureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoords);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(textureCoords[0]), textureCoords.data(), GL_STATIC_DRAW);
  }
}

void Model::draw() {
  if (vboVertices != 0) {
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(
      vPosition,  // attribute
      4,                  // number of elements per vertex, here (x,y,z,w)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
    );
  }

  if (vboNormals != 0) {
    glEnableVertexAttribArray(vNormal);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glVertexAttribPointer(
      vNormal,            // attribute
      3,                  // number of elements per vertex, here (x,y,z)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
    );
  }

  if (vboTextureCoords != 0) {
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uTexture, 0);

    glEnableVertexAttribArray(vTextureCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoords);
    glVertexAttribPointer(
      vTextureCoords,     // attribute
      2,                  // number of elements per vertex, here (x,y)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
    );
  }

  glUniform4fv(uMaterialAmbient, 1, materialAmbient);
  glUniform4fv(uMaterialDiffuse, 1, materialDiffuse);
  glUniform4fv(uMaterialSpecular, 1, materialSpecular);
  glUniform1f(uMaterialShininess, materialShininess);
  glUniform1f(uUseTexture, useTexture);

  if (iboElements != 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboElements);
    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  } else {
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  }

  if (vboVertices != 0)
    glDisableVertexAttribArray(vPosition);
  if (vboNormals != 0)
    glDisableVertexAttribArray(vNormal);
  if (vboTextureCoords != 0)
    glDisableVertexAttribArray(vTextureCoords);
}

void Model::load_obj(const char* filename) {
  std::ifstream in(filename, std::ios::in);
  if (!in) {
    std::cerr << "Cannot open " << filename << std::endl;
    exit(1);
  }

  std::string line;
  while (getline(in, line)) {
    if (line.substr(0, 2) == "v ") {
      std::istringstream s(line.substr(2));
      vec4 v;
      s >> v.x;
      s >> v.y;
      s >> v.z;
      v.w = 1.0;
      vertices.push_back(v);

    } else if (line.substr(0, 2) == "f ") {
      std::istringstream s(line.substr(2));
      GLushort a, b, c;
      s >> a;
      s >> b;
      s >> c;
      a--;
      b--;
      c--;
      addElements(a, b, c);
    } else if (line[0] == '#') { /* ignoring this line */
    } else { /* ignoring this line */
    }
  }

  calculateNormals();
}

vec4 unit(const vec4& p) {
  float len = p.x * p.x + p.y * p.y + p.z * p.z;

  vec4 t;
  if (len > DivideByZeroTolerance) {
    t = p / sqrt(len);
    t.w = 1.0;
  }

  return t;
}

void triangle(Model *model, const vec4& a, const vec4& b, const vec4& c) {
  model->addPoint(a);
  model->addPoint(b);
  model->addPoint(c);
}

void divide_triangle(Model *model, const vec4& a, const vec4& b, const vec4& c, int count) {
  if (count > 0) {
    vec4 v1 = unit(a + b);
    vec4 v2 = unit(a + c);
    vec4 v3 = unit(b + c);
    divide_triangle(model, a, v1, v2, count - 1);
    divide_triangle(model, c, v2, v3, count - 1);
    divide_triangle(model, b, v3, v1, count - 1);
    divide_triangle(model, v1, v3, v2, count - 1);
  } else {
    triangle(model, a, b, c);
  }
}

void tetrahedron(Model *model, int count) {
  vec4 v[4] = { vec4(0.0, 0.0, 1.0, 1.0), vec4(0.0, 0.942809, -0.333333, 1.0), vec4(-0.816497, -0.471405, -0.333333, 1.0), vec4(0.816497, -0.471405, -0.333333, 1.0) };

  divide_triangle(model, v[0], v[1], v[2], count);
  divide_triangle(model, v[3], v[2], v[1], count);
  divide_triangle(model, v[0], v[3], v[1], count);
  divide_triangle(model, v[0], v[2], v[3], count);

  model->calculateNormals();
}

void cube(Model *model) {
  // Vertices of a unit cube centered at origin, sides aligned with axes
  vec4 vertices[8] = {
    vec4(-0.5, -0.5, 0.5, 1.0),   // 0 left bottom front
    vec4(-0.5, 0.5, 0.5, 1.0),    // 1 left top front
    vec4(0.5, 0.5, 0.5, 1.0),     // 2 right top front
    vec4(0.5, -0.5, 0.5, 1.0),    // 3 right bottom front
    vec4(-0.5, -0.5, -0.5, 1.0),  // 4 left bottom back
    vec4(-0.5, 0.5, -0.5, 1.0),   // 5 left top back
    vec4(0.5, 0.5, -0.5, 1.0),    // 6 right top back
    vec4(0.5, -0.5, -0.5, 1.0)    // 7 right bottom back
  };

  //front
  triangle(model, vertices[1], vertices[0], vertices[3]);
  triangle(model, vertices[1], vertices[3], vertices[2]);

  //back
  triangle(model, vertices[6], vertices[7], vertices[4]);
  triangle(model, vertices[6], vertices[4], vertices[5]);

  //right
  triangle(model, vertices[2], vertices[3], vertices[7]);
  triangle(model, vertices[2], vertices[7], vertices[6]);

  //left
  triangle(model, vertices[5], vertices[4], vertices[0]);
  triangle(model, vertices[5], vertices[0], vertices[1]);

  //top
  triangle(model, vertices[5], vertices[1], vertices[2]);
  triangle(model, vertices[5], vertices[2], vertices[6]);

  //bottom
  triangle(model, vertices[0], vertices[4], vertices[7]);
  triangle(model, vertices[0], vertices[7], vertices[3]);

  model->calculateNormals();
}
