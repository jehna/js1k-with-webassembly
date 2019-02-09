#include "vector3.cpp"
#include <emscripten/emscripten.h>

struct Ray
{
  Vector3 position;
  Vector3 direction;
};

struct Color
{
  float r;
  float g;
  float b;

  static Color white;

  Color operator*(const float &value) const
  {
    Color result = {
        r * value,
        g * value,
        b * value};
    return result;
  };

  Color operator+(const Color &value) const
  {
    Color result = {
        r + value.r,
        g + value.g,
        b + value.b};
    return result;
  };
};

Color Color::white = {255, 255, 255};

struct Camera
{
  Vector3 position;
  float fieldOfView;
  Vector3 direction;

  static Camera main;
};

Camera Camera::main = {
    .position = {0., 1.8, 10.},
    .fieldOfView = 45.,
    .direction = {0., 3., 0.}};

struct Sphere
{
  Vector3 position;
  Color color;
  float specular;
  float lambert;
  float ambient;
  float radius;
};

struct Hit
{
  float distance;
  int sphereIndex;
};

static Vector3 lights[] = {
    {-30.,
     -10.,
     20.}};

static Sphere objects[] = {
    {.position = {0., 3.5, -3.},
     .color = {255, 200, 155},
     .specular = 0.2,
     .lambert = 0.7,
     .ambient = 0.1,
     .radius = 3.},
    {.position = {-4., 2, -1.},
     .color = {155, 155, 155},
     .specular = 0.1,
     .lambert = 0.9,
     .ambient = 0.0,
     .radius = 0.2}};

float NO_HIT = 999999999;

float sphereIntersection(Sphere sphere, Ray ray)
{
  Vector3 eye_to_center = sphere.position - ray.position;
  float v = dotProduct(eye_to_center, ray.direction);
  float eoDot = dotProduct(eye_to_center, eye_to_center);
  float discriminant = (sphere.radius * sphere.radius) - eoDot + (v * v);

  if (discriminant < 0)
  {
    return NO_HIT;
  }

  return v - sqrt(discriminant);
}

Hit rayCast(Ray ray)
{
  Hit closest = {NO_HIT, NULL};
  for (int i = 0; i < sizeof(objects) / sizeof(Sphere); i++)
  {
    Sphere object = objects[i];
    float dist = sphereIntersection(object, ray);
    if (dist != NO_HIT && dist < closest.distance)
    {
      closest.distance = dist;
      closest.sphereIndex = i;
    }
  }
  return closest;
};

bool isLightVisible(Vector3 position, Vector3 light)
{
  Ray ray = {
      position,
      normalize(position - light)};
  Hit hit = rayCast(ray);
  return hit.distance > -0.005;
}

Color trace(Ray ray, int depth);
Color surface(Ray ray, Sphere object, Vector3 intersection, Vector3 normal, int depth)
{
  float lambertAmount = 0;
  Color result = {0, 0, 0};

  if (object.lambert > 0)
  {
    for (int i = 0; i < sizeof(lights) / sizeof(Vector3); i++)
    {
      Vector3 lightPoint = lights[i];
      if (!isLightVisible(intersection, lightPoint))
        continue;

      float contribution = dotProduct(normalize(lightPoint - intersection), normal);
      if (contribution > 0)
        lambertAmount += contribution;
    }
  }

  if (object.specular > 0)
  {
    Ray reflectedRay = {
        intersection,
        reflectThrough(ray.direction, normal)};
    Color reflectedColor = trace(reflectedRay, depth + 1);

    result = result + (reflectedColor * object.specular);
  }

  lambertAmount = fmin(1, lambertAmount);

  return result + (object.color * (lambertAmount * object.lambert)) + (object.color * object.ambient);
}

Vector3 sphereNormal(Sphere sphere, Vector3 pos)
{
  return normalize(pos - sphere.position);
}

Color trace(Ray ray, int depth)
{
  if (depth > 3)
  {
    return Color::white;
  }

  Hit hit = rayCast(ray);
  if (hit.distance == NO_HIT)
  {
    return Color::white;
  }

  Vector3 intersection = ray.position + scale(ray.direction, hit.distance);
  return surface(ray, objects[hit.sphereIndex], intersection, sphereNormal(objects[hit.sphereIndex], intersection), depth);
};

static int width = 500;
static int height = 500;

#ifdef __cplusplus
extern "C"
{
#endif

  int EMSCRIPTEN_KEEPALIVE
  r(int x, int y)
  {
    Vector3 eyeVector = normalize(Camera::main.direction - Camera::main.position);
    Vector3 localRight = normalize(crossProduct(eyeVector, Vector3::up));
    Vector3 localUp = normalize(crossProduct(localRight, eyeVector));

    float fovRadians = M_PI * (Camera::main.fieldOfView / 2.) / 180.;
    float heightWidthRatio = height / width;
    float halfWidth = tan(fovRadians);
    float halfHeight = heightWidthRatio * halfWidth;
    float camerawidth = halfWidth * 2.;
    float cameraheight = halfHeight * 2.;
    float pixelWidth = camerawidth / (width - 1);
    float pixelHeight = cameraheight / (height - 1);

    Vector3 xComp = scale(localRight, (x * pixelWidth) - halfWidth);
    Vector3 yComp = scale(localUp, (y * pixelHeight) - halfHeight);

    Ray ray = {
        .position = Camera::main.position,
        .direction = normalize(eyeVector + xComp + yComp)};

    Color result = trace(ray, 0);

    int r = fmin(255, result.r);
    int g = fmin(255, result.g);
    int b = fmin(255, result.b);
    return (r & 255) << 16 | (g & 255) << 8 | (b & 255);
  }

#ifdef __cplusplus
}
#endif
