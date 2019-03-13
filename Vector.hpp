#pragma once

#include <iostream>
#include <cmath>

struct Vector {
    double x;
    double y;
    double z;

    Vector(double x = 0, double y = 0, double z = 0);

    Vector operator-(const Vector& v) const; // разность векторов
    Vector operator+(const Vector& v) const; // сложение векторов

    double operator*(const Vector& v) const; // скалярное умножение векторов
    Vector operator^(const Vector& v) const; // векторное умножение векторов
    
    Vector operator*(double a) const; // умножение на скаляр
    Vector operator/(double a) const; // деление на скаляр

    Vector operator-() const; // инверсия вектора (умножение на -1)

    Vector RotateX(double alpha) const; // вращение относительно оси OX
    Vector RotateY(double alpha) const; // вращение относительно оси OY
    Vector RotateZ(double alpha) const; // вращение относительно оси OZ

    double Length() const; // получение длины
    Vector Normalize() const; // нормализация

    Vector ToColor() const;

    static Vector Zero();
    static Vector Random();
};

Vector::Vector(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

// разность векторов
Vector Vector::operator-(const Vector& v) const {
    return Vector(x - v.x, y - v.y, z - v.z);
}

// сложение векторов
Vector Vector::operator+(const Vector& v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
}

// скалярное умножение векторов
double Vector::operator*(const Vector& v) const {
    return x * v.x + y * v.y + z * v.z;
}

// векторное умножение векторов
Vector Vector::operator^(const Vector& v) const {
    return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

// умножение на скаляр
Vector Vector::operator*(double a) const {
    return Vector(x * a, y * a, z * a);
}

// деление на скаляр
Vector Vector::operator/(double a) const {
    return Vector(x / a, y / a, z / a);
}

// инверсия вектора (умножение на -1)
Vector Vector::operator-() const {
    return Vector(-x, -y, -z);
}

// вращение относительно оси OX
Vector Vector::RotateX(double alpha) const {
    return Vector(x, cos(alpha) * y - sin(alpha) * z, sin(alpha) * y + cos(alpha) * z);
}

// вращение относительно оси OY
Vector Vector::RotateY(double alpha) const {
    return Vector(cos(alpha) * x - sin(alpha) * z, y, sin(alpha) * x + cos(alpha) * z);
}

// вращение относительно оси OZ
Vector Vector::RotateZ(double alpha) const {
    return Vector(cos(alpha) * x - sin(alpha) * y, sin(alpha) * x + cos(alpha) * y, z);
}

// получение длины
double Vector::Length() const {
    return sqrt(x * x + y * y + z * z);
}

// нормализация
Vector Vector::Normalize() const {
    double len = Length();

    if (len == 0)
        len = 1;

    return Vector(x / len, y / len, z / len);
}

Vector Vector::ToColor() const {
    double r = x < 0 ? 0 : x > 1 ? 255 : x * 255;
    double g = y < 0 ? 0 : y > 1 ? 255 : y * 255;
    double b = z < 0 ? 0 : z > 1 ? 255 : z * 255;

    return Vector(r, g, b);
}

Vector Vector::Zero() {
    return Vector(0, 0, 0);
}

Vector Vector::Random() {
    double x = ((double) rand()) / RAND_MAX;
    double y = ((double) rand()) / RAND_MAX;
    double z = ((double) rand()) / RAND_MAX;

    return Vector(x, y, z);
}