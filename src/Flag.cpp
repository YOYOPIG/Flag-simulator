#include "Flag.h"
#include <iostream>

Flag::Flag(Entity **entity)
{
    // Initialize vertices and indices
    float x = -1.0, y = 1.0, z = 0.0;
    int i = 0;
    while(i<900)
    {
		entity[int(i / 3)]->rigid.setPosition(x, y, z);
        vertices[i++] = x;
        vertices[i++] = y;
        vertices[i++] = z;
		
        x += 0.1;
        if(x >= 1.0)
        {
            x = -1.0;
            y -= 0.1;
        }
    }
    // Insert indices
    int endIndex = 19;
    int curIndex = 0;
    i=0;
    // top right ones
    while(i<798)
    {
        indices[i++] = curIndex;
        indices[i++] = curIndex+1;
        indices[i++] = curIndex+21;
        curIndex++;
        if(curIndex >= endIndex)
        {
            curIndex++;
            endIndex += 20;
        }
    }
    //bottom left ones
    endIndex = 19;
    curIndex = 0;
    while(i<1596)
    {
        indices[i++] = curIndex;
        indices[i++] = curIndex+21;
        indices[i++] = curIndex+20;
        curIndex++;
        if(curIndex >= endIndex)
        {
            curIndex++;
            endIndex += 20;
        }
    }
}

void Flag::updatePositions(Entity** entityArr)
{
    Entity *cur = entityArr[0];
    float x = -1.0, y = 1.0, z = 0.0;
    int i = 0;
    int entityCtr = 0;
    while(i<900)
    {
        vertices[i++] = cur->rigid.data.position.x;
        vertices[i++] = cur->rigid.data.position.y;
        vertices[i++] = cur->rigid.data.position.z;
        cur = entityArr[++entityCtr];
    }
    //updateNormals();
}

// void Flag::updateNormals()
// {
//     // Update face normals
//     for(int i=0;i<14;++i)
//     {
//         for(int j=0;j<19;++j)
//         {
//             int topLeft = i * 20 + j;
//             faceNormal[i][j] = glm::cross((getVertexPosition(topLeft+20)-getVertexPosition(topLeft)), (getVertexPosition(topLeft+1)-getVertexPosition(topLeft)));
//         }
//     }
//     // Use face normals to get vertex normals
//     for(int i=0;i<300;++i)
//     {
//         int tarX = i % 20; //cal blocks x and x-1
//         int tarY = i / 20; //cal blocks y and y-1
//         glm::vec3 result = glm::vec3(0,0,0);
//         if(tarX-1>=0)
//         {
//             if(tarY-1>=0)
//                 result += faceNormal[tarY-1][tarX-1];
//             result += faceNormal[tarY][tarX-1];
//         }
//         if(tarY-1>=0)
//             result += faceNormal[tarY-1][tarX];
//         result += faceNormal[tarY][tarX];

//         vertexNormals[i] = result;
//     }
// }

void Flag::bindBufferObjects()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0); 
	glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				// ..:: 初始化代码 :: ..
	// 1. 绑定顶点数组对象
	glBindVertexArray(VAO);
	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4. 设定顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Flag::drawFlag()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 1596, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::vec3 Flag::getVertexPosition(int vertexID)
{
    return glm::vec3(indices[3*vertexID], indices[3*vertexID+1], indices[3*vertexID+2]);
}