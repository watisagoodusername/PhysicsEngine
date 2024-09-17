#include <raylib.h>
#include <raymath.h>
#include <cmath>

bool pincircle(Vector2 ppos, Vector2 cpos, float radius) {// determines if a point is inside a circle
    float xdif = ppos.x - cpos.x;
    float ydif = ppos.y - cpos.y;
    float tdif = sqrt(xdif * xdif + ydif * ydif);//distance between points
    if (tdif < radius) {
        return true;
    }
    return false;
}

bool pinrect(Vector2 ppos, Vector2 cpos, Vector2 size) {// determines if a point is inside a rectangle
    float xdif = ppos.x - cpos.x;
    float ydif = ppos.y - cpos.y;
    if (-size.x / 2 < xdif and xdif < size.x / 2 and -size.y / 2 < ydif and ydif < size.y / 2) {
        return true;
    }
    return false;
}

bool circleoverlap(Vector2 pos1, Vector2 pos2, float rad1, float rad2) {
    float xdif = pos1.x - pos2.x;
    float ydif = pos1.y - pos2.y;
    float tdif = sqrt(xdif * xdif + ydif * ydif);//distance between points
    if (tdif < rad1 + rad2) {
        return true;
    }
    return false;
}

void resolvecirclecollision(Vector2& pos1, Vector2& pos2, float r1, float r2, float m1, float m2) {//move circles outside eachother
    float xdif = pos1.x - pos2.x;
    float ydif = pos1.y - pos2.y;
    float tdif = sqrt(xdif * xdif + ydif * ydif);//distance between points

    xdif /= tdif;
    ydif /= tdif;//normalised

    float rt = r1 + r2;

    float tomovetotal = rt - tdif;

    float tomove1 = (m2 * tomovetotal) / (m1 + m2);
    float tomove2 = (m1 * tomovetotal) / (m1 + m2);

    pos1.x += xdif * tomove1;
    pos1.y += ydif * tomove1;
    pos2.x -= xdif * tomove2;
    pos2.y -= ydif * tomove2;
}

void bounce(Vector2& vel1, Vector2& vel2, float m1, float m2, float angle, float restitution) {
    Vector2 rotatedv1 = Vector2Rotate(vel1, angle);
    Vector2 rotatedv2 = Vector2Rotate(vel2, angle);//rotates points so x axis is normal to makle calculations easier

    float v1 = (restitution * m2 * (rotatedv2.y - rotatedv1.y) + m1 * rotatedv1.y + m2 * rotatedv2.y) / (m1 + m2);
    float v2 = (restitution * m1 * (rotatedv1.y - rotatedv2.y) + m2 * rotatedv2.y + m1 * rotatedv1.y) / (m2 + m1);//final velocity calculations for 1D inelastic collisions

    rotatedv1.y = v1;
    rotatedv2.y = v2;

    vel1 = Vector2Rotate(rotatedv1, -angle);
    vel2 = Vector2Rotate(rotatedv2, -angle);//return updated velocities
}

void wallbounce(int lowx, int highx, int lowy, int highy, Vector2& pos, Vector2& vel, float xdistance, float ydistance) {
    //takes an object size xdistance by ydistance and reflects velocity if outside x/y bounds

    if (pos.x >= highx - xdistance) {
        vel.x = -abs(vel.x);
        pos.x = highx - xdistance;
    }
    else if (pos.x <= lowx + xdistance) {
        vel.x = abs(vel.x);
        pos.x = lowx + xdistance;
    }
    if (pos.y >= highy - ydistance) {
        vel.y = -abs(vel.y);
        pos.y = highy - ydistance;
    }
    else if (pos.y <= lowy + ydistance) {
        vel.y = abs(vel.y);
        pos.y = lowy + ydistance;
    }
}

