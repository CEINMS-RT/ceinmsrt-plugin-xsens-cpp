<img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQ1vHDQMUcbXRoh_hAcOvHvIXIQVk2dtlak3QBu-KU_PnGjMAwr6yHy9VdkSe04BuIF9_w&usqp=CAU" width=200>

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)]()

[CEINMS-rt]() installation and [use]().
Plugin [installation]() and [compilation](). How to use a [plugin]().

# Xsens Plugin

Plugin to receive kinematics data from the Xsens software and sensors. Optionally, you can also use it with Twincat (throught [ADS](https://www.beckhoff.com/en-ca/products/automation/twincat/tc1xxx-twincat-3-base/tc1000.html)) to combine data or send command to a robotics device using Twincat. This can be activeted by using the cmake option 'USE_TWINCAT' line 9 of the [CMakeLists.txt](https://github.com/CEINMS-RT/xsensPlugin/blob/master/CMakeLists.txt).

## Installation

### Xsens

* Install Xsens MVN Analyze (Tested version: 2020.0) [here](https://www.xsens.com/software-downloads?hsCtaTracking=62e6fd16-1936-4f65-9660-f85c413320da%7C3912eea5-ec35-43d9-b6fb-e2d1be46ee71), including its Developer Toolkit (under the 'Tools' tab, tested version: 1.0.7)
* Put path to Xsens MVN Developer Toolkit in PATH environment variables
* Compile the CEINMS

### Twincat

See [here](https://github.com/CEINMS-RT/xsensPlugin/blob/master/CMakeLists.txt).

## Getting Started

### Plugin available

#### Consumer nd producer plugin

``` xml
PluginPositionAndTorqueTwinCatAndXsens
```

Get ankle left and right data from Twincat. In fact this plugin does not use Xsens. Send joint torque data to Twincat.

``` xml
PluginPositionAndTorqueTwinCatAndXsensCalib
```

Get lower-limb kinematics data from Xsens. Send joint torque data to Twincat.

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

### Connection to Twincat

The inputs and outputs are composed of vector in ADS with the following name:

"NMS_Controller.Input.CEINMS_In" (data going to Twincat)

"NMS_Controller.Output.CEINMS_Out" (data going from Twincat)

The name of the task in Twincat is TcCOM object. The name of the variables is CEINMS_In for the input from CEINMS to Twincat and CEINMS_Out for the output from Twincat to CEINMS.
If your Twincat project does not follow this convention CEINMS will not connect and transmit the data.
This name can be found in PluginPositionAndTorqueTwinCat.cpp line 26 and 27.
The CEINMS_In is composed of the following data (LREAL) (see line 67):

* Left Ankle Position
* Right Ankle Position
* Left Ankle Torque (from the exo)
* Right Ankle Torque (from the exo)
* Random Signal
* Trigger

The CEINMS_Out is composed of the following data (LREAL) (see line 136):

* Left Ankle Torque (NMS)
* Right Ankle Torque (NMS)

## Citation

If you find this repository useful in your research, please consider giving a star ⭐ and cite our [IEEE TBME paper](https://spiral.imperial.ac.uk/bitstream/10044/1/48309/2/durandau%202017.pdf) and [IEEE T-RO paper](https://arxiv.org/abs/2108.00980) by using the following BibTeX entrys.

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
