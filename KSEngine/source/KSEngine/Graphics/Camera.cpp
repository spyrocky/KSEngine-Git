#include "KSEngine/Graphics/Camera.h"

Camera::Camera()
{
	Transform.Location = Vector3(0.0f,0.0f,-2.0f);
}

void Camera::Translate(Vector3 Location)
{
	Transform.Location = Location;
}

void Camera::UpdateDirectionVectors()
{
	Vector3 ForwardDirection{};

	ForwardDirection.x = cos(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));

	ForwardDirection.y = sin(glm::radians(Transform.Rotation.x));

	ForwardDirection.z = sin(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));

	ForwardDirection = glm::normalize(ForwardDirection);

	Direction.Forward = ForwardDirection;



}
