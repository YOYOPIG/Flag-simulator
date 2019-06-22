#include "Flag.h"
#include <iostream>

Flag::Flag()
{
    // Initialize vertices and indices
    float x = -1.0, y = 1.0, z = 0.0;
    int i = 0;
    while(i<900)
    {
        vertices[i++] = x;
        vertices[i++] = y;
        vertices[i++] = z;
        x += 0.1;
        if(x >= 1.0)
        {
            std::cout<<std::endl;
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
    for(int k=0;k<1596;++k)
    {
        std::cout<<indices[k]<<" ";
        if((k+1)%3==0)
            std::cout<<std::endl;
    }

}

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