#include "engine/objects/Path.hpp"
#include "engine/utilities/Utilities.hpp"
#include "engine/gengine-globals/scene.hpp"

#include <iostream>
#include <ostream>

#include "engine/gengine-globals/scene.hpp"

using namespace gengine;

Path::Path(const Vertex &tar, const Vertex &startPos, GENG_Path pathType, float speed = 1.0f)
        : target(tar), startPos(startPos), pathType(pathType), speed(speed),
        complete(false) {

    initDist = (target - startPos).mag();
    if (initDist <= 0.1)
        complete = 7;
    if (pathType == GENG_Path::REBOUND) {
        std::cerr << "rebound!\n";
        std::cerr << "start: " << startPos.to_string() << std::endl;
        std::cerr << "target: " << target.to_string() << std::endl;
        std::cerr << (startPos-target).to_string() << std::endl;
        this->startPos = (startPos - target).unit();
        std::cerr << "initializing: " << this->startPos.to_string() << std::endl;
        this->speed = speed * -1.0f;
    }
}

// Puts an object on a path
void Path::to_path(Vertex &pos) {
    if (complete == 7) {
        pos = target;
        return;
    }
    if (pathType == GENG_Path::BALLOON)
        balloon(pos);
    else if (pathType == GENG_Path::TORPEDO)
        torpedo(pos);
    else if (pathType == GENG_Path::SINE)
        sine(pos);
    else if (pathType == GENG_Path::REBOUND)
        rebound(pos);
    else
        linear(pos);

    if ((target-pos).mag() < 0.00001)
        complete = 7;
}

// Defines the equation of motion for linear movement.
void Path::linear(Vertex &pos) {
    // Get what direction we have to move via the unit vector
    Vertex unit = (target - pos).unit();
    // If one of the coordinates reaches the finish, all of them do. (Linear)
    if (overshoot(pos.x, target.x, speed*glb::scene.dt*unit[0]))
        complete |= 1;
     if (overshoot(pos.y, target.y, speed*glb::scene.dt*unit[1]))
         complete |= 2;
    if (overshoot(pos.z, target.z, speed*glb::scene.dt*unit[2]))
        complete |= 4;
}

// Defines the equation of motion for balloonish movement
void Path::balloon(Vertex &pos)
{
    //calculate our unit vector
    Vertex unit = (target - pos).unit();
    float DIST = target.dist(pos);
    Vertex dist = unit * glb::scene.dt * speed * ((DIST < 1.0f) ? 1.0f : DIST) / 70.0f;
    //minimum step of movement
    float minny = (target.dist(pos) * 0.005f);
    const float minStep = std::max(minny, 0.01f);
    // This way we can preserve minimum stepping
    for (int i = 0; i < 3; ++i) {
        if (abs(dist[i]) < minStep) {
            dist[i] = (unit[i] >= 0.0f ? minStep : -minStep);
        }
    }
    // Then we check if we overshoot or whether we're done or not!
    if (!(complete & 1) && overshoot(pos.x, target.x, dist[0]))
        complete |= 1;
    if (!(complete & 2) && overshoot(pos.y, target.y, dist[1]))
        complete |= 2;
    if (!(complete & 4) && overshoot(pos.z, target.z, dist[2]))
        complete |= 4;

}

// Defines the equation of motion for a torpedo-ish movement
#define PATH_TORPEDO_EQ(U, DIST) (U * (scene::dt * speed * (1.0f/DIST) * PATH_TORPEDO_AMPLIFY))
void Path::torpedo(Vertex &pos) {
    //calculate our unit vector
    Vertex unit = (target - pos).unit();
    Vertex dist = unit * glb::scene.dt * speed * (1.0f/target.dist(pos)) * 0.10f;
    if (dist.mag() < 0.00001f) {
        complete = 7;
        return;
    }
    //minimum step of movement
    const float minStep = 0.0025f * target.dist(pos);
    // This way we can preserve minimum stepping
    for (int i = 0; i < 3; ++i) {
        if (fabs(dist[i]) < minStep) {
            dist[i] = (unit.x >= 0.0f ? minStep : -minStep);
        }
    }
    // Then we check if we overshoot or whether we're done or not!
    if (!(complete & 1) && overshoot(pos.x, target.x, dist[0]))
        complete |= 1;
    if (!(complete & 2) && overshoot(pos.y, target.y, dist[1]))
        complete |= 2;
    if (!(complete & 4) && overshoot(pos.z, target.z, dist[2]))
        complete |= 4;
    // If we're reached all paths, (complete == PATH_COMPLETE)
}

void Path::sine(Vertex &pos) {
    Vertex unit = (target - startPos).unit();

    // real distance-based time instead of x-based time
    float distTraveled = (pos - startPos).mag();
    float totalDist = (target - startPos).mag();

    float time = distTraveled / speed;
    float totalTime = totalDist / speed;

    // Linear base motion
    pos += unit * speed * glb::scene.dt * 0.5;

    // Apply sine only to y
    float sineOffset = 10 * sinf(2.0f * utils::pi * time / totalTime);
    if (distTraveled < 0.001f)
        pos.y += unit[1] * distTraveled + sineOffset;
    else
        pos.y = startPos.y + unit[1] * distTraveled + sineOffset;

    // Overshoot on x/z only
    if (overshoot(pos.x, target.x, unit[0] * glb::scene.dt * speed))
        complete |= 1;
    if ((complete & 1) && abs(pos.y - startPos.y) < 0.01f)
        complete |= 2;
    // We dont need to check for y because otherwise itll bmess with movement.
    complete |= 2;
    if (overshoot(pos.z, target.z, unit[2] * glb::scene.dt * speed))
        complete |= 4;
}

void Path::rebound(Vertex &pos) {
    speed += 0.025f * ((fabsf(speed) > 1.0f) ? sqrtf(fabsf(speed)) : 1.0f);
    Vertex dist = startPos * (speed * -20.f);

    if (speed > 0.0f) {
        if (!(complete & 1) && overshoot(pos.x, target.x, dist[0]))
            complete |= 1;
        if (!(complete & 2) && overshoot(pos.y, target.y, dist[1]))
            complete |= 2;
        if (!(complete & 4) && overshoot(pos.z, target.z, dist[2]))
            complete |= 4;
    }
    else
        pos += dist;
}

// Lets us tell if given our movement, if we've overshot
bool Path::overshoot(float &o, float& t, float dist) const {
    const float newpos = o + dist;
    if ((dist > 0 && newpos > t) || (dist < 0 && newpos < t)) {
        o = t;
        return true;
    }
    o = newpos;
    return false;
}

// Tells us if a path is complete
bool Path::path_complete() const {
    return (complete == 7);
}


Vertex Path::get_target() {
    return target;
}

std::string Path::to_string() {
    std::string ret = "Target: "  + target.to_string();
    ret += "\nSpeed: " + std::to_string(speed);
    ret += "\nComplete: " + std::to_string(int(complete));
    ret += "\nPath Type: " + std::to_string(static_cast<int>(pathType));
    return ret;
}
