#ifndef MESHFRAGMENT_H
#define MESHFRAGMENT_H

#include <vector>
#include <string>
#include "element.h"

namespace icy { class MeshFragment; }

class icy::MeshFragment
{    
public:
    std::vector<icy::Node> nodes;
    std::vector<icy::Element> elems;

    void LoadFragment(std::string fileName);

};

#endif // MESHFRAGMENT_H
