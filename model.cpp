#include <H5Cpp.h>
#include "model.h"


void icy::Model::TransferValues()
{
    throw std::runtime_error("not implemented");
}

void icy::Model::Load(std::string fileName)
{
    H5::H5File file(fileName, H5F_ACC_RDONLY);

    // nodes of the new mesh
    H5::DataSet dataset_nodes_new = file.openDataSet("Nodes_New");

    hsize_t dims_out[2];
    H5::DataSpace dataspace = dataset_nodes_new.getSpace();
    dataspace.getSimpleExtentDims( dims_out, NULL);

    unsigned nNodes = dims_out[0];

    hsize_t offset[2] = {};
    hsize_t count[2] = {nNodes, 2};
    dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

    hsize_t dimsm[2] = {nNodes, 2};
    H5::DataSpace memspace(2, dimsm);

//    hsize_t offset_out[2] = {};
//    hsize_t count_out[2] = {nNodes,2};
//    memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );
    std::vector<double> dBuffer;
    dBuffer.resize(2*nNodes);
    dataset_nodes_new.read( dBuffer.data(), H5::PredType::NATIVE_DOUBLE, memspace, dataspace );

    mesh2.nodes.resize(nNodes);
    for(unsigned i=0;i<nNodes;i++) mesh2.nodes[i].Reset(i,dBuffer[i*2+0],dBuffer[i*2+1]);


    // elements of the new mesh
    H5::DataSet dataset_elems_nodes_new = file.openDataSet("Elements_New");

    dataspace = dataset_elems_nodes_new.getSpace();
    dataspace.getSimpleExtentDims( dims_out, NULL);

    unsigned nElems = dims_out[0];

    offset[0] = offset[1] = 0;
    count[0] = nElems;
    count[1] = 3;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

    H5::DataSpace memspace_elems_new(2, count);
//    memspace.selectHyperslab( H5S_SELECT_SET, count, offset);
    std::vector<int> iBuffer;
    iBuffer.resize(3*nElems);
    dataset_elems_nodes_new.read( iBuffer.data(), H5::PredType::NATIVE_INT, memspace_elems_new, dataspace);

    mesh2.elems.resize(nElems);
    for(unsigned i=0;i<nElems;i++)
        for(unsigned j=0;j<3;j++)
            mesh2.elems[i].nds[j] = &mesh2.nodes[iBuffer[i*3+j]];



    // nodes of the old mesh
    H5::DataSet dataset_nodes_old = file.openDataSet("Nodes_Old");

    dataspace = dataset_nodes_old.getSpace();
    dataspace.getSimpleExtentDims(dims_out, NULL);

    nNodes = dims_out[0];

    count[0] = nNodes;
    count[1] = 6;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

    H5::DataSpace memspace_nodes_old(2, count);

//    memspace_nodes_old.selectHyperslab(H5S_SELECT_SET, count, offset);
    dBuffer.resize(6*nNodes);
    dataset_nodes_old.read( dBuffer.data(), H5::PredType::NATIVE_DOUBLE, memspace_nodes_old, dataspace);

    mesh1.nodes.resize(nNodes);
    for(unsigned i=0;i<nNodes;i++)
    {
        Node &nd = mesh1.nodes[i];
        nd.Reset(i,dBuffer[i*6+0],dBuffer[i*6+1]);
        nd.xn << dBuffer[i*6+2],dBuffer[i*6+3];
        nd.vn << dBuffer[i*6+4],dBuffer[i*6+5];
    }

    // elements of the old mesh
    H5::DataSet dataset_elems_nodes_old = file.openDataSet("Elements_Old");
    dataspace = dataset_elems_nodes_old.getSpace();
    dataspace.getSimpleExtentDims( dims_out, NULL);

    nElems = dims_out[0];

    count[0] = nElems;
    count[1] = 3;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

    H5::DataSpace memspace_elems_old(2, count);
//    memspace_elems_old.selectHyperslab( H5S_SELECT_SET, count, offset);
    iBuffer.resize(3*nElems);
    dataset_elems_nodes_old.read( iBuffer.data(), H5::PredType::NATIVE_INT, memspace_elems_old, dataspace);

    mesh1.elems.resize(nElems);
    for(unsigned i=0;i<nElems;i++)
        for(unsigned j=0;j<3;j++)
            mesh1.elems[i].nds[j] = &mesh1.nodes[iBuffer[i*3+j]];

    // values on the old elements
    H5::DataSet dataset_elems_old_data = file.openDataSet("Elements_Old_Data");
    dataspace = dataset_elems_old_data.getSpace();

    count[0] = nElems;
    count[1] = 4;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

    H5::DataSpace memspace_elems_old_data(2, count);
    memspace_elems_old_data.selectHyperslab( H5S_SELECT_SET, count, offset);
    dBuffer.resize(4*nElems);
    dataset_elems_old_data.read( dBuffer.data(), H5::PredType::NATIVE_DOUBLE, memspace_elems_old_data, dataspace);

    mesh1.elems.resize(nElems);
    for(unsigned i=0;i<nElems;i++)
    {
        icy::Element &elem = mesh1.elems[i];
        elem.PiMultiplier << dBuffer[i*4+0], dBuffer[i*4+1],dBuffer[i*4+2],dBuffer[i*4+3];
    }
}




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
