#include "KSEngine/Collisions/Collosion.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "KSEngine/Graphics/Material.h"


Collision::Collision(Vector3 Location, Vector3 Offset)
{
	Transform.Location = Location;
	this->Offset = Offset;
	DebugMesh = nullptr;
	DebugShader = nullptr;

	cout << "Creating collision." << endl;
}

Collision::~Collision()
{
	DebugMesh = nullptr;
	DebugShader = nullptr;


	cout << "Collision Destroyed." << endl;
}

BoxCollision::BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimensions) :	Collision(Location,Offset)
{
	this->Dimensions = Dimensions;
	//Find min and max vectors in world space for this box
	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);
}

bool BoxCollision::IsOverlapping(Collision& OtherCol) {

	const BoxCollision& OtherBox = dynamic_cast<BoxCollision&>(OtherCol);

	// return true if the box is withing the other box
	// are the min value higher than max and visa versa
	return(		
		OtherBox.Min.x <= Max.x &&
		OtherBox.Max.x >= Min.x &&
		OtherBox.Min.y <= Max.y &&
		OtherBox.Max.y >= Min.y &&
		OtherBox.Min.z <= Max.z &&
		OtherBox.Max.z >= Min.z 			
		);

	return false;
}


Vector3 BoxCollision::FindCenter() {
	return Transform.Location + Offset;
}

void BoxCollision::DebugDraw(Vector3 Colour) {
	if (DebugMesh == nullptr) {
		DebugShader = make_shared<ShaderProgram>();
		DebugShader->InitVFShader({
			L"Game/Shaders/WireframeShader/WireframeShader.svert",
			L"Game/Shaders/WireframeShader/WireframeShader.sfrag"
			});

		DebugMesh = make_shared<Mesh>();
	}

}

void BoxCollision::SetLocation(Vector3 NewLocation) {
	Collision::SetLocation(NewLocation);
	//update min and maxwith the new location
	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);
}

void  BoxCollision::SetDimensions(Vector3 NewDimensions) {
	//set the dimension to the new dimention
	Dimensions = NewDimensions;
	//update min and max with the new dimension
	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);

}