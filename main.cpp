#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"
#include <emscripten/emscripten.h>

struct Sphere
{
  Vec3f center;
  float radius;

  Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}

  bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const
  {
    Vec3f L = center - orig;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > radius * radius)
      return false;
    float thc = sqrtf(radius * radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;
    if (t0 < 0)
      t0 = t1;
    if (t0 < 0)
      return false;
    return true;
  }
};

struct Light
{
  Light(const Vec3f &p, const float &i) : position(p), intensity(i) {}
  Vec3f position;
  float intensity;
};

float cast_ray(const Vec3f &orig, const Vec3f &dir, const Sphere &sphere, const Light &light)
{

  float sphere_dist = std::numeric_limits<float>::max();
  if (!sphere.ray_intersect(orig, dir, sphere_dist))
  {
    return 255.; // background color
  }

  Vec3f point = orig + dir * sphere_dist;
  Vec3f light_dir = (light.position - point).normalize();
  Vec3f N = (point - sphere.center).normalize();
  float diffuse_light_intensity = light.intensity * std::max(0.f, light_dir * N);
  return diffuse_light_intensity * 255.;
}

#ifdef __cplusplus
extern "C"
{
#endif

  float EMSCRIPTEN_KEEPALIVE
  r(int j, int i)
  {
    Sphere sphere = Sphere(Vec3f(-3, 0, -16), 8);
    Light light = Light(Vec3f(-20, 20, 20), 1.);
    const int fov = 300;
    const int width = 500;
    const int height = 500;
    float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
    float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
    Vec3f dir = Vec3f(x, y, -1).normalize();
    return cast_ray(Vec3f(0, 0, 0), dir, sphere, light) * 1.;
  }

#ifdef __cplusplus
}
#endif