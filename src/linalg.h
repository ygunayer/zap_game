#ifndef _GAME_LINALG_H_
#define _GAME_LINALG_H_

typedef struct Vec2 {
  float x;
  float y;
} Vec2;

typedef struct Vec3 {
  float x;
  float y;
  float z;
} Vec3;

typedef struct Vec4 {
  float x;
  float y;
  float z;
  float w;
} Vec4;

typedef struct Mat4x4 {
  float m[4][4]; // row-major
} Mat4x4;

#endif // _GAME_LINALG_H_
