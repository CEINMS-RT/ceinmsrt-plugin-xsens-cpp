#ifndef PluginPositionAndTorqueTwinCatAndXsens_H
#define PluginPositionAndTorqueTwinCatAndXsens_H

#define _HAS_STD_BYTE 0 // Added to solve this issue with Xsens SDK https://developercommunity.visualstudio.com/t/error-c2872-byte-ambiguous-symbol/93889

#include "execution.hxx"
#include "XMLInterpreter.h"
#include "ExecutionXmlReader.h"
#include <AngleAndComsumerPlugin.h>
#include <tcAdsClient.h>
#include "OpenSimFileLogger.h"

#include "udpserver.h"
#include "streamer.h"

#include <thread>
#include <chrono>
#include <mutex>

#include <GetTime.h>


#define M_PI 3.14159265359

#ifdef WIN32
class __declspec(dllexport) PluginPositionAndTorqueTwinCatAndXsens : public AngleAndComsumerPlugin {
#endif
#ifdef UNIX
	class  PluginPositionAndTorqueTwinCatAndXsens : public AngleAndComsumerPlugin {
#endif
	public:
		PluginPositionAndTorqueTwinCatAndXsens();
		~PluginPositionAndTorqueTwinCatAndXsens();

		void init(std::string& executableXMLFileName);

		void init(int portno);

		void setDofName(const std::vector<std::string>& dofName)
		{
			dofName_ = dofName;
		}

		void setDofTorque(const std::vector<double>& dofTorque);

		void setDofStiffness(const std::vector<double>& dofStiffness)
		{

		}

		void setMuscleName(const std::vector<std::string>& muscleName)
		{

		}

		void setOutputTimeStamp(const double& timeStamp)
		{

		}

		void setMuscleForce(const std::vector<double>& muscleForce)
		{

		}
		void setMuscleFiberLength(const std::vector<double>& muscleFiberLength)
		{

		}
		void setMuscleFiberVelocity(const std::vector<double>& muscleFiberVelocity)
		{

		}

		void setMuscleForcePassive(const std::vector<double>& muscleForcePassive)
		{

		}

		void setMuscleForceActive(const std::vector<double>& muscleForceActive)
		{

		}

		void setMuscleTendonStrain(const std::vector<double>& tendonStrain)
		{

		}


		void setTendonStrain(const std::vector<double>& tendonStrain)
		{

		}

		const double& GetAngleTimeStamp();

		const std::vector<std::string>& GetDofName()
		{
			return dofName_;
		}

		const std::map<std::string, double>& GetDataMap();

		const std::map<std::string, double>& GetDataMapTorque();

		void stop();

		void setDirectory(std::string outDirectory, std::string inDirectory = std::string());

		void setVerbose(int verbose)
		{
		}

		void setRecord(bool record)
		{
			record_ = record;
		}

	protected:
		enum VarName
		{
			Inputs,
			Outputs
		};

		std::vector<std::string> dofName_;
		double timeStamp_;
		double timeStampEthercat_;
		double timeStampXsens_;
		tcAdsClient* client_;
		std::vector<unsigned long> varNameVect_;
		OpenSimFileLogger<double>* logger_;
		std::map<std::string, double> dataAngle_;
		std::map<std::string, double> dataAngleXsens_;
		std::map<std::string, double> dataAngleEthercat_;
		std::map<std::string, double> dataTorque_;
		std::map<std::string, double> dataTorqueEthercat_;
		std::string outDirectory_;
		bool record_;

		UdpServer *udpServerXsens_;

		std::thread *ethercatThread_;
		std::mutex mtxEthercat_;
		std::mutex mtxTime_;
		bool threadStop_;
		int cpt_;

		void ethercatThread();
};

#endif