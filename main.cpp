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
  static Color sky;

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

Color lerp(Color a, Color b, float percent)
{
  return a * percent + b * (1 - percent);
}

Color Color::white = {255, 255, 255};
Color Color::sky = {5, 35, 84};

struct Camera
{
  Vector3 position;
  float fieldOfView;
  Vector3 direction;

  static Camera main;
};

Camera Camera::main = {
    .position = {0, 30, 0},
    .fieldOfView = 45.,
    .direction = {10, 25, 10}};

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

static int lookupTable[] = {74, 23, 40, 35, 27, 15, 42, 8, 29, 7, 19, 3, 28, 18, 13, 17, 77, 94, 76, 16, 83, 72, 31, 9, 91, 22, 2, 85, 20, 1, 89, 95, 87, 26, 92, 30, 80, 103, 125, 88, 119, 4, 69, 124, 86, 108, 24, 112, 115, 106, 6, 123, 10, 25, 55, 60, 113, 73, 61, 51, 122, 107, 5, 96, 64, 0, 90, 34, 110, 59, 14, 97, 65, 11, 12, 120, 67, 46, 126, 104, 50, 53, 98, 56, 38, 36, 105, 62, 49, 109, 45, 66, 52, 44, 58, 127, 71, 63, 68, 82, 114, 57, 79, 99, 118, 48, 100, 81, 37, 101, 33, 84, 70, 75, 78, 54, 41, 47, 117, 43, 102, 116, 39, 111, 121, 21, 32, 93};
const int tableLength = sizeof(lookupTable) / sizeof(int);

int lookup(int index)
{
  return lookupTable[index % tableLength];
}

float fade(const float value)
{
  return value * value * value * (value * (value * 6 - 15) + 10);
}

float grad(int hash, float x, float y)
{
  switch (hash & 3)
  {
  case 0:
    return x + y;
  case 1:
    return -x + y;
  case 2:
    return x - y;
  case 3:
    return -x - y;
  default:
    return 0; // never happens
  }
}

float lerp(float from, float to, float percent)
{
  return from + percent * (to - from);
}

float noise(const float x, const float y)
{

  int xi = (int)x;
  int yi = (int)y;
  float xx = x - xi;
  float yy = y - yi;
  float fx = fade(xx);
  float fy = fade(yy);

  int aa = lookup(lookup(xi) + yi);
  int ab = lookup(lookup(xi) + yi + 1);
  int ba = lookup(lookup(xi + 1) + yi);
  int bb = lookup(lookup(xi + 1) + yi + 1);

  float x1 = lerp(grad(aa, xx, yy),
                  grad(ba, xx - 1, yy),
                  fx);
  float x2 = lerp(grad(ab, xx, yy - 1),
                  grad(bb, xx - 1, yy - 1),
                  fx);
  float value = lerp(x1, x2, fy);

  return (value + 1) / 2;
}

float coolnoise(float x, float y)
{
  return noise(x / 20, y / 20) +
         noise(x / 10, y / 10) / 2 +
         noise(x / 5, y / 5) / 6 +
         noise(x / 2, y / 2) / 20;
}

float surfaceHeightAtPoint(float x, float y)
{
  return coolnoise(x, y) * 20;
}

const float eps = 0.01;

Color getShadingAtPosition(Vector3 position, Vector3 normal)
{
  const Vector3 sun = {-1, -0.5, 0};
  const Vector3 v3Intensity = sun - normal;
  const float intensity = dotProduct(v3Intensity, v3Intensity);
  const Color terrainColor = {100, 20, 20};
  return terrainColor * (intensity * intensity / 3);
}

Vector3 normalAtPosition(Vector3 position)
{
  Vector3 normal = {
      surfaceHeightAtPoint(position.x - eps, position.z) - surfaceHeightAtPoint(position.x + eps, position.z),
      2.0f * eps,
      surfaceHeightAtPoint(position.x, position.z - eps) - surfaceHeightAtPoint(position.x, position.z + eps)};
  return normalize(normal);
}

Color addFogAtDistance(Color c, float distance)
{
  return lerp(c, Color::sky, 24 / distance);
}

Color colorAtTerrainPoint(Ray ray, float hitDistance)
{
  const Vector3 hitPoint = ray.position + ray.direction * hitDistance;
  const Vector3 normal = normalAtPosition(hitPoint);
  const Color shading = getShadingAtPosition(hitPoint, normal);
  const Color withFog = addFogAtDistance(shading, hitDistance);
  return withFog;
}

bool rayCast(Ray ray, float &hitDistance)
{
  float step = 0.1f;
  const float minDistance = 10;
  const float maxDistance = 100.0f;
  for (float t = minDistance; t < maxDistance; t += step)
  {
    step *= 1.01;
    const Vector3 p = ray.position + ray.direction * t;
    if (p.y < surfaceHeightAtPoint(p.x, p.z))
    {
      hitDistance = t - 0.5f * step;
      return true;
    }
  }
  return false;
};

bool threshold(const float value, const float threshold)
{
  return value > threshold;
}

Color skyColor(int x, int y)
{
  bool showStar = threshold(noise(x * 0.2, y * 0.2), 0.9);
  return showStar ? Color::white : Color::sky;
}

Color trace(Ray ray, int x, int y, bool depth)
{
  if (depth)
  {
    return Color::sky;
  }

  float hitDistance;
  if (!rayCast(ray, hitDistance))
  {
    return skyColor(x, y);
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

    Color result = trace(ray, x, y, false);

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
