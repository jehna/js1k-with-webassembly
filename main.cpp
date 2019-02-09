#include "vector3.cpp"
#include <emscripten/emscripten.h>

#ifdef __cplusplus
extern "C"
{
#endif

  float EMSCRIPTEN_KEEPALIVE
  r(int j, int i)
  {
    Vector3 foo = Vector3::up;
    Vector3 bar = {0, 0, 0};
    return (foo + bar).y;
  }

#ifdef __cplusplus
}
#endif