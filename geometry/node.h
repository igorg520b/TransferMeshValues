#ifndef NODE_H
#define NODE_H

#include <Eigen/Core>

namespace icy { class Node; }

struct icy::Node
{
    int locId = -1;
    Eigen::Vector2d x_initial = Eigen::Vector2d::Zero();  // initial configuration
    Eigen::Vector2d xn = Eigen::Vector2d::Zero();
    Eigen::Vector2d vn = Eigen::Vector2d::Zero();     // position and velocity at step n

    void Reset(int locId_, double x, double y) {locId = locId_; x_initial << x,y;};
};

#endif // NODE_H
