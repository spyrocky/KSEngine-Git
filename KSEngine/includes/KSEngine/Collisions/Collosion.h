#pragma once
#include "KSEngine/CoreMinimal.h"
#include "KSEngine/Math/Transformations.h"

class Collision
{
public:
	Collision(Vector3 Location, Vector3 Offset);
	~Collision();
	//detect if two collisions are overlaping
	virtual bool IsOverlapping(Collision& OtherCol) { return false; }

	//get the center of the collision
	virtual Vector3 FindCenter() { return Vector3(0.0f); }

	//draw the collision for debuging
	virtual void DebugDraw(Vector3 Colour) {}

	//transform function
	//set the location of the collision
	virtual void SetLocation(Vector3 NewLocation) { Transform.Location = NewLocation; }
	// update the offset
	virtual void SetOffset(Vector3 NewOffset) { Offset = NewOffset; }
	// set the rotation of the collision
	virtual void SetRotation(Vector3 NewRotation) { Transform.Rotation = NewRotation; }
	// set the rotation of X the collision
	virtual void SetRotationX(float Value) { Transform.Rotation.x = Value; }
	// set the rotation of Y the collision
	virtual void SetRotationY(float Value) { Transform.Rotation.y = Value; }
	// set the rotation of Z the collision
	virtual void SetRotationZ(float Value) { Transform.Rotation.z = Value; }

protected:
	//transform for the collision world space
	CTransform Transform;

	//Offset for center location
	Vector3 Offset;
	// Visual mesh for debugging
	MeshPtr DebugMesh;
	ShaderPtr DebugShader;
	MaterialPtr DebugMaterial;
};

class BoxCollision :
	public Collision {
public:
	BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimensions);

	bool IsOverlapping(Collision& OtherCol) override;
	Vector3 FindCenter() override;
	//draw a mwsh to the dimension of the box
	//turn the mesh into a wireframe view to not obscure the actual object.
	//create the mesh ans a shader only if we need to debug the collision
	void DebugDraw(Vector3 Colour) override;
	void SetLocation(Vector3 NewLocation) override;
	
	//return the size of the box
	Vector3 GetDimensions() const { return Dimensions; }
	//set the dimention of the box 
	void SetDimensions(Vector3 NewDimensions);

protected:
	//the bounding box value
	Vector3 Dimensions;
	//the bounding vectors between the minimum and maximum size of the collision
	Vector3 Min;
	Vector3 Max;
};