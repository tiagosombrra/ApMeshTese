#ifndef _PJMESH_MESH_GENERATOR_H_
#define _PJMESH_MESH_GENERATOR_H_

#include "PJmesh/Definitions.h"

#include "Performer/MeshGenerator.h"

namespace PJmesh
{
	class MeshGenerator : public Performer::MeshGenerator
	{
	public:

		MeshGenerator();
		virtual ~MeshGenerator();

		virtual void setUseBoxTest(bool useBoxTest);
		virtual void setBox(const double *min, const double *max);
		virtual void setRangeProportion(double rangeProportion);

		virtual void setFront(MJMesh::Front *front);
		virtual void setBoundary(const MJMesh::Boundary *boundary);
		virtual void setMesh(Data::Mesh *mesh);

		virtual void setVersion(int version);
		virtual void setInternal(int internal);
		virtual void setFactor(double factor);
		virtual void setTol(double tol);
		virtual void setOffset(int offset);
		virtual void setFromone(int fromone);
		virtual int getFromone() const;
		virtual int getJmeshStatus() const;

		virtual void execute();

	protected:

		virtual void convertFrontToJmesh();
		virtual bool executeFrontAdvancing();
		virtual bool executeMeshBuilding();

	protected:

		int pjmesh_num_original_nodes;
		int pjmesh_num_original_faces;
		int *pjmesh_original_faces;
		int pjmesh_num_original_elems;
		int pjmesh_use_box_test;
		double pjmesh_box_min[3];
		double pjmesh_box_max[3];
		double pjmesh_range_proportion;

		MJMesh::Front *front;
		const MJMesh::Boundary *boundary;

		int version;
		int internal;
		int fromone;
		double factor;
		double tol;
		int offset;
		int num_node;
		int num_face;
		double **nodes;
		int *tnodes;
		int **faces;
		int num_gen_nodes;
		double *generated_nodes;
		int *tgenerated_nodes;
		int num_elems;
		int *elems;
		int status;

		Data::VertexVector vMap;

#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))
	public:

		double timeMeshGeneration;
		double timeToJmeshConversion;
		double timeFromJmeshConversion;
#endif //#if ((USE_TIMING) && (USE_TIME_MEASUREMENTS))

	};
}

#endif //#ifndef _PJMESH_MESH_GENERATOR_H_
