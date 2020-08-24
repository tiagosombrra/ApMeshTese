#include "Performer/MainDrive.h"

using namespace Performer;

const Status MainDrive::success =
	Status(MainDrive::SUCCESS, "success");
const Status MainDrive::notInit =
	Status(MainDrive::NOT_INIT, "not initiated");
const Status MainDrive::init =
	Status(MainDrive::INIT, "initiated");
const Status MainDrive::error =
	Status(MainDrive::ERROR, "error");

Performer::MainDrive::MainDrive() :
	Builder()
{
	this->setInputFileName("");
	this->setOutputFileName("");
}

Performer::MainDrive::~MainDrive()
{

}

void Performer::MainDrive::setInputFileName(const std::string &inputFileName)
{
	this->inputFileName = inputFileName;
}

const std::string &Performer::MainDrive::getInputFileName() const
{
	return this->inputFileName;
}

void Performer::MainDrive::setOutputFileName(const std::string &outputFileName)
{
	this->outputFileName = outputFileName;
}

const std::string &Performer::MainDrive::getOutputFileName() const
{
	return this->outputFileName;
}
