# Change Log
All notable changes to bmi160 Sensor API will be documented in this file. 

## v3.6.1, 23 Aug 2017

#### Changed
* SPI support for FIFO reading and parsing logic update

## v3.6.0, 04 Aug 2017

#### Added
* Added interfaces for the following features 
     - FOC
     - Manual Offset compensation
     - Offset compenation value update to NVM

## v3.5.0, 13 Apr 2017

#### Added
* Self-test feature for accel and gyro added

## v3.4.0, 31 Mar 2017

#### Added
* Auxiliary sensor interface in auto-mode(data-mode) support is implemented

## v3.3.0, 31 Mar 2017

#### Added
* Extracting of gyro data from FIFO is implemented.

## v3.2.1, 15 Mar 2017

#### Changed
* Aux init made compatible for all auxillary sensors.

## v3.2.0, 09 Mar 2017

#### Added
* Reading FIFO data and extracting of accel data from FIFO implemented.
* FIFO FULL Interrupt implemented only for Accel data.

## v3.1.0, 15 Feb 2017

#### Changed
* Condition for gyro BW corrected in order to set for all BW modes.
* Interrupt Active High level setting handled.
* Existing step detector code corrected for recommended settings.
* Disabling of step detector and step counter has been removed in low-g interrupt.

#### Added
* Error code implemented, if input parameter is out of range or invalid.
* Source of data (filter & prefilter) setting handled for slope, no-motion, tap, sig, high-g & Low-g interrupt.
* Error handling of Pre-filter data & Interrupt is done in low power mode. 
* Burst write handled for low & suspended power mode.
* Auxiliary read & write implemented in order to read the BMM150 data.
* Interrupt disable mechanism added.


