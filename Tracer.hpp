#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "Vector.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Ellipsoid.hpp"
#include "Flatness.hpp"

#include "bitmap/bitmap_image.hpp"

using namespace std;
using namespace chrono;

class Tracer {
	const double EPS = 1e-5; // маленькое число для сравнения вещественных чисел
	const double INF = 1e300; // бесконечность

	const int DEPTH = 1; // начальная глубина трассировки лучей
	const bool SHOW_INFO = false; // отображать ли информацию на картинке

	Vector background = Vector(0.0, 0.0, 0.0);

	int width; // ширина картинки
	int height; // высота картинки

	Camera camera; // камера
	vector<Shape*> shapes; // фигуры сцены
	vector<Light> lights; // источники освещения

	double **X; // координаты X
	double **Y; // координаты Y
	Vector **buffer; // буфер для запоминания цвета лучей

	bool RefractRay(const Vector &I, const Vector &N, double cosi, double eta_t, double eta_i, Vector &D); // преломление по закону Снеллиуса
	
	void ClosestIntersection(const Vector &O, const Vector &D, double tmin, double tmax, int &closestIndex, double &closest_t); // нахождение ближайшего пересечения с объектом
	bool HaveIntersection(const Vector &O, const Vector &D, double tmin, double tmax); // проверка наличия пересечения луча с каким-либо объектом сцены

	Vector TraceRay(const Vector &O, const Vector &D, double tmin, double tmax, int depth); // трассировка луча из точки O в направлении D

	void ComputeLighting(const Vector &P, const Vector &N, const Vector &D, const Material &material, Vector &diffuse, Vector &specular); // обсчёт освещённости точки P с нормалью N

public:
	Tracer(int width, int height);
    Tracer(const string& path);

	void InitScene(int k = -1); // инициализация сцены (камера, фигуры, источники света)
	void ChangeMaterials();

    void SaveScene(const string& path) const;

    int GetWidth() const;
    int GetHeight() const;

	void Trace(const string& path); // трассировка лучей и отображение результата на экране
};

Tracer::Tracer(int width, int height) {
	this->width = width; // получаем ширину
	this->height = height; // получаем высоту

	buffer = new Vector*[width]; // создаём цветовой буфер

	X = new double*[width]; // создаём массив координат по X
	Y = new double*[width]; // создаём массив координат по Y

	int size = width > height ? width : height;

	for (int i = 0; i < width; i++) {
		X[i] = new double[height];
		Y[i] = new double[height];
		buffer[i] = new Vector[height];

		for (int j = 0; j < height; j++) {
			X[i][j] = (double)(i) / size - 0.5;
			Y[i][j] = 0.5 - (double)(j) / size;
		}
	}

	Vector camPos(0, 0.9135, -2.1313);
	Vector camAngle(5.8, 0, 0);

	camera = Camera(camPos, camAngle); // создаём камеру
}

Tracer::Tracer(const string& path) {
    ifstream reader(path);

    if (!reader)
        throw string("incorrect path to file with scene");

    string line;
    width = 0;
    height = 0;

    while (getline(reader, line)) {
        if (line == "" || line[0] == '#')
            continue;

        if (line.substr(0, 7) == "camera ") {
            camera = Camera(line); // создаём камеру
        }
        else if (line.substr(0, 6) == "light ") {
            lights.push_back(Light(line));
        }
        else if (line.substr(0, 7) == "sphere ") {
            shapes.push_back(new Sphere(line));
        }
        else if (line.substr(0, 10) == "ellipsoid ") {
            shapes.push_back(new Ellipsoid(line));
        }
        else if (line.substr(0, 9) == "flatness ") {
            shapes.push_back(new Flatness(line));
        }
        else if (line.substr(0, 6) == "width ") {
            width = stoi(line.substr(5));
        }
        else if (line.substr(0, 7) == "height ") {
            height = stoi(line.substr(6));
        }
        else
            throw std::string("invalid line '") + line + "'";
    }

    if (width <= 0 || height <= 0)
        throw string("invalid width or height value(s)");

    buffer = new Vector*[width]; // создаём цветовой буфер

    X = new double*[width]; // создаём массив координат по X
    Y = new double*[width]; // создаём массив координат по Y

    int size = width > height ? width : height;

    for (int i = 0; i < width; i++) {
        X[i] = new double[height];
        Y[i] = new double[height];
        buffer[i] = new Vector[height];

        for (int j = 0; j < height; j++) {
            X[i][j] = (double)(i) / size - 0.5;
            Y[i][j] = 0.5 - (double)(j) / size;
        }
    }
}

// инициализация сцены (камера, фигуры, источники света)
void Tracer::InitScene(int k) {
    if (k == -1) {
	   k = time(0);
    }

    srand(k);

	cout << "k: " << k << "\t";

	shapes.clear();
	lights.clear();

	shapes.push_back(new Flatness(Vector(0, 0, 0), 0, 1, 0, 10, Material::GetRandom()));

	vector<Sphere*> spheres;

	int n = 5 + rand() % 5;

	for (int i = 0; i < n; i++) {
		double r = 0.15 + (((double)rand()) / RAND_MAX) * 0.15;

		double x = -0.9 + (((double)rand()) / RAND_MAX) * 1.8;
		double y = r;
		double z = -0.5 + (((double)rand()) / RAND_MAX) * 2;

		spheres.push_back(new Sphere(x, y, z, r, Material::GetRandom()));
		shapes.push_back(spheres[spheres.size() - 1]);

		lights.push_back(Light(LightType::Point, 0.5, Vector(x, y + r + EPS, z), Vector::Random()));
		lights.push_back(Light(LightType::Point, 0.5, Vector(x + r + EPS, y + r, z), Vector::Random()));
		lights.push_back(Light(LightType::Point, 0.5, Vector(x - r + EPS, y + r, z), Vector::Random()));
	}
}

void Tracer::SaveScene(const string& path) const {
    ofstream f(path);

    f << "width " << width << endl;
    f << "height " << height << endl << endl;
    f << camera.ToString() << endl << endl;

    for (size_t i = 0; i < lights.size(); i++)
        f << lights[i].ToString() << endl;

    f << endl;

    for (size_t i = 0; i < shapes.size(); i++)
        f << shapes[i]->ToString() << endl;

    f.close();
}

void Tracer::ChangeMaterials() {
	//int k = time(0);
	//srand(k);

	//cout << "Change materials: " << k << endl;

	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->material = Material::GetRandom();
}

int Tracer::GetWidth() const {
    return width;
}

int Tracer::GetHeight() const {
    return height;
}

// преломление по закону Снеллиуса
bool Tracer::RefractRay(const Vector &I, const Vector &N, double cosi, double eta_t, double eta_i, Vector &D) {
	double k;

	if (cosi < 0) {
		double eta = eta_t / eta_i;
		k = 1 - eta * eta * (1 - cosi * cosi);

		D = I * eta + N * (eta * cosi + sqrt(k));
	}
	else {
		double eta = eta_i / eta_t;
		k = 1 - eta * eta * (1 - cosi * cosi);

		D = I * eta + N * (eta * cosi - sqrt(k));
	} 

	return k >= 0;
}

// нахождение ближайшего пересечения с объектом
void Tracer::ClosestIntersection(const Vector &O, const Vector &D, double tmin, double tmax, int &closestIndex, double &closest_t) {
	closest_t = INF; // предполагаем, что нет пересечения, так что +бесконечность
	closestIndex = -1; // аналогично, предполагаем, что пересечения не будет

	// проходимся по всем фигурам
	for (size_t i = 0; i < shapes.size(); i++) {
		double t;
		// если есть пересечение и удовлетворяет параметрам луча (tmin, tmax) и меньше ближайшего
		if (shapes[i]->Intersect(O, D, t) && t >= tmin && t <= tmax && t < closest_t) {
			closest_t = t; // обновляем ближайшее расстоние
			closestIndex = i; // и запоминаем ближайшую фигуру
		}
	}
}

// проверка наличия пересечения луча с каким-либо объектом сцены
bool Tracer::HaveIntersection(const Vector &O, const Vector &D, double tmin, double tmax) {
	for (size_t i = 0; i < shapes.size(); i++) {
		double t;

		if (shapes[i]->Intersect(O, D, t) && t >= tmin && t <= tmax)
			return true; // если нашли хоть ождно пересечение, удовлетворяющее границам интервала, возврашаем истину
	}

	return false; // ничего не нашли, возвращаем ложь
}

// трассировка луча из точки O в направлении D
Vector Tracer::TraceRay(const Vector &O, const Vector &D, double tmin, double tmax, int depth) {
	int closestIndex = -1;
	double closest_t;

	ClosestIntersection(O, D, tmin, tmax, closestIndex, closest_t); // находим ближайший объект, с которым пересекается луч

	// если луч ни с чем не пересекается
	if (closestIndex == -1)
		return background; // возвращаем цвет фона

	// находим точку перемесения луча с объектом
	Vector P = O + D * closest_t;

	Vector N = shapes[closestIndex]->Normal(P); // получаем нормаль в этой точке
	Vector color = shapes[closestIndex]->GetColor(P); // получаем цвет объекта в точке
	Material m = shapes[closestIndex]->material; // получаем материал ближайшего объекта

	Vector diffuse, specular;
	ComputeLighting(P, N, D, m, diffuse, specular); // вычисляем освещённости источников света

	Vector localColor; // локальный цвет луча

	// диффузный и зеркальный цвет 
	localColor.x = color.x * diffuse.x + specular.x;
	localColor.y = color.y * diffuse.y + specular.y;
	localColor.z = color.z * diffuse.z + specular.z;

	if (depth < 0) // если достигли минимальной глубины
		return localColor; // то возвращаем диффузный и зеркальный цвет 

	double DN = D * N; // косинус угла между направлением луча и нормалью

	if (m.albedo[2] != 0) {
		Vector reflectD = D - N * (2 * DN); // находим направление луча отражения
		Vector reflectColor = TraceRay(P, reflectD, EPS, INF, depth - 1);

		// добавляем отражённый цвет
		localColor.x += reflectColor.x * m.albedo[2];
		localColor.y += reflectColor.y * m.albedo[2];
		localColor.z += reflectColor.z * m.albedo[2];
	}

	// находим направление луча преломления
	Vector refractD;

	if (m.albedo[3] != 0 && RefractRay(D, N, -DN, m.refractive, 1, refractD)) {				
		Vector refractColor = TraceRay(P, refractD, EPS, INF, depth - 1);

		// добавляем преломлённый цвет
		localColor.x += refractColor.x * m.albedo[3];
		localColor.y += refractColor.y * m.albedo[3];
		localColor.z += refractColor.z * m.albedo[3];
	}

	return localColor; // смешиваем цвета с соответствующими коэффициентами
}

// обсчёт освещённости точки P с нормалью N
void Tracer::ComputeLighting(const Vector &P, const Vector &N, const Vector &D, const Material& material, Vector &diffuse, Vector &specular) {
	diffuse = Vector::Zero(); // обнуляем диффузную яркость
	specular = Vector::Zero(); // обнуляем зеркальную яркость

	// проходимся по всем источникам освещения
	for (size_t i = 0; i < lights.size(); i++) {
		Vector L; // вектор направления луча от источника света
		double tmax; // максимальное расстояние

        if (lights[i].type == LightType::Ambient) {
            diffuse = diffuse + lights[i].color * lights[i].intensity;
            continue;
        }
		else if (lights[i].type == LightType::Point) {
			double x = lights[i].v.x - P.x; // направление точечного источника света есть разность между его положением и точкой
			double y = lights[i].v.y - P.y; // направление точечного источника света есть разность между его положением и точкой
			double z = lights[i].v.z - P.z; // направление точечного источника света есть разность между его положением и точкой

			double len = sqrt(x * x + y * y + z * z);

			tmax = len - EPS;

			L.x = x / len;
			L.y = y / len;
			L.z = z / len;
		}
		else {
			L.x = lights[i].v.x; // направление направленного источника света содержится в самом источнике
			L.y = lights[i].v.y; // направление направленного источника света содержится в самом источнике
			L.z = lights[i].v.z; // направление направленного источника света содержится в самом источнике

			tmax = INF;
		}

		// проверяем на нахождение в тени
		if (HaveIntersection(P, L, EPS, tmax))
			continue; // переходим к следующему источнику

		double LN = L * N; // косинус угла между источником света и нормалью
		double specularCos = (L - N * (2 * LN)) * D; // косинус угла между отражённым лучём и направлением луча

		if (LN > 0) {
			double br = LN * lights[i].intensity;

			diffuse.x += br * lights[i].color.x;
			diffuse.y += br * lights[i].color.y;
			diffuse.z += br * lights[i].color.z;
		}

		if (specularCos > 0) {
			specular.x += pow(specularCos, material.specular[0]) * lights[i].intensity * lights[i].color.x;
			specular.y += pow(specularCos, material.specular[1]) * lights[i].intensity * lights[i].color.y;
			specular.z += pow(specularCos, material.specular[2]) * lights[i].intensity * lights[i].color.z;
		}
	}

    diffuse.x *= material.albedo[0];
    diffuse.y *= material.albedo[0];
	diffuse.z *= material.albedo[0];

    specular.x *= material.albedo[1];
    specular.y *= material.albedo[1];
	specular.z *= material.albedo[1];
}

// трассировка лучей и отображение результата на экране
void Tracer::Trace(const string& path) {
	Vector O = camera.GetPosition();

	high_resolution_clock::time_point t0 = high_resolution_clock::now(); // начинаем замеры времени

	// параллельно испускаем лучи через каждый пиксель
	#pragma omp parallel for
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Vector D = camera.GetDir(X[i][j], Y[i][j]); // получаем вектор направления луча
			Vector color = TraceRay(O, D, 0, INF, DEPTH);

			buffer[i][j] = color.ToColor();
		}
	}

	high_resolution_clock::time_point t1 = high_resolution_clock::now(); // начинаем замеры времени

	auto dt = duration_cast<chrono::microseconds>(t1 - t0); // получаем время выполнения в микросекундах
	cout << "Tracing time: " << (dt.count() / 1000.0) << endl;

	bitmap_image image(width, height);

	// отрисовываем содержимое буфера на картинке
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			image.set_pixel(x, y, buffer[x][y].x, buffer[x][y].y, buffer[x][y].z);

	image.save_image(path);
}