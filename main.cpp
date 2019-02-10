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
     .radius = 3.}};

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

Color trace(Ray ray, bool depth);
Color surface(Ray ray, Sphere object, Vector3 intersection, Vector3 normal)
{
  float lambertAmount = 0;
  Color result = {0, 0, 0};

  if (object.lambert > 0)
  {
    for (int i = 0; i < sizeof(lights) / sizeof(Vector3); i++)
    {
      Vector3 lightPoint = lights[i];

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
    Color reflectedColor = trace(reflectedRay, true);

    result = result + (reflectedColor * object.specular);
  }

  lambertAmount = lambertAmount > 1 ? 1 : lambertAmount;

  return result + (object.color * (lambertAmount * object.lambert)) + (object.color * object.ambient);
}

Vector3 sphereNormal(Sphere sphere, Vector3 pos)
{
  return normalize(pos - sphere.position);
}

Color trace(Ray ray, bool depth)
{
  if (depth)
  {
    return Color::white;
  }

  Hit hit = rayCast(ray);
  if (hit.distance == NO_HIT)
  {
    return Color::white;
  }

  Vector3 intersection = ray.position + scale(ray.direction, hit.distance);
  return surface(ray, objects[hit.sphereIndex], intersection, sphereNormal(objects[hit.sphereIndex], intersection));
};

static int SIZE = 500;

#ifdef __cplusplus
extern "C"
{
#endif

  int EMSCRIPTEN_KEEPALIVE
  r(int index)
  {
    Vector3 eyeVector = normalize(Camera::main.direction - Camera::main.position);
    Vector3 localRight = normalize(crossProduct(eyeVector, Vector3::up));
    Vector3 localUp = normalize(crossProduct(localRight, eyeVector));

    float fovRadians = M_PI * (Camera::main.fieldOfView / 2.) / 180.;
    float heightWidthRatio = SIZE / SIZE;
    float halfWidth = tan(fovRadians);
    float halfHeight = heightWidthRatio * halfWidth;
    float camerawidth = halfWidth * 2.;
    float cameraheight = halfHeight * 2.;
    float pixelWidth = camerawidth / (SIZE - 1);
    float pixelHeight = cameraheight / (SIZE - 1);

    int pos = index / 4;
    int x = pos / SIZE;
    int y = pos % SIZE;
    Vector3 xComp = scale(localRight, (x * pixelWidth) - halfWidth);
    Vector3 yComp = scale(localUp, (y * pixelHeight) - halfHeight);

    Ray ray = {
        .position = Camera::main.position,
        .direction = normalize(eyeVector + xComp + yComp)};

    Color result = trace(ray, false);

    switch (index % 4)
    {
    case 0:
      return result.r;
    case 1:
      return result.g;
    case 2:
      return result.b;
    case 3:
      return 255;
    }
  }

#ifdef __cplusplus
}
#endif
