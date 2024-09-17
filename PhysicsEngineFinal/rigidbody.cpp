#include "rigidbody.h"
#include <raymath.h>

class rigidbody { // class to hold and type of rigidbody collider
    float radius;
    float mass;
    Vector2 velocity;
    Vector2 position;
    bool held = false;

    public:
        rigidbody() {
            radius = 0;
            mass = 0;
            position.x = 0;
            position.y = 0;
            velocity.x = 0;
            velocity.y = 0;
        }
        ballcollider(float r, float xpos, float ypos, float m = 1, float xvel = 0, float yvel = 0) {
            radius = r;
            mass = 3 * r * r * m;
            position.x = xpos;
            position.y = ypos;
            velocity.x = xvel;
            velocity.y = yvel;
        }

        void clicked(Vector2 mousepos, float maxspeed) {
            Vector2 dif = Vector2Subtract(mousepos, position);
            dif = Vector2ClampValue(dif, 0, maxspeed);
            velocity = dif;
            //position = Vector2Subtract(mousepos, dif);
        }

        void update(Vector2 mousepos, bool press, bool release) {
            if (press) {
                if (pincircle(mousepos, position, radius)) {
                    held = true;
                }
            }
            if (release) {
                held = false;
            }
            if (held) {
                clicked(mousepos, 50);
            }
            position = Vector2Add(position, velocity);
            wallbounce(0, 1000, 0, 1000, position, velocity, radius, radius);
        }

        void draw() {
            DrawCircleV(position, radius, BLACK);
            DrawCircleV(position, radius - 5, DARKBLUE);
        }

        float get_r() {
            return radius;
        }
        float get_m() {
            return mass;
        }
        Vector2 get_vel() {
            return velocity;
        }
        Vector2 get_pos() {
            return position;
        }
        void set_vel(Vector2 v) {
            velocity = v;
        }
        void set_pos(Vector2 p) {
            position = p;
        }
};