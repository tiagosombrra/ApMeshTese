#include "Performer/MeshImprover.h"

#include "Performer/MeshOptimizer.h"

const Performer::Status Performer::MeshImprover::notInit =
	Performer::Status(MeshImprover::NOT_INIT, "not initiated");
const Performer::Status Performer::MeshImprover::init =
	Performer::Status(MeshImprover::INIT, "initiated");
const Performer::Status Performer::MeshImprover::ended =
	Performer::Status(MeshImprover::ENDED, "ended");

Performer::MeshImprover::MeshImprover(Data::Mesh *mesh, UInt numImproves) :
	Builder()
{
	this->setMesh(mesh);

	this->setNumImproves(numImproves);

	this->status = MeshImprover::notInit;

	this->step = 0;
	this->currOpt = this->optimizers.end();
}

Performer::MeshImprover::~MeshImprover()
{
	while (!this->optimizers.empty())
	{
		delete this->optimizers.back();

		this->optimizers.pop_back();
	}
}

void Performer::MeshImprover::setMesh(Data::Mesh *mesh)
{
	this->mesh = mesh;
}

void Performer::MeshImprover::setNumImproves(UInt numImproves)
{
	this->numImproves = numImproves;
}

void Performer::MeshImprover::setOptimizers(const MeshOptimizerList &optimizers)
{
	this->optimizers = optimizers;

	this->currOpt = this->optimizers.end();
}

void Performer::MeshImprover::add(MeshOptimizer *opt)
{
	this->optimizers.push_back(opt);

	this->currOpt = this->optimizers.end();
}

void Performer::MeshImprover::execute()
{
	switch (this->status.getType())
	{
	case MeshImprover::NOT_INIT:
		if (!this->mesh)
		{
			this->status = MeshImprover::ended;

			return;
		}

		this->status = MeshImprover::init;

	case MeshImprover::INIT:
		while (this->step < this->numImproves)
		{
			if (this->currOpt == this->optimizers.end())
			{
				this->currOpt = this->optimizers.begin();
			}

			while (this->currOpt != this->optimizers.end())
			{
				if ((*this->currOpt)->getStatus().getType() == (*this->currOpt)->notInit.getType())
				{
					(*this->currOpt)->setMesh(this->mesh);
				}

				(*this->currOpt)->execute();

				if ((*this->currOpt)->getStatus().getType() == (*this->currOpt)->ended.getType())
				{
					(*this->currOpt)->setMesh(NULL);

					(*this->currOpt)->setStatus((*this->currOpt)->notInit);

					this->currOpt++;
				}

#if USE_GUI
				if (Data::isStepByStep()) break;
#endif //#if USE_GUI
			}

			if (this->currOpt == this->optimizers.end())
			{
				this->step++;
			}

#if USE_GUI
			if (Data::isStepByStep()) break;
#endif //#if USE_GUI
		}

		if ((this->step == this->numImproves) &&
			(this->currOpt == this->optimizers.end()))
		{
			this->status = MeshImprover::ended;
		}

#if USE_GUI
		if (Data::isStepByStep()) break;
#endif //#if USE_GUI

	case MeshImprover::ENDED:
		break;
	}

	/*the above swicth replaces these commented lines
	if (!this->mesh)
	{
		return;
	}

	for (UInt i = 0; i < this->numImproves; i++)
	{
		for (MeshOptimizerList::iterator iter = this->optimizers.begin();
			 iter != this->optimizers.end(); iter++)
		{
			(*iter)->setMesh(this->mesh);

			(*iter)->execute();

			(*iter)->setMesh(NULL);
		}
	}*/
}
