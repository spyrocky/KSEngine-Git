#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Vertex.h"
//Assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model()
{
	ModelFilePath = "";
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

	MeshStack.push_back(NewMesh);

	return true;
}

bool Model::ImportMeshFromFile(const char* ImportFilePath, ShaderPtr ModelShader)
{
	

	//create and import using assimp
	Assimp::Importer Importer;
	//attemp to import the new mesh base on the file path using the importer
	const aiScene* Scene = Importer.ReadFile(ImportFilePath, aiProcess_Triangulate);

	//check if the file import correctly
	if (!Scene || Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
		cout << "Model | Error importing model from " << ImportFilePath << " - " << Importer.GetErrorString() << endl;
		return false;
	}

	this->ModelShader = ModelShader;

	//save the file path
	ModelFilePath = ImportFilePath;

	//find and import all of the mesh using import scene
	FindAndImportSceneMeshes(Scene->mRootNode, Scene);

	// run trough all the new mat slot and assign the default mat
	for (UNint i=0; i<MaterialStack.size(); i++)
	{
		MaterialStack[i] = Game::GetGameInstance().GetDefaultGEngineMaterial();
	}


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
		cout << "Model | No Material slot of that index (" << SlotIndex << ") exists." << endl;
		return;
	}

	//reassign the array item
	MaterialStack[SlotIndex] = NewMaterial;
}

MaterialPtr Model::GetMaterialBySlot(UNint SlotIndex) const
{
	//check if the index is valid to array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | No Material slot of that index (" << SlotIndex << ") exists." << endl;
		return nullptr;
	}

	return MaterialStack[SlotIndex];
}

void Model::FindAndImportSceneMeshes(aiNode* Node, const aiScene* Scene)
{
	for (UNint i = 0; i < Node->mNumMeshes; i++) {
		UNint ImpMeshIndex = Node->mMeshes[i];
		//get the one of the mesh
		aiMesh* ImpMesh = Scene->mMeshes[ImpMeshIndex];

		//convert the import mesh into engine'
		MeshPtr ConvertedMesh = ConvertImportMeshToEngineMesh(ImpMesh, Scene);

		// add the new convert mesh into our mesh array
		if (ConvertedMesh != nullptr) {
			MeshStack.push_back(ConvertedMesh);
		}
	}

	//check through all mesh that are chrilden of the root mesh
	//not run if no chrilden
	for (UNint i = 0; i < Node->mNumChildren; i++)
	{
		FindAndImportSceneMeshes(Node->mChildren[i], Scene);
	}
}

MeshPtr Model::ConvertImportMeshToEngineMesh(aiMesh* ImportMesh, const aiScene* Scene)
{
	vector<Vertex> Vertices;
	vector<UNint> Indicies;

	for (UNint i = 0; i < ImportMesh->mNumVertices; i++)
	{
		Vertex LVertex;

		LVertex.Position = Vector3(
			ImportMesh->mVertices[i].x,
			ImportMesh->mVertices[i].y,
			ImportMesh->mVertices[i].z
		);

		LVertex.Normal = Vector3(
			ImportMesh->mNormals[i].x,
			ImportMesh->mNormals[i].y,
			ImportMesh->mNormals[i].z
		);

		LVertex.TextCoord = Vector2(
			ImportMesh->mTextureCoords[0][i].x,
			ImportMesh->mTextureCoords[0][i].y
		);

		Vertices.push_back(LVertex);
	}

	if (Vertices.size() < 3) {
		cout << "Model | One of the imported meshed doesn't have enough verticies at " << ModelFilePath << endl;
		return nullptr;
	}

	// next we find the indicies , we can use value of called face 
	// faces are basically 3 verticies
	for (UNint i = 0; i < ImportMesh->mNumFaces; i++)
	{
		//store in the face
		aiFace Face = ImportMesh->mFaces[i];

		//run through other loop to detect all of the indicies in the faces
		for (UNint j = 0; j < Face.mNumIndices; j++)
		{
			//add to the array
			Indicies.push_back(Face.mIndices[j]);
		}
	}

	// find and assign material slot
	UNint MaterialIndex = ImportMesh->mMaterialIndex;

	//resize the material stack to whatever the large material slot index is
	if (MaterialStack.size() < MaterialIndex + 1) {
		//change size of the array to the large index
		MaterialStack.resize(MaterialIndex + 1);


	}
	//create and assign the mesh]
	MeshPtr ConvertedMesh = make_shared<Mesh>();

	ConvertedMesh->CreatMesh(Vertices, Indicies, ModelShader, MaterialIndex);

	return ConvertedMesh;
}
