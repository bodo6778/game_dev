#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Globals.h"

class Camera
{
	Vector3 target, position, up, xAxis, yAxis, zAxis;
	Matrix viewMatrix, worldMatrix, projectionMatrix;
	GLfloat moveSpeed, rotateSpeed, nearP, farP, fov, deltaTime;
public:
	// Has default values.
	Camera(Vector3& position = Vector3{ 0.0f, 0.0f, 0.0f }, Vector3& target = Vector3{ 0.0f, 0.0f, 0.0f }, Vector3& up = Vector3{ 0.0f, 1.0f, 0.0f }, GLfloat moveSpeed = 3.0f, GLfloat rotateSpeed = 3.0f, GLfloat nearP = 0.2f, GLfloat farP = 1000.0f, GLfloat fov = 45.0f, GLfloat deltaTime = 0.0f);

	void moveOz(GLint directie);
	void moveOx(GLint directie);
	void moveOy(GLint directie);
	void rotateOy(GLint directie);
	void rotateOx(GLint directie);
	void rotateOz(GLint directie);

	// Getters and setters.
	void updateWorldView();

	Matrix& getViewMatrix();
	Matrix& getWorldMatrix();
	Matrix& getProjMatrix();

	Vector3 getTarget();
	void setTarget(Vector3&);

	Vector3 getPosition();
	void setPosition(Vector3&);

	Vector3 getUp();
	void setUp(Vector3&);

	GLfloat getRotateSpeed() const;
	void setRotateSpeed(GLfloat);

	GLfloat getDeltaTime() const;
	void setDeltaTime(GLfloat);

	GLfloat getMoveSpeed() const;
	void setMoveSpeed(GLfloat);

	GLfloat getNear() const;
	void setNear(GLfloat);

	GLfloat getFar() const;
	void setFar(GLfloat);

	GLfloat getFov() const;
	void setFov(GLfloat);

private:
	void refreshAxis();
};
