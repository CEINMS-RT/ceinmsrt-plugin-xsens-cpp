CMAKE_MINIMUM_REQUIRED (VERSION 3.10) 
PROJECT(Xsens)


SET(BOOST_MIN_VERSION "1.48.0")
SET(CMAKE_POSITION_INDEPENDENT_CODE ON) # For Dynamic loading
# SET(CMAKE_VERBOSE_MAKEFILE OFF)
# SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
SET(USE_TWINCAT ON)
OPTION(BUILD_DOC "Build documentation" OFF)


 IF(WIN32)
	IF(DEFINED ENV{XSD_HOME})
		set(XSD_EXECUTABLE "$ENV{XSD_HOME}/lib/Release/x64")
	endif()
	
		# No need to set anything here is installed in the default paths
	IF(DEFINED ENV{BOOST_INCLUDE_DIRS})
		set(BOOST_INCLUDE_DIRS $ENV{BOOST_INCLUDE_DIRS})
	else()
		set(BOOST_INCLUDE_DIRS "full/path/to/boost/lib64-msvc-14.1")
	endif()
	# message("boost include: ${BOOST_INCLUDE_DIRS}")

	IF(DEFINED ENV{Boost_LIBRARY})
		set(Boost_LIBRARY $ENV{Boost_LIBRARY})
	else()
		set(Boost_LIBRARY "full/path/to/boost")
	endif()
	# message("boost library: ${Boost_LIBRARY}")
	SET(Boost_USE_STATIC_LIBS ON)
	add_definitions( -DBOOST_ALL_NO_LIB )
	add_definitions( -DBoost_COMPILER="-vc141" )
 ENDIF(WIN32)



IF(UNIX)
    IF(CMAKE_COMPILER_IS_GNUCC)
         SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
    ENDIF(CMAKE_COMPILER_IS_GNUCC)
	SET(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin/Unix/)
	SET(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
	SET(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
	
	SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,,--add-stdcall-alias,--no-undefined,-allow-multiple-definition") # For Dynamic loading
	IF(CMAKE_VERSION VERSION_LESS "3.1")
		IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			SET(CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
		ENDIF()
	ELSE()
		SET(CMAKE_CXX_STANDARD 11)
	ENDIF()
ENDIF(UNIX)

FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} COMPONENTS timer chrono thread date_time program_options filesystem system unit_test_framework REQUIRED)
FIND_PACKAGE(XSD REQUIRED)
find_package(XercesC REQUIRED)
#FIND_PACKAGE(Doxygen)

# IF (DOXYGEN_FOUND)
    # # set input and output files
    # SET(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    # SET(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.out)

    # # request to configure the file
    # CONFIGURE_FILE(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # MESSAGE("Doxygen build started")

    # # note the option ALL which allows to build the docs together with the application 
    # ADD_CUSTOM_TARGET( doc_doxygen ALL
        # COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        # COMMENT "Generating API documentation with Doxygen"
        # VERBATIM )
# ELSE (DOXYGEN_FOUND)
  # MESSAGE("Doxygen need to be installed to generate the doxygen documentation")
# ENDIF (DOXYGEN_FOUND)


IF(USE_TWINCAT)
	INCLUDE_DIRECTORIES(C:/TwinCAT/AdsApi/TcAdsDll/Include)
	LINK_DIRECTORIES(C:/TwinCAT/AdsApi/TcAdsDll/x64
		C:/TwinCAT/AdsApi/TcAdsDll/x64/lib)
ENDIF(USE_TWINCAT)

INCLUDE_DIRECTORIES("C:/Program Files/Xsens/Xsens MVN Developer Toolkit 1.0.6/x64/include/")
LINK_DIRECTORIES("C:/Program Files/Xsens/Xsens MVN Developer Toolkit 1.0.6/x64/lib" )


 INCLUDE_DIRECTORIES(
	include
	src
	src/streaming_protocol
	../lib/AdsClient/src
 )
 
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(src)