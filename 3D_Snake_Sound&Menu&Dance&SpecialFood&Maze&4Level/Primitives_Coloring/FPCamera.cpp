#include "FPCamera.h"

FPCamera::FPCamera(void)
{
	//1 - set the initial camera parameters
	//2 - set the projection 
	this->Reset(0, 0, 0, 0, 0, -1, 0, 1, 0);
	SetPerspectiveProjection(45.0f, 4.0f / 4.0f, 1.0f, 50.0f);
}

FPCamera::~FPCamera(void)
{
}

glm::vec3 FPCamera::GetLookDirection()
{
	//return the direction vector with (-ve*)
	return -mDirection;
}


void FPCamera::Reset(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	//1 - set the needed three "points/vector" (eyePt,centerPt,upVec)
	glm::vec3 eyePt(eyeX, eyeY, eyeZ);
	glm::vec3 centerPt(centerX, centerY, centerZ);
	glm::vec3 upPt(upX, upY, upZ);

	//2 - calling reset to construct the view matrix with the created vec
	Reset(eyePt, centerPt, upPt);
}

void FPCamera::Reset(const glm::vec3 &eye, const glm::vec3 &center, glm::vec3 &up)
{
	
	//1 - set the values for the our calss datamembers (mPosition, mDirection, mRight, mUp)
	//2 - normalize all vectors (mDirection, mRight, mUp)
	//3 - update view matrix
	mPosition = eye;
	mDirection = eye - center;
	mRight = glm::cross(up, mDirection);
	mUp = glm::cross(mDirection, mRight);
	mUp = glm::normalize(mUp);
	mRight = glm::normalize(mRight);
	mDirection = glm::normalize(mDirection);
	UpdateViewMatrix();
}

glm::mat4 FPCamera::GetViewMatrix()
{
	return mViewMatrix;
}

void FPCamera::UpdateViewMatrix()
{
	//1 - calculate the center point
	//2 - construct the view matrix by calling the lookAt function
	glm::vec3 center = mPosition + this->GetLookDirection();
	mViewMatrix = glm::lookAt(mPosition,center,mUp);
}

glm::mat4 FPCamera::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void FPCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
	mProjectionMatrix = glm::perspective(FOV,aspectRatio,near,far);
}

void FPCamera::Slide(float stepR, float stepU, float stepD)
{
	mPosition += stepR *mRight;
	mPosition += stepU * mUp;
	//next transition needs to be tested!!.
	mPosition += stepD * mDirection;
}

void FPCamera::Yaw(float angleDegrees)
{
	mDirection = glm::rotate(mDirection, angleDegrees, mUp);
	mRight = glm::rotate(mRight, angleDegrees, mUp);

}

void FPCamera::Pitch(float angleDegrees)
{
	mUp = glm::rotate(mUp, angleDegrees, mRight);
	mDirection = glm::rotate(mDirection, angleDegrees, mRight);
}

void FPCamera::Roll(float angleDegrees)
{
	mRight = glm::rotate(mRight, angleDegrees, mDirection);
	mUp = glm::rotate(mUp, angleDegrees, mDirection);
}

void FPCamera::Walk(float dist)
{
	mPosition -= dist * mDirection;
}

void FPCamera::Strafe(float dist)
{
	mPosition += dist * mRight;
}

void FPCamera::Fly(float dist)
{
	mPosition += dist * mUp;
}





