#include "meshview.h"
#include <numeric>
#include <algorithm>
#include <iterator>

#include <vtkTextProperty.h>

icy::MeshView::MeshView()
{
    visualized_values->SetName("visualized_values");

    ugrid_deformable->SetPoints(points_deformable);
    dataSetMapper_deformable->SetInputData(ugrid_deformable);
    dataSetMapper_deformable->UseLookupTableScalarRangeOn();
    dataSetMapper_deformable->SetLookupTable(hueLut);

    actor_mesh->SetMapper(dataSetMapper_deformable);
    actor_mesh->GetProperty()->VertexVisibilityOff();
    actor_mesh->GetProperty()->EdgeVisibilityOn();
    actor_mesh->GetProperty()->SetColor(0.84938, 0.872213, 0.848103);
    actor_mesh->GetProperty()->SetEdgeColor(90.0/255.0, 90.0/255.0, 97.0/255.0);
    actor_mesh->GetProperty()->LightingOff();
    actor_mesh->GetProperty()->ShadingOff();
    actor_mesh->GetProperty()->SetInterpolationToFlat();
    actor_mesh->PickableOff();
    actor_mesh->GetProperty()->SetLineWidth(1);

    scalarBar->SetLookupTable(hueLut);

    scalarBar->SetMaximumWidthInPixels(130);
    scalarBar->SetBarRatio(0.07);
    scalarBar->SetMaximumHeightInPixels(400);
    scalarBar->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
    scalarBar->GetPositionCoordinate()->SetValue(0.90,0.015, 0.0);
    scalarBar->SetLabelFormat("%.1e");
    scalarBar->GetLabelTextProperty()->BoldOff();
    scalarBar->GetLabelTextProperty()->ItalicOff();
    scalarBar->GetLabelTextProperty()->ShadowOff();
    scalarBar->GetLabelTextProperty()->SetColor(0.1,0.1,0.1);

}


void icy::MeshView::ChangeVisualizationOption(VisOpt option)
{
    if(option == visualizingOption) return;
    visualizingOption = option;

    if(visualizingOption == VisOpt::none)
    {
        dataSetMapper_deformable->ScalarVisibilityOff();
        ugrid_deformable->GetPointData()->RemoveArray("visualized_values");
        ugrid_deformable->GetCellData()->RemoveArray("visualized_values");
        return;
    }
    else if(visualizingOption == VisOpt::displacement_x ||
            visualizingOption == VisOpt::displacement_y ||
            visualizingOption == VisOpt::velocity_x ||
            visualizingOption == VisOpt::velocity_y ||
            visualizingOption == VisOpt::velocity_mag)
    {
        ugrid_deformable->GetCellData()->RemoveArray("visualized_values");
        ugrid_deformable->GetPointData()->AddArray(visualized_values);
        ugrid_deformable->GetPointData()->SetActiveScalars("visualized_values");
        dataSetMapper_deformable->SetScalarModeToUsePointData();
        dataSetMapper_deformable->ScalarVisibilityOn();
    }
    else
    {
        ugrid_deformable->GetPointData()->RemoveArray("visualized_values");
        ugrid_deformable->GetCellData()->AddArray(visualized_values);
        ugrid_deformable->GetCellData()->SetActiveScalars("visualized_values");
        dataSetMapper_deformable->SetScalarModeToUseCellData();
        dataSetMapper_deformable->ScalarVisibilityOn();
    }
}

void icy::MeshView::UpdateMeshValues()
{
    if(nodes.size()==0)
    {
        dataSetMapper_deformable->ScalarVisibilityOff();
        ugrid_deformable->GetPointData()->RemoveArray("visualized_values");
        ugrid_deformable->GetCellData()->RemoveArray("visualized_values");
        return;
    }

    switch(visualizingOption)
    {
    // per node
    case VisOpt::displacement_x:
        visualized_values->SetNumberOfValues(nodes.size());
        for(std::size_t i=0;i<nodes.size();i++)
            visualized_values->SetValue(i, nodes[i].xn.x()-nodes[i].x_initial.x());
        break;

    case VisOpt::displacement_y:
        visualized_values->SetNumberOfValues(nodes.size());
        for(std::size_t i=0;i<nodes.size();i++)
            visualized_values->SetValue(i, nodes[i].xn.y()-nodes[i].x_initial.y());
        break;

    case VisOpt::velocity_x:
        visualized_values->SetNumberOfValues(nodes.size());
        for(std::size_t i=0;i<nodes.size();i++)
            visualized_values->SetValue(i, nodes[i].vn.x());
        break;

    case VisOpt::velocity_y:
        visualized_values->SetNumberOfValues(nodes.size());
        for(std::size_t i=0;i<nodes.size();i++)
            visualized_values->SetValue(i, nodes[i].vn.y());
        break;

    case VisOpt::velocity_mag:
        visualized_values->SetNumberOfValues(nodes.size());
        for(std::size_t i=0;i<nodes.size();i++)
            visualized_values->SetValue(i, nodes[i].vn.norm());
        break;


        // per element
    case VisOpt::Green_strain_xx:
        visualized_values->SetNumberOfValues(elems.size());
        for(std::size_t i=0;i<elems.size();i++)
            visualized_values->SetValue(i, elems[i].GreenStrain(0,0));
        break;

    case VisOpt::Green_strain_yy:
        visualized_values->SetNumberOfValues(elems.size());
        for(std::size_t i=0;i<elems.size();i++)
            visualized_values->SetValue(i, elems[i].GreenStrain(1,1));
        break;

    case VisOpt::Green_strain_xy:
        visualized_values->SetNumberOfValues(elems.size());
        for(std::size_t i=0;i<elems.size();i++)
            visualized_values->SetValue(i, elems[i].GreenStrain(0,1));
        break;

    case VisOpt::plasticity_norm:
        visualized_values->SetNumberOfValues(elems.size());
        for(std::size_t i=0;i<elems.size();i++)
            visualized_values->SetValue(i, (elems[i].PiMultiplier-Eigen::Matrix2d::Identity()).norm());
        break;

    default:
        break;
    }
    visualized_values->Modified();

    visualized_values->GetValueRange(minmax);
//    hueLut->SetTableRange(minmax[0], minmax[1]);
}


void icy::MeshView::UpdateMeshView()
{
    points_deformable->SetNumberOfPoints(nodes.size());

    double x[3]={};

    if(showMeshAsDeformed)
    {
        for(const icy::Node &nd : nodes)
        {
            x[0] = nd.xn[0];
            x[1] = nd.xn[1];
            points_deformable->SetPoint((vtkIdType)nd.locId, x);
        }
    }
    else
    {
        for(const icy::Node &nd : nodes)
        {
            x[0] = nd.x_initial[0];
            x[1] = nd.x_initial[1];
            points_deformable->SetPoint((vtkIdType)nd.locId, x);
        }
    }
    points_deformable->Modified();

    cellArray_deformable->Reset();

    // deformable material - elements
    for(icy::Element &tr : elems)
    {
        vtkIdType pts[3] = {tr.nds[0]->locId, tr.nds[1]->locId,tr.nds[2]->locId};
        cellArray_deformable->InsertNextCell(3, pts);
    }
    ugrid_deformable->SetCells(VTK_TRIANGLE, cellArray_deformable);


    if(visualizingOption != VisOpt::none) UpdateMeshValues();

}




