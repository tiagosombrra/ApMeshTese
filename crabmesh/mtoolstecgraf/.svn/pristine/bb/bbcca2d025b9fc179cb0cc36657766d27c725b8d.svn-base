#include "PMJMesh/Definitions.h"

#if (!USE_GUI)

#include "PMJMesh/MainDrive.h"
#include "PMJMesh/Communicator.h"

#include <cstdarg>
#include <list>

/*enum Operator
{
	NO = 0,
	EQ,
	NE,
	GT,
	GE,
	LT,
	LE,
	ELSE
};

template <typename Type, typename MapType = Type>
class ValueTest
{
public:
	ValueTest();
	ValueTest(enum Operator op, bool defaultValue, Type testValue, MapType realValue);
	virtual ~ValueTest();

	virtual void call(MapType value) = 0;

public:
	enum Operator op;
	bool defaultValue;
	Type testValue;
	MapType realValue;
};

template <typename Type, typename MapType>
ValueTest<Type, MapType>::ValueTest() :
	op(NO), defaultValue(false), testValue(), realValue()
{

}

template <typename Type, typename MapType>
ValueTest<Type, MapType>::ValueTest(enum Operator _op, bool _defaultValue, Type _testValue, MapType _realValue) :
	op(_op), defaultValue(_defaultValue), testValue(_testValue), realValue(_realValue)
{

}

template <typename Type, typename MapType>
ValueTest<Type, MapType>::~ValueTest()
{

}

template <typename Type, typename MapType = Type>
class Function : public ValueTest<Type, MapType>
{
public:
	Function();
	Function(enum Operator op, bool defaultValue, Type testValue, MapType realValue, void (*function)(MapType));
	virtual ~Function();

	virtual void call(MapType value);

public:
	void (*function)(MapType);
};

template <typename Type, typename MapType>
Function<Type, MapType>::Function() :
	ValueTest<Type, MapType>(), function(NULL)
{

}

template <typename Type, typename MapType>
Function<Type, MapType>::Function(enum Operator op, bool defaultValue, Type testValue, MapType realValue, void (*_function)(MapType)) :
	ValueTest<Type, MapType>(op, defaultValue, testValue, realValue), function(_function)
{

}

template <typename Type, typename MapType>
Function<Type, MapType>::~Function()
{

}

template <typename Type, typename MapType>
void Function<Type, MapType>::call(MapType value)
{
	if (this->defaultValue)
	{
		(*this->function)(this->realValue);
	}
	else
	{
		(*this->function)(value);
	}
}

template <typename Class, typename Type, typename MapType = Type>
class Method : public ValueTest<Type, MapType>
{
public:
	Method();
	Method(enum Operator op, bool defaultValue, Type testValue, MapType realValue, Class *object, void (Class::*method)(MapType));
	virtual ~Method();

	virtual void call(MapType value);

public:
	Class *object;
	void (Class::*method)(MapType);
};

template <typename Class, typename Type, typename MapType>
Method<Class, Type, MapType>::Method() :
	ValueTest<Type, MapType>(), object(NULL), method(NULL)
{

}

template <typename Class, typename Type, typename MapType>
Method<Class, Type, MapType>::Method(enum Operator op, bool defaultValue, Type testValue, MapType realValue, Class *_object, void (Class::*_method)(MapType)) :
	ValueTest<Type, MapType>(op, defaultValue, testValue, realValue), object(_object), method(_method)
{

}

template <typename Class, typename Type, typename MapType>
Method<Class, Type, MapType>::~Method()
{

}

template <typename Class, typename Type, typename MapType>
void Method<Class, Type, MapType>::call(MapType value)
{
	if (this->defaultValue)
	{
		(this->object->*method)(this->realValue);
	}
	else
	{
		(this->object->*method)(value);
	}
}

class InputMapItem
{
public:
	virtual ~InputMapItem() {};

	virtual unsigned int process(unsigned int i, unsigned int argc, const char *argv[], bool used[]) const = 0;
};

template <typename Type, typename MapType = Type>
class IInputMapItem : public InputMapItem
{
public:
	virtual ~IInputMapItem();

	virtual unsigned int process(unsigned int i, unsigned int argc, const char *argv[], bool used[]) const;

protected:

	bool processForType(const Type &value) const;
	bool process(const char *argv) const;
	bool test(const Type &value, const ValueTest<Type, MapType> *test) const;

protected:
	bool silent;
	unsigned int numParameters;
	std::string error;
	std::list<ValueTest<Type, MapType> *> tests;
};

template <typename Type, typename MapType>
IInputMapItem<Type, MapType>::~IInputMapItem()
{
	while (!this->tests.empty())
	{
		delete this->tests.front();
		this->tests.pop_front();
	}
}

template <typename Type, typename MapType>
bool IInputMapItem<Type, MapType>::test(const Type &value, const ValueTest<Type, MapType> *test) const
{
	switch (test->op)
	{
	case NO:
		return true;
	case EQ:
		return (value == test->testValue);
	case NE:
		return (value != test->testValue);
	case GT:
		return (value >  test->testValue);
	case GE:
		return (value >= test->testValue);
	case LT:
		return (value <  test->testValue);
	case LE:
		return (value <= test->testValue);
	case ELSE:
		return false;
	}

	return false;
}

template <typename Type, typename MapType>
bool IInputMapItem<Type, MapType>::processForType(const Type &value) const
{
	ValueTest<Type, MapType> *felse = NULL;

	for (typename std::list<ValueTest<Type, MapType> *>::const_iterator iter = this->tests.begin();
		 iter != this->tests.end(); iter++)
	{
		if (this->test(value, (*iter)))
		{
			(*iter)->call(static_cast<MapType>(value));

			return true;
		}
		else if ((!felse) && ((*iter)->op == ELSE))
		{
			felse = (*iter);
		}
	}

	if (felse)
	{
		felse->call(static_cast<MapType>(value));

		return true;
	}

	if (!this->silent)
	{
		std::cout << this->error << std::endl;
	}

	return false;
}

template <typename Type, typename MapType>
bool IInputMapItem<Type, MapType>::process(const char *argv) const
{
	std::string s = argv;

	Type value = Data::convert<Type>(s);

	return this->processForType(value);
}

template <>
bool IInputMapItem<const std::string &, const std::string &>::process(const char *argv) const
{
	const std::string &value = Data::convert(argv);

	return this->processForType(value);
}

template <typename Type, typename MapType>
unsigned int IInputMapItem<Type, MapType>::process(unsigned int i, unsigned int argc, const char *argv[], bool used[]) const
{
	used[i] = true;

	unsigned int count = 1;

	if (this->numParameters == 0)
	{
		const char *c = "";

		this->process(c);
	}
	else
	{
		for (unsigned int j = 1; j <= this->numParameters; j++)
		{
			if (i + j >= argc)
			{
				if (!this->silent)
				{
					std::cout << this->error << std::endl;
				}

				continue;
			}

			try
			{
				if (this->process(argv[i + j]))
				{
					used[i + j] = true;

					count++;
				}
			}
			catch (std::ios_base::failure &e)
			{
				if (!this->silent)
				{
					std::cout << this->error << std::endl;
				}
			}
		}
	}

	return count;
}

template <typename Type, typename MapType = Type>
class FInputMapItem : public IInputMapItem<Type, MapType>
{
public:
	FInputMapItem(void (*function)(MapType), MapType value);
	FInputMapItem(void (*function)(MapType), Type value, MapType realValue);
	FInputMapItem(void (*function)(MapType), unsigned int numParameters, const std::string &error);
	FInputMapItem(bool silent, unsigned int numParameters, const std::string &error, unsigned int numFunctions, ...);
};

template <typename Type, typename MapType>
FInputMapItem<Type, MapType>::FInputMapItem(void (*function)(MapType), MapType value)
{
	this->silent = true;
	this->numParameters = 0;
	this->error = "";

	Function<Type, MapType> *func = new Function<Type, MapType>(
		NO, true, value, value, function);

	this->tests.push_back(func);
}

template <typename Type, typename MapType>
FInputMapItem<Type, MapType>::FInputMapItem(void (*function)(MapType), Type value, MapType realValue)
{
	this->silent = true;
	this->numParameters = 0;
	this->error = "";

	Function<Type, MapType> *func = new Function<Type, MapType>(
		NO, true, value, realValue, function);

	this->tests.push_back(func);
}

template <typename Type, typename MapType>
FInputMapItem<Type, MapType>::FInputMapItem(void (*function)(MapType), unsigned int numParameters, const std::string &error)
{
	this->silent = false;
	this->numParameters = numParameters;
	this->error = error;

	Function<Type, MapType> *func = new Function<Type, MapType>(
		NO, false, static_cast<Type>(0), static_cast<MapType>(0), function);

	this->tests.push_back(func);
}

template <typename Type, typename MapType>
FInputMapItem<Type, MapType>::FInputMapItem(bool silent, unsigned int numParameters, const std::string &error, unsigned int numFunctions, ...)
{
	this->silent = silent;
	this->numParameters = numParameters;
	this->error = error;

	va_list functions;
	va_start(functions, 5*numFunctions);

	for (unsigned int i = 0; i < numFunctions; i++)
	{
		Function<Type, MapType> *func = new Function<Type, MapType>(
			va_arg(functions, enum Operator),
			va_arg(functions, bool),
			va_arg(functions, Type),
			va_arg(functions, MapType),
			va_arg(functions, void (*)(Type)));

		this->tests.push_back(func);
	}

	va_end(functions);
}

template <typename Class, typename Type, typename MapType = Type>
class CInputMapItem : public IInputMapItem<Type, MapType>
{
public:
	CInputMapItem(Class *object, void (Class::*method)(MapType), MapType value);
	CInputMapItem(Class *object, void (Class::*method)(MapType), Type value, MapType realValue);
	CInputMapItem(Class *object, void (Class::*method)(MapType), unsigned int numParameters, const std::string &error);
	CInputMapItem(bool silent, unsigned int numParameters, const std::string &error, Class *object, unsigned int numMethods, ...);
};

template <typename Class, typename Type, typename MapType>
CInputMapItem<Class, Type, MapType>::CInputMapItem(Class *object, void (Class::*method)(MapType), MapType value)
{
	this->silent = true;
	this->numParameters = 0;
	this->error = "";

	Method<Class, Type, MapType> *func = new Method<Class, Type, MapType>(
		NO, true, value, value, object, method);

	this->tests.push_back(func);
}

template <typename Class, typename Type, typename MapType>
CInputMapItem<Class, Type, MapType>::CInputMapItem(Class *object, void (Class::*method)(MapType), Type value, MapType realValue)
{
	this->silent = true;
	this->numParameters = 0;
	this->error = "";

	Method<Class, Type, MapType> *func = new Method<Class, Type, MapType>(
		NO, true, value, realValue, object, method);

	this->tests.push_back(func);
}

template <typename Class, typename Type, typename MapType>
CInputMapItem<Class, Type, MapType>::CInputMapItem(Class *object, void (Class::*method)(MapType), unsigned int numParameters, const std::string &error)
{
	this->silent = false;
	this->numParameters = numParameters;
	this->error = error;

	Method<Class, Type, MapType> *func = new Method<Class, Type, MapType>(
		NO, false, static_cast<Type>(0), static_cast<MapType>(0), object, method);

	this->tests.push_back(func);
}

template <typename Class, typename Type, typename MapType>
CInputMapItem<Class, Type, MapType>::CInputMapItem(bool silent, unsigned int numParameters, const std::string &error, Class *object, unsigned int numMethods, ...)
{
	this->silent = silent;
	this->numParameters = numParameters;
	this->error = error;

	va_list methods;
	va_start(methods, 5*numMethods);

	for (unsigned int i = 0; i < numMethods; i++)
	{
		Method<Class, Type, MapType> *func = new Method<Class, Type, MapType>(
			va_arg(methods, enum Operator),
			va_arg(methods, bool),
			va_arg(methods, Type),
			va_arg(methods, MapType),
			object,
			va_arg(methods, void (Class::*)(Type)));

		this->tests.push_back(func);
	}

	va_end(methods);
}

class InputMap
{
public:
	InputMap();
	~InputMap();

	template <typename Type>
	void add(const std::string &option, const std::string &alternate, void (*function)(Type));
	template <typename Class, typename Type>
	void add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type));

	template <typename Type>
	void add(const std::string &option, const std::string &alternate, void (*function)(Type), Type value);
	template <typename Class, typename Type>
	void add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type), Type value);

	template <typename Type, typename MapType>
	void add(const std::string &option, const std::string &alternate, void (*function)(MapType), Type value, MapType realValue);
	template <typename Class, typename Type, typename MapType>
	void add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(MapType), Type value, MapType realValue);

	template <typename Type>
	void add(const std::string &option, const std::string &alternate, void (*function)(Type), unsigned int numParameters, const std::string &error);
	template <typename Class, typename Type>
	void add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type), unsigned int numParameters, const std::string &error);

	bool process(unsigned int argc, const char *argv[]) const;

protected:
	typedef std::map<std::string, InputMapItem *> StringInputMap;

	StringInputMap map;
};

InputMap::InputMap()
{

}

InputMap::~InputMap()
{
	while (!this->map.empty())
	{
		InputMapItem *item = (*this->map.begin()).second;

		this->map.erase(this->map.begin());

		for (StringInputMap::iterator iter = this->map.begin();
			 iter != this->map.end();)
		{
			if ((*iter).second == item)
			{
				this->map.erase(iter);

				iter = this->map.begin();
			}
			else
			{
				iter++;
			}
		}

		delete item;
	}
}

template <typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, void (*function)(Type))
{
	InputMapItem *item = this->map[option] = new FInputMapItem<Type>(function, true);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Class, typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type))
{
	InputMapItem *item = this->map[option] = new CInputMapItem<Class, Type>(object, method, true);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, void (*function)(Type), Type value)
{
	InputMapItem *item = this->map[option] = new FInputMapItem<Type>(function, value);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Class, typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type), Type value)
{
	InputMapItem *item = this->map[option] = new CInputMapItem<Class, Type>(object, method, value);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Type, typename MapType>
void InputMap::add(const std::string &option, const std::string &alternate, void (*function)(MapType), Type value, MapType realValue)
{
	InputMapItem *item = this->map[option] = new FInputMapItem<Type, MapType>(function, value, realValue);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Class, typename Type, typename MapType>
void InputMap::add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(MapType), Type value, MapType realValue)
{
	InputMapItem *item = this->map[option] = new CInputMapItem<Class, Type, MapType>(object, method, value, realValue);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, void (*function)(Type), unsigned int numParameters, const std::string &error)
{
	InputMapItem *item = this->map[option] = new FInputMapItem<Type>(function, numParameters, error);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

template <typename Class, typename Type>
void InputMap::add(const std::string &option, const std::string &alternate, Class *object, void (Class::*method)(Type), unsigned int numParameters, const std::string &error)
{
	InputMapItem *item = this->map[option] = new CInputMapItem<Class, Type>(object, method, numParameters, error);

	if (!alternate.empty())
	{
		this->map[alternate] = item;
	}
}

bool InputMap::process(unsigned int argc, const char *argv[]) const
{
	bool used[argc];

	for (unsigned int i = 0; i < argc; i++)
	{
		used[i] = false;
	}

	for (unsigned int i = 0; i < argc;)
	{
		std::string option = Data::convert(argv[i]);

		StringInputMap::const_iterator iter = this->map.find(option);

		if (iter == this->map.end())
		{
			std::cout << "Error: invalid option: " << option << std::endl;

			return false;
		}

		i += (*iter).second->process(i, argc, argv, used);
	}

	return true;
}*/

std::string PMJMesh::help(const std::string &programName)
{
	std::stringstream str;

	str << "Usage: " << programName << " PMJMesh <input_filename> <options>\n"
		<< "Options:\n"
		<< "-o <output_filename> (--output <output_filename>): Save output mesh file (.txt or .dat)\n"
		<< "default: Do not produce output mesh file\n"
		<< "\n"
		<< "-sm (--smallest): Advance based on smallest front element\n"
		<< "default: Advance next on list\n"
		<< "\n"
#if USE_NUMERICAL
		<< "-ht (--hotelling): Apply Hotelling transform\n"
		<< "default: Do not apply Hotelling transform\n"
		<< "\n"
#endif //#if USE_NUMERICAL
		<< "-u (--uniform): Generate a uniform mesh (by largest element)\n"
		<< "-nu (--non-uniform): Do not generate a uniform mesh (by largest element)\n"
		<< "default: Do not generate a uniform mesh\n"
		<< "\n"
#if USE_OPENMP
		<< "-p (--parallel): Enable shared memory parallelism (number of running threads defined by the environment variable OMP_NUM_THREADS or by the total number of cores in the system)\n"
		<< "-p <number_of_threads> (--parallel <number_of_threads>): Enable shared memory parallelism and specify the number of running threads\n"
		//<< "default: Disable shared memory parallelism (same as \"-p 0\" or \"-p 1\")\n"
		<< "default: Disable shared memory parallelism (same as \"-p 0\")\n"
		<< "\n"
#endif //#if USE_OPENMP
		<< "-a (--a-priori): Enable generation of interfacing mesh a priori\n"
		<< "default: Interfacing mesh generated a posteriori\n"
		<< "\n"
		<< "-mr (--master-refinement): Perform minimum tree refinement in master\n"
		<< "-sr (--slave-refinement): Postpone minimum tree refinement to slaves\n"
		<< "default: Minimum tree refinement performed in slaves\n"
		<< "observation: Disabled when --a-priori is enabled (-mr)\n"
		<< "\n"
		<< "-per <percentage> (--percentage <percentage>): Percentage of the maximum load for tasks not to be sent to slaves (this maximum load is the load of the heaviest task)\n"
		<< "default: <percentage> = 0.0 (all tasks with load greater than 0 will be sent to slaves)\n"
		<< "\n"
		<< "-g (--greedy): Use Greedy load balancer\n"
		<< "-wg (--weighted-greedy): Use Weighted Greedy load balancer\n"
		<< "-s (--spring): Use Spring load balancer\n"
		<< "-d (--demand): Use Demand load balancer\n"
		<< "default: Use Demand load balancer\n"
		<< "\n"
		<< "-l (--load): (Quad/OctTree decomposition) Use load as the average of the total load by the number of worker slaves\n"
		<< "-l <load> (--load <load>): (Quad/OctTree decomposition) Use a specific maximum load (if <load> = 0, -l is implied)\n"
		<< "-n (--number-of-partitions): (Quad/OctTree decomposition) Use the number of worker slaves as a minimum number of partitions\n"
		<< "-n <number_of_partitions> (--number-of-partitions <number_of_partitions>): (Quad/OctTree decomposition) Use the number_of_partitions as a minimum number of partitions (if <number_of_partitions> = 0, -n is implied)\n"
		<< "-bl (--load-bsp-number-of-partitions): (Load-based BSP decomposition) Use the number of worker slaves as a minimum number of partitions\n"
		<< "-bl <number_of_partitions> (--load-bsp-number-of-partitions <number_of_partitions>): (Load-based BSP decomposition) Use the number_of_partitions as the number of partitions (if <number_of_partitions> = 0, -bl is implied)\n"
		<< "-bn (--bsp-number-of-partitions): (BSP decomposition) Use the number of worker slaves as the number of partitions\n"
		<< "-bn <number_of_partitions> (--bsp-number-of-partitions <number_of_partitions>): (BSP decomposition) Use the number_of_partitions as the number of partitions (if <number_of_partitions> = 0, -bn is implied)\n"
		<< "default: Use average load (Quad/OctTree decomposition)\n"
		<< "\n"
		<< "-rl <layers> (--remove-layers-in-load <layers>): Remove <layers> layers of cells out of load calculation\n"
		<< "default: <layers> = 2\n"
		<< "observation: Disabled when --a-priori is enabled\n"
		<< "\n"
		<< "-hi (--height-independent-in-load): Load calculation in a cell does not depend on its height\n"
		<< "-hd (--height-dependent-in-load): Load estimation in a cell depends on its height\n"
		<< "default: Load calculation is height-dependent\n"
		<< "\n"
		<< "-nsk (--do-not-skip-inner-in-decomposition): Create inner empty subdomains\n"
		<< "-sk (--skip-inner-in-decomposition): Skip creation of inner empty subdomains\n"
		<< "default: Inner empty subdomains are not created\n"
		<< "\n"
		<< "-if (--create-inner-front): Create inner front in empty subdomains\n"
		<< "-nif (--do-not-create-inner-front): Do not create inner front in empty subdomains\n"
		<< "default: Inner front in empty subdomains are not created\n"
		<< "observation: Only works when inner empty subdomains are created (-sk)"
		<< "\n"
		<< "-r <range_proportion> (--range-proportion <range_proportion>): Percentage of the search region range (radius) that should not cross the bounding box of a subdomain (second modification in the serial advancing front technique)\n"
		<< "default: <range_proportion> = 1.0\n"
		<< "\n"
		<< "-ni (--not-improve): Do not improve mesh in slaves\n"
		<< "default: Improve mesh in slaves\n"
		<< "\n"
#if (!USE_DELETE_FELS)
		<< "-si <layer> (--slave-not-improve <layer>): Do not improve <layer> layers of elements in slaves, counting from the front\n"
		<< "default: <layer> = 0, meaning that all elements will be improved in slaves\n"
		<< "\n"
#endif //#if (!USE_DELETE_FELS)
		<< "-mi <layer> (--master-improve <layer>): Improve <layer> layers of elements in master, count from the front\n"
		<< "default: <layer> = 1, meaning that one layer of elements will be improved in master\n"
		<< "\n"
		<< "-uf (--update-full): Update full mesh and front\n"
		<< "-up (--update-partial): Update partial mesh and full front\n"
		<< "-upnb (--update-partial-no-boundary): Update partial mesh and full front, but exclude mesh adjacent to boundary\n"
		<< "-ue (--update-empty): Update only front\n"
		<< "-uenb (--update-empty-no-boundary): Update only front, but exclude mesh adjacent to boundary\n"
		<< "-ui (--update-final): Update only front, but only final mesh\n"
		<< "default: Update only front excluding mesh adjacent to boundary\n"
		<< "observation: --update-full and --update-empty are the only options when --a-priori is enabled\n"
		<< "\n"
		<< "-sh <shift_mode> (--shift <shift_mode>): Set shift mode, which should be one of the following\n"
		<< "<shift_mode> = 0: no shift performed\n"
		<< "<shift_mode> = 1: 1 side only (lateral)\n"
		<< "<shift_mode> = 2: 2 sides only (diagonal (edge, in 3D))\n"
		<< "<shift_mode> = 3: 1 and 2 sides (lateral and diagonal (edge, in 3D))\n"
		<< "<shift_mode> = 4: 3 sides only (diagonal (vertex, in 3D))\n"
		<< "<shift_mode> = 5: 1 and 3 sides (lateral and diagonal (vertex, in 3D))\n"
		<< "<shift_mode> = 6: 2 and 3 sides (diagonals (edge and vertex, in 3D))\n"
		<< "<shift_mode> = 7: 1, 2 and 3 sides (lateral and diagonals (edge and vertex, in 3D))\n"
		<< "default: <shift_mode> = 1\n"
		<< "observation: modes 4, 5, 6 and 7 are for 3D only, and imply, respectively, 1, 1, 2 and 3 modes in 2D\n"
		<< "observation: any mode different than the above will set the mode to the default value\n";

	return str.str();
}

int PMJMesh::execute(int argc, char *argv[])
{
	PMJMesh::MainDrive *drive = new PMJMesh::MainDrive();

#if (!USE_WORKER_FILE_LOADER)
	if (drive->getComm()->isMaster())
#endif //#if (!USE_WORKER_FILE_LOADER)
	{
		drive->setBoundarySorted(false);

#if USE_NUMERICAL
        drive->setHotellingTransformEnabled(false);
#endif //#if USE_NUMERICAL

		drive->setUsingUniformRefinement(false);

		drive->setParallel(false);

		drive->setAlgorithm(GLOBAL_SHIFT);

		drive->setMinRefinementPostponed(true);

		drive->setPercentageNonWorkingTask(0.0);

		drive->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);

		drive->setPartitionerType(BOX_LOAD);

		drive->setUsePartitioningThreshold(false);

		drive->setRemoveLayersInLoad(2);

		drive->setHeightDependentInLoad(true);

		drive->setSkippingInnerInDecomposition(true);

		drive->setCreateInnerFrontInDecomposition(false);

		drive->setRangeProportion(1.0);

		drive->setSmoothInSlavesEnabled(true);

#if (!USE_DELETE_FELS)
		drive->setUnsmoothingLayer(0);
#endif //#if (!USE_DELETE_FELS)

		drive->setSmoothingLayer(1);

		drive->setUpdateType(EMPTY_NO_BOUNDARY);

		drive->setShiftType(LATERAL);

		std::string input, output;

		/*if (argc == 2)
		{
			input = Data::convert(argv[1]);
		}
		else*/
		{
			/*InputMap map;

			map.add(std::string("-o"), std::string("--output"),
				dynamic_cast<Performer::MainDrive *>(drive), &PMJMesh::MainDrive::setOutputFileName, 1,
				std::string("Error: please, enter output filename"));

			map.add(std::string("-sm"), std::string("--smallest"),
				dynamic_cast<MJMesh::MainDrive *>(drive), &PMJMesh::MainDrive::setBoundarySorted);

			//map.add(std::string("-p"), std::string("--parallel"),
			//	drive, &PMJMesh::MainDrive::setParallel);

			map.add(std::string("-a"), std::string("--a-priori"),
				drive, &PMJMesh::MainDrive::setAlgorithm, true, A_PRIORI_INTERFACE_MESH);

			map.add(std::string("-sr"), std::string("--slave-refinement"),
				drive, &PMJMesh::MainDrive::setMinRefinementPostponed);

			map.add(std::string("-per"), std::string("--percentage"),
				drive, &PMJMesh::MainDrive::setPercentageNonWorkingTask, 1,
				std::string("Error: please, enter percentage of load for tasks not to be sent to workers"));

			map.add(std::string("-g"), std::string("--greedy"),
				drive, &PMJMesh::MainDrive::setLoadBalancerType, true, Parallel::GREEDY_SERIAL_STATIC);
			map.add(std::string("-wg"), std::string("--weighted-greedy"),
				drive, &PMJMesh::MainDrive::setLoadBalancerType, true, Parallel::WEIGHTED_GREEDY_SERIAL_STATIC);
			map.add(std::string("-s"), std::string("--spring"),
				drive, &PMJMesh::MainDrive::setLoadBalancerType, true, Parallel::SPRING_SERIAL_STATIC);
			map.add(std::string("-d"), std::string("--demand"),
				drive, &PMJMesh::MainDrive::setLoadBalancerType, true, Parallel::DEMAND_PARALLEL_DYNAMIC);*/

			bool used[argc];

			for (int i = 0; i < argc; i++)
			{
				used[i] = false;
			}

			used[0] = true;

			for (int i = 0; i < argc; i++)
			{
				if (used[i])
				{
					continue;
				}

				if ('-' != argv[i][0])
				{
					input = Data::convert(argv[i]);

					continue;
				}

				if ((std::string("-o") == argv[i]) || (std::string("--output") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						used[i+1] = true;

						output = Data::convert(argv[i+1]);
					}
					else
					{
						std::cout << "Error: please, enter output filename" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-sm") == argv[i]) || (std::string("--smallest") == argv[i]))
				{
					used[i] = true;

					drive->setBoundarySorted(true);
				}
#if USE_NUMERICAL
                else if ((std::string("-ht") == argv[i]) || (std::string("--hotelling") == argv[i]))
                {
                    used[i] = true;

                    drive->setHotellingTransformEnabled(true);
                }
#endif //#if USE_NUMERICAL
				else if ((std::string("-u") == argv[i]) || (std::string("--uniform") == argv[i]))
				{
					used[i] = true;

					drive->setUsingUniformRefinement(true);
				}
				else if ((std::string("-nu") == argv[i]) || (std::string("--non-uniform") == argv[i]))
				{
					used[i] = true;

					drive->setUsingUniformRefinement(false);
				}
#if USE_OPENMP
				else if ((std::string("-p") == argv[i]) || (std::string("--parallel") == argv[i]))
				{
					used[i] = true;

					drive->setParallel(true);

					if (i < argc - 1)
					{
						try
						{
							unsigned int nt = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (nt == 0)
							{
								drive->setParallel(false);
							}
							else
							{
								drive->setParallel(true);
								static_cast<PMJMesh::Communicator *>(drive->getComm())->setMaxThreads(nt);

#if USE_POOL
								PMJMesh::resizeThreadPools();
#endif //#if USE_POOL
							}
						}
						catch (...)
						{

						}
					}
				}
#endif //#if USE_OPENMP
				else if ((std::string("-a") == argv[i]) || (std::string("--a-priori") == argv[i]))
				{
					used[i] = true;

					drive->setAlgorithm(A_PRIORI_INTERFACE_MESH);

					if (drive->isMinRefinementPostponed())
					{
						drive->setMinRefinementPostponed(false);
					}

					if ((drive->getUpdateType() != FULL) ||
						(drive->getUpdateType() != EMPTY))
					{
						drive->setUpdateType(EMPTY);
					}

					if (drive->getShiftType() != NONE)
					{
						drive->setShiftType(NONE);
					}
				}
				else if ((std::string("-mr") == argv[i]) || (std::string("--master-refinement") == argv[i]))
				{
					used[i] = true;

					if (drive->getAlgorithm() == GLOBAL_SHIFT)
					{
						drive->setMinRefinementPostponed(false);
					}
				}
				else if ((std::string("-sr") == argv[i]) || (std::string("--slave-refinement") == argv[i]))
				{
					used[i] = true;

					if (drive->getAlgorithm() == GLOBAL_SHIFT)
					{
						drive->setMinRefinementPostponed(true);
					}
				}
				else if ((std::string("-per") == argv[i]) || (std::string("--percentage") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							double percentage = Data::convert<double>(argv[i+1]);

							used[i+1] = true;

							if (percentage > 0.0)
							{
								drive->setPercentageNonWorkingTask(percentage);
							}
						}
						catch (...)
						{
							std::cout << "Error: please, enter percentage of load for tasks not to be sent to workers" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter percentage of load for tasks not to be sent to workers" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-g") == argv[i]) || (std::string("--greedy") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::GREEDY_SERIAL_STATIC);
				}
				else if ((std::string("-wg") == argv[i]) || (std::string("--weighted-greedy") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::WEIGHTED_GREEDY_SERIAL_STATIC);
				}
				else if ((std::string("-s") == argv[i]) || (std::string("--spring") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::SPRING_SERIAL_STATIC);
				}
				else if ((std::string("-d") == argv[i]) || (std::string("--") == argv[i]))
				{
					used[i] = true;

					drive->setLoadBalancerType(Parallel::DEMAND_PARALLEL_DYNAMIC);
				}
				else if ((std::string("-l") == argv[i]) || (std::string("--load") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(BOX_LOAD);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							double load = Data::convert<double>(argv[i+1]);

							used[i+1] = true;

							if (load > 0.0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(load);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-n") == argv[i]) || (std::string("--number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(BOX_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-bl") == argv[i]) || (std::string("--load-bsp-number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(SMART_BSP_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-bn") == argv[i]) || (std::string("--bsp-number-of-partitions") == argv[i]))
				{
					used[i] = true;

					drive->setPartitionerType(BSP_NUMBER);

					drive->setUsePartitioningThreshold(false);

					if (i < argc - 1)
					{
						try
						{
							unsigned int np = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							if (np > 0)
							{
								drive->setUsePartitioningThreshold(true);

								drive->setPartitioningThreshold(np);
							}
						}
						catch (...)
						{

						}
					}
				}
				else if ((std::string("-rl") == argv[i]) || (std::string("--remove-layers-in-load") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int layers = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							drive->setRemoveLayersInLoad(layers);
						}
						catch (...)
						{
							std::cout << "Error: please, enter number of layers of cells to be removed out of load calculation" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter number of layers of cells to be removed out of load calculation" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-hi") == argv[i]) || (std::string("--height-independent-in-load") == argv[i]))
				{
					used[i] = true;

					drive->setHeightDependentInLoad(false);
				}
				else if ((std::string("-hd") == argv[i]) || (std::string("--height-dependent-in-load") == argv[i]))
				{
					used[i] = true;

					drive->setHeightDependentInLoad(true);
				}
				else if ((std::string("-nsk") == argv[i]) || (std::string("--do-not-skip-inner-in-decomposition") == argv[i]))
				{
					used[i] = true;

					drive->setSkippingInnerInDecomposition(false);
				}
				else if ((std::string("-sk") == argv[i]) || (std::string("--skip-inner-in-decomposition") == argv[i]))
				{
					used[i] = true;

					drive->setSkippingInnerInDecomposition(true);
				}
				else if ((std::string("-if") == argv[i]) || (std::string("--create-inner-front") == argv[i]))
				{
					used[i] = true;

					drive->setCreateInnerFrontInDecomposition(true);
				}
				else if ((std::string("-nif") == argv[i]) || (std::string("--do-not-create-inner-front") == argv[i]))
				{
					used[i] = true;

					drive->setCreateInnerFrontInDecomposition(false);
				}
				else if ((std::string("-r") == argv[i]) || (std::string("--range-proportion") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							double rangeProportion = Data::convert<double>(argv[i+1]);

							used[i+1] = true;

							if (rangeProportion >= 0.0)
							{
								drive->setRangeProportion(rangeProportion);
							}
						}
						catch (...)
						{
							std::cout << "Error: please, enter percentage of the search region range (radius) that should not cross the bounding box of a subdomain" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter percentage of the search region range (radius) that should not cross the bounding box of a subdomain" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-ni") == argv[i]) || (std::string("--not-improve") == argv[i]))
				{
					used[i] = true;

					drive->setSmoothInSlavesEnabled(false);
				}
#if (!USE_DELETE_FELS)
				else if ((std::string("-si") == argv[i]) || (std::string("--slave-not-improve") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int layer = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							drive->setUnsmoothingLayer(layer);
						}
						catch (...)
						{
							std::cout << "Error: please, enter number of layers of elements that will not be improved in workers" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter number of layers of elements that will not be improved in workers" << std::endl;

						delete drive;

						return 1;
					}
				}
#endif //#if (!USE_DELETE_FELS)
				else if ((std::string("-mi") == argv[i]) || (std::string("--master-improve") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int layer = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							drive->setSmoothingLayer(layer);
						}
						catch (...)
						{
							std::cout << "Error: please, enter number of layers of elements that will be improved in master" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter number of layers of elements that will be improved in master" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-uf") == argv[i]) || (std::string("--update-full") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(FULL);
				}
				else if ((std::string("-up") == argv[i]) || (std::string("--update-partial") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PARTIAL);

					if (drive->getAlgorithm() == A_PRIORI_INTERFACE_MESH)
					{
						drive->setUpdateType(EMPTY);
					}
				}
				else if ((std::string("-upnb") == argv[i]) || (std::string("--update-partial-no-boundary") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(PARTIAL_NO_BOUNDARY);

					if (drive->getAlgorithm() == A_PRIORI_INTERFACE_MESH)
					{
						drive->setUpdateType(PARTIAL);
					}
				}
				else if ((std::string("-ue") == argv[i]) || (std::string("--update-empty") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(EMPTY);
				}
				else if ((std::string("-uenb") == argv[i]) || (std::string("--update-empty-no-boundary") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(EMPTY_NO_BOUNDARY);

					if (drive->getAlgorithm() == A_PRIORI_INTERFACE_MESH)
					{
						drive->setUpdateType(PARTIAL);
					}
				}
				else if ((std::string("-ui") == argv[i]) || (std::string("--update-final") == argv[i]))
				{
					used[i] = true;

					drive->setUpdateType(FINAL);

					if (drive->getAlgorithm() == A_PRIORI_INTERFACE_MESH)
					{
						drive->setUpdateType(PARTIAL);
					}
				}
				else if ((std::string("-sh") == argv[i]) || (std::string("--shift") == argv[i]))
				{
					used[i] = true;

					if (i < argc - 1)
					{
						try
						{
							unsigned int shift = Data::convert<unsigned int>(argv[i+1]);

							used[i+1] = true;

							switch (shift)
							{
							case 0:
								drive->setShiftType(NONE);
								break;

							case 1:
								drive->setShiftType(LATERAL);
								break;

							case 2:
								drive->setShiftType(DIAGONAL);
								break;

							case 3:
								drive->setShiftType(LATERAL_DIAGONAL);
								break;

							case 4:
								drive->setShiftType(DIAGONAL_VERTEX);
								break;

							case 5:
								drive->setShiftType(LATERAL_DIAGONAL_VERTEX);
								break;

							case 6:
								drive->setShiftType(DIAGONAL_DIAGONAL_VERTEX);
								break;

							case 7:
								drive->setShiftType(LATERAL_DIAGONAL_DIAGONAL_VERTEX);
								break;

							default:
								std::cout << "invalid shift mode" << std::endl;

								delete drive;

								return 1;
								break;
							}
						}
						catch (...)
						{
							std::cout << "Error: please, enter shift mode" << std::endl;

							delete drive;

							return 1;
						}
					}
					else
					{
						std::cout << "Error: please, enter shift mode" << std::endl;

						delete drive;

						return 1;
					}
				}
				else if ((std::string("-m") == argv[i]) || (std::string("--mean") == argv[i]))
				{
					std::cout << "Error: deprecated option: " << argv[i] << ", please use -l or --load instead" << std::endl;

					delete drive;

					return 1;
				}
				else
				{
					std::cout << "Error: invalid option: " << argv[i] << std::endl;

					delete drive;

					return 1;
				}
			}
		}

		if (input.empty())
		{
			std::cout << "Error: no input file given" << std::endl;

			delete drive;

			return 1;
		}

		drive->setInputFileName(input);
		drive->setOutputFileName(output);
	}

#if USE_WORKER_FILE_LOADER
	if (!drive->loadFile())
#else
	if ((!drive->loadFile()) && (this->comm->isMaster()))
#endif //#if USE_WORKER_FILE_LOADER
	{
		std::cout << "Error: could not load file" << std::endl;

		delete drive;

		return 1;
	}

	drive->execute();

	drive->saveFile();

	if (drive->getComm()->isMaster())
	{
		if (drive->getStatus().getType() >= drive->error.getType())
		{
			std::cout << "Error: mesh could not be generated: " << drive->getStatus().getMessage() << std::endl;
		}
		else
		{
			std::cout << drive->qualityString() << std::endl;
		}
	}

	delete drive;

	//std::cout << "done" << std::endl;

	return 0;
}
#endif //#if (!USE_GUI)

#include "PMJMesh/QuadTree.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTree.h"
#include "PMJMesh/OctTreeCell.h"

void PMJMesh::setPosition(UInt dimension, Data::BoxTreeCell *cell, enum Data::Position pos)
{
	if (dimension == 2)
	{
		static_cast<PMJMesh::QuadTreeCell *>(cell)->setPosition(pos);
	}
	else
	{
		static_cast<PMJMesh::OctTreeCell *>(cell)->setPosition(pos);
	}
}

void PMJMesh::setPosition(UInt dimension, const Data::BoxTree *tree, ULInt id, enum Data::Position pos)
{
	PMJMesh::setPosition(dimension, static_cast<Data::BoxTreeCell *>(tree->node(id)), pos);
}

enum Data::Position PMJMesh::getPosition(UInt dimension, const Data::BoxTreeCell *cell)
{
	return (dimension == 2) ?
		static_cast<const PMJMesh::QuadTreeCell *>(cell)->getPosition() :
		static_cast<const PMJMesh::OctTreeCell *>(cell)->getPosition();
}

enum Data::Position PMJMesh::getPosition(UInt dimension, const Data::BoxTree *tree, ULInt id)
{
	return PMJMesh::getPosition(dimension, static_cast<Data::BoxTreeCell *>(tree->node(id)));
}

#if USE_POOL
#include "Data/Point2D.h"
#include "Data/Point3D.h"
#include "Data/Triangle2D.h"
#include "Data/Tetrahedron.h"
#include "Data/Box.h"
#include "Data/VertexBoxSearchTreeCell.h"
#include "Data/FrontElementBoxSearchTreeCell.h"
#include "MJMesh/Edge2D.h"
#include "MJMesh/Triangle3D.h"
#include "MJMesh/Vertex.h"
#include "MJMesh/FrontElement2D.h"
#include "MJMesh/FrontElement3D.h"
#include "MJMesh/Element2D.h"
#include "MJMesh/Element3D.h"
#include "MJMesh/QuadTreeCell.h"
#include "MJMesh/OctTreeCell.h"
#include "MJMesh/VertexBoxSearchTreeCell.h"
#include "MJMesh/FrontElementBoxSearchTreeCell.h"
#include "PMJMesh/QuadTreeCell.h"
#include "PMJMesh/OctTreeCell.h"
#include <Parallel/ThreadManager.h>

static bool built = false;

void PMJMesh::initThreadPools(const Parallel::ThreadManager *tm)
{
	if (built)
	{
		return;
	}

	if (tm->isInParallel())
	{
		Int tid = tm->threadId();

		if (tid > 0)
		{
			Data::Point2D::poolMake();
			//Data::EPoint2D::poolMake();
			Data::Point3D::poolMake();
			//Data::EPoint3D::poolMake();
			Data::Triangle2D::poolMake();
			Data::Tetrahedron::poolMake();
			MJMesh::Edge2D::poolMake();
			MJMesh::Triangle3D::poolMake();
			MJMesh::Vertex::poolMake();
			MJMesh::FrontElement2D::poolMake();
			MJMesh::FrontElement3D::poolMake();
			MJMesh::Element2D::poolMake();
			MJMesh::Element3D::poolMake();
#if USE_TREE_CELL_POOL
			Data::Box::poolMake();
#if USE_EXPERIMENTAL_5
			Data::VertexBoxSearchTreeCell2D::poolMake();
			Data::FrontElementBoxSearchTreeCell2D::poolMake();
			Data::VertexBoxSearchTreeCell3D::poolMake();
			Data::FrontElementBoxSearchTreeCell3D::poolMake();
#else
			Data::VertexBoxSearchTreeCell::poolMake();
			Data::FrontElementBoxSearchTreeCell::poolMake();
#endif //#if USE_EXPERIMENTAL_5
			MJMesh::QuadTreeCell::poolMake();
			MJMesh::OctTreeCell::poolMake();
#if USE_EXPERIMENTAL_5
			MJMesh::VertexBoxSearchTreeCell2D::poolMake();
			MJMesh::FrontElementBoxSearchTreeCell2D::poolMake();
			MJMesh::VertexBoxSearchTreeCell3D::poolMake();
			MJMesh::FrontElementBoxSearchTreeCell3D::poolMake();
#else
			MJMesh::VertexBoxSearchTreeCell::poolMake();
			MJMesh::FrontElementBoxSearchTreeCell::poolMake();
#endif //#if USE_EXPERIMENTAL_5
			PMJMesh::QuadTreeCell::poolMake();
			PMJMesh::OctTreeCell::poolMake();
#endif //#if USE_TREE_CELL_POOL
		}

		return;
	}

	if (tm->getMaxThreads() > 1)
	{
		#pragma omp parallel shared(tm)
		{
			PMJMesh::initThreadPools(tm);
		}
	}

	built = true;
}

#if USE_OPENMP
void PMJMesh::mergeThreadPools()
{
	if (!built)
	{
		return;
	}

	#pragma omp master
	{
		Data::Point2D::poolMerge();
		//Data::EPoint2D::poolMerge();
		Data::Point3D::poolMerge();
		//Data::EPoint3D::poolMerge();
		Data::Triangle2D::poolMerge();
		Data::Tetrahedron::poolMerge();
		MJMesh::Edge2D::poolMerge();
		MJMesh::Triangle3D::poolMerge();
		MJMesh::Vertex::poolMerge();
		MJMesh::FrontElement2D::poolMerge();
		MJMesh::FrontElement3D::poolMerge();
		MJMesh::Element2D::poolMerge();
		MJMesh::Element3D::poolMerge();
#if USE_TREE_CELL_POOL
		Data::Box::poolMerge();
#if USE_EXPERIMENTAL_5
		Data::VertexBoxSearchTreeCell2D::poolMerge();
		Data::FrontElementBoxSearchTreeCell2D::poolMerge();
		Data::VertexBoxSearchTreeCell3D::poolMerge();
		Data::FrontElementBoxSearchTreeCell3D::poolMerge();
#else
		Data::VertexBoxSearchTreeCell::poolMerge();
		Data::FrontElementBoxSearchTreeCell::poolMerge();
#endif //#if USE_EXPERIMENTAL_5
		MJMesh::QuadTreeCell::poolMerge();
		MJMesh::OctTreeCell::poolMerge();
#if USE_EXPERIMENTAL_5
		MJMesh::VertexBoxSearchTreeCell2D::poolMerge();
		MJMesh::FrontElementBoxSearchTreeCell2D::poolMerge();
		MJMesh::VertexBoxSearchTreeCell3D::poolMerge();
		MJMesh::FrontElementBoxSearchTreeCell3D::poolMerge();
#else
		MJMesh::VertexBoxSearchTreeCell::poolMerge();
		MJMesh::FrontElementBoxSearchTreeCell::poolMerge();
#endif //#if USE_EXPERIMENTAL_5
		PMJMesh::QuadTreeCell::poolMerge();
		PMJMesh::OctTreeCell::poolMerge();
#endif //#if USE_TREE_CELL_POOL
	}
}
#endif //#if USE_OPENMP

void PMJMesh::finalizeThreadPools(const Parallel::ThreadManager *tm)
{
	if (!built)
	{
		return;
	}

	if (tm->isInParallel())
	{
		Int tid = tm->threadId();

		if (tid > 0)
		{
			Data::Point2D::poolUnmake();
			//Data::EPoint2D::poolUnmake();
			Data::Point3D::poolUnmake();
			//Data::EPoint3D::poolUnmake();
			Data::Triangle2D::poolUnmake();
			Data::Tetrahedron::poolUnmake();
			MJMesh::Edge2D::poolUnmake();
			MJMesh::Triangle3D::poolUnmake();
			MJMesh::Vertex::poolUnmake();
			MJMesh::FrontElement2D::poolUnmake();
			MJMesh::FrontElement3D::poolUnmake();
			MJMesh::Element2D::poolUnmake();
			MJMesh::Element3D::poolUnmake();
#if USE_TREE_CELL_POOL
			Data::Box::poolUnmake();
#if USE_EXPERIMENTAL_5
			Data::VertexBoxSearchTreeCell2D::poolUnmake();
			Data::FrontElementBoxSearchTreeCell2D::poolUnmake();
			Data::VertexBoxSearchTreeCell3D::poolUnmake();
			Data::FrontElementBoxSearchTreeCell3D::poolUnmake();
#else
			Data::VertexBoxSearchTreeCell::poolUnmake();
			Data::FrontElementBoxSearchTreeCell::poolUnmake();
#endif //#if USE_EXPERIMENTAL_5
			MJMesh::QuadTreeCell::poolUnmake();
			MJMesh::OctTreeCell::poolUnmake();
#if USE_EXPERIMENTAL_5
			MJMesh::VertexBoxSearchTreeCell2D::poolUnmake();
			MJMesh::FrontElementBoxSearchTreeCell2D::poolUnmake();
			MJMesh::VertexBoxSearchTreeCell3D::poolUnmake();
			MJMesh::FrontElementBoxSearchTreeCell3D::poolUnmake();
#else
			MJMesh::VertexBoxSearchTreeCell::poolUnmake();
			MJMesh::FrontElementBoxSearchTreeCell::poolUnmake();
#endif //#if USE_EXPERIMENTAL_5
			PMJMesh::QuadTreeCell::poolUnmake();
			PMJMesh::OctTreeCell::poolUnmake();
#endif //#if USE_TREE_CELL_POOL
		}

		return;
	}

	if (tm->getMaxThreads() > 1)
	{
		#pragma omp parallel shared(tm)
		{
			PMJMesh::finalizeThreadPools(tm);
		}
	}

	built = false;
}

#if USE_OPENMP
void PMJMesh::resizeThreadPools()
{
	Data::Point2D::poolResize();
	//Data::EPoint2D::poolResize();
	Data::Point3D::poolResize();
	//Data::EPoint3D::poolResize();
	Data::Triangle2D::poolResize();
	Data::Tetrahedron::poolResize();
#if USE_TREE_CELL_POOL
	Data::Box::poolResize();
#if USE_EXPERIMENTAL_5
	Data::VertexBoxSearchTreeCell2D::poolResize();
	Data::FrontElementBoxSearchTreeCell2D::poolResize();
	Data::VertexBoxSearchTreeCell3D::poolResize();
	Data::FrontElementBoxSearchTreeCell3D::poolResize();
#else
	Data::VertexBoxSearchTreeCell::poolResize();
	Data::FrontElementBoxSearchTreeCell::poolResize();
#endif //#if USE_EXPERIMENTAL_5
#endif //#if USE_TREE_CELL_POOL
	MJMesh::Edge2D::poolResize();
	MJMesh::Triangle3D::poolResize();
	MJMesh::Vertex::poolResize();
	MJMesh::FrontElement2D::poolResize();
	MJMesh::FrontElement3D::poolResize();
	MJMesh::Element2D::poolResize();
	MJMesh::Element3D::poolResize();
#if USE_TREE_CELL_POOL
	MJMesh::QuadTreeCell::poolResize();
	MJMesh::OctTreeCell::poolResize();
#if USE_EXPERIMENTAL_5
	MJMesh::VertexBoxSearchTreeCell2D::poolResize();
	MJMesh::FrontElementBoxSearchTreeCell2D::poolResize();
	MJMesh::VertexBoxSearchTreeCell3D::poolResize();
	MJMesh::FrontElementBoxSearchTreeCell3D::poolResize();
#else
	MJMesh::VertexBoxSearchTreeCell::poolResize();
	MJMesh::FrontElementBoxSearchTreeCell::poolResize();
#endif //#if USE_EXPERIMENTAL_5
	PMJMesh::QuadTreeCell::poolResize();
	PMJMesh::OctTreeCell::poolResize();
#endif //#if USE_TREE_CELL_POOL
}
#endif //#if USE_OPENMP
#endif //#if USE_POOL
