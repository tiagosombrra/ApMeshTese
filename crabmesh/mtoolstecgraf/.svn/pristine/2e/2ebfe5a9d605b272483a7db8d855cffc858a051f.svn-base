#include "PMTools2/MeshGenerator.h"

#if USE_MTOOLS

#include "PMTools/MshRegion3D.h"

using namespace PMTools2;

PMTools2::MeshGenerator::MeshGenerator() :
    PMTools::MeshGenerator::MeshGenerator()
{
    this->useUniformRefinement = true;
    this->useUniformSize = true;
}

PMTools2::MeshGenerator::~MeshGenerator()
{

}

void PMTools2::MeshGenerator::setUseUniformRefinement(bool useUniformRefinement)
{
    this->useUniformRefinement = useUniformRefinement;
}

void PMTools2::MeshGenerator::setUseUniformSize(bool useUniformSize)
{
    this->useUniformSize = useUniformSize;
}

PMTools::MshRegion3D *PMTools2::MeshGenerator::makeMshRegion() const
{
    PMTools::MshRegion3D *mshRegion = PMTools::MeshGenerator::makeMshRegion();

    mshRegion->SetUseUniformRefinement(this->useUniformRefinement);
    mshRegion->SetUseUniformSize(this->useUniformSize);

    return mshRegion;
}

#endif //#if USE_MTOOLS
