# WallpaperRayTracer
Generator of spheres on flatness wallpaper based on ray tracing algorithm

## Functions
* set the width and height of the generating images
* generate random scene for n images by m different materials
* generate scene by text description by n different materials
* save generated random scenes to description text file

## Build and run
* for Windows: `make && tracer.exe`
* for Linux: `make && ./tracer`

## Scene file
This is a usual text file with simple lines of description:

```
width [number in px]
height [number in px]

camera [position_x] [position_y] [position_z] [angle_x] [angle_y]

light [point/ambient/dir] [brightness: 0..1] [color red: 0..255] [color green: 0..255] [color blue: 0..255] [x] [y] [z]

sphere [position_x] [position_y] [position_z] [radius] [red: 0..255] [green: 0..255] [blue: 0..255] [specular_red] [specular_green] [specular_blue] [refractive] [albedo 0] [albedo 1] [albedo 2] [albedo 3]

ellipsoid [position_x] [position_y] [position_z] [radius_x] [radius_y] [radius_z] [red: 0..255] [green: 0..255] [blue: 0..255] [specular_red] [specular_green] [specular_blue] [refractive] [albedo 0] [albedo 1] [albedo 2] [albedo 3]

flatness [a] [b] [c] [position_x] [position_y] [position_z] [size] [red: 0..255] [green: 0..255] [blue: 0..255] [specular_red] [specular_green] [specular_blue] [refractive] [albedo 0] [albedo 1] [albedo 2] [albedo 3]
```

#### Simple scene file with 3 spheres
```
width 1600
height 900

#camera x y z angle_x angle_y
camera 0 1.83 -7.6 5.83 0

# light [dir/point/ambient] intensity red green blue x y z
light ambient 0.1 255 255 255
light point 0.3 255 0 0 -1 1 1
light point 0.4 0 255 0 0 1 1
light point 0.5 0 0 255 1 1 1

# shpere x y z r red green blue specular refractive albedo0 albedo1 albedo2 albedo3
sphere 0.0 0.0 0.0 0.5 255 255 255 500 500 500 1 1 0.5 0.2 0
sphere 0 0.0 0.8 0.5 255 0 255 500 500 500 1 1 0.5 0.3 0

sphere 1.0 0.0 0.0 0.5 255 255 255 500 500 500 1 1 0.5 0.2 0
sphere -1 0.0 0.0 0.5 255 0 255 500 500 500 1 1 0.5 0.3 0

# flatness a b c x0 y0 z0 size red green blue specular refractive albedo0 albedo1 albedo2 albedo3
flatness 0 1 0 0 -0.5 0 10 0 90 170 10 10 10 1 1 1 0.0 0.0
```

## Examples
#### Generate 20 random images 1920x1080 by 10 materials
```
Enter mode (random / file): random
Enter width and height: 1920 1080
Enter scene mode (random, numbers): random
Enter number of images: 20
Enter number of materials: 10
```
#### Generate 1 image from a scene file by 8 materials
```
Enter mode (random / file): file
Enter path to file with scene: scenes/scene.txt
Enter number of materials: 8
```

#### Generate 1 image by 8 materials with number(s)
```
Enter mode (random / file): random
Enter width and height: 1920 1080
Enter scene mode (random, numbers): numbers
Enter numbers: 1552500309
Enter number of materials: 8
```

## Wallpaper examples
![wallpaper 1](https://github.com/dronperminov/WallpaperRayTracer/blob/master/wallpapers/2.bmp "Wallpaper 1")
![wallpaper 2](https://github.com/dronperminov/WallpaperRayTracer/blob/master/wallpapers/4.bmp "Wallpaper 2")
![wallpaper 3](https://github.com/dronperminov/WallpaperRayTracer/blob/master/wallpapers/6.bmp "Wallpaper 3")
![wallpaper 4](https://github.com/dronperminov/WallpaperRayTracer/blob/master/wallpapers/7.bmp "Wallpaper 4")
![wallpaper 5](https://github.com/dronperminov/WallpaperRayTracer/blob/master/wallpapers/10.bmp "Wallpaper 5")