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
	
	// move camera in 3d space to alocation
	void Translate(Vector3 Location);

	//Find the current direction vectors based on the rotation of YAW and PITCH of the camera
	void UpdateDirectionVectors();

	//return the transform data
	CTransform GetTransforms() const { return Transform; }
	//return the current facing directions
	CDirection GetDirection() const { return Directions; }

	//return the extra camera infor
	// speed , FOV, near clip, far clip
	STCameraData GetCameraData() const { return CameraData;  }
	//return a view matrix that considers positions and rotations
	glm::mat4 GetViewMatrix() const;
	//rotate the camera based on yaw
	void RotatePitch(float Amount);

	//rotate the camera based on pitch
	void RotateYaw(float Amount);

private:
	//Find the current direction vectors based on the rotation of YAW and PITCH of the camera
	//void UpdateDirectionVectors();

private:
	// trasform of the camenra
	// locartion , rotation , scale - we dont need that
	CTransform Transform;

	//forward , right , up directions
	CDirection Directions;
	
	// hold all the camera effect
	STCameraData CameraData;
};