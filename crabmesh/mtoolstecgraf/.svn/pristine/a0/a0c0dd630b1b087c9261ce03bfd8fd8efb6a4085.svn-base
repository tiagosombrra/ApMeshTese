#ifndef _PERFORMER_LAPLACIAN_SMOOTHER_H_
#define _PERFORMER_LAPLACIAN_SMOOTHER_H_

#include "Performer/Definitions.h"

#include "Performer/MeshSmoother.h"

namespace Performer
{
	class LaplacianSmoother : public Performer::MeshSmoother
	{
	public:

		LaplacianSmoother(
			UInt dimension,
			Data::Mesh *mesh = NULL,
			bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0,
			bool sortPointsDisplacement = false);
		virtual ~LaplacianSmoother();

		virtual void setDimension(UInt dimension);

		virtual void setTestAdjacency(bool testAdjacency);

		virtual void setFactor(Real factor);

		virtual void setSortPointsDisplacement( bool sortPointsDisplacement );

		virtual bool smooth(Data::Vertex *v) const;

	protected:

		virtual bool smooth(Data::Vertex *v, const Data::ElementList &elements, const Data::PointSet &points) const;

		virtual void findAdjacency(const Data::ElementList &elements, const Data::Vertex *v, Data::PointSet &adjacency) const;

		virtual void findAdjacency(const Data::Vertex *v, Data::PointSet &adjacency) const;

		virtual void findAdjacentElements(const Data::Vertex *v, Data::ElementList &elements) const;

		virtual bool isAdjacencyOk(const Data::Vertex *v, const Data::ElementList &elements) const;

		virtual void findDisplacement(const Data::Point *point,
			const Real position[], const Data::PointSet &adjacency, Real displacement[]) const;

	protected:

		UInt dimension;

		bool testAdjacency;

		Real factor;

		bool sortPointsDisplacement;
	};
}

#endif //#ifndef _PERFORMER_LAPLACIAN_SMOOTHER_H_
