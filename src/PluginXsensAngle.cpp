#include <PluginXsensAngle.h>

PluginXsens::PluginXsens() : record_(false), outDirectory_("output"), threadStop_(false), emptyMap_({ {std::string(),0.} })
{
}

PluginXsens::~PluginXsens()
{
}

void PluginXsens::init(std::string xmlName, std::string executionName)
{
	//XSENS TCP/IP Connection
	std::string hostDestinationAddress = "localhost";
	//std::string hostDestinationAddress = "192.168.1.2";
	int port = 9763;
	udpServerXsens_ = new UdpServer(hostDestinationAddress, (uint16_t)port, record_, outDirectory_, false);

	timeStamp_ = rtb::getTime();
}

const std::map<std::string, double>& PluginXsens::GetDataMap()
{
#ifdef CALIBRATE
	mtxEthercat_.lock();
	dataAngle_ = dataAngleEthercat_;
	mtxEthercat_.unlock();
#else
	//mtxEthercat_.lock();
	dataAngle_ = udpServerXsens_->getAngle();
	//mtxEthercat_.unlock();
#endif
	return dataAngle_;
}

const std::map<std::string, double>& PluginXsens::GetDataMapTorque()
{
	//std::cout << "WARNING: PluginXsens plugin does not calculate ID torque." << std::endl;
	return emptyMap_;
}


const double& PluginXsens::getTime()
{
#ifdef CALIBRATE
	mtxTime_.lock();
	timeStamp_ = timeStampEthercat_;
	mtxTime_.unlock();
#else
	//mtxEthercat_.lock();
	timeStamp_ = udpServerXsens_->getAngleDataTime();
	//mtxEthercat_.unlock();
#endif
	return timeStamp_;
}

void PluginXsens::stop()
{
	threadStop_ = true;
	//ethercatThread_->join();
	//delete ethercatThread_;

	delete udpServerXsens_;
}

void PluginXsens::setDirectories(std::string outDirectory, std::string inDirectory)
{
	outDirectory_ = outDirectory;
}

#ifdef UNIX
extern "C" EMGAndAnglePlugin * create() {
	return new PluginXsens;
}

extern "C" void destroy(EMGAndAnglePlugin * p) {
	delete p;
}
#endif

#ifdef WIN32 // __declspec (dllexport) id important for dynamic loading
extern "C" __declspec (dllexport) ProducersPluginVirtual * __cdecl create() {
	return new PluginXsens;
}

extern "C" __declspec (dllexport) void __cdecl destroy(ProducersPluginVirtual * p) {
	delete p;
}
#endif