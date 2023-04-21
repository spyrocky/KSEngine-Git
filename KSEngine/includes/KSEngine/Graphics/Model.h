#pragma once
#include "KSEngine/CoreMinimal.h"
#include "KSEngine/Math/Transformations.h"

class Model {
public:
	Model();
	~Model();

	// create the simple shape on the engine shape
	bool CreateSimpleMesh(GeometricShapes Shape, ShaderPtr ModelShader);

	//draw the model to the screen
	void Draw();

	//change material relevent to the slot index
	// this will error log if the slot doesn't exist
	void SetMaterialBySlot(UNint SlotIndex, MaterialPtr NewMaterial);


public:
	//position, rotation and scale of all the mesh in the model
	CTransform Transform;

private:
	//All the mesh relate to this model
	MeshPtrStack MeshStack;
	MaterialPtrStack MaterialStack;
	//the model shader
	ShaderPtr ModelShader;
};