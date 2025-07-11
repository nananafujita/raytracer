#include "camera.h"
#include "macros.h"
#include "hittable.h"

#include <cmath>

// applying phong reflection model
void Camera::calculate_intensity(const Light& l, HitInfo& hit, const Vec3& light_dir, Vec3& intensity) const
{
    // define object-specific light elements
    Vec3 diffuse, specular;
    double shine;
    if (hit.is_sphere) {
        auto obj = dynamic_cast<const Sphere*>(hit.object);
        diffuse = obj->diffuse;
        specular = obj->specular;
        shine = obj->shine;
    } else {
        auto obj = dynamic_cast<const Triangle*>(hit.object);
        hit.normal = interpolate(hit.bary, obj->v[0].normal, obj->v[1].normal, obj->v[2].normal);
        normalize(hit.normal);
        diffuse = interpolate(hit.bary, obj->v[0].diffuse, obj->v[1].diffuse, obj->v[2].diffuse);
        specular = interpolate(hit.bary, obj->v[0].specular, obj->v[1].specular, obj->v[2].specular);
        shine = (hit.bary.x * obj->v[0].shine) + (hit.bary.y * obj->v[1].shine) + (hit.bary.z * obj->v[2].shine);
    }

    double LdotN = dot(light_dir, hit.normal);
    LdotN = max(0.0, LdotN);
    Vec3 reflect = hit.normal * (2.0 * LdotN) - light_dir;
    normalize(reflect);

    Vec3 v = -hit.pos;
    normalize(v);
    double RdotV = dot(reflect, v);
    RdotV = max(0.0, RdotV);

    intensity += l.color * ((diffuse * LdotN) + (specular * pow(RdotV, shine)));
}

void Camera::apply_shadow(HitInfo& p, Vec3& intensity) const
{
    Vec3 view_direction = -p.pos;
    normalize(view_direction);

    for (int l=0; l<Scene::num_lights; l++) {
        Vec3 light_dir = lights[l].pos - p.pos;
        double light_dist = length(light_dir);
        normalize(light_dir);

        bool in_shadow = false;
        HitInfo hit;
        for (int s=0; s<num_spheres; s++) {
            if (p.is_sphere && s == p.idx) continue;
            if (spheres[s].hit(p.pos, light_dir, hit) && light_dist - hit.t > EPSILON) {
                in_shadow = true;
                break;
            }
        }
        if (in_shadow) continue;

        for (int t=0; t<num_triangles; t++) {
            if (!p.is_sphere && t == p.idx) continue;
            if (triangles[t].hit(p.pos, light_dir, hit) && light_dist - hit.t > EPSILON) {
                in_shadow = true;
                break;
            }
        }

        if (!in_shadow) {
            calculate_intensity(lights[l], p, light_dir, intensity);
        }
    }
}


void Camera::render(Vec3* intensities)
{
    double half_angle = (fov / 2) * M_PI / 180.0;               // angle (in radians) between center and top of image plane
    double x = image_width / image_height * tan(half_angle);    // half width of image plane
    double y = tan(half_angle);                                 // half height of image plane
    double z = -1.0;
    double step_width = 2 * x / image_width;                    // step width on the image plane
    double step_height = 2 * y / image_height;                                

    // loop through camera rays
    for (int i=0; i<image_height; i++) {
        for (int j=0; j<image_width; j++) {
            Vec3 dir(j * step_width - x, i * step_height - y, z);
            normalize(dir);

            bool intersected = false;
            HitInfo hit_info;
            HitInfo closest;

            Vec3 intensity(ambient_light.x, ambient_light.y, ambient_light.z);
            for (int k=0; k<num_spheres; k++) {
                if (spheres[k].hit(origin, dir, hit_info)) {
                    if (!intersected || hit_info.t < closest.t) { // camera is at origin, so we can do length() directly
                        hit_info.idx = k;
                        closest = hit_info;
                        intersected = true;
                    }
                }
            }
            for (int k=0; k<num_triangles; k++) {
                if (triangles[k].hit(origin, dir, hit_info)) {
                    if (!intersected || hit_info.t < closest.t) {
                        hit_info.idx = k;
                        closest = hit_info;
                        intersected = true;
                    }
                }
            }

            Vec3 total_intensity;
            if (intersected) {
                apply_shadow(closest, intensity);
                total_intensity.x = max(0.0, min(1.0, intensity.x));
                total_intensity.y = max(0.0, min(1.0, intensity.y));
                total_intensity.z = max(0.0, min(1.0, intensity.z));
            } else {
                total_intensity.x = 1.0;
                total_intensity.y = 1.0;
                total_intensity.z = 1.0;
            }
            intensities[i * WIDTH + j] = total_intensity;
        }
    }
}
