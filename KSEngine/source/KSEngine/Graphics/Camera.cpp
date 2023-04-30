#include "KSEngine/Graphics/Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "KSEngine/Game.h"
#include "KSEngine/Collisions/Collosion.h"


Camera::Camera()
{
	
	UpdateDirectionVectors();

	Transform.Location += -Directions.Forward * 2.0f;

	//@Param1 - Position of collision
	//@Param2 - Offset of the location
	//@Param3 - size of the camera colision
	CameraCollision = make_shared<BoxCollision>(Transform.Location,Vector3(0.0f),Vector3(1.0f));

}

void Camera::Translate(Vector3 Location)
{
	Transform.Location = Location;

	UpdateDirectionVectors();
}

void Camera::UpdateDirectionVectors()
{
	Vector3 ForwardDirection{};

	ForwardDirection.x = cos(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));

	ForwardDirection.y = sin(glm::radians(Transform.Rotation.x));

	ForwardDirection.z = sin(glm::radians(Transform.Rotation.y)) * cos(glm::radians(Transform.Rotation.x));

	ForwardDirection = glm::normalize(ForwardDirection);

	Directions.Forward = ForwardDirection;

	Directions.Right = glm::normalize(glm::cross(Directions.Forward, Vector3(0.0f,1.0f,0.0f)));

	Directions.Up = glm::normalize(glm::cross(Directions.Right, Directions.Forward));

}

void Camera::AddMovementInput(Vector3 Direction)
{
	//ignore the rest of the function if no direction is given
	if (glm::length(Direction) == 0)
		return;

	//divide the vector by its length
	//dont normalize if the direction is 0
	Direction = glm::normalize(Direction);

	//set the velocity of the camera using speed and input direction
	Vector3 Vel = Direction * (CameraData.Speed * Game::GetGameInstance().GetFDeltaTime());

	//create a new location from the camera based on its position and the current velocity
	Vector3 NewPosition = Transform.Location + Vel;

	//make sure the camera has actually been told to move
	if (Transform.Location != NewPosition) {
		//move camera to the new position
		Translate(NewPosition);
	}
}

glm::mat4 Camera::GetViewMatrix() const
{
	// eye is the view position od the camera
	// center is ewhat the eye is looking at so we cam silply plus the ryr position by the forward direction
	// - as the forward directin is always normalized to a max of 1
	// the up direction is the location up
	return glm::lookAt(Transform.Location, Transform.Location + Directions.Forward, Directions.Up);
}

void Camera::RotatePitch(float Amount)
{
	//rotating -89 or +89 will result in yaw flip and flip the cam
	Transform.Rotation.x += Amount;

	//clamp the result between two max values to avoid the flip
	if (Transform.Rotation.x > 89.0f)
		Transform.Rotation.x = 89.0f;

	if (Transform.Rotation.x < -89.0f)
		Transform.Rotation.x = -89.0f;

	UpdateDirectionVectors();
}

void Camera::RotateYaw(float Amount)
{
	//can currently increase to a max number
	Transform.Rotation.y += Amount * CameraData.LookSensetivity;

	//when the yaw gets to 360 change it to 0
	Transform.Rotation.y = glm::mod(Transform.Rotation.y, 360.0f);

	UpdateDirectionVectors();
}

void Camera::Update()
{
	if (CameraCollision != nullptr) {
		CameraCollision->SetLocation(Transform.Location);
		CameraCollision->DebugDraw(Vector3(255.0f));

		//cout << "Camera collision is running..." << endl;
	}
}
