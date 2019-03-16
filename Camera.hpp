#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "Vector.hpp"

class Camera {
	double PI = 3.141592653589793238;
	double PI2 = PI * 2;

	Vector position;
	Vector angle;

	Vector Sin, Cos;

	void UpdateSinCos();

public:
	Camera();
	Camera(const Vector &position, const Vector &angle);
	Camera(const std::string& s);
	Camera(const Camera& camera);

	Vector GetDir(double x, double y); // получение направления луча из камеры

	const Vector& GetPosition() const;
	const Vector& GetAngle() const;

	void Rotate(double x, double y); // вращение камеры на x y по осям Ox, Oy
	
	void Left(double movement = 0.5); // движение влево
	void Right(double movement = 0.5); // движение вправо

	void Up(double movement = 0.5); // движение вверх
	void Down(double movement = 0.5); // движение вниз

	void Forward(double movement = 0.5); // движение вперёд
	void Backward(double movement = 0.5); // движение назад

	std::string ToString() const;
};

Camera::Camera() {
	position = Vector(0, 0, 0);
	angle = Vector(0, 0, 0);

	UpdateSinCos();
}

Camera::Camera(const Vector &position, const Vector &angle) {
	this->position = position;
	this->angle = angle;

	UpdateSinCos();
}

Camera::Camera(const std::string& s) {
	std::stringstream ss(s);
	std::vector<std::string> args;
	std::string arg;

	while (ss >> arg)
		args.push_back(arg);

	if (args.size() != 6)
		throw std::string("invalid camera string: '") + s + "'";

	position.x = stod(args[1]);
	position.y = stod(args[2]);
	position.z = stod(args[3]);

	angle.x = stod(args[4]);
	angle.y = stod(args[5]);
	angle.z = 0;

	UpdateSinCos();
}

Camera::Camera(const Camera& camera) {
	position = camera.position;
	angle = camera.angle;

	UpdateSinCos();
}

void Camera::UpdateSinCos() {
	Sin.x = sin(angle.x);
	Sin.y = sin(angle.y);

	Cos.x = cos(angle.x);
	Cos.y = cos(angle.y);
}

// получение направления луча из камеры
Vector Camera::GetDir(double x, double y) {
	double y1 = Cos.x * y + Sin.x;
	double z1 = -Sin.x * y + Cos.x;

	double x11 = Cos.y * x + Sin.y * z1;
	double z11 = -Sin.y * x + Cos.y * z1;

	double len = sqrt(x11 * x11 + y1 * y1 + z11 * z11);

	return Vector(x11 / len, y1 / len, z11 / len);
}

const Vector& Camera::GetPosition() const {
	return position;
}

const Vector& Camera::GetAngle() const {
	return angle;
}

// вращение камеры на x y по осям Ox, Oy
void Camera::Rotate(double x, double y) {
	angle.x = fmod(angle.x + x + PI2, PI2);
	angle.y = fmod(angle.y + y + PI2, PI2);

	UpdateSinCos();
}

// движение влево
void Camera::Left(double movement) {
	position.x -= cos(angle.y) * movement;
	position.z += sin(angle.y) * movement;
}

// движение вправо
void Camera::Right(double movement) {
	position.x += cos(angle.y) * movement;
	position.z -= sin(angle.y) * movement;
}

// движение вверх
void Camera::Up(double movement) {
	position.y += movement;
}

// движение вниз
void Camera::Down(double movement) {
	position.y -= movement;
}

// движение вперёд
void Camera::Forward(double movement) {
	position.y += sin(angle.x) * movement;
	position.x -= cos(angle.y + PI / 2) * cos(angle.x) * movement;
	position.z += sin(angle.y + PI / 2) * cos(angle.x) * movement;
}

// движение назад
void Camera::Backward(double movement) {
	position.y -= sin(angle.x) * movement;
	position.x -= cos(angle.y - PI / 2) * cos(angle.x) * movement;
	position.z += sin(angle.y - PI / 2) * cos(angle.x) * movement;
}

std::string Camera::ToString() const {
	std::string camera = "camera ";
	camera += std::to_string(position.x) + " ";
	camera += std::to_string(position.y) + " ";
	camera += std::to_string(position.z) + " ";

	camera += std::to_string(angle.x) + " ";
	camera += std::to_string(angle.y);

	return camera;
}