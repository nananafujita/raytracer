#ifndef CAMERA_H
#define CAMERA_H

#include "scene.h"
#include "vec3.h"

class Camera {
public:
    Camera() { origin = Vec3(); }
    
    double height() { return image_height; }
    double width() { return image_width; }

    void render(Vec3* intensities);
private:
    Vec3 origin;
    int image_width = 1280;
    int image_height = 960;
    int samples = 100;
    double fov = 60.0;

    void apply_shadow(HitInfo& p, Vec3& intensity) const;
    void calculate_intensity(const Light& l, HitInfo& hit, const Vec3& light_dir, Vec3& intensity) const;
};

#endif