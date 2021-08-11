#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include "mainwindow.h"
#include "meshview.h"
#include "./ui_mainwindow.h"

//#include "node.h"

MainWindow::~MainWindow() {delete ui;}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // VTK
    qt_vtk_widget1 = std::make_unique<QVTKOpenGLNativeWidget>();
    qt_vtk_widget2 = std::make_unique<QVTKOpenGLNativeWidget>();

    qt_vtk_widget1->setRenderWindow(renderWindow1);
    qt_vtk_widget2->setRenderWindow(renderWindow2);

    renderer1->SetBackground(1.0,1.0,1.0);
    renderer1->AddActor(model.mesh1.actor_mesh);
    renderer1->AddActor(model.mesh1.scalarBar);

    renderer2->SetBackground(1.0,1.0,1.0);
    renderer2->AddActor(model.mesh2.actor_mesh);
    renderer2->AddActor(model.mesh2.scalarBar);

    renderWindow1->AddRenderer(renderer1);
    renderWindow1->GetInteractor()->SetInteractorStyle(interactor1);

    renderWindow2->AddRenderer(renderer2);
    renderWindow2->GetInteractor()->SetInteractorStyle(interactor2);

    // splitter
    splitter_main = new QSplitter(Qt::Orientation::Horizontal);
    splitter_main->addWidget(qt_vtk_widget1.get());
    splitter_main->addWidget(qt_vtk_widget2.get());
    splitter_main->setStretchFactor(0,500);
    splitter_main->setStretchFactor(1,500);
    setCentralWidget(splitter_main);

    // toolbar - comboboxes
    comboBox_visualizations = new QComboBox();
    ui->toolBar->addWidget(comboBox_visualizations);

    // populate combobox
    QMetaEnum qme = QMetaEnum::fromType<icy::MeshView::VisOpt>();
    for(int i=0;i<qme.keyCount();i++) comboBox_visualizations->addItem(qme.key(i));

    connect(comboBox_visualizations, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index){ comboboxIndexChanged_visualizations(index); });


    // read/restore saved settings
    renderer1->GetActiveCamera()->ParallelProjectionOn();
    renderer2->GetActiveCamera()->ParallelProjectionOn();

    renderWindow1->Render();
    renderWindow2->Render();

//    comboBox_visualizations->setCurrentIndex(settings.value("vis_option").toInt());

}



void MainWindow::on_action_quit_triggered() { this->close(); }



void MainWindow::comboboxIndexChanged_visualizations(int index)
{
    qDebug() << "comboboxIndexChanged_visualizations " << index;
    model.ChangeVisualizationOption((icy::MeshView::VisOpt)index);
    renderWindow1->Render();
    renderWindow2->Render();
}



void MainWindow::on_actionOpen_triggered()
{
    QDir dir("sample_mesh");
    QString fileName = QFileDialog::getOpenFileName(this, "Open Scene",
                                                    dir.path(),
                                                    "Scene Files (*.h5)");
    if (fileName.isEmpty()) return;
    model.Load(fileName.toStdString());
    model.UpdateMeshView();
    renderWindow1->Render();
    renderWindow2->Render();
}


void MainWindow::on_actionUndeformed_State_triggered(bool checked)
{
    model.SetViewDeformed(!checked);
    renderWindow1->Render();
    renderWindow2->Render();
}


void MainWindow::on_actionTransfer_Mesh_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Mesh values were not transferred");
    msgBox.exec();
}

