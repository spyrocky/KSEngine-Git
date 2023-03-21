#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "KSEngine/Graphics/Texture.h"
#include "KSEngine/Graphics/VertexArrayObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "KSEngine/Game.h"


Mesh::Mesh()
{
	cout << "Mesh | Initialised Mesh." << endl;

	//Transform.Location.x = 0.5f;
	//Transform.Scale.y = 0.5f;
}

Mesh::~Mesh()
{
	MeshShader = nullptr;
	MeshTextures.clear();
	MeshVAO = nullptr;

	cout << "Mesh | Mesh Destroyed." << endl;
}

bool Mesh::CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, TexturePtrStack MeshTextures)
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
	this->MeshTextures = MeshTextures;

	cout << "Mesh | Mesh created successfully." << endl;

	return true;
}

void Mesh::Draw()
{
	//Activate the shader that this mesh uses
	MeshShader->RunShader();

	//Acticate the required texturesr for this mesh
	for (unInt Index = 0; Index < MeshTextures.size(); Index++) {
		//activating the texture through openGL
		MeshTextures[Index]->ActivateTexture(Index);
		//setting the textures number as the active texture in the shader
		MeshShader->SetInt("TextureColour", Index);
		//binding the texture to the shader
		MeshTextures[Index]->BindTexture();
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

	// create the world and screen transformation for this object
	Game::GetGameInstance().GetGraphicsEngine()->ApplyScreenTransformation(MeshShader);



	//draw the VAO
	MeshVAO->Draw();
}