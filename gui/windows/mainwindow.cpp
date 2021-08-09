#include <QFileDialog>
#include <algorithm>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mesh.h"

MainWindow::~MainWindow() {delete ui;}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    worker = new BackgroundWorker(&model);

//    connect(&controller.model, SIGNAL(requestGeometryUpdate()), SLOT(render_results()));
    connect(&model, SIGNAL(stepAborted()),SLOT(updateGUI()));
    connect(&model, SIGNAL(stepCompleted()), SLOT(updateGUI()));
    connect(worker, SIGNAL(workerPaused()), SLOT(background_worker_paused()));

    // property browser
    pbrowser = new ObjectPropertyBrowser(this);

    // VTK
    qt_vtk_widget = new QVTKOpenGLNativeWidget();
    qt_vtk_widget->setRenderWindow(renderWindow);

    renderer->SetBackground(1.0,1.0,1.0);
    renderer->AddActor(model.mesh->actor_collisions);
    renderer->AddActor(model.mesh->actor_mesh_deformable);
    renderer->AddActor(model.mesh->actor_boundary_all);
    renderer->AddActor(model.mesh->actor_boundary_intended_indenter);

    renderWindow->AddRenderer(renderer);
    renderWindow->GetInteractor()->SetInteractorStyle(specialSelector2D);
    specialSelector2D->mw = this;
    renderer->AddActor(specialSelector2D->actor);


    // splitter
    splitter_main = new QSplitter(Qt::Orientation::Horizontal);
    splitter_main->addWidget(pbrowser);
    splitter_main->addWidget(qt_vtk_widget);
    splitter_main->setStretchFactor(0,100);
    splitter_main->setStretchFactor(1,500);
    setCentralWidget(splitter_main);

    // toolbar - comboboxes
    comboBox_visualizations = new QComboBox();
    ui->toolBar->addWidget(comboBox_visualizations);

    // populate combobox
    QMetaEnum qme = QMetaEnum::fromType<icy::Model::VisOpt>();
    for(int i=0;i<qme.keyCount();i++) comboBox_visualizations->addItem(qme.key(i));

    connect(comboBox_visualizations, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index){ comboboxIndexChanged_visualizations(index); });


    // slider
    slider1 = new QSlider(Qt::Horizontal);
    ui->toolBar->addWidget(slider1);
    slider1->setTracking(true);
    slider1->setMinimum(0);
    slider1->setMaximum(1000);
    connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    labelStepCount = new QLabel();
//    ui->toolBar->addWidget(labelStepCount);

    // statusbar
    statusLabel = new QLabel("-");
    statusLabelStepFactor = new QLabel(" --- ");
    labelVolumeChange = new QLabel(" --- ");
    labelElapsedTime = new QLabel(" --- ");

    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Fixed);
    statusLabelStepFactor->setSizePolicy(sp);
    statusLabelStepFactor->setFixedWidth(100);
    labelStepCount->setSizePolicy(sp);
    labelStepCount->setFixedWidth(100);
    labelVolumeChange->setSizePolicy(sp);
    labelVolumeChange->setFixedWidth(100);
    labelElapsedTime->setSizePolicy(sp);
    labelElapsedTime->setFixedWidth(100);

    ui->statusbar->addWidget(statusLabel);
    ui->statusbar->addPermanentWidget(labelElapsedTime);
    ui->statusbar->addPermanentWidget(labelStepCount);
    ui->statusbar->addPermanentWidget(statusLabelStepFactor);
    ui->statusbar->addPermanentWidget(labelVolumeChange);

    // read/restore saved settings
    QSettings settings(m_sSettingsFile);
    splitter_main->restoreState(settings.value("splitter_main").toByteArray());

    vtkCamera* camera = renderer->GetActiveCamera();
    camera->ParallelProjectionOn();

    QVariant var = settings.value("camData");
    if(!var.isNull()) {
        QByteArray arr = var.toByteArray();
        double *vec = (double*)arr.constData();
        camera->SetPosition(vec[0],vec[1],vec[2]);
        camera->SetFocalPoint(vec[3],vec[4],vec[5]);
        camera->SetViewUp(vec[6],vec[7],vec[8]);
        camera->SetViewAngle(vec[9]);
    }
    camera->Modified();

    renderer->AddActor(scalarBar);
    scalarBar->SetLookupTable(model.mesh->hueLut);

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
    scalarBar->SetVisibility(index != 0);
    renderWindow->Render();
}


