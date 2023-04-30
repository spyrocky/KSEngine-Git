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
	//create a vector 3 top initialise a 0 direction
	Vector3 ForwardDirection;

	//cosine of the Yaw * cosine of the Pitch
	ForwardDirection.x = cos(glm::radians(Transform.Rotation.y)) *
		cos(glm::radians(Transform.Rotation.x));
	//sin of the pitch
	ForwardDirection.y = sin(glm::radians(Transform.Rotation.x));
	//sin of the Yaw and the cosine of the Pitch
	ForwardDirection.z = sin(glm::radians(Transform.Rotation.y)) *
		cos(glm::radians(Transform.Rotation.x));

	//normalise the direction to update the values to be between 0 and 1
	ForwardDirection = glm::normalize(ForwardDirection);

	//set the forward direction
	Directions.Forward = ForwardDirection;

	//cross product - will allow us to get our right and up vectors from the forward vector
	//cross product is the axis that is perpendicular to two other axis
	//set the right direction to always be perpendicular to the world up
	Directions.Right = glm::normalize(glm::cross(Directions.Forward, Vector3(0.0f, 1.0f, 0.0f)));
	//up direction is based on the local rotation of the forward and right directions
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
	Transform.Rotation.x += Amount * CameraData.LookSensetivity;

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
		//CameraCollision->DebugDraw(Vector3(255.0f));
		//cout << "Camera collision is running..." << endl;
	}
}
