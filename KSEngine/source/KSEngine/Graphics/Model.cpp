#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Game.h"


Model::Model()
{
}

Model::~Model()
{
	MeshStack.clear();
	MaterialStack.clear();
	ModelShader = nullptr;
}

bool Model::CreateSimpleMesh(GeometricShapes Shape, ShaderPtr ModelShader)
{
	MeshPtr NewMesh = make_shared<Mesh>();

	MeshStack.clear();
	MaterialStack.clear();
	this->ModelShader = ModelShader;

	if (!NewMesh->CreateSimpleShape(Shape, ModelShader, 0)) {
		return false;
	}

	MaterialStack.push_back(Game::GetGameInstance().GetDefaultGEngineMaterial());

	return true;
}

void Model::Draw()
{
	//cycle through the mesh and draw each one
	for (MeshPtr LMesh : MeshStack) {
		LMesh->Transform = this->Transform;

		LMesh->Draw(MaterialStack[LMesh->GetMaterialSlot()]);
	}
}

void Model::SetMaterialBySlot(UNint SlotIndex, MaterialPtr NewMaterial)
{
	//check if the index is valid to array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | No Material slot exists of that index (" << SlotIndex << ") exists." << endl;
		return;
	}

	//reassign the array item
	MaterialStack[SlotIndex] = NewMaterial;
}
