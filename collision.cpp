#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

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

void resolvecollision(Vector2& vel1, Vector2& vel2, float angle) {// inelastic bounce 2 objects with no rotation (doesnt work yet)

    Vector2 rotatedv1 = Vector2Rotate(vel1, -angle);
    Vector2 rotatedv2 = Vector2Rotate(vel2, -angle);//rotates points so x axis is normal to makle calculations easier

    rotatedv1.y *= -1;
    rotatedv2.y *= -1;//calculations for 1D collision(not right)

    vel1 = Vector2Rotate(rotatedv1, angle);
    vel2 = Vector2Rotate(rotatedv2, angle);//return updated velocities
}

static Vector2 wallbounce(int lowx, int highx, int lowy, int highy, Vector2 pos, Vector2 vel, float xdistance, float ydistance) {
    //takes an object size xdistance by ydistance and reflects velocity if outside x/y bounds
    
    Vector2 finalvel = vel;
    if (pos.x >= highx - xdistance) {
        finalvel.x = -abs(vel.x);
    }
    else if (pos.x <= lowx + xdistance) {
        finalvel.x = abs(vel.x);
    }
    if (pos.y >= highy - ydistance) {
        finalvel.y = -abs(vel.y);
    }
    else if (pos.y <= lowy + ydistance) {
        finalvel.y = abs(vel.y);
    } 
    return finalvel;
}

class ballcollider {
    float radius;
    float mass;
    Vector2 velocity;
    Vector2 position;
    bool held = false;

public:
    ballcollider() {
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
        position = Vector2Subtract(mousepos, dif);
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
        velocity = wallbounce(0, 1000, 0, 1000, position, velocity, radius, radius);
        position = Vector2Add(position, velocity);
    }

    void draw() {
        DrawCircleV(position, radius, BLACK);
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
};

class rectcollider {
    float sizex;
    float sizey;
    float mass;
    Vector2 position;
    Vector2 velocity;
    bool held = false;

public:
    rectcollider() {
        sizex = 0;
        sizey = 0;
        mass = 0;
        position.x = 0;
        position.y = 0;
        velocity.x = 0;
        velocity.y = 0;
    }
    rectcollider(float xsize, float ysize, float xpos, float ypos, float m = 1, float xvel = 0, float yvel = 0) {
        sizex = xsize;
        sizey = ysize;
        mass = xsize * ysize * m;
        position.x = xpos;
        position.y = ypos;
        velocity.x = xvel;
        velocity.y = yvel;
    }

    void clicked(Vector2 mousepos) {
        Vector2 dif = Vector2Subtract(mousepos, position);
        velocity = dif;
    }

    void update(Vector2 mousepos, bool press, bool release) {
        if (press) {
            if (pincircle(mousepos, position, sizex / 2)) {
                held = true;
            }
        }
        if (release) {
            held = false;
        }
        if (held) {
            clicked(mousepos);
        }
        velocity = wallbounce(000, 1000, 00, 1000, position, velocity, sizex / 2, sizey / 2);
        position = Vector2Add(position, velocity);
    }

    float get_xsize() {
        return sizex;
    }
    float get_ysize() {
        return sizey;
    }
    Vector2 get_vel() {
        return velocity;
    }
    Vector2 get_pos() {
        return position;
    }
    Vector2 get_corner() {
        Vector2 cornerpos;
        cornerpos.x = position.x - sizex / 2;
        cornerpos.y = position.y - sizey / 2;
        return cornerpos;
    }
    void set_vel(Vector2 v) {
        velocity = v;
    }
};

class rigidbody {
    rectcollider rcol;
    ballcollider bcol;

public:
    std::string shape;

    rigidbody(rectcollider c) {
        rcol = c;
        bcol = ballcollider();
        shape = "rect";
    }
    rigidbody(ballcollider c) {
        bcol = c;
        rcol = rectcollider();
        shape = "ball";
    }

    // UPDATE
    void update(Vector2 mousepos, bool press, bool release) {
        if (shape == "ball") {
            bcol.update(mousepos, press, release);
        }
        else {
            rcol.update(mousepos, press, release);
        }
    }

    ballcollider* get_ball() {
        return &bcol;
    }

    rectcollider* get_rect() {
        return &rcol;
    }
};

class createobject {
    Vector2 start;
    Vector2 end;
    Vector2 centre;

    Vector2 size() {
        Vector2 s;
        s.x = abs(start.x - end.x);
        s.y = abs(start.y - end.y);
        return s;
    }

public:
    bool active;
    std::string objecttype;
    createobject(std::string otype) {
        objecttype = otype;
        start.x = 0;
        start.y = 0;
        end.x = 0;
        end.y = 0;
        centre.x = 0;
        centre.y = 0;
        active = false;
    }

    float radius() {
        return abs(start.x - end.x) / 2;
    }

    void initcreation(Vector2 mousepos) {
        start = mousepos;
        active = true;
    }
    ballcollider finishcreationb(Vector2 mousepos) {
        end = mousepos;
        active = false;

        Vector2 s = size();

        if (start.x < end.x) { centre.x = start.x + s.x / 2; }
        else { centre.x = end.x + s.x / 2; }
        if (start.y < end.y) { centre.y = start.y + s.y / 2; }
        else { centre.y = end.y + s.y / 2; }

        return ballcollider(radius(), centre.x, centre.y);
    }
    rectcollider finishcreationr(Vector2 mousepos) {
        end = mousepos;
        active = false;

        Vector2 s = size();

        if (start.x < end.x) { centre.x = start.x + s.x / 2; }
        else { centre.x = end.x + s.x / 2; }
        if (start.y < end.y) { centre.y = start.y + s.y / 2; }
        else { centre.y = end.y + s.y / 2; }

        return rectcollider(s.x, s.y, centre.x, centre.y);
    }
    Vector2 get_start() {
        return start;
    }
    Vector2 get_centre(Vector2 mousepos) {
        end = mousepos;

        Vector2 s = size();

        if (start.x < end.x) { centre.x = start.x + s.x / 2; }
        else { centre.x = end.x + s.x / 2; }
        if (start.y < end.y) { centre.y = start.y + s.y / 2; }
        else { centre.y = end.y + s.y / 2; }

        return centre;
    }
    std::string get_object() {
        return objecttype;
    }
};

int main(void) {

    int screenwidth = 1000;
    int screenheight = 1000;
    InitWindow(screenwidth, screenheight, "physics");

    SetTargetFPS(60);

    createobject ct("ball");

    std::vector<rigidbody> balls = { ballcollider(20, 50, 50, 1, 5, 4), rectcollider(25, 20, 100, 100, 1, 2, 9) };

    bool game = true;
    while (game)
    {
        if (WindowShouldClose()) {
            game = false;
        }

        Vector2 mousepos = GetMousePosition();

        bool mousedown = false;
        if (IsMouseButtonDown(0)) {
            mousedown = true;
        }
        if (IsMouseButtonPressed(1)) {
            if (!ct.active) {
                ct.initcreation(mousepos);
            }
        }
        if (IsMouseButtonReleased(1)) {
            if (ct.active) {
                if (ct.objecttype == "ball") {
                    balls.push_back(ct.finishcreationb(mousepos));
                }
                else {
                    balls.push_back(ct.finishcreationr(mousepos));
                }
            }
        }
        if (IsMouseButtonPressed(2)) {
            if (ct.objecttype == "ball") {
                ct.objecttype = "rect";
            }
            else {
                ct.objecttype = "ball";
            }
        }

       size_t ballscount = balls.size();

        for (int i = 0; i < ballscount; i++) {
            balls.at(i).update(mousepos, IsMouseButtonPressed(0), IsMouseButtonReleased(0));
            if (balls.at(i).shape == "ball") {
                ballcollider* current = balls.at(i).get_ball();
                if (balls.size() >= i) {
                    for (int j = i + 1; j < ballscount; j++) {
                        if (balls.at(j).shape == "ball") {
                            ballcollider* compare = balls.at(j).get_ball();

                            Vector2 v1 = current->get_vel();
                            Vector2 v2 = compare->get_vel(); //velocity values, passed by value

                            Vector2 p1 = current->get_pos();
                            Vector2 p2 = compare->get_pos();// position values

                            if (pincircle(p1, p2, 2 * current->get_r())) {
                                Vector2 dif = Vector2Subtract(p1, p2);// distance between circles

                                float angle = atan2(dif.y, dif.x);

                                resolvecollision(v1, v2, angle);

                                current->set_vel(v1);
                                compare->set_vel(v2);
                            }
                        }
                    }
                }
            }
        }

        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        if (ct.active) {
            Vector2 pos = ct.get_centre(mousepos);
            if (ct.objecttype == "ball") {
                DrawCircleV(pos, ct.radius(), DARKGRAY);
            }
            else {
                pos.x -= static_cast<int>((mousepos.x - ct.get_start().x) / 2);
                pos.y -= static_cast<int>((mousepos.y - ct.get_start().y) / 2);
                DrawRectangle(pos.x, pos.y, static_cast<int>(mousepos.x - ct.get_start().x), static_cast<int>(mousepos.y - ct.get_start().y), DARKGRAY);
            }
        }

        for (int i = 0; i < ballscount; i++) {
            ballcollider currentb = *balls.at(i).get_ball();
            rectcollider currentr = *balls.at(i).get_rect();

            if (balls.at(i).shape == "ball") {
                Vector2 pos = currentb.get_pos();
                currentb.draw();
            }
            else {
                Vector2 pos = currentr.get_corner();
                DrawRectangle(pos.x, pos.y, currentr.get_xsize(), currentr.get_ysize(), BLACK);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
