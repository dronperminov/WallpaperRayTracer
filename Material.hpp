#pragma once

#include <iostream>

// материал
struct Material {
	double refractive; // коэффициент преломления
	double albedo[4]; // коэффициент смешивания: diffuse specular reflected refracted
	Vector diffuse; // диффузный цвет
	double specular[3]; // зеркальность

	Material();
	Material(double refractive, double alb0, double alb1, double alb2, double alb3, Vector diffuse, double specular1, double specular2, double specular3);

	// получение случайного материала
	static Material GetRandom();
};

Material::Material() {
	refractive = 1;

	albedo[0] = 1;	
	albedo[1] = 0;	
	albedo[2] = 0;	
	albedo[3] = 0;

	diffuse.x = 1;
	diffuse.y = 1;
	diffuse.z = 1;

	specular[0] = 0;
	specular[1] = 0;
	specular[2] = 0;
}

Material::Material(double refractive, double alb0, double alb1, double alb2, double alb3, Vector diffuse, double specular1, double specular2, double specular3) {
	this->refractive = refractive;
	
	albedo[0] = alb0;
	albedo[1] = alb1;
	albedo[2] = alb2;
	albedo[3] = alb3;

	this->diffuse = diffuse;
	
	this->specular[0] = specular1;
	this->specular[1] = specular2;
	this->specular[2] = specular3;
}

Material Material::GetRandom() {
	Material material;

	material.refractive = 0.75 + (rand() * 0.5) / RAND_MAX;
	material.diffuse = Vector::Random();

	material.specular[0] = rand() % 2000;
	material.specular[1] = rand() % 2000;
	material.specular[2] = rand() % 2000;

	material.albedo[0] = ((double)rand()) / RAND_MAX;
	material.albedo[1] = ((double)rand()) / RAND_MAX * 10;
	material.albedo[2] = ((double)rand()) / RAND_MAX;
	material.albedo[3] = ((double)rand()) / RAND_MAX;

	return material;
}