#pragma once
#include "glm/glm.hpp"

typedef glm::vec3 Vector3;

struct CTransform {
	Vector3 Location = Vector3(0.0f);
	Vector3 Rotation = Vector3(0.0f);
	Vector3 Scale = Vector3(1.0f);

	//create the boolean operations of CTransform
	bool operator==(CTransform& OtherT) {
		return Location == OtherT.Location && Rotation == OtherT.Rotation && Scale == OtherT.Scale;
	}

	bool operator!=(CTransform& OtherT) {
		return Location != OtherT.Location || Rotation != OtherT.Rotation || Scale != OtherT.Scale;
	}
};