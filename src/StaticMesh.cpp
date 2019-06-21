#include "StaticMesh.h"
#include "tiny_obj_loader.h"
#include <fstream>
#include <string>
#include <cstring>
#include <regex>

StaticMesh::StaticMesh()
    : m_uv(false), m_normal(false)
{

}

StaticMesh StaticMesh::LoadMesh(const std::string &filename, bool flat)
{
	std::string obj_path;
	if (flat)
	{
		getFlatMesh(filename);
		obj_path = "../resource/flat_model.obj";
	}
	else
	{
		obj_path = filename;
	}
    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, obj_path.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}

    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    // Upload postion array
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
        shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (shapes[0].mesh.texcoords.size() > 0) {

        // Upload texCoord array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
            shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_uv = true;
    }

    if (shapes[0].mesh.normals.size() > 0) {
        // Upload normal array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
            shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_normal = true;
    }

    // Setup index buffer for glDrawElements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
        shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

    ret.numIndices = shapes[0].mesh.indices.size();

    glBindVertexArray(0);
    return ret;
}

void StaticMesh::getFlatMesh(const std::string &filename)
{
	std::ifstream infile(filename);
	std::ofstream outfile("../resource/flat_model.obj");
	std::string line;

	while (std::getline(infile, line))
	{
		if (line.at(0) != 'f')
		{
			outfile << line << std::endl;
			continue;
		}
		std::string newline;
		line.append(" ");
		int ctr = 0;
		std::string firstVal = "/";

		// Find first value
		for (int i = 0; i < line.size(); ++i)
		{
			if (ctr == 2)
			{
				if (line.at(i) == ' ')
					break;
				else
					firstVal += line.at(i);
			}
			else
			{
				if (line.at(i) == '/')
					ctr++;
			}
		}
		firstVal += " ";

		std::regex re("/[0-9]* ");
		newline = std::regex_replace(line, re, firstVal);
		newline = newline.substr(0, newline.size() - 1);
		outfile << newline << std::endl;
	}
	infile.close();
	outfile.close();
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
}

bool StaticMesh::hasNormal() const
{
	return m_normal;
}

bool StaticMesh::hasUV() const 
{
	return m_uv;
}
