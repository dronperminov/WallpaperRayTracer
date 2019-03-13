#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Vector.hpp"

// тип источника света
enum class LightType {
    Directional, // направленный источник
    Point, // точечный источник
    Ambient // постоянный источник
};

struct Light {
    LightType type; // тип источника
    double intensity; // интенсивность
    Vector color;
    Vector v; // направление/положение

    Light(LightType type, double intensity, Vector v, Vector color);
    Light(const std::string& s);

    std::string ToString() const;
};

Light::Light(LightType type, double intensity, Vector v, Vector color) {
    this->type = type;
    this->intensity = intensity;

    if (type == LightType::Directional) {
        this->v = v.Normalize(); // нормализуем вектор для направленного источника света
    }
    else {
        this->v = Vector(v.x, v.y, v.z);
    }

    this->color = color;
}

Light::Light(const std::string& s) {
    std::stringstream ss(s);
    std::vector<std::string> args;
    std::string arg;

    while (ss >> arg)
        args.push_back(arg);

    if (args.size() != 9 && args.size() != 6)
        throw std::string("invalid light string: '") + s + "'";

    if (args[1] == "dir") {
        type = LightType::Directional;
    }
    else if (args[1] == "point") {
        type = LightType::Point;
    }
    else if (args[1] == "ambient") {
        type = LightType::Ambient;
    }
    else {
        throw std::string("invalid LightType");
    }

    intensity = stod(args[2]);

    color.x = stod(args[3]) / 255;
    color.y = stod(args[4]) / 255;
    color.z = stod(args[5]) / 255;

    if (type != LightType::Ambient) {
        v.x = stod(args[6]);
        v.y = stod(args[7]);
        v.z = stod(args[8]);
    }

    if (type == LightType::Directional)
        v = v.Normalize();
}

std::string Light::ToString() const {
    std::string light = "light ";

    if (type == LightType::Ambient) {
        light += "ambient ";
    }
    else if (type == LightType::Point) {
        light += "point ";
    }
    else if (type == LightType::Directional) {
        light += "dir ";
    }

    light += std::to_string(intensity) + " ";
    light += std::to_string((int) (255 * color.x)) + " ";
    light += std::to_string((int) (255 * color.y)) + " ";
    light += std::to_string((int) (255 * color.z)) + " ";

    light += std::to_string(v.x) + " ";
    light += std::to_string(v.y) + " ";
    light += std::to_string(v.z) + " ";

    return light;
}