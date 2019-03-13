#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Shape.hpp"

class Sphere : public Shape {
	double r; // радиус
	double rr; // радиус в квадрате
	Vector center;

public:
	Sphere(double x, double y, double z, double r, Material material);
	Sphere(const std::string &s);
	
	bool Intersect(const Vector &O, const Vector &D, double &t); // пересечение сферы с лучём
	bool Intersect(double x, double y, double z, double R);

	Vector Normal(const Vector &P); // получение нормали в точке

	std::string ToString() const;
};

Sphere::Sphere(double x, double y, double z, double r, Material material) {
	center = Vector(x, y, z);

	this->r = r;
	this->material = material;

	rr = r * r; // вычисляем квадрат радиуса
}

Sphere::Sphere(const std::string &s) {
	std::stringstream ss(s);
	std::vector<std::string> args;
	std::string arg;

	while (ss >> arg)
		args.push_back(arg);

	if (args.size() != 16)
		throw std::string("invalid sphere string: '") + s + "'";

	center.x = stod(args[1]);
	center.y = stod(args[2]);
	center.z = stod(args[3]);

	r = stod(args[4]);
	rr = r * r;

	double red = stod(args[5]) / 255;
	double green = stod(args[6]) / 255;
	double blue = stod(args[7]) / 255;

	Vector diffuse(red, green, blue);

	double specular1 = stod(args[8]);
	double specular2 = stod(args[9]);
	double specular3 = stod(args[10]);
	double refractive = stod(args[11]);

	double alb0 = stod(args[12]);
	double alb1 = stod(args[13]);
	double alb2 = stod(args[14]);
	double alb3 = stod(args[15]);

	material = Material(refractive, alb0, alb1, alb2, alb3, diffuse, specular1, specular2, specular3);
}

// пересечение сферы с лучём
bool Sphere::Intersect(const Vector &O, const Vector &D, double &t) {
	Vector OC = O - center;

	double k2 = -(OC * D); // второй коэффициент квадратного уравнения
	double disc = k2 * k2 - OC * OC + rr; // считаем дискриминант

	// если он отрицательный
	if (disc < EPS) {
		t = INF; // t - бесконечность
		return false; // пересечения нет
	}

	disc = sqrt(disc); // извлекаем корень из дискриминанта

	t = k2 - disc; // первый корень

	if (t < EPS) // если первое пересечение позади начала луча
		t = k2 + disc; // второй корень; // считаем пересечением больший корень

	return t > EPS; // пересечение есть, если оно впереди луча
}

bool Sphere::Intersect(double x, double y, double z, double R) {
	double dst = sqrt((center.x - x) * (center.x - x) + (center.y - y) * (center.y - y) + (center.z - z) * (center.z - z));

	return dst <= r + R;
}

// получение нормали в точке
Vector Sphere::Normal(const Vector &P) {
	return Vector((P.x - center.x) / r, (P.y - center.y) / r, (P.z - center.z) / r); // нормаль к сфере - вектор между точкой и центром сферы
}

// shpere x y z r red green blue specular1 specular2 specular3 refractive albedo0 albedo1 albedo2 albedo3
std::string Sphere::ToString() const {
	std::string sphere = "sphere ";

	sphere += std::to_string(center.x) + " ";
	sphere += std::to_string(center.y) + " ";
	sphere += std::to_string(center.z) + " ";

	sphere += std::to_string(r) + " ";

	sphere += std::to_string((int) (material.diffuse.x * 255)) + " ";
	sphere += std::to_string((int) (material.diffuse.y * 255)) + " ";
	sphere += std::to_string((int) (material.diffuse.z * 255)) + " ";

	sphere += std::to_string(material.specular[0]) + " ";
	sphere += std::to_string(material.specular[1]) + " ";
	sphere += std::to_string(material.specular[2]) + " ";

	sphere += std::to_string(material.refractive) + " ";

	sphere += std::to_string(material.albedo[0]) + " ";
	sphere += std::to_string(material.albedo[1]) + " ";
	sphere += std::to_string(material.albedo[2]) + " ";
	sphere += std::to_string(material.albedo[3]);

	return sphere;
}