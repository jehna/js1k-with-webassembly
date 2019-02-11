#include <math.h>

/*extern "C"
{
  float R(float input);
}*/

struct Vector3
{
  float x;
  float y;
  float z;

  static Vector3 up;
  static Vector3 zero;

  Vector3 operator+(const Vector3 &value) const
  {
    Vector3 result = {
        x + value.x,
        y + value.y,
        z + value.z};
    return result;
  };

  Vector3 operator-(const Vector3 &value) const
  {
    Vector3 result = {
        x - value.x,
        y - value.y,
        z - value.z};
    return result;
  };

  Vector3 operator*(const float &value) const
  {
    Vector3 result = {
        x * value,
        y * value,
        z * value};
    return result;
  };
};

Vector3 Vector3::up = {0., 1., 0.};
Vector3 Vector3::zero = {0., 0., 0.};

float dotProduct(Vector3 a, Vector3 b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
};

Vector3 crossProduct(Vector3 a, Vector3 b)
{
  Vector3 result =
      {(a.y * b.z) - (a.z * b.y),
       (a.z * b.x) - (a.x * b.z),
       (a.x * b.y) - (a.y * b.x)};
  return result;
};

Vector3 scale(Vector3 value, float scale)
{
  Vector3 result = {
      value.x * scale,
      value.y * scale,
      value.z * scale};
  return result;
};

float length(Vector3 value)
{
  return sqrt(dotProduct(value, value));
};

Vector3 normalize(Vector3 value)
{
  return scale(value, 1. / length(value));
};

Vector3 reflectThrough(Vector3 value, Vector3 normal)
{
  Vector3 direction = scale(normal, dotProduct(value, normal));
  return scale(direction, 2) - value;
};