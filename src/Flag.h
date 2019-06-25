#ifndef FLAG_H
#define FLAG_H

#include <glad/glad.h>
#include <string>
#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Flag {
public:
    Flag(Entity **entity);
    void updatePositions(Entity** entityArr);
    void updateNormals();
    void bindBufferObjects();
    void drawFlag();
    glm::vec3 getVertexPosition(int vertexID);
    bool Dbug = false;

private:
    const int rectCount = 266;
    unsigned int VBO[2];
    unsigned int VAO;
    unsigned int IBO;
    // 300 vertices (20x15)
    float vertices[900];
    unsigned int indices[1596]; // 19*14 = 266 rects, 532 triangles, 1596 points
    glm::vec3 faceNormal[14][19];
    glm::vec3 vertexNormals[300];
};

#endif