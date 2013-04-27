#ifndef IVEC2_H
#define IVEC2_H

/** Simple integer coordinate 2d vector */
class iVec2 {
public:
  int32_t x, y;
  iVec2(int32_t x_, int32_t y_) :x(x_), y(y_) {}

  iVec2 operator+ (const iVec2 rhs) const {
    return iVec2(x + rhs.x, y + rhs.y);
  }

  bool operator < (const iVec2 rhs) const {
    // should impose a total ordering i think/hope, should make this suitable for std::map-ing
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }
};

#endif

