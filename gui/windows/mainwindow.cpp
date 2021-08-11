#include <QFileDialog>
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
    qt_vtk_widget = new QVTKOpenGLNativeWidget();
    qt_vtk_widget->setRenderWindow(renderWindow);

    renderer->SetBackground(1.0,1.0,1.0);
    renderer->AddActor(model.mesh1.actor_mesh);
//    renderer->AddActor(model.mesh1.scalarBar);

    renderWindow->AddRenderer(renderer);
    renderWindow->GetInteractor()->SetInteractorStyle(specialSelector2D);
//    specialSelector2D->mw = this;

    // splitter
    splitter_main = new QSplitter(Qt::Orientation::Horizontal);
    QLabel *lbl = new QLabel("hello1");
    splitter_main->addWidget(lbl);
    splitter_main->addWidget(qt_vtk_widget);
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
    vtkCamera* camera = renderer->GetActiveCamera();
    camera->ParallelProjectionOn();
    camera->Modified();


    renderWindow->Render();

//    comboBox_visualizations->setCurrentIndex(settings.value("vis_option").toInt());

}

void MainWindow::showEvent( QShowEvent*)
{
}

void MainWindow::closeEvent( QCloseEvent* event )
{

}

void MainWindow::on_action_quit_triggered() { this->close(); }



void MainWindow::comboboxIndexChanged_visualizations(int index)
{
    qDebug() << "comboboxIndexChanged_visualizations " << index;
//    model.ChangeVisualizationOption((icy::Model::VisOpt)index);
    renderWindow->Render();
}


