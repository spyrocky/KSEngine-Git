#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/ShaderProgram.h"
//#include "KSEngine/Graphics/Texture.h"
#include "KSEngine/Graphics/Material.h"
#include "KSEngine/Graphics/VertexArrayObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "KSEngine/Game.h"
#include "KSEngine/Graphics/GraphicsEngine.h"


Mesh::Mesh()
{
	cout << "Mesh | Initialised Mesh." << endl;

	//Transform.Location.x = 0.5f;
	//Transform.Scale.y = 0.5f;
}

Mesh::~Mesh()
{
	MeshShader = nullptr;
	//MeshTextures.clear();
	//MeshMaterial = nullptr;
	MeshVAO = nullptr;

	cout << "Mesh | Mesh Destroyed." << endl;
}

bool Mesh::CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, UNint MaterialSlot)
{
	cout << "Creating Mesh." << endl;

	//Create the VAO
	MeshVAO = make_shared<VAO>(Shape);

	//validate the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh | Failed to create Mesh." << endl;
		return false;
	}

	//assign the shader and textures
	this->MeshShader = MeshShader;
	this->MaterialSlot = MaterialSlot;


	cout << "Mesh | Mesh created successfully." << endl;

	return true;
}

void Mesh::Draw(MaterialPtr MeshMaterial)
{
	//Activate the shader that this mesh uses
	MeshShader->RunShader();

	//Run matrial for this mesh
	//activate all required texture for the material
	if (MeshMaterial != nullptr) {
		MeshMaterial->Draw(MeshShader);
	}


	//initialise a static variable to check if any changes to transform
	static CTransform OldTransform;

	if (Transform != OldTransform) {
		OldTransform = Transform;

		glm::mat4 MatTransform = glm::mat4(1.0f);

		//translate - rotate - scale --- in this order
		//translate
		MatTransform = glm::translate(MatTransform, Transform.Location);
		//rotate
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.x), Vector3(1.0f, 0.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.y), Vector3(0.0f, 1.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.z), Vector3(0.0f, 0.0f, 1.0f));
		//scale
		MatTransform = glm::scale(MatTransform, Transform.Scale);

		//update the shader with the new transforms
		MeshShader->SetMat4("model", MatTransform);
	}

	//create the world and screen positions for this object
	Game::GetGameInstance().GetGraphicsEngine()->ApplyScreenTransformations(MeshShader);

	//draw the VAO
	MeshVAO->Draw();
}

bool Mesh::CreatMesh(vector<Vertex> Verticies, vector<UNint> Indicies, ShaderPtr MeshShader, UNint MaterialSlot)
{
	cout << "Creating Mesh." << endl;

	//Create the VAO
	MeshVAO = make_shared<VAO>(Verticies, Indicies);

	//validate the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh | Failed to create Mesh." << endl;
		return false;
	}

	//assign the shader and textures
	this->MeshShader = MeshShader;
	this->MaterialSlot = MaterialSlot;


	cout << "Mesh | Mesh created successfully." << endl;

	return false;
}
