#ifndef _TOOLS_BOUNDARY_LAPLACIAN_SMOOTHER_H_
#define _TOOLS_BOUNDARY_LAPLACIAN_SMOOTHER_H_

#include "Tools/Definitions.h"
#include "Tools/Tool.h"
#include "MJMesh/LaplacianSmoother.h"
#if USE_EXPERIMENTAL_3
#include "Data/Vector2D.h"
#include "Data/Vector3D.h"
#endif //#if USE_EXPERIMENTAL_3

namespace Tools
{
	class BoundaryLaplacianSmoother :
		public MJMesh::LaplacianSmoother,
		public Tools::Tool
	{
	public:

		BoundaryLaplacianSmoother(
			UInt dimension,
			Data::Boundary *boundary = NULL,
			Data::Mesh *mesh = NULL,
			bool testAdjacency = true,
			//Real factor = 0.5);
			Real factor = 1.0,
			bool sortPointsDisplacement = false);
		virtual ~BoundaryLaplacianSmoother();

		virtual void setTolerance(Real tolerance);
		virtual void setNormalTestEnabled(bool normalTestEnabled);

		virtual bool isSmoothed() const;

		virtual void execute();

		using MJMesh::LaplacianSmoother::smooth;

	protected:

		virtual bool smooth(Data::Vertex *v, const Data::ElementList &elements, const Data::PointSet &points) const;

		virtual void findAdjacentElements(const Data::Vertex *v, Data::ElementList &elements) const;

		virtual Real quality(const Data::Element *e) const;
		virtual bool worse(Real q1, Real q2) const;

		virtual bool isAdjacencyOk(const Data::Vertex *v, const Data::ElementList &elements) const;

		virtual void findDisplacement(const Data::Point *point,
			const Real position[], const Data::PointSet &adjacency, Real displacement[]) const;

#if USE_ATTRIBS
		virtual void clearAttribs() const;
#endif //#if USE_ATTRIBS

	protected:

		Data::Boundary *boundary;

		Real tolerance;

		bool normalTestEnabled;

#if USE_EXPERIMENTAL_3
		mutable Data::Vector2D normal2D;
		mutable Data::Vector3D normal3D;
#else
		mutable Data::Vector normal;
#endif //#if USE_EXPERIMENTAL_3
		mutable Real worst;

		mutable bool smoothed;
	};
}

#endif //#ifndef _TOOLS_BOUNDARY_LAPLACIAN_SMOOTHER_H_
