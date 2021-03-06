#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSizePolicy>
#include <QPushButton>
#include <QSplitter>
#include <QLabel>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QProgressBar>
#include <QMenu>
#include <QList>
#include <QDebug>
#include <QComboBox>
#include <QMetaEnum>
#include <QDir>

#include <QVTKOpenGLNativeWidget.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkDataSetMapper.h>
#include <vtkCamera.h>

//#include <vtkOBJReader.h>
//#include <vtkNamedColors.h>
#include <vtkProperty.h>
//#include <vtkVersion.h>
//#include <vtkWindowToImageFilter.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkMultiBlockDataSet.h>
//#include <vtkCompositePolyDataMapper2.h>
//#include <vtkPoints.h>
//#include <vtkIdList.h>
#include <vtkDataSetSurfaceFilter.h>
//#include <vtkAxesActor.h>

//#include <vtkDataSetSurfaceFilter.h>
//#include <vtkScalarBarActor.h>
//#include <vtkTextProperty.h>

//#include <vtkAreaPicker.h>
//#include <vtkPointPicker.h>
//#include <vtkProp3DCollection.h>
//#include <vtkRenderWindowInteractor.h>

//#include "SpecialSelector2D.h"
#include <vtkInteractorStyleRubberBand2D.h>

#include <iostream>
#include <string>
//#include <vector>
//#include <map>
//#include <stdexcept>
//#include <cstdint>

#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void comboboxIndexChanged_visualizations(int index);

    void on_action_quit_triggered();

    void on_actionOpen_triggered();

    void on_actionUndeformed_State_triggered(bool checked);

    void on_actionTransfer_Mesh_triggered();

private:
    icy::Model model;
    QString m_sSettingsFile = "ic4_config";
    QComboBox *comboBox_visualizations;

    // splitter and the right side window
    QSplitter *splitter_main;

    // VTK
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow1, renderWindow2;
    std::unique_ptr<QVTKOpenGLNativeWidget> qt_vtk_widget1, qt_vtk_widget2;
//    QVTKOpenGLNativeWidget *
    vtkNew<vtkRenderer> renderer1, renderer2;
    vtkNew<vtkInteractorStyleRubberBand2D> interactor1, interactor2;


    const QString wtitle = "icFlow4: Finite Element Simulation";

    friend class SpecialSelector2D;


};
#endif // MAINWINDOW_H
