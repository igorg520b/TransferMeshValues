#ifndef MODEL_H
#define MODEL_H

#include "meshview.h"

#include <string>
#include <iostream>
#include <QObject>

namespace icy {class Model;}

class icy::Model
{
public:
    MeshView mesh1, mesh2;

    void TransferValues();  // transfer the values from mesh1 to mesh2

    void Load(std::string fileName);

    void ChangeVisualizationOption(icy::MeshView::VisOpt option);
    void SetViewDeformed(bool option);
    void UpdateMeshView();
    void UpdateMeshValues();
};

#endif // MODEL_H
