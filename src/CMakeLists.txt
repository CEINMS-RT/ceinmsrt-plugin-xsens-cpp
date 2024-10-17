IF(USE_TWINCAT)
	ADD_LIBRARY( PluginPositionAndTorqueTwinCatAndXsens SHARED PluginPositionAndTorqueTwinCatAndXsens.cpp )
	ADD_LIBRARY( PluginPositionAndTorqueTwinCatAndXsensCalib SHARED PluginPositionAndTorqueTwinCatAndXsens.cpp )
	target_compile_definitions(PluginPositionAndTorqueTwinCatAndXsensCalib PUBLIC CALIBRATE XSENS_LOWER)
ENDIF(USE_TWINCAT)

ADD_LIBRARY( PluginXsensAngleAndConsumer SHARED PluginXsens.cpp )

ADD_LIBRARY( PluginXsensAngle SHARED PluginXsensAngle.cpp )

ADD_LIBRARY( PluginXsensAngleLowerLimb SHARED PluginXsensAngle.cpp )

ADD_LIBRARY( PluginXsensAngleUpperLimb SHARED PluginXsensAngle.cpp )

ADD_LIBRARY( xsensLib 
	streaming_protocol/angularsegmentkinematicsdatagram.cpp
	streaming_protocol/centerofmassdatagram.cpp
	streaming_protocol/datagram.cpp
	streaming_protocol/eulerdatagram.cpp
	streaming_protocol/jointanglesdatagram.cpp
	streaming_protocol/linearsegmentkinematicsdatagram.cpp
	streaming_protocol/metadatagram.cpp
	streaming_protocol/parsermanager.cpp
	streaming_protocol/positiondatagram.cpp
	streaming_protocol/quaterniondatagram.cpp
	streaming_protocol/scaledatagram.cpp
	streaming_protocol/streamer.cpp
	streaming_protocol/timecodedatagram.cpp
	streaming_protocol/trackerkinematicsdatagram.cpp
	streaming_protocol/udpserver.cpp
)
SET_TARGET_PROPERTIES(xsensLib PROPERTIES COMPILE_FLAGS "-XSENS_UPPER")
SET_TARGET_PROPERTIES(xsensLib PROPERTIES COMPILE_FLAGS "-XSENS_LOWER")

ADD_LIBRARY( xsensLibLowerLimb 
	streaming_protocol/angularsegmentkinematicsdatagram.cpp
	streaming_protocol/centerofmassdatagram.cpp
	streaming_protocol/datagram.cpp
	streaming_protocol/eulerdatagram.cpp
	streaming_protocol/jointanglesdatagram.cpp
	streaming_protocol/linearsegmentkinematicsdatagram.cpp
	streaming_protocol/metadatagram.cpp
	streaming_protocol/parsermanager.cpp
	streaming_protocol/positiondatagram.cpp
	streaming_protocol/quaterniondatagram.cpp
	streaming_protocol/scaledatagram.cpp
	streaming_protocol/streamer.cpp
	streaming_protocol/timecodedatagram.cpp
	streaming_protocol/trackerkinematicsdatagram.cpp
	streaming_protocol/udpserver.cpp
)
SET_TARGET_PROPERTIES(xsensLibLowerLimb PROPERTIES COMPILE_FLAGS "-DXSENS_LOWER")

ADD_LIBRARY( xsensLibUpperLimb 
	streaming_protocol/angularsegmentkinematicsdatagram.cpp
	streaming_protocol/centerofmassdatagram.cpp
	streaming_protocol/datagram.cpp
	streaming_protocol/eulerdatagram.cpp
	streaming_protocol/jointanglesdatagram.cpp
	streaming_protocol/linearsegmentkinematicsdatagram.cpp
	streaming_protocol/metadatagram.cpp
	streaming_protocol/parsermanager.cpp
	streaming_protocol/positiondatagram.cpp
	streaming_protocol/quaterniondatagram.cpp
	streaming_protocol/scaledatagram.cpp
	streaming_protocol/streamer.cpp
	streaming_protocol/timecodedatagram.cpp
	streaming_protocol/trackerkinematicsdatagram.cpp
	streaming_protocol/udpserver.cpp
)

SET_TARGET_PROPERTIES(xsensLibUpperLimb PROPERTIES COMPILE_FLAGS "-DXSENS_UPPER")

TARGET_LINK_LIBRARIES(xsensLib
	xstypes64
	FileLogger
)

TARGET_LINK_LIBRARIES(xsensLibUpperLimb
	xstypes64
	FileLogger
)

TARGET_LINK_LIBRARIES(xsensLibLowerLimb
	xstypes64
	FileLogger
)

IF(USE_TWINCAT)
	TARGET_LINK_LIBRARIES( PluginPositionAndTorqueTwinCatAndXsens
		TcAdsDll
		FileIO
		XMLExecutionInterpreter
		${Boost_LIBRARIES}
		xsensLib
		${XERCES_LIBRARIES}
		AdsClient
		FileLogger 
	)

	TARGET_LINK_LIBRARIES( PluginPositionAndTorqueTwinCatAndXsensCalib
		TcAdsDll
		FileIO
		XMLExecutionInterpreter
		${Boost_LIBRARIES}
		xsensLib
		${XERCES_LIBRARIES}
		AdsClient
		FileLogger		
	)
	#SET_TARGET_PROPERTIES(PluginPositionAndTorqueTwinCatAndXsensCalib PROPERTIES COMPILE_FLAGS "-DXSENS_LOWER")
	target_compile_features(PluginPositionAndTorqueTwinCatAndXsensCalib PUBLIC cxx_std_17)
	target_compile_definitions(PluginPositionAndTorqueTwinCatAndXsensCalib PUBLIC -D_HAS_STD_BYTE=0)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			target_link_libraries(PluginPositionAndTorqueTwinCatAndXsensCalib stdc++fs)
	endif()
	
	
	target_compile_features(PluginPositionAndTorqueTwinCatAndXsens PUBLIC cxx_std_17)
	target_compile_definitions(PluginPositionAndTorqueTwinCatAndXsens PUBLIC -D_HAS_STD_BYTE=0)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			target_link_libraries(PluginPositionAndTorqueTwinCatAndXsens stdc++fs)
	endif()

ENDIF(USE_TWINCAT)

TARGET_LINK_LIBRARIES( PluginXsensAngleAndConsumer 
	FileIO
	XMLExecutionInterpreter
	${Boost_LIBRARIES}
	xsensLib
	${XERCES_LIBRARIES}
	AdsClient 
	FileLogger
)

TARGET_LINK_LIBRARIES( PluginXsensAngle 
	FileIO
	XMLExecutionInterpreter
	${Boost_LIBRARIES}
	xsensLib
	${XERCES_LIBRARIES}
	ws2_32
	winmm
	FileLogger
)

TARGET_LINK_LIBRARIES( PluginXsensAngleLowerLimb 
	FileIO
	XMLExecutionInterpreter
	${Boost_LIBRARIES}
	xsensLibLowerLimb
	${XERCES_LIBRARIES}
	ws2_32
	winmm
	FileLogger
)

TARGET_LINK_LIBRARIES( PluginXsensAngleUpperLimb 
	FileIO
	XMLExecutionInterpreter
	${Boost_LIBRARIES}
	xsensLibUpperLimb
	${XERCES_LIBRARIES}
	ws2_32
	winmm
	FileLogger
)

ADD_EXECUTABLE(TestPluginXsens TestXsens.cpp
)

TARGET_LINK_LIBRARIES(TestPluginXsens
	PluginXsensAngleAndConsumer
	FileLogger
)
