#include "element.h"
#include "node.h"


void icy::Element::Reset(Node *nd0, Node *nd1, Node *nd2)
{
    nds[0] = nd0;
    nds[1] = nd1;
    nds[2] = nd2;

    GreenStrain = F = PiMultiplier = Dm = DmInv = Eigen::Matrix2d::Zero();
}

