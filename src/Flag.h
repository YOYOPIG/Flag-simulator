#ifndef FLAG_H
#define FLAG_H

#include <glad/glad.h>
#include <string>
#include "Entity.h"
class Flag {
public:
    Flag(Entity **entity);
    void updatePositions(Entity** entityArr);
    void bindBufferObjects();
    void drawFlag();
    
private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    // 300 vertices (20x15)
    float vertices[900];
    unsigned int indices[1596]; // 19*14 = 266 rects, 532 triangles, 1596 points
};

#endif