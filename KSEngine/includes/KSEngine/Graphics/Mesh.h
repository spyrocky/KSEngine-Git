#pragma once
#include "KSEngine/CoreMinimal.h"
#include "KSEngine/Math/Transformations.h"


struct Vertex;

class Mesh {
public:
	Mesh();
	~Mesh();

	//Create the mesh out of VAO, a shader and textures
	bool CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, UNint MaterialSlot);

	//handle the drawing of all the required classes
	void Draw(MaterialPtr MeshMaterial);

	//this holds the position, rotation and scale of the mesh
	CTransform Transform;
	//return material slot by the model to the mesh
	UNint GetMaterialSlot() const { return MaterialSlot; }

	//create a mesh VAO that predefine model
	bool CreatMesh(vector<Vertex> Verticies, vector<UNint> Indicies, ShaderPtr MeshShader, UNint MaterialSlot);

private:
	//Store the shader this mesh requires
	ShaderPtr MeshShader;

	//Assign material to the mesh
	//MaterialPtr MeshMaterial;
	UNint MaterialSlot;


	//Create a new VAO for the mesh
	VAOPtr MeshVAO;

};