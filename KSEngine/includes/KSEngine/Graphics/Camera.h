#pragma once
#include "KSEngine/Math/Transformations.h"

struct STCameraData {
	//how fast the camera can move
	float Speed = 5.0f;
	//Basically the zoom
	//How much of view space the camera can see - in degree
	// think pereipheral vision
	float FOV = 70.0f;
	//how close can models get to the camera before disappearing
	float NearClip = 0.01f;
	// * opposite of near clip *
	float FarClip = 1000.0f;
};

class Camera {
public:
	Camera();
	~Camera();

	//return the transform data
	CTransform GetTransforms() const { return Transform; }
	//return the current facing directions
	CDirection GetDirection() const { return Direction; }

	//STCameraData 

private:
	// trasform of the camenra
	// locartion , rotation , scale - we dont need that
	CTransform Transform;

	//forward , right , up directions
	CDirection Direction;
	
	// hold all the camera effect
	STCameraData Cameradata;
};