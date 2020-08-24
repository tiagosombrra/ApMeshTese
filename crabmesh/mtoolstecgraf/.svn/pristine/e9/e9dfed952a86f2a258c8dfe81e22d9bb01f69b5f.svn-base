#ifndef _PMJMESH_PARTITION_H_
#define _PMJMESH_PARTITION_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Partition.h"
#include "Data/Box.h"
#include "MJMesh/Boundary.h"

namespace PMJMesh
{
	class Partition : public Parallel::Partition
#if USE_GUI
		, public Data::Drawable
#endif //#if USE_GUI
	{
	public:

		Partition();
		Partition(Data::Box *box, Real load);
		virtual ~Partition();

		virtual void setBox(Data::Box *box);
		virtual Data::Box *getBox() const;

		virtual void setSubfront(PMJMesh::Front *subfront);
		virtual PMJMesh::Front *getSubfront() const;

		virtual Real getLoad() const;

		///TODO consider edges adjacent to points in 3 dimensions
		virtual UInt packSize(const Parallel::Communicator *comm,
			ULInt numVertices, ULInt numElements) const;
		virtual UInt packSize(const Parallel::Communicator *comm) const;
		///TODO consider edges adjacent to points in 3 dimensions
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			//Data::VertexList &vertices, Data::FrontElementList &boundary, Data::FrontElementList &elements,
			//Data::FrontElementList &rejected, Data::FrontElementList &crossing) const;
			const Data::Front *front) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		///TODO consider edges adjacent to points in 3 dimensions
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

#if USE_GUI
		virtual void setR(Real r) const;
		virtual Real getR() const;

		virtual void setG(Real g) const;
		virtual Real getG() const;

		virtual void setB(Real b) const;
		virtual Real getB() const;

		virtual void highlight() const;
		virtual void unhighlight() const;
		virtual bool isHighlighted() const;

		using Data::Drawable::draw;
		virtual void draw() const;

		virtual void drawId() const;
		virtual void drawId(Real r, Real g, Real b) const;

		virtual std::string text() const;
#endif //#if USE_GUI

/*//debug
#if USE_GUI
    public:

        Data::GraphNodeList cells;
        MJMesh::Boundary boundary;
#endif //#if USE_GUI
//endebug*/

	protected:

		Data::Box *box;
		PMJMesh::Front *subfront;
	};
}

#endif //#ifndef _PMJMESH_PARTITION_H_
