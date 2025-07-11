#include "hittable.h"
#include "macros.h"

#include <cmath>
#include <cstdio>

bool Sphere::hit(const Vec3& orig, const Vec3& dir, HitInfo& hit)
{
  Vec3 origin_sphere = orig - center;

  // a = dot(direction, direction) = 1.0 because direction is normalized
  double b = 2.0 * dot(dir, origin_sphere);
  double c = dot(origin_sphere, origin_sphere) - pow(radius, 2.0);

  // calculate discriminant
  double discriminant = pow(b, 2) - 4.0 * c;
  if (discriminant < 0) return false;

  double t0 = (-b - sqrt(discriminant)) / 2.0;
  double t1 = (-b + sqrt(discriminant)) / 2.0;

  // sphere is behind ray
  if (t0 <= 0 && t1 <= 0) return false;

  // use closest t; check if camera is inside sphere
  hit.t = fmin(t0, t1);
  int normal_direction = 1;
  if (hit.t <= 0) {
      hit.t = fmax(t0, t1);
      normal_direction = -1;
  }
  hit.pos = orig + (dir * hit.t);

  // define hit normal: negate normal if camera is inside sphere
  hit.normal = hit.pos - center;
  normalize(hit.normal);
  hit.normal *= normal_direction;

  hit.is_sphere = true;
  return true;
}


double Triangle::area(const Vec3& a, const Vec3& b, const Vec3& c, char ignore_axis)
{
  if (ignore_axis == 'x')       return 0.5 * (((b.y - a.y) * (c.z - a.z)) - ((c.y - a.y) * (b.z - a.z)));   // y-z plane
  else if (ignore_axis == 'y')  return 0.5 * (((b.x - a.x) * (c.z - a.z)) - ((c.x - a.x) * (b.z - a.z)));   // x-z plane
  else if (ignore_axis == 'z')  return 0.5 * (((b.x - a.x) * (c.y - a.y)) - ((c.x - a.x) * (b.y - a.y)));   // x-y plane
  else {
    printf("invalid zero coord.\n");
    return 0.0;
  }
}

// Ray-Plane intersection using the plane equation: ax_by+cz+d=0
bool Triangle::hit(const Vec3& orig, const Vec3& dir, HitInfo& hit) {

  Vec3 v0v1 = v[1].pos - v[0].pos;
  Vec3 v0v2 = v[2].pos - v[0].pos, v0v2;
  Vec3 normal = cross(v0v1, v0v2);
  normalize(normal);

  // Plane that triangle lies on
  double d = -dot(normal, v[0].pos);

  // Ray and Triangle are parallel (perpendicular to traingle normal)
  if (fabs(dot(normal, dir)) < EPSILON) return false;

  // Intersection distance between camera and triangle
  double t = -(dot(normal, orig) + d) / dot(normal, dir);
  if (t <= 0 ) return false;

  // Ray 
  hit.pos = orig + (dir * t);

  // Inside-Outside Test
  Vec3 v1v2 = v[2].pos - v[1].pos;
  Vec3 v2v0 = v[0].pos - v[2].pos;

  Vec3 v0hp = hit.pos - v[0].pos;
  Vec3 v1hp = hit.pos - v[1].pos;
  Vec3 v2hp = hit.pos - v[2].pos;

  Vec3 cross0 = cross(v0v1, v0hp);   double dot01 = dot(cross0, normal);
  Vec3 cross1 = cross(v1v2, v1hp);   double dot12 = dot(cross1, normal);
  Vec3 cross2 = cross(v2v0, v2hp);   double dot20 = dot(cross2, normal);

  if (dot01 < 0 || dot12 < 0 || dot20 < 0) return false;

  // Project onto X-Y plane unless perpendicular
  Vec3 projected_v0 = v[0].pos;
  Vec3 projected_v1 = v[1].pos;
  Vec3 projected_v2 = v[2].pos;
  Vec3 projected_hit_point = hit.pos;

  char ignore_axis;  
  if (fabs(normal.x - 0.0) >= EPSILON) ignore_axis = 'x';
  else if (fabs(normal.y - 0.0) >= EPSILON) ignore_axis = 'y';
  else if (fabs(normal.z - 0.0) >= EPSILON) ignore_axis = 'z';
  else return false;

  double tri_area = area(v[0].pos, v[1].pos, v[2].pos, ignore_axis);
  hit.bary.x = area(hit.pos, v[1].pos, v[2].pos, ignore_axis) / tri_area;
  hit.bary.y = area(v[0].pos, hit.pos, v[2].pos, ignore_axis) / tri_area;
  hit.bary.z = area(v[0].pos, v[1].pos, hit.pos, ignore_axis) / tri_area;

  hit.is_sphere = false;
  return true;
}