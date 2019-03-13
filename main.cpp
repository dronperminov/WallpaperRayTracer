#include <iostream>

#include "Tracer.hpp"

int main() {
	string mode;
	cout << "Enter mode (random / file): ";
	cin >> mode;

	while (mode != "random" && mode != "file") {
		cout << "Try again: ";
		cin >> mode;
	}

		try {
		if (mode == "random") {
			int width;
			int height;

			cout << "Enter width and height: ";
			cin >> width >> height;

			Tracer tracer(width, height);

			string sceneMode;
			cout << "Enter scene mode (random, numbers): ";
			cin >> sceneMode;

			while (sceneMode != "random" && sceneMode != "numbers") {
				cout << "Try again: ";
				cin >> sceneMode;
			}

			int images;
			vector<int> numbers;

			if (sceneMode == "numbers") {
				string s;
				cout << "Enter numbers: ";
				cin.ignore();
				getline(cin, s);

				stringstream ss(s);
				int number;

				while (ss >> number)
					numbers.push_back(number);

				images = numbers.size();
			}
			else {
				cout << "Enter number of images: ";
				cin >> images;
			}

			int materials;
			cout << "Enter number of materials: ";
			cin >> materials;
			
			cout << "Start tracer:" << endl;
			cout << "  width: " << width << endl;
			cout << "  height: " << height << endl;
			cout << "  images: " << images << endl;
			cout << "  materials: " << materials << endl << endl;

			for (int i = 0; i < images; i++) {
				cout << "Image " << (i + 1) << endl;

				if (sceneMode == "random") {
					tracer.InitScene();
				}
				else if (sceneMode == "numbers") {
					tracer.InitScene(numbers[i]);
				}

				for (int j = 0; j < materials; j++) {
					string path = "wallpapers/" + to_string(i + 1) + "_m_" + to_string(j + 1) + ".bmp";
					string scenePath = "scenes/scene_" + to_string(i + 1) + "_m_" + to_string(j + 1) + ".txt";
					
					tracer.Trace(path);
					tracer.SaveScene(scenePath);

					if (materials > 1) {
						tracer.ChangeMaterials();
					}
				}
			}
		}
		else {
			string filepath;
			cout << "Enter path to file with scene: ";
			cin >> filepath;

			Tracer tracer(filepath);

			int materials;
			cout << "Enter number of materials: ";
			cin >> materials;

			cout << "Start tracer:" << endl;
			cout << "  width: " << tracer.GetWidth() << endl;
			cout << "  height: " << tracer.GetHeight() << endl;
			cout << "  materials: " << materials << endl << endl;

			for (int i = 0; i < materials; i++) {
				string path = "wallpapers/scene_m_" + to_string(i + 1) + ".bmp";
				
				tracer.Trace(path);

				if (materials > 1) {
					tracer.ChangeMaterials();
				}
			}
		}
	}
	catch (string s) {
		cout << "Error: " << s << endl;
	}
}