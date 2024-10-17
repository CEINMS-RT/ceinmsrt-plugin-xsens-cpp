#ifndef PluginPositionXsens_H
#define PluginPositionXsens_H

#define _HAS_STD_BYTE 0 // Added to solve this issue with Xsens SDK https://developercommunity.visualstudio.com/t/error-c2872-byte-ambiguous-symbol/93889

#include <ProducersPluginVirtual.h>
#include "OpenSimFileLogger.h"

#include "udpserver.h"
#include "streamer.h" 

#include <thread>
#include <mutex>

#include <getTime.h>


/**
 * @brief Plugin for CEINMS-RT to received oint angle and pelvis 6 DOF from Xsens Analyze.
*/

#ifdef WIN32
class __declspec(dllexport) PluginXsens : public ProducersPluginVirtual {
#endif
#ifdef UNIX
	class  PluginXsens : public ProducersPluginVirtual {
#endif
	public:
		PluginXsens();

		~PluginXsens();

		/**
		 * @brief Initialisation function call first by CEINMS-RT
		 * @param executableXMLFileName  Path to the execution.xml
		*/
		void init(std::string xmlName, std::string executionName);

		void reset()
		{

		}

		/**
		 * @brief Last function to be called before closing the plugin
		*/
		void stop();





		/**
		 * @brief Function to give angle data to CEINMS-RT
		 * @return An std map with the name of the DOF as the key and the joint angle as an output
		*/
		const std::map<std::string, double>& GetDataMap();

		/**
		 * @brief Function to give angle torque data to CEINMS-RT
		 * @return An std map with the name of the DOF as the key and the joint angle torque as an output
		*/
		const std::map<std::string, double>& GetDataMapTorque();

		/**
		 * @brief Function for giving the timestamp of the current data angle to CEINMS-RT
		 * @return the timestamp of the joint angle
		*/
		const double& getTime();
	

		/**
		 * @brief Get the directories from CEINMS-RT to read data from or save data to
		 * @param outDirectory Path to directory to save data to
		 * @param inDirectory Path to directory to read data from (if needed)
		*/
		void setDirectories(std::string outDirectory, std::string inDirectory = std::string());
		
		/**
		 * @brief set the level of verbose for debugging
		 * @param verbose Three level can be asked (0: no verbose;1: normal verbose; 2: debug level 1; 3: debug level 2)
		*/
		void setVerbose(int verbose)
		{
		}

		/**
		 * @brief Function to tell us if any data need to be recorded
		 * @param record True record; false do not save data
		*/
		void setRecord(bool record)
		{
			record_ = record;
		}

		/*void init(std::string executionXMLFile = std::string(), std::string subjectCEINMSXMLFile = std::string());
			 void reset() {}
		 void stop();
		 const map<string, double>& GetDataMap(); //< For having Data and name correspondence
		 const map<string, double>& GetDataMapTorque(); //< For having Data and name correspondence
		 const double& getTime();
		 void setDirectories(std::string outDirectory, std::string inDirectory = std::string());
		 void setVerbose(int verbose) {}
		 void setRecord(bool record) {}*/

	protected:
		double timeStamp_;
		OpenSimFileLogger<double>* logger_;
		std::map<std::string, double> dataAngle_;

		/**
		 * @brief CEINMS requires a torque but this plugin sets it to 0
		*/
		std::map<std::string, double> dataTorque_;
		std::string outDirectory_;
		bool record_;

		/**
		 * @brief return empty map when GetDataMapTorque() is called, because this plugin doesn't calculate torque
		*/
		std::map<std::string, double> emptyMap_;

		/**
		 * @brief Xsens UDP server conenction
		*/
		UdpServer* udpServerXsens_;

		set<string> nameSet_;
		bool threadStop_;
		std::vector<std::string> dofName_;
		std::mutex mtxEthercat_;
};

#endif