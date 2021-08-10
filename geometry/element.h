#ifndef ELEMENT123_H
#define ELEMENT123_H

//#include <Eigen/Geometry>

#include "node.h"

namespace icy { class Element; class Node; }

struct icy::Element
{
    icy::Node* nds[3];          // initialized when the geometry is loaded or remeshed

    Eigen::Matrix2d Dm, DmInv;  // reference shape matrix
    Eigen::Matrix2d PiMultiplier;   // multiplicative plasticity
    Eigen::Matrix2d F;  // deformation gradient
    Eigen::Matrix2d GreenStrain;

    void Reset(Node *nd0, Node *nd1, Node *nd2);
};

#endif // ELEMENT123_H
