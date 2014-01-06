#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "transform.h"
#include <FreeImage.h>
#include <stdio.h>

using namespace std;

#include "raytracer.h"

void SaveScreenshot(string fname, BYTE* image, int width, int height) {
        
        FIBITMAP* img = FreeImage_ConvertFromRawBits(image, width, height, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
        
        std::cout << "Saving screenshot: " << fname << "\n";

        FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}

BYTE* RayTrace (Camera camera, const Scene& scene)  {
        RayTracer ray_tracer;
        int width = scene.width;
        int height = scene.height;
        int pix = width * height;
        BYTE* image = new BYTE[3*pix];

        for (int y = 0 ; y < height ; y++) {
			for (int x = 0 ; x < width ; x++) { 
				Ray ray = ray_tracer.RayThruPixel(camera, y+0.5, x+0.5, height, width);
                Color color = ray_tracer.GetColor(ray, scene, 0, y+0.5, x+0.5);
                int base = 3 * ((height-y-1) * width + x);
                
				image[base + 0] = color.Bbyte();
                image[base + 1] = color.Gbyte();
                image[base + 2] = color.Rbyte();
                
			}
		}
        return image;
}

int main(int argc, char* argv[]) {

    FreeImage_Initialise();
        
    Scene scene;
    scene.readfile(argv[1]);

	cout << "Objects: " << scene.objects.size() << "; Lights: " << scene.lights.size() << "; Pixels: " << scene.width*scene.height << ";\n";
    cout << "Starting Recursive Ray Tracing.\n";
    
    BYTE* image = RayTrace(scene.camera, scene);
    SaveScreenshot(scene.resultFile, image, scene.width, scene.height);

	cout << "Recursive Ray Tracing completed.\n";

    delete image;
    
    FreeImage_DeInitialise();
    return 0;

}