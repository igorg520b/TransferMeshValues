#ifndef FL333_H
#define FL333_H

#include <gmsh.h>

#include <vector>

#include "element.h"

#include <vtkNew.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkDoubleArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkScalarBarActor.h>

#include <QObject>

namespace icy { class MeshView; class Model;}

class icy::MeshView : public QObject
{
    Q_OBJECT
public:
    MeshView();
    std::vector<icy::Node> nodes;
    std::vector<icy::Element> elems;

    // VTK
    vtkNew<vtkScalarBarActor> scalarBar;
    vtkNew<vtkActor> actor_mesh;

    enum VisOpt { none, displacement_x, displacement_y,
                  velocity_x, velocity_y, velocity_mag,
                  Green_strain_xx, Green_strain_yy, Green_strain_xy, plasticity_norm };
    Q_ENUM(VisOpt)


private:

    void ChangeVisualizationOption(VisOpt option);
    void UpdateMeshView();
    void UpdateMeshValues();
    bool showMeshAsDeformed = true; // deformed vs initial
    double minmax[2];
    VisOpt visualizingOption = VisOpt::none;

    vtkNew<vtkPoints> points_deformable;
    vtkNew<vtkDoubleArray> visualized_values;

    // elements
    vtkNew<vtkUnstructuredGrid> ugrid_deformable;
    vtkNew<vtkCellArray> cellArray_deformable;
    vtkNew<vtkDataSetMapper> dataSetMapper_deformable;
    vtkNew<vtkLookupTable> hueLut;

    friend icy::Model;
};

#endif
