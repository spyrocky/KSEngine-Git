#include "KSEngine/Graphics/Collosion.h"


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

BoxCollision::BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimensions) {
	this->Dimensions = Dimensions;

	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);
}

bool BoxCollision::IsOverlapping(Collision& OtherCol) {

	const BoxCollision& OtherBox = dynamic_cast<BoxCollision&>(OtherCol);

	return false;
}


Vector3 BoxCollision::FindCenter() {
	return Transform.Location + Offset;
}

void BoxCollision::DebugDraw(Vector3 Colour) {

}

void BoxCollision::SetLocation(Vector3 NewLocation) {
	Collision::SetLocation(NewLocation);

	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);
}

void  BoxCollision::SetDimensions(Vector3 NewDimensions) {
	Dimensions = NewDimensions;

	Min = FindCenter() - (Dimensions / 2.0f);
	Max = FindCenter() + (Dimensions / 2.0f);

}