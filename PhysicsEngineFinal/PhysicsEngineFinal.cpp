#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "PhysicsFunctions.h"


class ballcollider {// class for ball rigidbody
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

class rectcollider { //class for rect rigidbody
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
            if (pinrect(mousepos, get_corner(), Vector2{ sizex, sizey })) {
                held = true;
            }
        }
        if (release) {
            held = false;
        }
        if (held) {
            clicked(mousepos);
        }
        wallbounce(000, 1000, 00, 1000, position, velocity, sizex / 2, sizey / 2);
        position = Vector2Add(position, velocity);
    }

    void draw() {
        DrawRectangle(position.x - sizex / 2, position.y - sizey / 2, sizex, sizey, BLACK);
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

class createobject {//contains information about object to be created 
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
    ballcollider finishcreationb(Vector2 mousepos) {//creates a ball collider
        end = mousepos;
        active = false;

        Vector2 s = size();

        if (start.x < end.x) { centre.x = start.x + s.x / 2; }
        else { centre.x = end.x + s.x / 2; }
        if (start.y < end.y) { centre.y = start.y + s.y / 2; }
        else { centre.y = end.y + s.y / 2; }

        return ballcollider(radius(), centre.x, centre.y);
    }
    rectcollider finishcreationr(Vector2 mousepos) {//creates a rectangle collider
        end = mousepos;
        active = false;

        Vector2 s = size();

        if (start.x < end.x) { centre.x = start.x + s.x / 2; }
        else { centre.x = end.x + s.x / 2; }
        if (start.y < end.y) { centre.y = start.y + s.y / 2; }
        else { centre.y = end.y + s.y / 2; }

        return rectcollider(s.x, s.y, centre.x, centre.y);
    }
    void draw(Vector2 mousepos) {
        if (active) {
            Vector2 pos = get_centre(mousepos);
            if (objecttype == "ball") {
                DrawCircleV(pos, radius(), DARKGRAY);
            }
            else {
                pos.x -= static_cast<int>((mousepos.x - get_start().x) / 2);
                pos.y -= static_cast<int>((mousepos.y - get_start().y) / 2);
                DrawRectangle(pos.x, pos.y, static_cast<int>(mousepos.x - get_start().x), static_cast<int>(mousepos.y - get_start().y), DARKGRAY);
            }
        }
    }

    Vector2 get_start() {
        return start;
    }
    Vector2 get_centre(Vector2 mousepos) {//center position for circle creation
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

void circlecollide(ballcollider* current, ballcollider* compare) {
    Vector2 p1 = current->get_pos();
    Vector2 p2 = compare->get_pos();// position values

    float r1 = current->get_r();
    float r2 = compare->get_r();

    if (circleoverlap(p1, p2, r1, r2)) {
        Vector2 dif = Vector2Subtract(p1, p2);// distance between circles

        float angle = atan2(dif.x, dif.y);

        Vector2 v1 = current->get_vel();
        Vector2 v2 = compare->get_vel(); //velocity values, passed by value

        float m1 = current->get_m();
        float m2 = compare->get_m();// mass

        bounce(v1, v2, m1, m2, angle, 0.95);//velocity calculations
        resolvecirclecollision(p1, p2, r1, r2, m1, m2);//changes p1 and p2 so they arent inside each other

        current->set_pos(p1);
        compare->set_pos(p2);//applies position

        current->set_vel(v1);
        compare->set_vel(v2);//applies velocity
    }
}

int main() {

    int screenwidth = 1000;
    int screenheight = 1000;
    InitWindow(screenwidth, screenheight, "physics");

    SetTargetFPS(60);

    createobject ct("ball");

    std::vector<ballcollider> bodies = { ballcollider(20, 50, 50, 1, 5, 4) };
    bodies.shrink_to_fit();

    Image wizardimg = LoadImage("Resources/Wizardofphysics.png"); // Loaded in CPU memory (RAM)
    Image wizardimg2 = LoadImage("Resources/Wizardofphysics.png");

    ImageResizeNN(&wizardimg, 160, 160);
    ImageResize(&wizardimg2, 170, 170);

    Texture2D wizard = LoadTextureFromImage(wizardimg);// Image converted to texture, GPU memory (VRAM)
    Texture2D wizardshadow = LoadTextureFromImage(wizardimg2);

    UnloadImage(wizardimg);
    UnloadImage(wizardimg2);

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
                    bodies.push_back(ct.finishcreationb(mousepos));
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

        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < 795; i++) {
                bodies.push_back(ballcollider(GetRandomValue(3, 14), GetRandomValue(10, 990), GetRandomValue(10, 990)));
            }
            for (int i = 0; i < 200; i++) {
                bodies.push_back(ballcollider(GetRandomValue(14, 34), GetRandomValue(10, 990), GetRandomValue(10, 990)));
            }
            for (int i = 0; i < 5; i++) {
                bodies.push_back(ballcollider(GetRandomValue(34, 70), GetRandomValue(10, 990), GetRandomValue(10, 990)));
            }
        }
        if (IsKeyPressed(KEY_B)) {
            for (int i = 0; i < 1500; i++) {
                bodies.push_back(ballcollider(GetRandomValue(8, 12), GetRandomValue(10, 990), GetRandomValue(10, 990)));
            }
        }
        if (IsKeyPressed(KEY_W)) {
            bodies.push_back(ballcollider(GetRandomValue(8, 12), 850, 930, 1750, GetRandomValue(-5, -1), GetRandomValue(-5, -1)));
        }

        size_t bodycount = bodies.size();

        for (int i = 0; i < bodycount; i++) {//update every rigidbody
            bodies.at(i).update(mousepos, IsMouseButtonPressed(0), IsMouseButtonReleased(0));
        }

        for (int i = 0; i < bodycount; i++) {//updates velocities
            //if (bodies.at(i).shape == "ball") {
                ballcollider* current = &bodies.at(i);
                if (bodies.size() >= i) {
                    for (int j = i + 1; j < bodycount; j++) {
                        //if (bodies.at(j).shape == "ball") {
                            ballcollider* compare = &bodies.at(j);

                            circlecollide(current, compare);
                            
                        //}
                    }
                }
            //}
        }

        BeginDrawing();

        ClearBackground(LIGHTGRAY);

        ct.draw(mousepos);

        for (int i = 0; i < bodycount; i++) {
            bodies.at(i).draw();
        }

        DrawTexture(wizardshadow, screenwidth - wizard.width, screenheight - wizard.height, CLITERAL(Color){ 0, 0, 0, 64 });
        DrawTexture(wizard, screenwidth - wizard.width, screenheight - wizard.height, WHITE);

        EndDrawing();
    }

    UnloadTexture(wizard);
    UnloadTexture(wizardshadow);

    CloseWindow();

    return 0;
}