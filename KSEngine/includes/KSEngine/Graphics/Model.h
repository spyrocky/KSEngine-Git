#pragma once
#include "KSEngine/CoreMinimal.h"
#include "KSEngine/Math/Transformations.h"

struct aiNode;
struct aiScene;
struct aiMesh;

class Model {
public:
	Model();
	~Model();

	// create the simple shape on the engine shape
	bool CreateSimpleMesh(GeometricShapes Shape, ShaderPtr ModelShader);

	bool ImportMeshFromFile(const char* ImportFilePath, ShaderPtr ModelShader);

	//draw the model to the screen
	void Draw();

	//change material relevent to the slot index
	// this will error log if the slot doesn't exist
	void SetMaterialBySlot(UNint SlotIndex, MaterialPtr NewMaterial);
	// get a material in the material stack if it exists
	MaterialPtr GetMaterialBySlot(UNint SlotIndex) const;

	// get the directory the model as imported form
	const char* GetFilePath() const { return ModelFilePath; }

	//return the collision for this model
	CollisionPtr GetCollision() const { return ModelCollision; };

	// add a collision to the model
	CollisionPtr AddCollisionToModel(Vector3 Dimensions, Vector3 Offset = Vector3(0.0f));

private:
	//find all the meshes in import the file/scene
	void FindAndImportSceneMeshes(aiNode* Node, const aiScene* Scene);

	//convert Mesh from importer to the engine class
	MeshPtr ConvertImportMeshToEngineMesh(aiMesh* ImportMesh, const aiScene* Scene);

public:
	//position, rotation and scale of all the mesh in the model
	CTransform Transform;

private:
	//All the mesh relate to this model
	MeshPtrStack MeshStack;

	//all the materials relevent to the mesh in this model
	MaterialPtrStack MaterialStack;
	//the model shader
	ShaderPtr ModelShader;


	//import file path
	const char* ModelFilePath;

	CollisionPtr ModelCollision;



};