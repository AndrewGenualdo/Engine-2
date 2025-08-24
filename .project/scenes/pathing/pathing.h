//
// Created by cobble on 7/11/2025.
//

#ifndef PATHING_H
#define PATHING_H

#include "cobb/core/scene.h"
#include "cobb/core/camera.h"
#include "cobb/misc/fontRenderer.h"
#include <glm/gtx/rotate_vector.hpp>
#include "ew/ewMath/ewMath.h"
#include "cobb/shapes/circle2d.h"
#include "cobb/shapes/line2d.h"

class PathingScene : public Scene {

public:
    static Camera camera;
    static Window *window;

    PathingScene() = default;
    explicit PathingScene(Window *w);

    void load() override;
    void draw() override;
    void cleanup() override;
    void keyPress(int key, int action, int mods) override;
    void mouseMove(float x, float y) override;

private:
    static FontRenderer fontRenderer;
    static Texture2d blank;



    struct Wall {
        Line2d wall;

        Wall(vec2 p1, vec2 p2) {
            wall = Line2d(p1, p2, vec4(1));
        }
    };

    static std::vector<Wall> walls;

    struct Unit {
        Circle2d unit;
        float speed;
        std::vector<vec2> pathfinding;

        Unit() = default;

        Unit(vec2 pos, float size, float speed) {
            unit = Circle2d(pos, size, 0.03, vec4(1));
            this->speed = speed;
        }

        // 2D cross product
        static float cross(const vec2& a, const vec2& b) {
            return a.x * b.y - a.y * b.x;
        }

        // Returns a perpendicular unit vector to the line (clockwise)
        static vec2 perpendicularRight(const vec2& dir) {
            return vec2(dir.y, -dir.x);
        }

        // Returns the intersection point of [p1, p2] and [q1, q2],
        // or vec2(-1.0f) if they don't intersect
        static vec2 segmentIntersectionPoint(const vec2& p1, const vec2& p2, const vec2& q1, const vec2& q2) {
            vec2 p = p2 - p1;
            vec2 q = q2 - q1;

            float denominator = cross(p, q);

            if (denominator == 0.0f) {
                // Parallel or colinear — no intersection
                return vec2(-1.0f);
            }

            vec2 qp = q1 - p1;
            float t = cross(qp, q) / denominator;
            float u = cross(qp, p) / denominator;

            if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
                // Segments intersect — return point of intersection
                return p1 + t * p;
            }

            return vec2(-1); // No intersection
        }


        void addPathPoint(vec2 point) {
            pathfinding.insert(pathfinding.begin(), point);
        }


        static vec2 rotateAroundPoint(const vec2& origin, const vec2& point, float degrees) {
            // Convert degrees to radians for glm trig functions
            float radians = glm::radians(-degrees);

            // Translate point so origin is at (0,0)
            vec2 translated = point - origin;

            // Create a rotation matrix
            mat2 rotationMatrix = {
                vec2(cos(radians), sin(radians)),
                vec2(-sin(radians), cos(radians))
            };

            // Rotate the translated point
            vec2 rotated = rotationMatrix * translated;

            // Translate back to original position
            return rotated + origin;
        }
    private:
        void getToPoint(int steps, vec2 goal, float deltaTime) {
            if(steps > 20) return;
            vec2 p = pathfinding.size() == 0 ? unit.pos() : pathfinding[0];
            int closestWall = -1;
            float wallDist = 9999999;

            for(int i = 0; i < walls.size(); i++) {
                vec2 dir = normalize(walls[i].wall._end - walls[i].wall._start);
                vec2 p1 = walls[i].wall._start - dir * unit.size();
                vec2 p2 = walls[i].wall._end + dir * unit.size();
                vec2 inter = segmentIntersectionPoint(p, goal, p1, p2);
                if(inter != vec2(-1)) {
                    float dist = distance(p, inter);
                    if(dist < wallDist) {
                        closestWall = i;
                    }
                }
            }


            if(closestWall != -1) {


                //vec2 w1 = walls[closestWall].wall._start;// - dir * unit.size() * 1.05f;
                //vec2 w2 = walls[closestWall].wall._end;// + dir * unit.size() * 1.05f;
                //vec2 detour = distance(p, w1) < distance(p, w2) ? w1 : w2;



                Line2d wall = walls[closestWall].wall;
                vec2 dir = normalize(wall._end - wall._start);
                float radius = unit.size();

                vec2 right = perpendicularRight(dir);
                vec2 left = -right;

                vec2 startBot = wall._start + right * radius; //red -1
                vec2 startTop  = wall._start + left * radius; //green 1
                vec2 endBot = wall._end + right * radius; //blue 1
                vec2 endTop  = wall._end + left * radius; //magenta -1

                float angleChangeDir = 1;

                vec2 wallStart = startBot;
                if(distance(p, startTop) < distance(p, wallStart)) wallStart = startTop;
                if(distance(p, endBot) < distance(p, wallStart)) wallStart = endBot;
                if(distance(p, endTop) < distance(p, wallStart)) wallStart = endTop;

                vec2 wallEndpoint = wall._start;
                if(wallStart == endBot || wallStart == endTop) wallEndpoint = wall._end;

                if(wallStart == startBot || wallStart == endTop) angleChangeDir = -1;
                if(wallStart == startBot) std::cout << "red" << std::endl;
                if(wallStart == startTop) std::cout << "green" << std::endl;
                if(wallStart == endBot) std::cout << "blue" << std::endl;
                if(wallStart == endTop) std::cout << "magenta" << std::endl;

                addPathPoint(wallStart);

                vec2 currentPos = wallStart;
                bool isColliding = segmentIntersectionPoint(currentPos, goal, wall._start, wall._end) != vec2(-1);
                int t = 0;
                while(isColliding) {
                    t++;
                    std::cout << "checking collision..." << std::endl;
                    vec2 goalDir = normalize(goal - currentPos) * speed * deltaTime;
                    std::cout << "Goal Dir: " << goalDir.x << ", " << goalDir.y << std::endl;
                    vec2 goalPos = currentPos + goalDir;
                    bool validDir = distance(goalPos, ew::closestPointOnLine(wall._start, wall._end, goalPos)) > radius;
                    int s = 0;
                    while(!validDir) {
                        s++;
                        goalPos = rotateAroundPoint(currentPos, goalPos, 1.0f * angleChangeDir);
                        //validDir = distance(goalPos, ew::closestPointOnLine(wall._start, wall._end, goalPos)) >= radius;
                        //validDir = distance(wallEndpoint, ew::closestPointOnLine(goalPos, goal, wallEndpoint)) > radius;
                        //if(!validDir) std::cout << "Invalid point: " << goalPos.x << ", " << goalPos.y << std::endl;
                        //else std::cout << "Valid point: " << goalPos.x << ", " << goalPos.y << std::endl;
                        if(s >= 360) {
                            std::cout << "borked" << std::endl;
                            break;
                        }
                    }
                    addPathPoint(goalPos);
                    std::cout << "added point: " << goalPos.x << ", " << goalPos.y << " | with angle: " << s << std::endl;
                    currentPos = pathfinding.size() == 0 ? unit.pos() : pathfinding[0];
                    isColliding = segmentIntersectionPoint(currentPos, goal, wall._start, wall._end) != vec2(-1);
                    if(t > 100) break;
                }

            }
            addPathPoint(goal);
        }
    public:
        void getToPoint(vec2 goal, float deltaTime) {
            getToPoint(0, goal, deltaTime);
        }
    };

    constexpr static int MAX_UNITS = 1;
    Unit units[MAX_UNITS];


    static void drawBackground(bool pride);

};



#endif //PATHING_H
