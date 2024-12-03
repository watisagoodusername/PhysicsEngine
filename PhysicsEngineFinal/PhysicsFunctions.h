#include <raymath.h>

bool pincircle(Vector2 ppos, Vector2 cpos, float radius);

bool pinrect(Vector2 ppos, Vector2 cpos, Vector2 size);

bool circleoverlap(Vector2 pos1, Vector2 pos2, float rad1, float rad2);

void resolvecirclecollision(Vector2& pos1, Vector2& pos2, float r1, float r2, float m1, float m2);

void bounce(Vector2& vel1, Vector2& vel2, float m1, float m2, float angle, float restitution);

void wallbounce(int lowx, int highx, int lowy, int highy, Vector2& pos, Vector2& vel, float xdistance, float ydistance);
