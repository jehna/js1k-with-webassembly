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
    .position = {10, 10, 10},
    .fieldOfView = 45.,
    .direction = {0, 5, 0}};

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

float surfaceHeightAtPoint(const float x, const float y)
{
  return sin(x) + sin(y);
}

const float eps = 0.1;

Color getShadingAtPosition(Vector3 position, Vector3 normal)
{
  const Vector3 sun = {1, -0.5, 0};
  const Vector3 v3Intensity = sun - normal;
  const float intensity = dotProduct(v3Intensity, v3Intensity);
  const Color terrainColor = {100, 20, 20};
  return terrainColor * intensity;
}

Vector3 normalAtPosition(Vector3 position)
{
  Vector3 normal = {
      surfaceHeightAtPoint(position.x - eps, position.z) - surfaceHeightAtPoint(position.x + eps, position.z),
      2.0f * eps,
      surfaceHeightAtPoint(position.x, position.z - eps) - surfaceHeightAtPoint(position.x, position.z + eps)};
  return normalize(normal);
}

Color colorAtTerrainPoint(Ray ray, float hitDistance)
{
  const Vector3 hitPoint = ray.position + ray.direction * hitDistance;
  const Vector3 normal = normalAtPosition(hitPoint);
  const Color shasing = getShadingAtPosition(hitPoint, normal);
  return shasing;
}

bool rayCast(Ray ray, float &hitDistance)
{
  const float step = 0.1f;
  const float minDistance = 0.001f;
  const float maxDistance = 50.0f;
  for (float t = minDistance; t < maxDistance; t += step)
  {
    const Vector3 p = ray.position + ray.direction * t;
    if (p.y < surfaceHeightAtPoint(p.x, p.z))
    {
      hitDistance = t - 0.5f * step;
      return true;
    }
  }
  return false;
};

Color trace(Ray ray, bool depth)
{
  if (depth)
  {
    return Color::white;
  }

  float hitDistance;
  if (!rayCast(ray, hitDistance))
  {
    return Color::white;
  }

  return colorAtTerrainPoint(ray, hitDistance);
};

static int SIZE = 240;

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
    int x = pos % SIZE;
    int y = SIZE - pos / SIZE;
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
