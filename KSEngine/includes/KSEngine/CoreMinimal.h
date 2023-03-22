#pragma once

#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int UNint;

//class/type definitions
class GraphicsEngine;		//forward declaring for circular dependency
typedef shared_ptr<GraphicsEngine> GraphicsEnginePtr;

class VertexArrayObject;
typedef VertexArrayObject VAO;
typedef shared_ptr<VAO> VAOPtr;

class Texture;
typedef shared_ptr<Texture> TexturePtr;
typedef vector<TexturePtr> TexturePtrStack;

class ShaderProgram;
typedef shared_ptr<ShaderProgram> ShaderPtr;

class Mesh;
typedef shared_ptr<Mesh> MeshPtr;
typedef vector<MeshPtr> MeshPtrStack;

//useful for shape data and VAOs
typedef vector<float>  PositionMatrix;
typedef vector<UNint> IndicesMatrix;

//structure to handle matrices VAOs
struct ShapeData {
	vector<float> PositionMatrix;
	vector<UNint> IndicesMatrix;
};

//list of the geometric shapes in VAO
enum class GeometricShapes {
	Triangle = 0,
	Polygon,
	Circle,
	Cube
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