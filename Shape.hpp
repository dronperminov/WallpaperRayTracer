#pragma once

#include <iostream>

class Shape {
protected:
	const double EPS = 1e-4; // маленькое число для сравнения вещественных чисел
	const double INF = 1e300; // бесконечность

public:
    Material material; // материал

    virtual bool Intersect(const Vector &O, const Vector &D, double& t) = 0; // проверка на пересечение с лучём
    virtual Vector Normal(const Vector &P) = 0; // получение нормали

    // получение цвета в точке P
    virtual Vector GetColor(const Vector &P) {
        return material.diffuse;
    }

    virtual std::string ToString() const = 0; // перевод в строку
};