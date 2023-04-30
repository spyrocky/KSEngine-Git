#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Vertex.h"
#include "KSEngine/Collisions/Collosion.h"
//Assimp
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

Model::Model()
{
	ModelFilePath = "";
	ModelCollision = nullptr;
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
	
	cout << "Attempting to import model" << endl;

	//create and import using assimp
	Assimp::Importer Importer;
	//attemp to import the new mesh base on the file path using the importer
	const aiScene* Scene = Importer.ReadFile(ImportFilePath, aiProcess_Triangulate);

	//check if the file imported correctly
	//!Scene means the file path probably didn't work
	//AI_SCENE_FLAGS_INCOOMPLETE means the file is probably corrupted or cannot be read by ASSIMP
	//!Scene->mRootNode means it's also either corrupted or not supported by ASSIMP
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
		cout << "Assign gray" << endl;
	}

	cout << MaterialStack.size() << endl;

	return true;
}

void Model::Draw()
{
	if (ModelCollision != nullptr)
	{
		ModelCollision->DebugDraw(Vector3(255.0f));
		ModelCollision->SetLocation(Transform.Location);
	}
	//cycle through the meshes and draw each one
	for (MeshPtr LMesh : MeshStack)
	{
		//assign the model transformations to the mesh
		LMesh->Transform = this->Transform;

		//draw the mesh using the material slot it has been assign
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

CollisionPtr Model::AddCollisionToModel(Vector3 Dimensions, Vector3 Offset)
{
	ModelCollision = make_shared<BoxCollision>(Transform.Location, Offset, Dimensions);

	return ModelCollision;
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
			cout << "Found and converted Mesh" << endl;
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
	//initialise local versions of the mesh vertices and indices
	vector<Vertex> Vertices;
	vector<UNint> Indices;

	//loop through all of the vertices and store their location, normal value and tex coord
	for (UNint i = 0; i < ImportMesh->mNumVertices; i++) {
		//initialise a single vertex
		Vertex LVertex;

		//find the position of the vertex
		LVertex.Position = Vector3(
			ImportMesh->mVertices[i].x,
			ImportMesh->mVertices[i].y,
			ImportMesh->mVertices[i].z
		);

		//find the normal value of the vertex (facing direction)
		LVertex.Normal = Vector3(
			ImportMesh->mNormals[i].x,
			ImportMesh->mNormals[i].y,
			ImportMesh->mNormals[i].z
		);

		//find the texture coordinates of the vertex
		LVertex.TextCoord = Vector2(
			ImportMesh->mTextureCoords[0][i].x,
			ImportMesh->mTextureCoords[0][i].y
		);

		

		//add the vertex into the vertices array
		Vertices.push_back(LVertex);
	}

	//make sure there are more than 3 vertices or stop creating the mesh
	if (Vertices.size() <= 3) {
		cout << "Model | One of the imported meshes doesn't have enough vertices at " << ModelFilePath << endl;
		return nullptr;
	}

	//next we find the indices
	//to find the indices we can use a value called faces
	//faces are basically 3 vertices
	for (UNint i = 0; i < ImportMesh->mNumFaces; i++) {
		//store the face
		aiFace Face = ImportMesh->mFaces[i];

		//run through another loop to detect all of the indices in the face
		for (UNint j = 0; j < Face.mNumIndices; j++) {
			//add each index to the indices array
			Indices.push_back(Face.mIndices[j]);
		}
	}

	//find and assign material slots
	UNint MaterialIndex = ImportMesh->mMaterialIndex;

	//resize the material stack to whatever the largest material slot index is
	if (MaterialStack.size() < MaterialIndex + 1) {
		//change the size of the array to the largest index + 1
		MaterialStack.resize(MaterialIndex + 1);
	}

	//create and assign the mesh
	MeshPtr ConvertedMesh = make_shared<Mesh>();

	ConvertedMesh->CreatMesh(Vertices, Indices, ModelShader, MaterialIndex);

	return ConvertedMesh;
}
