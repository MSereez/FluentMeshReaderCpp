    
    fReader_.readbasicStructure(filename);
    fReader_.getNodes();
    fReader_.getInteriorFaces();
    fReader_.getBoundaryFaces();
    fReader_.sortallvectors();
    
    unsortedMesh unsortedMesh_(fReader_);    
    unsortedMesh *mesh = new unsortedMesh(unsortedMesh_);