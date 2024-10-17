#include <PluginPositionAndTorqueTwinCatAndXsens.h>

PluginPositionAndTorqueTwinCatAndXsens::PluginPositionAndTorqueTwinCatAndXsens() : record_(false), outDirectory_("output"), threadStop_(false), cpt_(0)
{
	//std::cout << "start plugin ethercat " << this << std::endl;
}

PluginPositionAndTorqueTwinCatAndXsens::~PluginPositionAndTorqueTwinCatAndXsens()
{

}

void PluginPositionAndTorqueTwinCatAndXsens::init(std::string& executableXMLFileName)
{
	ExecutionXmlReader executionCfg(executableXMLFileName);
	init(atoi(executionCfg.getComsumerPort().c_str()));
}

void PluginPositionAndTorqueTwinCatAndXsens::init(int portno)
{
	//std::cout << "Init plugin ethercat " << this << std::endl;
	//Ethercat ADS connection

	client_ = new tcAdsClient(portno);//350
	std::string varInputs = "NMS_Controller.Input.CEINMS_In";
	std::string varOutputs = "NMS_Controller.Output.CEINMS_Out";
	varNameVect_.resize(2);
	varNameVect_[VarName::Inputs] = client_->getVariableHandle(&varInputs[0], varInputs.size());
	varNameVect_[VarName::Outputs] = client_->getVariableHandle(&varOutputs[0], varOutputs.size());

	if (varNameVect_[VarName::Inputs] == -1 || varNameVect_[VarName::Outputs] == -1)
		exit(-1);

	//XSENS TCP/IP Connection
	std::string hostDestinationAddress = "localhost";
	int port = 9763;
	udpServerXsens_ = new UdpServer(hostDestinationAddress, (uint16_t)port, record_, outDirectory_);

	timeStamp_ = rtb::getTime();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	ethercatThread_ = new std::thread(&PluginPositionAndTorqueTwinCatAndXsens::ethercatThread, this);
}

void PluginPositionAndTorqueTwinCatAndXsens::ethercatThread()
{
	double timeLocal;
	std::map<std::string, double> ikDataLocal;
	std::map<std::string, double> idDataLocal;
	std::vector<std::string> dofNameLocal = dofName_;

	if (record_)
	{
		//std::cout << this << " : " << cpt_ << std::endl;
		//cpt_++;
		logger_ = new OpenSimFileLogger<double>(outDirectory_);
		logger_->addLog(Logger::IK, dofNameLocal);
		logger_->addLog(Logger::ID, dofNameLocal);
		logger_->addLogCSV("RandomSignal");
		logger_->addLogCSV("Trigger");
	}

	while (!threadStop_)
	{

		int length = 2; //Lankle, Rankle
		std::vector<double> temp;
		std::vector<double> dataIK, dataSaveIK;
		std::vector<double> dataID, dataSaveID;
		dataIK.resize(length);
		dataID.resize(length);
		temp.resize(6);

		double randSignal, trigger;

		timeLocal = rtb::getTime();

		client_->read(varNameVect_[VarName::Outputs], &temp[0], 6 * sizeof(double));
		dataIK[0] = temp[1]; // Right Ankle Position
		dataIK[1] = temp[0]; // Left Ankle Position
		dataID[0] = temp[3]; // Right Ankle Torque
		dataID[1] = temp[2];// Left Ankle Torque
		randSignal = temp[4];
		trigger = temp[5];
#ifdef CALIBRATE
		mtxEthercat_.lock();
		std::map<std::string, double> dataAngleXsensLoc = udpServerXsens_->getAngle();
		//timeLocal = udpServerXsens_->getAngleDataTime(); // If use the xsens we need to use the time from Xsens The model is sensivtive to tiem because of the fiber velocity
		mtxEthercat_.unlock();
#endif
		for (std::vector<std::string>::const_iterator it = dofNameLocal.begin(); it != dofNameLocal.end(); it++)
		{

			if (*it == "ankle_angle_r")
			{
				dataSaveIK.push_back(dataIK[0]);// +M_PI / 2);
				ikDataLocal[*it] = dataIK[0];// + M_PI / 2;
				dataSaveID.push_back(dataID[0]);
				idDataLocal[*it] = dataID[0];
			}
			else if (*it == "ankle_angle_l")
			{
				dataSaveIK.push_back(dataIK[1]);// + M_PI / 2);
				ikDataLocal[*it] = dataIK[1];//+ M_PI / 2;
				dataSaveID.push_back(dataID[1]);
				idDataLocal[*it] = dataID[1];
			}
#ifdef CALIBRATE
			else if (*it == "knee_angle_r")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "knee_angle_l")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "hip_flexion_r")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "hip_flexion_l")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "pelvis_rotation")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "pelvis_list")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "pelvis_tilt")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
			else if (*it == "pelvis_ty")
			{
				dataSaveIK.push_back(dataAngleXsensLoc[*it]);
				ikDataLocal[*it] = dataAngleXsensLoc[*it];
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
#endif
			else
			{
				ikDataLocal[*it] = 0;
				dataSaveIK.push_back(0);
				idDataLocal[*it] = 0;
				dataSaveID.push_back(0);
			}
		}

		mtxEthercat_.lock();
		dataAngleEthercat_ = ikDataLocal;
		dataTorqueEthercat_ = idDataLocal;
		mtxEthercat_.unlock();
		mtxTime_.lock();
		timeStampEthercat_ = timeLocal;
		mtxTime_.unlock();

		if (record_)
		{
			logger_->log(Logger::IK, timeLocal, dataSaveIK);
			logger_->log(Logger::ID, timeLocal, dataSaveID);
			logger_->logCSV("RandomSignal", timeLocal, randSignal);
			logger_->logCSV("Trigger", timeLocal, trigger);
		}

	}
}

void PluginPositionAndTorqueTwinCatAndXsens::setDofTorque(const std::vector<double>& dofTorque)
{
	int length = 2; //Lankle, Rankle
	// TODO ADD gain
	std::vector<double> temp;
	temp.resize(2);
	for (std::vector<std::string>::const_iterator it = dofName_.begin(); it != dofName_.end(); it++)
	{
		int cpt = std::distance<std::vector<std::string>::const_iterator>(dofName_.begin(), it);
		if (*it == "ankle_angle_l")
		{
			temp[1] = dofTorque[cpt];
			//client_->write(varNameVect_[VarName::TorqueControlLeftAnkle], &temp[cpt], sizeof(double));
			//std::cout << "ankle_angle_l" << temp[cpt] << std::endl << std::flush;
		}
		if (*it == "ankle_angle_r")
		{
			temp[0] = dofTorque[cpt];
			//client_->write(varNameVect_[VarName::TorqueControlRightAnkle], &temp[cpt], sizeof(double));
			//std::cout << "ankle_angle_r" << temp[cpt] << std::endl << std::flush;
		}
	}
	if (dofTorque.size() >= 2)
		client_->write(varNameVect_[VarName::Inputs], &temp[0], 2 * sizeof(double));
}

const std::map<std::string, double>& PluginPositionAndTorqueTwinCatAndXsens::GetDataMap()
{
#ifndef CALIBRATE
	mtxEthercat_.lock();
	dataAngle_ = dataAngleEthercat_;
	mtxEthercat_.unlock();
#else
	mtxEthercat_.lock();
	dataAngle_ = udpServerXsens_->getAngle();
	mtxEthercat_.unlock();
#endif
	return dataAngle_;
}

const std::map<std::string, double>& PluginPositionAndTorqueTwinCatAndXsens::GetDataMapTorque()
{
	mtxEthercat_.lock();
	dataTorque_ = dataTorqueEthercat_;
	mtxEthercat_.unlock();
	return dataTorque_;
}

const double& PluginPositionAndTorqueTwinCatAndXsens::GetAngleTimeStamp()
{
#ifndef CALIBRATE
	mtxTime_.lock();
	timeStamp_ = timeStampEthercat_;
	mtxTime_.unlock();
#else
	mtxEthercat_.lock();
	timeStamp_ = udpServerXsens_->getAngleDataTime();
	mtxEthercat_.unlock();
#endif
	return timeStamp_;
}

void PluginPositionAndTorqueTwinCatAndXsens::stop()
{
	threadStop_ = true;
	ethercatThread_->join();
	delete ethercatThread_;
	if (record_)
	{
		logger_->stop();
		delete logger_;
	}
	for (std::vector<unsigned long>::const_iterator it = varNameVect_.begin(); it != varNameVect_.end(); it++)
		client_->releaseVariableHandle(*it);
	client_->disconnect();
	delete client_;
	delete udpServerXsens_;
}

void PluginPositionAndTorqueTwinCatAndXsens::setDirectory(std::string outDirectory, std::string inDirectory)
{
	outDirectory_ = outDirectory;
}

#ifdef UNIX
extern "C" AngleAndComsumerPlugin* create() {
	return new PluginPositionAndTorqueTwinCatAndXsens;
}

extern "C" void destroy(AngleAndComsumerPlugin* p) {
	delete p;
}
#endif

#ifdef WIN32 // __declspec (dllexport) id important for dynamic loading
extern "C" __declspec (dllexport) AngleAndComsumerPlugin* __cdecl create() {
	return new PluginPositionAndTorqueTwinCatAndXsens;
}

extern "C" __declspec (dllexport) void __cdecl destroy(AngleAndComsumerPlugin* p) {
	delete p;
}
#endif