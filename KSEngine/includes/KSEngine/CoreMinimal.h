#pragma once

#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int unInt;

// class/type definitions
class GraphicsEngine;
typedef shared_ptr<GraphicsEngine> GraphicsEnginePtr;

class VertexArrayObject;
typedef VertexArrayObject VAO;
typedef shared_ptr<VAO> VAOPtr;
typedef vector<VAOPtr> VAOStack;

class Texture;
typedef shared_ptr<Texture> TexturePtr;
typedef vector<TexturePtr> TexturePtrStack;

class ShaderProgram;
typedef shared_ptr<ShaderProgram> ShaderPtr;

class Mesh;
typedef shared_ptr<Mesh> MeshPtr;
typedef vector<MeshPtr> MeshPtrStack;



// useful for shape data and VAO Matrices
typedef vector<float> PositionMatrix;
typedef vector<unInt> IndicesMatrix;

//structure to handle matrices VAOs
struct ShapeData {
	vector<float> PositionMatrix;
	vector<unInt> IndicesMatrix;
};

//list of the geometric shapes in VAO
enum class GeometricShapes {
	Triangle = 0,
	Polygon,
	Circle,

	Heart,
	Cube,

};

//store the parameters required to create a shader program
struct VFShaderParams {
	const wchar_t* VertexShaderPath;
	const wchar_t* FragmentShaderPath;
};

//types of shader acceptable in our program
enum class ShaderTypes {
	Vertex = 0,
	Fragment
};