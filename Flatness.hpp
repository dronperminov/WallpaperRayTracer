#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Shape.hpp"

// плоскость
class Flatness : public Shape {
protected:
	Vector n; // нормаль
	Vector size; // синусы нормали
	Vector center;
	double d; // смещение

public:
	Flatness(Vector center, double a, double b, double c, double size, Material material);
	Flatness(const std::string& s);

	bool Intersect(const Vector &O, const Vector &D, double &t); // пересечение луча и плоскости

	Vector Normal(const Vector &P); // получение нормали

	std::string ToString() const;
};

Flatness::Flatness(Vector center, double a, double b, double c, double size, Material material) {
	n = Vector(a, b, c);

	this->center = center;
	this->d = -(center * n) / n.Length();
	this->material = material;

	n = n.Normalize();

	this->size = Vector(sqrt(1 - n.x * n.x), sqrt(1 - n.y * n.y), sqrt(1 - n.z * n.z)) * size;
}

Flatness::Flatness(const std::string& s) {
	std::stringstream ss(s);
	std::vector<std::string> args;
	std::string arg;

	while (ss >> arg)
		args.push_back(arg);

	if (args.size() != 19)
		throw std::string("invalid flatness string: '") + s + "'";

	n.x = stod(args[1]);
	n.y = stod(args[2]);
	n.z = stod(args[3]);

	center.x = stod(args[4]);
	center.y = stod(args[5]);
	center.z = stod(args[6]);

	double size = stod(args[7]);

	double red = stod(args[8]) / 255;
	double green = stod(args[9]) / 255;
	double blue = stod(args[10]) / 255;

	Vector diffuse(red, green, blue);

	double specular1 = stod(args[11]);
	double specular2 = stod(args[12]);
	double specular3 = stod(args[13]);
	
	double refractive = stod(args[14]);

	double alb0 = stod(args[15]);
	double alb1 = stod(args[16]);
	double alb2 = stod(args[17]);
	double alb3 = stod(args[18]);

	material = Material(refractive, alb0, alb1, alb2, alb3, diffuse, specular1, specular2, specular3);

	d = -(center * n) / n.Length();
	n = n.Normalize();

	this->size = Vector(sqrt(1 - n.x * n.x), sqrt(1 - n.y * n.y), sqrt(1 - n.z * n.z)) * size;
}

// пересечение луча и плоскости
bool Flatness::Intersect(const Vector &O, const Vector &D, double &t) {
	double dir = D * n; // находим косинус между нарпавлением луча и нормалью

	// если косинус близок к нулю
	if (fabs(dir) < EPS)
		return false; // значит луч параллелен плоскости, пересечения нет

	t = (-d - O * n) / dir; // находим расстояние от начала луча до плоскости

	// находим точку пересечения луча с плоскостью
	Vector P = O + D * t - center;

	// если точка дальше размера плосокости
	if (fabs(P.x) > size.x + EPS || fabs(P.y) > size.y + EPS || fabs(P.z) > size.z + EPS)
		return false; // пересечения нет

	return true; // пересечение есть
}

// получение нормали
Vector Flatness::Normal(const Vector &P) {
	return n;
}

// flatness a b c x0 y0 z0 size red green blue specular1 specular2 specular3 refractive albedo0 albedo1 albedo2 albedo3
std::string Flatness::ToString() const {
	std::string flatness = "flatness ";

	flatness += std::to_string(n.x) + " ";
	flatness += std::to_string(n.y) + " ";
	flatness += std::to_string(n.z) + " ";

	flatness += std::to_string(center.x) + " ";
	flatness += std::to_string(center.y) + " ";
	flatness += std::to_string(center.z) + " ";

	flatness += std::to_string(size.x / sqrt(1 - n.x * n.x)) + " ";

	flatness += std::to_string((int) (material.diffuse.x * 255)) + " ";
	flatness += std::to_string((int) (material.diffuse.y * 255)) + " ";
	flatness += std::to_string((int) (material.diffuse.z * 255)) + " ";

	flatness += std::to_string(material.specular[0]) + " ";
	flatness += std::to_string(material.specular[1]) + " ";
	flatness += std::to_string(material.specular[2]) + " ";

	flatness += std::to_string(material.refractive) + " ";

	flatness += std::to_string(material.albedo[0]) + " ";
	flatness += std::to_string(material.albedo[1]) + " ";
	flatness += std::to_string(material.albedo[2]) + " ";
	flatness += std::to_string(material.albedo[3]);

	return flatness;
}