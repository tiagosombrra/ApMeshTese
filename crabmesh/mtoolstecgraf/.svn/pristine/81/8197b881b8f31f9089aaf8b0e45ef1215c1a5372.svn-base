#ifndef _PMJMESH_MJMESH_TASK_H_
#define _PMJMESH_MJMESH_TASK_H_

#include "PMJMesh/Definitions.h"

#include "Parallel/Task.h"
#include "Data/Drawable.h"

namespace PMJMesh
{
	class MJMeshTask : public Parallel::Task
#if USE_GUI
		, public Data::Drawable
#endif //#if USE_GUI
	{
	public:

		MJMeshTask();
		MJMeshTask(PMJMesh::Partition *partition,
			PMJMesh::WorkerMainDrive *builder,
			PMJMesh::Communicator *comm = NULL);
		virtual ~MJMeshTask();

		virtual void setPartition(Parallel::Partition *p);

		virtual void setMesh(MJMesh::Mesh *mesh);
		virtual MJMesh::Mesh *getMesh() const;

		virtual PMJMesh::Front *getFront() const;

		virtual Real load() const;

		virtual void makeBuilder();

		virtual void resizeGenerated(UInt size);
		virtual void setGenerated(UInt shift, bool generated);
		virtual bool isGenerated(UInt shift) const;
		virtual bool isCompletelyGenerated() const;

		virtual UInt packSize(const Parallel::Communicator *comm) const;
		virtual Parallel::Package pack(const Parallel::Communicator *comm) const;
		virtual void unpack(const Parallel::Communicator *comm, const Parallel::Package &p, Int &position);

		virtual void execute();

#if USE_THREAD_COLOR
		virtual void colorizeMesh(UInt process = -1) const;
#endif //#if USE_THREAD_COLOR

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

		virtual void drawGeneratedState() const;

		virtual std::string text() const;
#endif //#if USE_GUI

	protected:

		virtual UInt packSize(const Parallel::Communicator *comm,
			ULInt numMeshVertices, ULInt numMeshFrontElements, ULInt numMeshElements,
			ULInt numFrontVertices, ULInt numFrontFrontElements) const;

		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position) const;
		virtual void pack(const Parallel::Communicator *comm, char *buffer, Int size, Int &position,
			//Data::VertexList &meshVertices, Data::FrontElementList &meshFels, Data::ElementList &meshEls,
			const Data::Mesh *mesh,
			//Data::VertexList &frontVertices, Data::FrontElementList &frontBoundary, Data::FrontElementList &frontElements,
			//Data::FrontElementList &frontRejected, Data::FrontElementList &frontCrossing) const;
			const Data::Front *front) const;

	protected:

		enum TaskType taskType;

		Data::BoolVector generated;
	};
}

#endif //#ifndef _PMJMESH_MJMESH_TASK_H_
