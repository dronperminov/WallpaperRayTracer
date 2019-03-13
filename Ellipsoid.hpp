#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Shape.hpp"

class Ellipsoid : public Shape {
	double a, b, c; // радиусы
	double aabb, aacc, bbcc, aabbcc;
	
	Vector center;

public:
	Ellipsoid(double x, double y, double z, double a, double b, double c, Material material);
	Ellipsoid(const std::string &s);
	
	bool Intersect(const Vector &O, const Vector &D, double &t); // пересечение сферы с лучём
	bool Intersect(double x, double y, double z, double R);

	Vector Normal(const Vector &P); // получение нормали в точке

	std::string ToString() const;
};

Ellipsoid::Ellipsoid(double x, double y, double z, double a, double b, double c, Material material) {
	center = Vector(x, y, z);

	this->a = a;
	this->b = b;
	this->c = c;

	aabb = a * a * b * b;
	aacc = a * a * c * c;
	bbcc = b * b * c * c;
	aabbcc = a * a * b * b * c * c;

	this->material = material;
}

Ellipsoid::Ellipsoid(const std::string &s) {
	std::stringstream ss(s);
	std::vector<std::string> args;
	std::string arg;

	while (ss >> arg)
		args.push_back(arg);

	if (args.size() != 18)
		throw std::string("invalid ellipsoid string: '") + s + "'";

	center.x = stod(args[1]);
	center.y = stod(args[2]);
	center.z = stod(args[3]);

	a = stod(args[4]);
	b = stod(args[5]);
	c = stod(args[6]);

	aabb = a * a * b * b;
	aacc = a * a * c * c;
	bbcc = b * b * c * c;
	aabbcc = a * a * b * b * c * c;

	double red = stod(args[7]) / 255;
	double green = stod(args[8]) / 255;
	double blue = stod(args[9]) / 255;

	Vector diffuse(red, green, blue);

	double specular1 = stod(args[10]);
	double specular2 = stod(args[11]);
	double specular3 = stod(args[12]);
	double refractive = stod(args[13]);

	double alb0 = stod(args[14]);
	double alb1 = stod(args[15]);
	double alb2 = stod(args[16]);
	double alb3 = stod(args[17]);

	material = Material(refractive, alb0, alb1, alb2, alb3, diffuse, specular1, specular2, specular3);
}

// пересечение сферы с лучём
bool Ellipsoid::Intersect(const Vector &O, const Vector &D, double &t) {
	double x10 = O.x - center.x;
	double y10 = O.y - center.y;
	double z10 = O.z - center.z;

	double a = bbcc * D.x * D.x + aacc * D.y * D.y + aabb * D.z * D.z;
	double b = (bbcc * x10 * D.x + aacc * y10 * D.y + aabb * z10 * D.z);
	double c = bbcc * x10 * x10 + aacc * y10 * y10 + aabb * z10 * z10 - aabbcc;

	double disc = b * b - a * c;

	if (disc < EPS) {
		t = INF; // t - бесконечность
		return false; // пересечения нет
	}

	disc = sqrt(disc); // извлекаем корень из дискриминанта

	t = (-b - disc) / a; // первый корень

	if (t < EPS) // если первое пересечение позади начала луча
		t = (-b + disc) / a; // второй корень; // считаем пересечением больший корень

	return t > EPS; // пересечение есть, если оно впереди луча
}

// получение нормали в точке
Vector Ellipsoid::Normal(const Vector &P) {
	return Vector((P.x - center.x) / a, (P.y - center.y) / b, (P.z - center.z) / c); // нормаль к сфере - вектор между точкой и центром сферы
}

// ellipsoid x y z a b c red green blue specular1 specular2 specular3 refractive albedo0 albedo1 albedo2 albedo3
std::string Ellipsoid::ToString() const {
	std::string ellipsoid = "ellipsoid ";

	ellipsoid += std::to_string(center.x) + " ";
	ellipsoid += std::to_string(center.y) + " ";
	ellipsoid += std::to_string(center.z) + " ";

	ellipsoid += std::to_string(a) + " ";
	ellipsoid += std::to_string(b) + " ";
	ellipsoid += std::to_string(c) + " ";

	ellipsoid += std::to_string((int) (material.diffuse.x * 255)) + " ";
	ellipsoid += std::to_string((int) (material.diffuse.y * 255)) + " ";
	ellipsoid += std::to_string((int) (material.diffuse.z * 255)) + " ";

	ellipsoid += std::to_string(material.specular[0]) + " ";
	ellipsoid += std::to_string(material.specular[1]) + " ";
	ellipsoid += std::to_string(material.specular[2]) + " ";

	ellipsoid += std::to_string(material.refractive) + " ";

	ellipsoid += std::to_string(material.albedo[0]) + " ";
	ellipsoid += std::to_string(material.albedo[1]) + " ";
	ellipsoid += std::to_string(material.albedo[2]) + " ";
	ellipsoid += std::to_string(material.albedo[3]);

	return ellipsoid;
}