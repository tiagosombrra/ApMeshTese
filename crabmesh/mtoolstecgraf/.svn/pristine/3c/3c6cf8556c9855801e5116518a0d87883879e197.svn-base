#ifndef _TOOLS_SURFACES_IDENTIFIER_H_
#define _TOOLS_SURFACES_IDENTIFIER_H__

#include "Tools/Definitions.h"
#include "Tools/ExternalTool.h"
#include "Tools/AdjacenciesIdentifier.h"

namespace Tools
{
	class SurfacesIdentifier : public Tools::ExternalTool
	{
	public:

		SurfacesIdentifier(const Tool *tool = NULL);
		virtual ~SurfacesIdentifier();

		virtual void setBoundary(const Data::Boundary *boundary);
		virtual void setAngle(Real angle);

		virtual const Data::MeshList &getSurfaces() const;

		virtual void execute();

	protected:

		virtual Data::Mesh *makeMesh() const;
		virtual Data::Vertex *makeVertex(const Data::Vertex *v, Data::VertexHashMap &map) const;
		virtual Data::FrontElement *makeFrontElement(Data::Vertex *v1, Data::Vertex *v2, Data::VertexHashMap &map, const Data::Vector3D &normal) const;
		virtual Data::Element *makeElement(Data::/*Front*/Element *fel, Data::VertexHashMap &map) const;

		virtual Real angle(const Data::/*Front*/Element *fel1, const Data::/*Front*/Element *fel2) const;

		virtual void identifySurface(const Data::/*Front*/Element *fel, const AdjacenciesIdentifier::AdjacencyMap &adjacencies,
			Data::/*Front*/ElementHash &used, Data::/*Front*/ElementList &surface, Data::VertexList &edges, Data::VectorVector &normals) const;

	protected:

		typedef std::unordered_map<ULInt, std::unordered_map<ULInt, ULInt> > EdgeIdMap;

	protected:

		const Data::Boundary *boundary;

		Real angleThreshold;

		Data::MeshList surfaces;

		Performer::IdManager *idManager;

		mutable EdgeIdMap idMap;

	};
}

#endif //#ifndef _TOOLS_SURFACES_IDENTIFIER_H_
