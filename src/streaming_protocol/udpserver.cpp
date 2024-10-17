/*! \file
	\section FileCopyright Copyright Notice
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

#include "udpserver.h"

DWORD WINAPI udpThreadFunc(LPVOID param) //this is terrible :(
{
	UdpServer* server = (UdpServer*)param;
	server->readMessages();
	return 0;
}

UdpServer::UdpServer(XsString address, uint16_t port, bool record, std::string outDirectory,bool recordAsXsens)
	: m_started(false)
	, m_stopping(false), record_(record), outDirectory_(outDirectory), timePast_(-1.0), recordAsXsens_(recordAsXsens)
{
	std::vector<std::string> dofName;
#ifdef XSENS_UPPER
	dofName.push_back("L5_S1_Flex_Ext");
	dofName.push_back("L5_S1_Lat_Bending");
	dofName.push_back("L5_S1_axial_rotation");
	dofName.push_back("L4_L3_Flex_Ext");
	dofName.push_back("L4_L3_Lat_Bending");
	dofName.push_back("L4_L3_axial_rotation");
	dofName.push_back("arm_flex_r");
	dofName.push_back("arm_add_r");
	dofName.push_back("arm_rot_r");
	dofName.push_back("arm_flex_l");
	dofName.push_back("arm_add_l");
	dofName.push_back("arm_rot_l");
#endif
#ifdef XSENS_LOWER
	dofName.push_back("hip_rotation_r");
	dofName.push_back("hip_adduction_r");
	dofName.push_back("hip_flexion_r");
	dofName.push_back("knee_angle_r");
	dofName.push_back("ankle_angle_r");
	dofName.push_back("hip_rotation_l");
	dofName.push_back("hip_adduction_l");
	dofName.push_back("hip_flexion_l");
	dofName.push_back("knee_angle_l");
	dofName.push_back("ankle_angle_l");
#endif

	timeMtx_.lock();
	timeStamp_ = rtb::getTime();
	timeMtx_.unlock();

	m_port = port;
	m_hostName = address;

	m_parserManager.reset(new ParserManager());
	m_socket.reset(new XsSocket(IpProtocol::IP_UDP, NetworkLayerProtocol::NLP_IPV4));

	XsResultValue res = m_socket->bind(m_hostName, m_port);//bind

	if (record_)
	{
		logger_ = new OpenSimFileLogger<double>(outDirectory_);
		if(recordAsXsens_)
			logger_->addLog(Logger::IKXSENS, dofName);
		else
			logger_->addLog(Logger::IK, dofName);
	}

	xsensCom_ = new std::thread(&UdpServer::readMessages, this);

}

UdpServer::~UdpServer()
{
	//stopThread();

	m_stopping = true;
	xsensCom_->join();
	delete xsensCom_;

	if (record_)
	{
		logger_->stop();
		delete logger_;
	}
}

std::map<std::string, double> UdpServer::getAngle()
{
	dataMtx_.lock();
	std::map<std::string, double> dataMap = dataMap_;
	dataMtx_.unlock();
	return dataMap;
}

void UdpServer::readMessages()
{
	XsByteArray buffer;
	double timeLocal;
	std::map<std::string, double> dataMap;
	std::vector<double> dataSaveIK;
	while (!m_stopping)
	{
		
		timeLocal = rtb::getTime();
		int rv = m_socket->read(buffer);
		if (buffer.size() > 0)
		{

			timeLocal = rtb::getTime();
			void* data = m_parserManager->readDatagram(buffer);

			if (Datagram::messageType(buffer) == StreamingProtocol::SPPoseEuler)
			{
				std::vector<Kinematics>* dataEulerLocal;
				dataEulerLocal = static_cast<std::vector<Kinematics>*>(data);

				dataEuler_ = *dataEulerLocal;

			}
			if (Datagram::messageType(buffer) == StreamingProtocol::SPJointAngles)
			{
				if (data != nullptr)
				{
					std::vector<Joint>* dataAngleLocal;
					dataAngleLocal = static_cast<std::vector<Joint>*>(data);
					Joint temp;

					dataAngle_ = *dataAngleLocal;





					//std::map<std::string, double> dataMap;
					//std::vector<double> dataSaveIK;

					dataSaveIK.clear();
					//if (dataEuler_.size() != 0 && dataAngle_.size() != 0)
					if (dataAngle_.size() != 0)
					{
						//dataMap["pelvis_tilt"] = -XsMath_deg2rad(dataEuler_[0].rotation[1]);//X Axis Rot
						//dataMap["pelvis_list"] = XsMath_deg2rad(dataEuler_[0].rotation[0]);//Y Axis Rot
						//dataMap["pelvis_rotation"] = XsMath_deg2rad(dataEuler_[0].rotation[2]);//Z Axis Rot

						//dataMap["pelvis_tx"] = (dataEuler_[0].pos[0]);//X Axis Rot
						//dataMap["pelvis_ty"] = (dataEuler_[0].pos[2]);//Z Axis Rot
						//dataMap["pelvis_tz"] = (dataEuler_[0].pos[1]);//Y Axis Rot
#ifdef XSENS_UPPER
						dataMap["L5_S1_Flex_Ext"] = XsMath_deg2rad(dataAngle_[1].rotation[2]);
						dataMap["L5_S1_Lat_Bending"] = XsMath_deg2rad(dataAngle_[1].rotation[0]);
						dataMap["L5_S1_axial_rotation"] = XsMath_deg2rad(dataAngle_[1].rotation[1]);

						dataMap["L3_L4_Flex_Ext"] = XsMath_deg2rad(dataAngle_[2].rotation[2]);
						dataMap["L3_L4_Lat_Bending"] = XsMath_deg2rad(dataAngle_[2].rotation[0]);
						dataMap["L3_L4_axial_rotation"] = XsMath_deg2rad(dataAngle_[2].rotation[1]);

						dataMap["arm_flex_r"] = XsMath_deg2rad(dataAngle_[8].rotation[2]);
						dataMap["arm_add_r"] = XsMath_deg2rad(dataAngle_[8].rotation[0]);
						dataMap["arm_rot_r"] = XsMath_deg2rad(dataAngle_[8].rotation[1]);

						dataMap["arm_flex_l"] = XsMath_deg2rad(dataAngle_[12].rotation[2]);
						dataMap["arm_add_l"] = XsMath_deg2rad(dataAngle_[12].rotation[0]);
						dataMap["arm_rot_l"] = XsMath_deg2rad(dataAngle_[12].rotation[1]);

						dataSaveIK.push_back(dataMap["L5_S1_Flex_Ext"]);
						dataSaveIK.push_back(dataMap["L5_S1_Lat_Bending"]);
						dataSaveIK.push_back(dataMap["L5_S1_axial_rotation"]);

						dataSaveIK.push_back(dataMap["L3_L4_Flex_Ext"]);
						dataSaveIK.push_back(dataMap["L3_L4_Lat_Bending"]);
						dataSaveIK.push_back(dataMap["L3_L4_axial_rotation"]);

						dataSaveIK.push_back(dataMap["arm_flex_r"]);
						dataSaveIK.push_back(dataMap["arm_add_r"]);
						dataSaveIK.push_back(dataMap["arm_rot_r"]);

						dataSaveIK.push_back(dataMap["arm_flex_l"]);
						dataSaveIK.push_back(dataMap["arm_add_l"]);
						dataSaveIK.push_back(dataMap["arm_rot_l"]);
#endif
#ifdef XSENS_LOWER
						dataMap["hip_rotation_r"] = XsMath_deg2rad(dataAngle_[14].rotation[1]);//Y Axis Rot
						dataMap["hip_adduction_r"] = -XsMath_deg2rad(dataAngle_[14].rotation[0]);//X Axis Rot
						dataMap["hip_flexion_r"] = XsMath_deg2rad(dataAngle_[14].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["hip_rotation_r"]);
						dataSaveIK.push_back(dataMap["hip_adduction_r"]);
						dataSaveIK.push_back(dataMap["hip_flexion_r"]);
						dataMap["knee_angle_r"] = -XsMath_deg2rad(dataAngle_[15].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["knee_angle_r"]);
						dataMap["ankle_angle_r"] = XsMath_deg2rad(dataAngle_[16].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["ankle_angle_r"]);
						dataMap["hip_rotation_l"] = XsMath_deg2rad(dataAngle_[18].rotation[1]);//X Axis Rot
						dataMap["hip_adduction_l"] = -XsMath_deg2rad(dataAngle_[18].rotation[0]);//Y Axis Rot
						dataMap["hip_flexion_l"] = XsMath_deg2rad(dataAngle_[18].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["hip_rotation_l"]);
						dataSaveIK.push_back(dataMap["hip_adduction_l"]);
						dataSaveIK.push_back(dataMap["hip_flexion_l"]);
						dataMap["knee_angle_l"] = -XsMath_deg2rad(dataAngle_[19].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["knee_angle_l"]);
						dataMap["ankle_angle_l"] = XsMath_deg2rad(dataAngle_[20].rotation[2]);//Z Axis Rot
						dataSaveIK.push_back(dataMap["ankle_angle_l"]);
#endif
						if (record_)
						{
							if (timeLocal != timePast_)
							{
								timePast_ = timeLocal;
								if (recordAsXsens_)
									logger_->log(Logger::IKXSENS, timeLocal, dataSaveIK);
								else
									logger_->log(Logger::IK, timeLocal, dataSaveIK);
							}
						}

						timeMtx_.lock();
						timeStamp_ = timeLocal;
						timeMtx_.unlock();
						dataMtx_.lock();
						dataMap_ = dataMap;
						dataMtx_.unlock();


					}
				}
			}
			//m_socket->flush();
			//Sleep(7);
			//m_socket->flush();
		}
		buffer.clear();
	}

	std::cout << "Stopping receiving packets..." << std::endl << std::endl;

	m_stopping = false;
	m_started = false;
}


double UdpServer::getAngleDataTime()
{
	double timeLocal;
	timeMtx_.lock();
	timeLocal = timeStamp_;
	timeMtx_.unlock();
	return timeLocal;
}

void UdpServer::startThread()
{
	if (m_started)
		return;

	m_started = true;
	m_stopping = false;
	xsensCom_->join();
	delete xsensCom_;
	//xsStartThread(udpThreadFunc, this, 0);
}

void UdpServer::stopThread()
{
	if (!m_started)
		return;
	m_stopping = true;
	while (m_started)
		XsTime::msleep(10);
}
