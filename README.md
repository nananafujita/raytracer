# Raytracer
Features:
- Visualization window in GLFW
- Analytic ray-triangle and ray-sphere intersections using barycentric coordinates
- Phong shading
- Anti-aliasing

Sample output:
![Sample output image of a series of spheres](sampleOutput1.jpg)
![Sample output image of a snowman made of spheres and triangles](sampleOutput2.jpg)

mkdir build
cmake ..
make 
.raytracer model/
