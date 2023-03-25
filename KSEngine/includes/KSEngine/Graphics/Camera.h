#pragma once
#include "KSEngine/Math/Transformations.h"

class Camera {
public:
	Camera();
	~Camera();

private:
	// trasform of the camenra
	// locartion , rotation , scale - we dont need that
	CTransform Transform;

	//forward , right , up directions
	CDirection Direction;
	//how fast the camera can move
	float Speed;
	//Basically the zoom
	//How much of view space the camera can see - in degree
	// think pereipheral vision
	float FOV;
	//how close can models get to the camera before disappearing
	float NearClip;
	// * opposite of near clip *
	float FarClip;
};