#include "generics/Path.hpp"

#include <iostream>
#include <ostream>

#include "Constants.hpp"
#include "utilities/Utilities.hpp"

Path::Path(const Vertex &tar, const Vertex &startPos, uint8_t pathType, float speed = 1.0f)
        : target(tar), startPos(startPos), pathType(pathType), speed(speed),
        complete(false) {

    initDist = (target - startPos).mag();
    if (initDist <= 0.1)
        complete = PATH_COMPLETE;
    Vertex test = (target - startPos);
}

// Puts an object on a path
void Path::to_path(Vertex &pos) {
    if (complete == PATH_COMPLETE) {
        pos = target;
        return;
    }
    if (pathType == PATH_BALLOON)
        balloon(pos);
    else if (pathType == PATH_TORPEDO)
        torpedo(pos);
    else if (pathType == PATH_SINE)
        sine(pos);
    else
        linear(pos);

    if ((target-pos).mag() < 0.00001)
        complete = PATH_COMPLETE;
}

// Defines the equation of motion for linear movement.
#define PATH_LINEAR_EQ(U) (speed * scene::dt* U)
void Path::linear(Vertex &pos) {
    // Get what direction we have to move via the unit vector
    Vertex unit = (target - pos).unit();
    // If one of the coordinates reaches the finish, all of them do. (Linear)
    if (overshoot(pos.x, target.x, PATH_LINEAR_EQ(unit[0])))
        complete |= PATH_COMPLETE_X;
     if (overshoot(pos.y, target.y, PATH_LINEAR_EQ(unit[1])))
         complete |= PATH_COMPLETE_Y;
    if (overshoot(pos.z, target.z, PATH_LINEAR_EQ(unit[2])))
        complete |= PATH_COMPLETE_Z;
}

// Defines the equation of motion for balloonish movement
#define PATH_BALLOON_EQ(U, DIST) (U * (scene::dt * speed * (DIST) / PATH_BALLOON_DAMP))
void Path::balloon(Vertex &pos)
{
    //calculate our unit vector
    Vertex unit = (target - pos).unit();
    float DIST = target.dist(pos);
    Vertex dist = PATH_BALLOON_EQ(unit, (DIST < 1.0f) ? 1.0f : DIST);
    //minimum step of movement
    float minny = (target.dist(pos) * 0.025f);
    const float minStep = std::max(minny, 0.01f);
    // This way we can preserve minimum stepping
    for (int i = 0; i < 3; ++i) {
        if (abs(dist[i]) < minStep) {
            dist[i] = (unit[i] >= 0.0f ? minStep : -minStep);
        }
    }
    // Then we check if we overshoot or whether we're done or not!
    if (!(complete & PATH_COMPLETE_X) && overshoot(pos.x, target.x, dist[0]))
        complete |= PATH_COMPLETE_X;
    if (!(complete & PATH_COMPLETE_Y) && overshoot(pos.y, target.y, dist[1]))
        complete |= PATH_COMPLETE_Y;
    if (!(complete & PATH_COMPLETE_Z) && overshoot(pos.z, target.z, dist[2]))
        complete |= PATH_COMPLETE_Z;

}

// Defines the equation of motion for a torpedo-ish movement
#define PATH_TORPEDO_EQ(U, DIST) (U * (scene::dt * speed * (1.0f/DIST) * PATH_TORPEDO_AMPLIFY))
void Path::torpedo(Vertex &pos) {
    //calculate our unit vector
    Vertex unit = (target - pos).unit();
    Vertex dist = PATH_TORPEDO_EQ(unit, target.dist(pos));
    if (dist.mag() < 0.00001f) {
        complete = PATH_COMPLETE;
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
    if (!(complete & PATH_COMPLETE_X) && overshoot(pos.x, target.x, dist[0]))
        complete |= PATH_COMPLETE_X;
    if (!(complete & PATH_COMPLETE_Y) && overshoot(pos.y, target.y, dist[1]))
        complete |= PATH_COMPLETE_Y;
    if (!(complete & PATH_COMPLETE_Z) && overshoot(pos.z, target.z, dist[2]))
        complete |= PATH_COMPLETE_Z;
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
    pos += unit * speed * scene::dt;

    // Apply sine only to y
    float sineOffset = 10*sinf(2.0f * utils::pi * time / totalTime);
    pos.y = startPos.y + unit[1] * distTraveled + sineOffset;
    // Overshoot on x/z only
    if (overshoot(pos.x, target.x, PATH_LINEAR_EQ(unit[0])))
        complete |= PATH_COMPLETE_X;
    // We dont need to check for y because otherwise itll bmess with movement.
    complete |= PATH_COMPLETE_Y;
    if (overshoot(pos.z, target.z, PATH_LINEAR_EQ(unit[2])))
        complete |= PATH_COMPLETE_Z;
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
    return (complete == PATH_COMPLETE);
}


Vertex Path::get_target() {
    return target;
}

std::string Path::to_string() {
    std::string ret = "Target: "  + target.to_string();
    ret += "\nSpeed: " + std::to_string(speed);
    ret += "\nComplete: " + std::to_string(int(complete));
    ret += "\nPath Type: " + std::to_string(pathType);
    return ret;
}
