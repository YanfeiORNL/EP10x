// EnergyPlus, Copyright (c) 1996-2020, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef EnergyPlusAPIDataTransfer_h_INCLUDED
#define EnergyPlusAPIDataTransfer_h_INCLUDED

#include <EnergyPlus/TypeDefs.h>
#include <EnergyPlus/api/EnergyPlusAPI.hh>

#ifdef __cplusplus
extern "C" {
#endif

/// This file provides data transfer API functions

// ----- GENERIC QUERY FUNCTIONS

/// \brief Gets available API data for the current simulation
/// \details This function returns a string of API data in CSV form for the current simulation
/// The data can be easily parsed and then used in subsequent API code.
/// \return A const char * pointing to a CSV formatted string
ENERGYPLUSLIB_API const char * listAllAPIDataCSV();
/// \brief Provides a user-facing check for whether API data is ready to be accessed
/// \details Many parts of a simulation need to be set up to complete a run.
///          At the early stages of a simulation, most data has not been allocated and set up.
///          Calling to retrieve variable, meter, or actuator values prior to this point can cause problems.
///          This function allows a user to call the simulation to check whether data is ready for access.
///          Do not call for variable, meter, actuator, or any other internal exchange data prior to this returning true.
/// \return Returns 0 (success) once the data is ready, otherwise returns 1.
ENERGYPLUSLIB_API int apiDataFullyReady();

// ----- FUNCTIONS RELATED TO VARIABLES

/// \brief Marks a variable as requested in a simulation
/// \details To optimize memory and cpu usage, the simulation ignores variables not requested in the IDF.
///          This function allows marking variables as used even if they are not in the input file.
/// \param[in] type Variable Type, such as "System Node Mass Flow Rate", or "Site Outdoor Air DryBulb Temperature"
/// \param[in] key Variable Key, such as "Node 32", or "Environment"
/// \remark This function should be called prior to executing the simulation.
ENERGYPLUSLIB_API void requestVariable(const char* type, const char* key);
/// \brief Gets a handle to a variable
/// \details Looks up a handle to a variable within a running simulation.
///          Variables are identified by a key and type.
///          Variables are only available if they are explicitly listed as Output:Variable objects in the input file,
///          or if API calls to `requestVariable` to mark this variable as requested prior to a simulation run.
///          This function will return a handle of -1 if a match is not found for this type/key combination.
/// \param[in] type Variable Type, such as "System Node Mass Flow Rate", or "Site Outdoor Air DryBulb Temperature"
/// \param[in] key Variable Key, such as "Node 32", or "Environment"
/// \return The integer handle to a variable, or -1 if the variable was not found
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
/// \see requestVariable
ENERGYPLUSLIB_API int getVariableHandle(const char* type, const char* key);
/// \brief Gets the current value of a variable
/// \details This function uses the integer handle of a variable and retrieves the current value of the variable.
/// \param[in] handle The handle id to a variable which can be retrieved using the `getVariableHandle` function.
/// \return The current value of the variable, in floating point form.
/// \throws std::runtime_error Throws a std::runtime_error if there is a problem looking up the variable, most likely a handle out of range.
/// \see getVariableHandle
ENERGYPLUSLIB_API Real64 getVariableValue(int handle);

// ----- FUNCTIONS RELATED TO METERS

/// \brief Gets a handle to a meter
/// \details Looks up a handle to a meter within a running simulation.
///          Meters are identified by a single name string.
/// \param[in] meterName The name of the meter to which a reference is retrieved
/// \return The integer handle to a meter, or -1 if the meter was not found
/// \remark Note the behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int getMeterHandle(const char* meterName);
/// \brief Gets the current value of a meter
/// \details Looks up the value of an existing meter within a running simulation.
/// \param[in] handle The handle id of the meter, which can be retrieved using the `getMeterHandle` function
/// \return The floating point value of a meter at the current time
/// \throws std::runtime_error Throws a std::runtime_error if the meter handle is out of range.
/// \remark Note the behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
/// \see getMeterHandle
ENERGYPLUSLIB_API Real64 getMeterValue(int handle);

// ----- FUNCTIONS RELATED TO ACTUATORS

/// \brief Gets a handle to an actuator
/// \details Looks up a handle to an actuator within a running simulation.
///          Actuators are identified by three parameters: key, type, and control type
/// \param[in] componentType The component type for the actuator, such as "Weather Data"
/// \param[in] controlType The specific control type for the actuator, such as "Outdoor Dew Point"
/// \param[in] uniqueKey The unique key for this actuator, such as "Environment"
/// \return The integer handle to an actuator, or -1 if the actuator was not found
/// \remark Note the behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int getActuatorHandle(const char* componentType, const char* controlType, const char* uniqueKey);
/// \brief Resets an actuator so EnergyPlus will calculate the value as normal
/// \details When an actuator value is set using `setActuatorValue`, a flag is set so the actuated value will not be
///          overridden.  If the user desires to revert back and let EnergyPlus calculate the actuated value, this
///          function will reset the flag.  The user can always set the actuated value again anytime.
/// \param[in] handle The integer handle to the actuator, which can be retrieved using the `getActuatorHandle` function
/// \remark Note the behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
/// \see getActuatorHandle
/// \see setActuatorValue
ENERGYPLUSLIB_API void resetActuator(int handle);
/// \brief Sets the value of an actuator in EnergyPlus
/// \details Actuators are variables in the simulation which can be overridden.
///          Calculations made outside of EnergyPlus are performed and used to update values inside EnergyPlus via actuators.
/// \param[in] handle The integer handle to the actuator, which can be retrieved using the `getActuatorHandle` function
/// \param[in] value The floating point value to be assigned to the actuator in the simulation
/// \remark Note the behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
/// \see getActuatorHandle
/// \see setActuatorValue
ENERGYPLUSLIB_API void setActuatorValue(int handle, Real64 value);

// ----- FUNCTIONS RELATED TO STATIC "INTERNAL VARIABLES"

/// \brief Gets a handle to an internal variable
/// \details Internal variables are essentially "static" data -- think zone volume or component sizing.
///          Internal variables are identified by a key and type.
/// \param[in] type Internal Variable Type, such as "Zone Floor Area"
/// \param[in] key Internal Variable Key, such as "Zone 1"
/// \return The integer handle to an independent variable, or -1 if the variable was not found
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int getInternalVariableHandle(const char* type, const char* key);
/// \brief Gets the current value of an internal variable
/// \details This function uses the integer handle of a variable and retrieves the static value of the variable.
/// \param[in] handle The handle id to a variable which can be retrieved using the `getInternalVariableHandle` function.
/// \return The current value of the variable, in floating point form.
/// \see getInternalVariableHandle
ENERGYPLUSLIB_API Real64 getInternalVariableValue(int handle);

// ----- FUNCTIONS RELATED TO PYTHON PLUGIN GLOBAL VARIABLES (ONLY USED FOR PYTHON PLUGIN SYSTEM)

/// \brief Gets a handle to a Python Plugin "Global" variable
/// \details When using Python Plugins, it is sometimes necessary to share data between plugins.
///          These global variables are declared in the input file first, and then can be read/write by any number of plugins.
///          Plugin global variables are identified by name only.  This function returns -1 if a match is not found.
/// \param[in] name The name of the plugin global variable, which is declared in the EnergyPlus input file
/// \return The integer handle to a plugin global variable, or -1 if a match is not found.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int getPluginGlobalVariableHandle(const char* name);
/// \brief Gets the current value of a Python Plugin "Global" variable
/// \details When using Python Plugins, the value of the shared "global" variables can change at any time.
///          This function returns the current value of the variable.
/// \param[in] handle The handle id to a Python Plugin "Global" variable, which can be retrieved using the `getPluginGlobalVariableHandle` function.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle is out of range.
/// \see apiDataFullyReady
/// \see getPluginGlobalVariableHandle
ENERGYPLUSLIB_API Real64 getPluginGlobalVariableValue(int handle);
/// \brief Sets the value of a Python Plugin "Global" variable
/// \details When using Python Plugins, the value of the shared "global" variables can change at any time.
///          This function sets the variable to a new value.
/// \param[in] handle The handle id to a Python Plugin "Global" variable, which can be retrieved using the `getPluginGlobalVariableHandle` function.
/// \param[in] value The floating point value to be assigned to the global variable
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle is out of range.
/// \see apiDataFullyReady
/// \see getPluginGlobalVariableHandle
ENERGYPLUSLIB_API void setPluginGlobalVariableValue(int handle, Real64 value);

// ----- FUNCTIONS RELATED TO PYTHON PLUGIN TREND VARIABLES (ONLY USED FOR PYTHON PLUGIN SYSTEM)

/// \brief Gets a handle to a Python Plugin "Trend" variable
/// \details When using Python Plugins, variable history can be stored/accessed using "trend" variables.
///          These trend variables are declared in the input file first, and then can be read/write by any number of plugins.
///          Plugin trend variables are identified by name only.  If a trend is not found, this function will return -1.
/// \param[in] name The name of the plugin trend variable, which is declared in the EnergyPlus input file
/// \return The integer handle to a plugin trend variable, or -1 if a match is not found.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int getPluginTrendVariableHandle(const char* name);
/// \brief Gets the current value of a Python Plugin "Trend" variable at a specific history point
/// \details When using Python Plugins, the value of the "trend" variable can be retrieved from previous timesteps, up
///          to the number of history terms defined in the input file.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] timeIndex The number of timesteps backward to traverse the trend when returning this value.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or timeIndex is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableValue(int handle, int timeIndex);
/// \brief Gets the average value of a Python Plugin "Trend" variable over a given number of history points
/// \details When using Python Plugins, the average value of the "trend" variable over a number of previous timesteps can be retrieved, up
///          to the number of history terms defined in the input file.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] count The number of timesteps backward to traverse the trend when returning this value.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or count is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableAverage(int handle, int count);
/// \brief Gets the minimum value of a Python Plugin "Trend" variable over a given number of history points
/// \details When using Python Plugins, the minimum value of the "trend" variable over a number of previous timesteps can be retrieved, up
///          to the number of history terms defined in the input file.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] count The number of timesteps backward to traverse the trend when returning this value.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or count is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableMin(int handle, int count);
/// \brief Gets the maximum value of a Python Plugin "Trend" variable over a given number of history points
/// \details When using Python Plugins, the maximum value of the "trend" variable over a number of previous timesteps can be retrieved, up
///          to the number of history terms defined in the input file.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] count The number of timesteps backward to traverse the trend when returning this value.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or count is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableMax(int handle, int count);
/// \brief Gets the summation of a Python Plugin "Trend" variable over a given number of history points
/// \details When using Python Plugins, the summation of the "trend" variable over a number of previous timesteps can be retrieved, up
///          to the number of history terms defined in the input file.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] count The number of timesteps backward to traverse the trend when returning this value.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or count is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableSum(int handle, int count);
/// \brief Gets the average trajectory of a Python Plugin "Trend" variable over a given number of history points
/// \detail For many control applications, it is useful to know the average trajectory of a trend variable over time.
///         To calculate this, the program will sample the history of the trend over the user-specified number of time history terms,
///         perform a regression, and return the slope of this regression line.  If positive, the trend is, on average, increasing,
///         and decreasing if negative.
/// \param[in] handle The handle id to a Python Plugin "Trend" variable, which can be retrieved using the `getPluginTrendVariableHandle` function.
/// \param[in] count The number of timesteps backward to traverse the trend when calculate this average direction.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \remark This function only has meaning in the context of Python Plugin workflows, not in regular API workflows.
/// \throws std::runtime_error Throws a std::runtime_error if the handle or count is out of range.
/// \see apiDataFullyReady
/// \see getPluginTrendVariableHandle
ENERGYPLUSLIB_API Real64 getPluginTrendVariableDirection(int handle, int count);

// ----- FUNCTIONS RELATED TO MISC CURRENT SIMULATION STATE

/// \brief Returns the current year of the simulation.
/// \details A simulation can span multiple years and will always have a "meaningful" year that is either user-defined explicitly,
///          determined based on other inputs in the input file, or chosen as the current year.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int year();

/// \brief Returns the current month of the simulation, from 1 for January to 12 for December.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int month();

/// \brief Returns the current day of month of the simulation, from 1 to 28, 29, 30, or 31, based on the month.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int dayOfMonth();

/// \brief Returns the current day of week of the simulation, from 1 for Sunday to 7 on Saturday
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int dayOfWeek();

/// \brief Returns the current day of year of the simulation, from 1 for January 1 to 365 (or 366 for a leap year) for December 31.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int dayOfYear();

/// \brief Returns the current daylight savings time status, which is 0 (zero) if DST is off, or 1 (one) if DST is on.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int daylightSavingsTimeIndicator();

/// \brief Returns the current hour of the day in 0-23 form
/// \details The hour within the simulation ranges from 0 for timesteps from 12:00am to 12:59am up to 23 for timesteps from 11:00pm to 11:59pm
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int hour();

/// \brief Returns the current time of the simulation
/// \details For this function, the current time represents the end time of the current time step.
///          The time is returned as floating point fractional hours of the day, and since it represents the end of the
///          current time step, the value will go from just over zero at the beginning of a day to 24.0 at the last time step of the day.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API Real64 currentTime();

/// \brief Returns the minutes into the hour from 1 to 60
/// \details The minutes into the simulation will return the value for the end of the current system time step.
///          This function returns an integer value, but because the HVAC system time step is constantly adjusted, the end of
///          the system time step may actually occur at a fractional part of a minute.  This function truncates the seconds
///          portion during integerization.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int minutes();

/// \brief Returns the duration of the current HVAC system simulation time step, in fractional hours
/// \details The HVAC time step will vary throughout the simulation as needed to maintain convergence while being cautious about program runtime.
///          This function returns the current value of the time step
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API Real64 systemTimeStep();

/// \brief Returns the holiday index for the current day of the simulation
/// \details Days in the simulation year can be regular days, in which case this returns zero, or integers 1-N based on holiday definitions in the input.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int holidayIndex();

/// \brief Returns 0 if the sun is down, or 1 if the sun is up
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int sunIsUp();

/// \brief Returns 0 if it is not currently raining, or 1 if it is currently raining
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int isRaining();

/// \brief Returns the current warmup flag status, 0 if not warming up, 1 if warming up
/// \details During an EnergyPlus simulation, at the beginning of each run period or design day, the first day is
///          repeated until the simulation achieves convergence.  This eliminates the dependence on guesses to initial
///          values used when initializing the entire simulation state.  It can be useful to ignore operations during
///          warmup, and this flag allows checking that status.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int warmupFlag();

/// \brief Returns an integer indicator for the current simulation environment type
/// \details During an EnergyPlus simulation, there are often multiple stages of the simulation run.  These include
///          design days followed by a run period most commonly, but there are also sizing simulation environments.
///          The full list of values is:
///          - Design (Sizing) Day = 1
///          - Design (Sizing) RunPeriod = 2
///          - Weather File Run Period = 3
///          - HVAC-Sizing Design Day = 4
///          - HVAC-Sizing Run Period = 5
///          - Weather Data Processing Environment = 6
///
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
ENERGYPLUSLIB_API int kindOfSim();

/// \brief Returns the current environment number for a specific siulation
/// \details As EnergyPlus simulates, it runs through multiple phases that often include multiple design days followed
///          by a run period.  This function returns the current index which is just incremented for each new phase.
///          This has limited value in a general sense, but for some very tightly defined workflows, this could give
///          some information.  It is more advised to check the result of the `kindOfSim` function to get a reliable environment type.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
/// \see kindOfSim
ENERGYPLUSLIB_API int currentEnvironmentNum();

/// \brief Returns an index to a construction in the simulation
/// \details Some actuators allow specifying different constructions to allow switchable construction control.
///          This function returns an index that can be used in those functions.  The construction is specified by name.
/// \param[in] constructionName The name of the construction to be looked up.
/// \remark The behavior of this function is not well-defined until the `apiDataFullyReady` function returns true.
/// \see apiDataFullyReady
/// \see kindOfSim
ENERGYPLUSLIB_API int getConstructionHandle(const char* constructionName);

#ifdef __cplusplus
}
#endif

#endif // EnergyPlusAPIDataTransfer_h_INCLUDED
