<img src="https://github.com/CEINMS-RT/ceinmsrt-core-cpp/blob/main/CEINMS-RT_V2_ICON.png" width="100%" alt="CEINMS-RT logo">

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)]()

CEINMS-RT [installation](https://ceinms-docs.readthedocs.io/en/latest/Installation%20%5BWindows%5D.html) and [use](https://ceinms-docs.readthedocs.io/en/latest/Tutorial%20%5BWindows%5D%5BUbuntu%5D.html).
Plugin [installation](#xsens-plugin) and [compilation](https://ceinms-docs.readthedocs.io/en/latest/Compilation%20%5BWindows%5D.html). How to use a [plugin](#getting-started).

# Xsens Plugin

Plugin to receive kinematics data from the Xsens software and sensors. Optionally, you can also use it with TwinCAT (throught [ADS](https://www.beckhoff.com/en-ca/products/automation/twincat/tc1xxx-twincat-3-base/tc1000.html)) to combine data or send command to a robotics device using TwinCAT. This can be activated by using the CMake option ``USE_TWINCAT`` in [CMakeLists.txt](CMakeLists.txt#L9).

## Installation

### Xsens

* Install Xsens MVN Analyze (Tested version: 2020.0) [here](https://www.xsens.com/software-downloads?hsCtaTracking=62e6fd16-1936-4f65-9660-f85c413320da%7C3912eea5-ec35-43d9-b6fb-e2d1be46ee71), including its Developer Toolkit (under the 'Tools' tab, tested version: 1.0.7)
* Put path to Xsens MVN Developer Toolkit in PATH environment variables
* Compile the CEINMS

### TwinCAT

See [here](CMakeLists.txt).

## Getting Started

### Plugin available

#### Consumer and producer plugin

``` xml
PluginPositionAndTorqueTwinCatAndXsens
```

Get ankle left and right data from TwinCAT. In fact this plugin does not use Xsens. Send joint torque data to TwinCAT.

``` xml
PluginPositionAndTorqueTwinCatAndXsensCalib
```

Get lower-limb kinematics data from Xsens. Send joint torque data to TwinCAT.

#### Angle only plugin

``` xml
PluginXsensAngle
```

Get lower-limb and upper-limb kinematics data from Xsens.

``` xml
PluginXsensAngleLowerLimb
```

Get upper-limb kinematics data from Xsens.

``` xml
PluginXsensAngleUpperLimb 
```

Get lower-limb kinematics data from Xsens.

### Connection to Xsens

* Open Xsens MVN software
* Click the Network Streamer for options and make sure that 1 stream is selected with port 9763, protocol UDP, stream rate max. In the datagram below select only "Joint Angles".
* Optionally, if you want to replay a saved xsens data file, click "File -> Open" to open a file from example data
* Run CEINMS (with PluginXsens as angle device)
* If recording in CEINMS (-r option) is enabled, the output folder will contain a file named "ikXsens" with the joint angles if using the twincat plugin otherwise it is save as the ik.sto.

### Connection to TwinCAT

The inputs and outputs are composed of vector in ADS with the following name:

"NMS_Controller.Input.CEINMS_In" (data going to TwinCAT)

"NMS_Controller.Output.CEINMS_Out" (data going from TwinCAT)

The name of the task in TwinCAT is TcCOM object. The name of the variables is ``CEINMS_In`` for the input from CEINMS to TwinCAT and ``CEINMS_Out`` for the output from TwinCAT to CEINMS.
If your TwinCAT project does not follow this convention CEINMS will not connect and transmit the data.
This name can be found [here](src/PluginPositionAndTorqueTwinCatAndXsens.cpp#L26).
The ``CEINMS_In`` is composed of the following data (LREAL) ([see](src/PluginPositionAndTorqueTwinCatAndXsens.cpp#L67)):

* Left Ankle Position
* Right Ankle Position
* Left Ankle Torque (from the exo)
* Right Ankle Torque (from the exo)
* Random Signal
* Trigger

The ``CEINMS_Out`` is composed of the following data (LREAL) ([see](src/PluginPositionAndTorqueTwinCatAndXsens.cpp#L195)):

* Left Ankle Torque (NMS)
* Right Ankle Torque (NMS)

## Citation

If you find this repository useful in your research, please consider giving a star ⭐ and cite our [IEEE TBME paper](https://spiral.imperial.ac.uk/bitstream/10044/1/48309/2/durandau%202017.pdf) and [IEEE T-RO paper](https://arxiv.org/abs/2108.00980) by using the following BibTeX entries.

```BibTeX
@article{durandau2017robust,
  title={Robust real-time musculoskeletal modeling driven by electromyograms},
  author={Durandau, Guillaume and Farina, Dario and Sartori, Massimo},
  journal={IEEE transactions on biomedical engineering},
  volume={65},
  number={3},
  pages={556--564},
  year={2017},
  publisher={IEEE}
}
```

```BibTeX
@article{durandau2022neuromechanical,
  title={Neuromechanical model-based adaptive control of bilateral ankle exoskeletons: biological joint torque and electromyogram reduction across walking conditions},
  author={Durandau, Guillaume and Rampeltshammer, Wolfgang F and van der Kooij, Herman and Sartori, Massimo},
  journal={IEEE Transactions on Robotics},
  volume={38},
  number={3},
  pages={1380--1394},
  year={2022},
  publisher={IEEE}
}
```

## License

CEINMS-rt and plugins are licensed under the [Apache License](LICENSE).
