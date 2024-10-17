#include <PluginXsens.h>
#include <cstdlib>
#include <csignal>
#include <GetTime.h>
#include <iomanip> 

bool endMain;

void SigintHandler(int sig)
{
	endMain = true;;
}

int main(void)
{
	endMain = false;
	signal(SIGINT, SigintHandler);

	PluginXsens plugin;
	/*std::vector<std::string> dofName;
	dofName.push_back("r_ankle");
	dofName.push_back("l_ankle");
	dofName.push_back("r_knee");
	dofName.push_back("l_knee");
	dofName.push_back("r_hip");
	dofName.push_back("l_hip");*/

	std::cout << "start" << std::endl;

	/*std::vector<double> torque;
	torque.push_back(0);
	torque.push_back(1);
	torque.push_back(2);
	torque.push_back(3);

	plugin.setDofName(dofName);*/
	//plugin.setDirectory("Test");
	//plugin.setRecord(true);
	//plugin.init();
	std::cout << "init done" << std::endl;
	while (!endMain)
	{
		double timeInint = rtb::getTime();
		std::cout << "timeEMG received" << std::endl;
		//double time = plugin.GetAngleTimeStamp();
		std::cout << "plugin time received" << std::endl;
		//std::cout << std::setprecision(16) << timeEMG::getTime() << " a" << std::endl;
		std::map<std::string, double> temp = plugin.GetDataMap();
		std::cout << "plugin data received" << std::endl;
		//std::cout << "Time to get Data Angle: " << std::setprecision(16) << timeEMG::getTime() - timeInint << std::endl;
		std::cout << "Angle data: " << temp["knee_angle_r"] << " : " << temp["ankle_angle_r"] << "  time: " << time << std::endl;
		/*timeInint = timeEMG::getTime();
		std::map<std::string, double> tempT = plugin.GetDataMapTorque();
		std::cout << "Time to get Data torque: " << std::setprecision(16) << timeEMG::getTime() - timeInint << " Torque data: " << tempT["ankle_angle_r"] << " : " << tempT["ankle_angle_l"] << std::endl;
		plugin.setDofTorque(torque);*/
		Sleep(100); //100
	}
	plugin.stop();

	return 0;
}

