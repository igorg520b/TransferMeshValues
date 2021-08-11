#include <H5Cpp.h>
#include "model.h"


void icy::Model::TransferValues()
{
    throw std::runtime_error("not implemented");
}

void icy::Model::Load(std::string fileName)
{
    H5::H5File file(fileName, H5F_ACC_RDONLY);
    constexpr unsigned ElemDataFields = 4;
    H5::IntType datatype_int(H5::PredType::NATIVE_INT);
    H5::FloatType datatype_double(H5::PredType::NATIVE_DOUBLE);

    H5::DataSet dataset_nodes_new = file.openDataSet("Nodes_New");

    hsize_t dims_out[2];
    H5::DataSpace dataspace = dataset_nodes_new.getSpace();
    int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);

    std::cout << dims_out[0] << "; " << dims_out[1] << std::endl;
    unsigned nNodes = dims_out[1];


    hsize_t offset[2] = {};
    hsize_t count[2] = {2, nNodes};
    dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

    hsize_t dimsm[2] = {1, nNodes};
    H5::DataSpace memspace(2, dimsm);

    hsize_t offset_out[2] = {};
    hsize_t count_out[3] = {2, nNodes};
    memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );
    std::vector<double> dBuffer;
    dBuffer.resize(2*nNodes);
    dataset_nodes_new.read( dBuffer.data(), H5::PredType::NATIVE_DOUBLE, memspace, dataspace );
    std::cout << dBuffer[0] << ", " << dBuffer[1] << std::endl;
}

/*
 *
 *



// NEW MESH (no displacements, velocity, elem data)
double *nodes_buffer = new double[nodes.size()*2];
for(unsigned i=0;i<nodes.size();i++)
{
    Node* nd = nodes[i];
    nodes_buffer[i*2+0] = nd->x_initial.x();
    nodes_buffer[i*2+1] = nd->x_initial.y();
}
hsize_t dimsf_nodes_new[2] = {nodes.size(), 2};
H5::DataSpace dataspace_nodes_new(2, dimsf_nodes_new);
H5::DataSet dataset_nodes_new = file.createDataSet("Nodes_New", datatype_double, dataspace_nodes_new);
dataset_nodes_new.write(nodes_buffer, H5::PredType::NATIVE_DOUBLE);

int *elems_buffer_nodes = new int[elems.size()*3];
for(unsigned i=0;i<elems.size();i++)
{
    Element* e = elems[i];
    int idx0 = e->nds[0]->locId;
    int idx1 = e->nds[1]->locId;
    int idx2 = e->nds[2]->locId;
    if(idx0>=nodes.size() || idx1>=nodes.size() || idx2>=nodes.size()) throw std::out_of_range("nodal index error");
    elems_buffer_nodes[i*3+0] = e->nds[0]->locId;
    elems_buffer_nodes[i*3+1] = e->nds[1]->locId;
    elems_buffer_nodes[i*3+2] = e->nds[2]->locId;
}

hsize_t dimsf_elems_nodes_new[2] = {elems.size(), 3};
H5::DataSpace dataspace_elems_nodes_new(2, dimsf_elems_nodes_new);
H5::DataSet dataset_elems_nodes = file.createDataSet("Elements_New", datatype_int, dataspace_elems_nodes_new);

dataset_elems_nodes.write(elems_buffer_nodes, H5::PredType::NATIVE_INT);

delete[] nodes_buffer;
delete[] elems_buffer_nodes;



// OLD MESH (includes nodal displacements, velocities, and per-element data)
nodes_buffer = new double[nodes_tmp.size()*6];
for(unsigned i=0;i<nodes_tmp.size();i++)
{
    Node* nd = nodes_tmp[i];
    if(nd->x_initial.x()==0 && nd->x_initial.y()==0) std::cout << "node " << i << " is zero\n";
    nodes_buffer[i*6+0] = nd->x_initial.x();
    nodes_buffer[i*6+1] = nd->x_initial.y();
    nodes_buffer[i*6+2] = nd->xn.x();
    nodes_buffer[i*6+3] = nd->xn.y();
    nodes_buffer[i*6+4] = nd->vn.x();
    nodes_buffer[i*6+5] = nd->vn.y();
}
hsize_t dimsf_nodes_old[2] = {nodes_tmp.size(), 6};
H5::DataSpace dataspace_nodes_old(2, dimsf_nodes_old);
H5::DataSet dataset_old = file.createDataSet("Nodes_Old", datatype_double, dataspace_nodes_old);
dataset_old.write(nodes_buffer, H5::PredType::NATIVE_DOUBLE);

double *elems_buffer_data = new double[elems_tmp.size()*ElemDataFields];
elems_buffer_nodes = new int[elems_tmp.size()*3];
for(unsigned i=0;i<elems_tmp.size();i++)
{
    Element* e = elems_tmp[i];
    elems_buffer_nodes[i*3+0] = e->nds[0]->locId;
    elems_buffer_nodes[i*3+1] = e->nds[1]->locId;
    elems_buffer_nodes[i*3+2] = e->nds[2]->locId;
    elems_buffer_data[i*ElemDataFields+0] = e->PiMultiplier(0,0);
    elems_buffer_data[i*ElemDataFields+1] = e->PiMultiplier(0,1);
    elems_buffer_data[i*ElemDataFields+2] = e->PiMultiplier(1,0);
    elems_buffer_data[i*ElemDataFields+3] = e->PiMultiplier(1,1);
}

hsize_t dimsf_elems_nodes_old[2] = {elems_tmp.size(), 3};
hsize_t dimsf_elems_data_old[2] = {elems_tmp.size(), ElemDataFields};
H5::DataSpace dataspace_elems_nodes_old(2, dimsf_elems_nodes_old);
H5::DataSpace dataspace_elems_data_old(2, dimsf_elems_data_old);

H5::DataSet dataset_elems_nodes_old = file.createDataSet("Elements_Old", datatype_int, dataspace_elems_nodes_old);
H5::DataSet dataset_elems_data_old = file.createDataSet("Elements_Old_Data", datatype_double, dataspace_elems_data_old);

dataset_elems_nodes_old.write(elems_buffer_nodes, H5::PredType::NATIVE_INT);
dataset_elems_data_old.write(elems_buffer_data, H5::PredType::NATIVE_DOUBLE);

delete[] nodes_buffer;
delete[] elems_buffer_nodes;
delete[] elems_buffer_data;*/



void icy::Model::ChangeVisualizationOption(icy::MeshView::VisOpt option)
{
    mesh1.ChangeVisualizationOption(option);
    mesh2.ChangeVisualizationOption(option);
    UpdateMeshValues();
}

void icy::Model::SetViewDeformed(bool option)
{
    mesh1.showMeshAsDeformed = mesh2.showMeshAsDeformed = option;
    UpdateMeshView();
}

void icy::Model::UpdateMeshView()
{
    mesh1.UpdateMeshView();
    mesh2.UpdateMeshView();
}

void icy::Model::UpdateMeshValues()
{
    mesh1.UpdateMeshValues();
    mesh2.UpdateMeshValues();

    double minmax[2] = {std::min(mesh1.minmax[0],mesh2.minmax[0]),std::max(mesh1.minmax[1],mesh2.minmax[1])};
    mesh1.hueLut->SetTableRange(minmax[0], minmax[1]);
    mesh2.hueLut->SetTableRange(minmax[0], minmax[1]);
}