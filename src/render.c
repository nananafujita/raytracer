#include "render.h"

// using analytic method of testing whether the ray and sphere intersect
// direction must be normalized
int hit_sphere(Sphere sphere, double origin[3], double direction[3], double hit_point[3], double hit_normal[3])
{
    double origin_sphere[3];
    subtract(origin, sphere.pos, origin_sphere);

    // a = 1.0
    double b = 2.0 * dot(direction, origin_sphere);
    double c = dot(origin_sphere, origin_sphere) - pow(sphere.radius, 2.0);

    // calculate discriminant
    double discriminant = pow(b, 2) - 4.0 * c;
    if (discriminant < 0) return 0;

    double t0 = (-b - sqrt(discriminant)) / 2.0;
    double t1 = (-b + sqrt(discriminant)) / 2.0;

    // sphere is behind ray
    if (t0 <= 0 && t1 <= 0) return 0;

    double t_intersect = fmin(t0, t1);
    int normal_direction = 1;

    if (t_intersect <= 0) {
        t_intersect = fmax(t0, t1);
        normal_direction = -1;
    }
    multiply(direction, t_intersect, hit_point);
    add(origin, hit_point, hit_point);

    subtract(hit_point, sphere.pos, hit_normal);
    normalize(hit_normal);

    if (normal_direction < 0)  multiply(hit_normal, -1.0, hit_normal);
    return 1;
}


double triangleArea(double a[3], double b[3], double c[3], int zeroCoord)
{
  if (zeroCoord == 0)   return 0.5 * (((b[1] - a[1]) * (c[2] - a[2])) - ((c[1] - a[1]) * (b[2] - a[2])));   // y-z plane
  if (zeroCoord == 1)   return 0.5 * (((b[0] - a[0]) * (c[2] - a[2])) - ((c[0] - a[0]) * (b[2] - a[2])));   // x-z plane
  if (zeroCoord == 2)   return 0.5 * (((b[0] - a[0]) * (c[1] - a[1])) - ((c[0] - a[0]) * (b[1] - a[1])));   // x-y plane
  else {
    printf("invalid zero coord.\n");
    return 0.0;
  }
}

// Ray-Plane intersection using the plane equation: ax_by+cz+d=0
int hit_triangle(int idx, double origin[3], double direction[3], double hit_point[3], double triangle_normal[3], double barycentric[3]) {
    
    double v0v1[3];
    subtract(triangles[idx].vertices[1].pos, triangles[idx].vertices[0].pos, v0v1);
    double v0v2[3];
    subtract(triangles[idx].vertices[2].pos, triangles[idx].vertices[0].pos, v0v2);
    cross(v0v1, v0v2, triangle_normal);

    double area = length(triangle_normal) / 2.0;
    normalize(triangle_normal);

    // Plane that triangle lies on
    double a = triangle_normal[0];
    double b = triangle_normal[1];
    double c = triangle_normal[2];
    double d = -dot(triangle_normal, triangles[idx].vertices[0].pos);

    // Ray and Triangle are parallel (perpendicular to traingle normal)
    if (fabs(dot(triangle_normal, direction)) < EPSILON) return 0;

    // Intersection distance between camera and triangle
    double t = -(dot(triangle_normal, origin) + d) / dot(triangle_normal, direction);
    if (t <= 0 ) return 0;

    // Ray 
    double ray[3];
    ray[0] = origin[0] + (direction[0] * t); 
    ray[1] = origin[1] + (direction[1] * t); 
    ray[2] = origin[2] + (direction[2] * t); 

    // Inside-Outside Test
    double AtoB[3];   subtract(triangles[idx].vertices[1].pos, triangles[idx].vertices[0].pos, AtoB);
    double BtoC[3];   subtract(triangles[idx].vertices[2].pos, triangles[idx].vertices[1].pos, BtoC);
    double CtoA[3];   subtract(triangles[idx].vertices[0].pos, triangles[idx].vertices[2].pos, CtoA);

    double AtoRay[3];   subtract(ray, triangles[idx].vertices[0].pos, AtoRay);
    double BtoRay[3];   subtract(ray, triangles[idx].vertices[1].pos, BtoRay);
    double CtoRay[3];   subtract(ray, triangles[idx].vertices[2].pos, CtoRay);

    double crossA[3];  cross(AtoB, AtoRay, crossA);   double dotAB = dot(crossA, triangle_normal);
    double crossB[3];  cross(BtoC, BtoRay, crossB);   double dotBC = dot(crossB, triangle_normal);
    double crossC[3];  cross(CtoA, CtoRay, crossC);   double dotCA = dot(crossC, triangle_normal);

    if (dotAB < 0 || dotBC < 0 || dotCA < 0) return 0;

    hit_point[0] = ray[0];  hit_point[1] = ray[1];  hit_point[2] = ray[2];

    double u = dotBC;
    double v = dotCA;
    double denom = dot(triangle_normal, triangle_normal);
    u /= denom;
    v /= denom;

    // Project onto X-Y plane unless perpendicular
    int projectedZero = 2; // z = 0, project onto xy plane
    while (fabs(triangle_normal[projectedZero] - 0.0) < EPSILON)
    { projectedZero--; }
    if (projectedZero < 0){
        printf("invalid (<0) projected zero coordinate.\n");
        return 0;
    }

    double projectedTriangle0[3];
    double projectedTriangle1[3];
    double projectedTriangle2[3];
    double projectedIntersection[3];

    for (int i=0; i<3; i++) {
        if (i == projectedZero) {
        projectedTriangle0[i] = 0.0;
        projectedTriangle1[i] = 0.0;
        projectedTriangle2[i] = 0.0;
        projectedIntersection[i] = 0.0;
        } else {
        projectedTriangle0[i] = triangles[idx].vertices[0].pos[i];
        projectedTriangle1[i] = triangles[idx].vertices[1].pos[i];
        projectedTriangle2[i] = triangles[idx].vertices[2].pos[i];
        projectedIntersection[i] = ray[i];
        }
    }

    // Check clockwise/counter-clockwise orientation
    int clockwise = 1;
    double projected01[3];
    double projected02[3];
    for (int i=0; i<3; i++) {
        projected01[i] = projectedTriangle1[i] - projectedTriangle0[i];
        projected02[i] = projectedTriangle2[i] - projectedTriangle0[i];
    }
    double projectedCross[3];
    cross(projected01, projected02, projectedCross);
    if (projectedCross[projectedZero] < 0) clockwise = 0;

    double triArea = triangleArea(projectedTriangle0, projectedTriangle1, projectedTriangle2, projectedZero);
    double alpha = triangleArea(projectedIntersection, projectedTriangle1, projectedTriangle2, projectedZero) / triArea;
    double beta = triangleArea(projectedTriangle0, projectedIntersection, projectedTriangle2, projectedZero) / triArea;
    double gamma = triangleArea(projectedTriangle0, projectedTriangle1, projectedIntersection, projectedZero) / triArea;

    barycentric[0] = alpha;
    barycentric[1] = beta;
    barycentric[2] = gamma;
    return 1;
    /*// identify normal of the plane one which the triangle lies
    double v01[3]; 
    subtract(triangle.vertices[1].pos, triangle.vertices[0].pos, v01);
    double v02[3]; 
    subtract(triangle.vertices[2].pos, triangle.vertices[0].pos, v02);
    cross(v01, v02, triangle_normal);
    normalize(triangle_normal);

    // plane is parallel to camera ray; not visible.
    if (fabs(dot(triangle_normal, direction)) < EPSILON) return 0;
    
    // definition of a plane: ax+by+cz+d
    // a, b, c defined by hit_normal
    double d = -dot(triangle_normal, triangle.vertices[0].pos);

    // value of t along the ray that hits the plane 
    // if t <= 0, the plane is behind or at the camera; not visible
    double t_intersect = -(dot(triangle_normal, origin) + d) / dot(triangle_normal, direction);
    if (t_intersect <= 0) return 0;

    multiply(direction, t_intersect, hit_point);
    add(origin, hit_point, hit_point);

    // find barycentric coordinates to determine if hit point is inside triangle
    double v0hP[3];
    subtract(hit_point, triangle.vertices[0].pos, v0hP);

    double dot0101 = dot(v01, v01);
    double dot0102 = dot(v01, v02);
    double dot0202 = dot(v02, v02);
    double dot0hp01 = dot(v0hP, v01);
    double dot0hp02 = dot(v0hP, v02);
    double denominator = dot0101 * dot0202 - dot0102 * dot0102;
    barycentric[0] = (dot0202 * dot0hp01 - dot0102 * dot0hp02) / denominator;
    barycentric[1] = (dot0101 * dot0hp02 - dot0102 * dot0hp01) / denominator;
    barycentric[2] = 1.0 - barycentric[0] - barycentric[1];
    if (barycentric[0] < -EPSILON || barycentric[1] < -EPSILON || barycentric[2] < -EPSILON ||
        barycentric[0] > 1.0 + EPSILON || barycentric[1] > 1.0 + EPSILON || barycentric[2] > 1.0 + EPSILON) {
            return 0;
    }
    
    return 1;*/
}

void define_pixel(int is_sphere, int idx, double hit_point[3], double hit_normal[3], double hit_barycentric[3], struct Pixel* p)
{
    p->is_sphere = is_sphere;
    p->idx = idx;
    p->pos[0] = hit_point[0];           p->pos[1] = hit_point[1];           p->pos[2] = hit_point[2];
    p->color[0] = ambient_light[0];     p->color[1] = ambient_light[1];     p->color[2] = ambient_light[2];
    if (is_sphere) {
        for (int i=0; i<3; i++) {
            p->normal[i] = hit_normal[i];
            p->diffuse[i] = spheres[idx].diffuse[i];
            p->specular[i] = spheres[idx].specular[i];
            p->bary[i] = -1.0;
        }
        p->shininess = spheres[idx].shininess;
    } else {
        interpolate(hit_barycentric, triangles[idx].vertices[0].normal, triangles[idx].vertices[1].normal, triangles[idx].vertices[2].normal, p->normal);
        normalize(p->normal);
        interpolate(hit_barycentric, triangles[idx].vertices[0].diffuse, triangles[idx].vertices[1].diffuse, triangles[idx].vertices[2].diffuse, p->diffuse);
        interpolate(hit_barycentric, triangles[idx].vertices[0].specular, triangles[idx].vertices[1].specular, triangles[idx].vertices[2].specular, p->specular);

        p->bary[0] = hit_barycentric[0];    p->bary[1] = hit_barycentric[1];   p->bary[2] = hit_barycentric[2]; 

        p->shininess = (hit_barycentric[0] * triangles[idx].vertices[0].shininess) + 
                        (hit_barycentric[1] * triangles[idx].vertices[1].shininess) + 
                        (hit_barycentric[2] * triangles[idx].vertices[2].shininess);
    }
}

// applying phong reflection model
void calculate_intensity(Light* l, Pixel* p, double light_dir[3], double intensity[3])
{
    double LdotN = dot(light_dir, p->normal);
    //printf("normal: %f %f %f\n", p->normal[0], p->normal[1], p->normal[2]);
    LdotN = max(0.0, LdotN);
    double reflect[3];
    for (int i=0; i<3; i++) {
        reflect[i] = 2.0 * LdotN * p->normal[i] - light_dir[i];
    }
    normalize(reflect);

    double v[3] = {-p->pos[0], -p->pos[1], -p->pos[2]};
    normalize(v);
    double RdotV = dot(reflect, v);
    RdotV = max(0.0, RdotV);
    for(int i=0; i<3; i++) {
        intensity[i] += l->color[i] * ((p->diffuse[i] * LdotN) + (p->specular[i] * pow(RdotV, p->shininess)));
    }
    printf("%f %f\n", LdotN, RdotV);
}

void apply_shadow(Pixel* p, double intensity[3])
{
    double view_direction[3] = {-p->pos[0], -p->pos[1], -p->pos[2]};
    normalize(view_direction);

    for (int l=0; l<num_lights; l++) {
        double light_direction[3];
        double light_dist_squared;
        subtract(lights[l].pos, p->pos, light_direction);
        light_dist_squared = squared_length(light_direction);
        normalize(light_direction);

        int in_shadow = 0;
        for (int s=0; s<num_spheres; s++) {
            if (p->is_sphere && s == p->idx) continue;
            double obstacle_hit_point[3];
            double obstacle_normal[3];
            if (hit_sphere(spheres[s], p->pos, light_direction, obstacle_hit_point, obstacle_normal)) {
                double hit_point_to_obstacle[3];
                subtract(obstacle_hit_point, p->pos, hit_point_to_obstacle);
                if (light_dist_squared - squared_length(hit_point_to_obstacle) > EPSILON) {
                    in_shadow = 1;
                    break;
                }
            }
        }
        if (in_shadow) continue;

        for (int t=0; t<num_triangles; t++) {
            if (!p->is_sphere && t == p->idx) continue;
            double obstacle_hit_point[3];
            double obstacle_normal[3];
            double obstacle_barycentric[3];
            if (hit_triangle(t, p->pos, light_direction, obstacle_hit_point, obstacle_normal, obstacle_barycentric)){
                double hit_point_to_obstacle[3];
                subtract(obstacle_hit_point, p->pos, hit_point_to_obstacle);
                if (light_dist_squared - squared_length(hit_point_to_obstacle) > EPSILON) {
                    in_shadow = 1;
                    break;
                }
            }
        }

        if (!in_shadow) {
            calculate_intensity(&lights[l], p, light_direction, intensity);
        }
    }
}


void render(Vec3* intensities)
{
    printf("raytracing...");
    double origin[3] = {0, 0, 0};
    double half_angle = (FOV / 2) * M_PI / 180.0;                   // angle (in radians) between center and top of image plane
    double x = (float) WIDTH / (float) HEIGHT * tan(half_angle);    // half width of image plane
    double y = tan(half_angle);                                     // half height of image plane
    double z = -1.0;
    double step_width = 2 * x / WIDTH;                              // step width on the image plane
    double step_height = 2 * y / HEIGHT;                                

    // loop through camera rays
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            int intersected = 0;
            double closest_dist = 0.0;

            double direction[3] = {j * step_width - x, i * step_height - y, z};
            normalize(direction);

            int closest_is_sphere;
            int closest_index;
            double closest_point[3];
            double closest_normal[3];
            double closest_barycentric[3];

            double hit_point[3];
            double hit_normal[3];
            double hit_barycentric[3];

            double intensity[3] = {ambient_light[0], ambient_light[1], ambient_light[2]};
            for (int s=0; s<num_spheres; s++) {
                if (hit_sphere(spheres[s], origin, direction, hit_point, hit_normal)) {
                    if (!intersected || squared_length(hit_point) < closest_dist) { // camera is at origin, so we can do length() directly
                        closest_is_sphere = 1;
                        closest_index = s;
                        for (int k=0; k<3; k++) {   
                            closest_point[k] = hit_point[k];
                            closest_normal[k] = hit_normal[k];
                            closest_barycentric[k] = -1;
                        }
                        intersected = 1;
                        closest_dist = squared_length(hit_point);
                    }
                }
            }
            for (int t=0; t<num_triangles; t++) {
                if (hit_triangle(t, origin, direction, hit_point, hit_normal, hit_barycentric)) {
                    if (!intersected || squared_length(hit_point) < closest_dist) {
                        printf("hit triangle\n");
                        closest_is_sphere = 0;
                        closest_index = t;
                        for (int k=0; k<3; k++) {   
                            closest_point[k] = hit_point[k];
                            closest_normal[k] = hit_normal[k];
                            closest_barycentric[k] = hit_barycentric[k];
                        }
                        intersected = 1;
                        closest_dist = squared_length(hit_point);
                    }
                }
            }
            Vec3 total_intensity;
            if (intersected) {
                Pixel p;
                define_pixel(closest_is_sphere, closest_index, closest_point, closest_normal, closest_barycentric, &p);
                apply_shadow(&p, intensity);
                total_intensity.x = max(0.0, min(1.0, intensity[0]));
                total_intensity.y = max(0.0, min(1.0, intensity[1]));
                total_intensity.z = max(0.0, min(1.0, intensity[2]));
                printf("i: %d, j: %d = %f %f %f\n", j, i, intensity[0], intensity[1], intensity[2]);
            } else {
                total_intensity.x = 1.0;
                total_intensity.y = 1.0;
                total_intensity.z = 1.0;
            }
            intensities[i * WIDTH + j] = total_intensity;
        }
    }
    printf("done.\n");
}
