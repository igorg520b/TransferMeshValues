#ifndef SPECIALSELECTOR2D_H
#define SPECIALSELECTOR2D_H

//#include "vtkInteractionStyleModule.h" // For export macro
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkArcSource.h>
//#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>

class vtkUnsignedCharArray;

class MainWindow;

class SpecialSelector2D : public vtkInteractorStyleRubberBand2D
{
public:
    static SpecialSelector2D* New();
    vtkTypeMacro(SpecialSelector2D, vtkInteractorStyleRubberBand2D);

//    SpecialSelector2D();

//    MainWindow* mw;

    void OnLeftButtonDown() override {};
    void OnLeftButtonUp() override {};
    void OnRightButtonDown() override {};
    void OnRightButtonUp() override {};
//    void OnMouseMove() override;

//    void OnMouseWheelForward() override;
//    void OnMouseWheelBackward() override;

};

#endif
