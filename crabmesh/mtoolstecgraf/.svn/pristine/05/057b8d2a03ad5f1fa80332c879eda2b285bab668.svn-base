#ifndef _PERFORMER_MAIN_DRIVE_H_
#define _PERFORMER_MAIN_DRIVE_H_

#include "Performer/Definitions.h"
#include "Performer/Builder.h"

namespace Performer
{
	class MainDrive : virtual public Performer::Builder
	{
	public:

		MainDrive();
		virtual ~MainDrive();

		virtual void setInputFileName(const std::string &inputFileName);
		virtual const std::string &getInputFileName() const;

		virtual void setOutputFileName(const std::string &outputFileName);
		virtual const std::string &getOutputFileName() const;

		virtual bool loadFile() = 0;
		virtual bool saveFile() const = 0;

		virtual void execute() = 0;

	protected:

		virtual bool loadInputFile(const std::string &inputFileName) = 0;
		virtual bool saveInputFile(const std::string &outputFileName) const = 0;

		virtual bool loadOutputFile(const std::string &inputFileName) = 0;
		virtual bool saveOutputFile(const std::string &outputFileName) const = 0;

	public:

		enum
		{
			SUCCESS = 0,
			NOT_INIT,
			INIT,

			ERROR = 100,
		};

		static const Status notInit;
		static const Status init;
		static const Status success;
		static const Status error;

	protected:

		std::string inputFileName;
		std::string outputFileName;
	};
}

#endif //#ifndef _PERFORMER_MAIN_DRIVE_H_
