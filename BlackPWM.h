#ifndef BLACKPWM_H_
#define BLACKPWM_H_

#include "BlackCore.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <stdint.h>

using namespace std;

namespace BlackLib
{

    /*!
    * This enum is used for selecting PWM name.
    */
    enum pwmName            {   P8_13                   = 0,
                                P8_19                   = 1,
                                P9_14                   = 2,
                                P9_16                   = 3,
                                P9_21                   = 4,
                                P9_22                   = 5,
                                P9_42                   = 6,

                                EHRPWM2B                = P8_13,
                                EHRPWM2A                = P8_19,
                                EHRPWM1A                = P9_14,
                                EHRPWM1B                = P9_16,
                                EHRPWM0B                = P9_21,
                                EHRPWM0A                = P9_22,
                                ECAP0                   = P9_42
                            };

    /*!
    * This string array is used for mapping pwm name.
    */
    const std::string pwmNameMap[7] = { "P8_13",
                                        "P8_19",
                                        "P9_14",
                                        "P9_16",
                                        "P9_21",
                                        "P9_22",
                                        "P9_42",
                                    };

    /*!
    * This enum is used for setting PWM polarity.
    */
    enum polarityType       {   straight                = 0,
                                reverse                 = 1
                            };




    // ######################################### BLACKCOREPWM DECLARATION STARTS ########################################## //

    /*! @brief Preparation phase of Beaglebone Black, to use PWM.
     *
     *    This class is core of the BlackPWM class. It includes private functions which are doing base processes
     *    for using pwms and protected functions which are using for exporting private variables to
     *    derived class(es).
     */
    class BlackCorePWM : virtual private BlackCore
    {
        private:
            errorCorePWM    *pwmCoreErrors;             /*!< @brief is used to hold the errors of BlackCorePWM class */
            std::string     pwmTestPath;                /*!< @brief is used to hold the pwm_test (pwm device driver) path */
            pwmName         pwmPinName;                 /*!< @brief is used to hold the selected pwm @b pin name */

            /*! @brief Loads PWM overlays to device tree.
            *
            *  This function loads @b "am33xx_pwm" and @b "bone_pwm_P?_?" overlay to device tree.
            *  Question marks at the second overlay, represents port and pin numbers of selected PWM
            *  output. This overlays perform pinmuxing and generate device drivers.
            *  @return True if successful, else false.
            */
            bool            loadDeviceTree();

            /*! @brief Finds full name of pwm_test.
            *
            *  This function searches @b "ocp.X" directory to find directory starts with
            *  @b "pwm_test_?_?." by using searchDirectoryOcp() protected function at BlackCore class.
            *  @return Full name of pwm_test directory if successfull, else BlackLib::PWM_TEST_NAME_NOT_FOUND string.
            *  @sa BlackCore::searchDirectoryOcp()
            */
            std::string     findPwmTestName(pwmName pwm);


        protected:
            /*! @brief Exports pwm period file name to derived class.
            *
            *  @return Pwm period file name.
            */
            std::string     getPeriodFilePath();

            /*! @brief Exports pwm duty file name to derived class.
            *
            *  @return Pwm duty file name.
            */
            std::string     getDutyFilePath();

            /*! @brief Exports pwm run file name to derived class.
            *
            *  @return Pwm run file name.
            */
            std::string     getRunFilePath();

            /*! @brief Exports pwm polarity file name to derived class.
            *
            *  @return Pwm polarity file name.
            */
            std::string     getPolarityFilePath();

            /*! @brief Exports errorCorePWM struct to derived class.
            *
            *  @return errorCorePWM struct pointer.
            */
            errorCorePWM    *getErrorsFromCorePWM();

        public:

            /*! @brief Constructor of BlackCorePWM class.
            *
            *  This function initializes errorCorePWM struct and calls device tree loading and
            *  pwm test name finding functions.
            *
            *  @sa BlackCorePWM::loadDeviceTree()
            *  @sa BlackCorePWM::findPwmTestName()
            *  @sa pwmName
            */
                            BlackCorePWM(pwmName pwm);

            /*! @brief Destructor of BlackCorePWM class.
            *
            * This function deletes errorCorePWM struct pointer.
            */
            virtual         ~BlackCorePWM();

            /*! @brief First declaration of this function.
            */
            virtual std::string getValue() = 0;
    };
    // ########################################## BLACKCOREPWM DECLARATION ENDS ########################################### //










    // ########################################### BLACKPWM DECLARATION STARTS ############################################ //

    /*! @brief Interacts with end user, to use PWM.
     *
     *    This class is end node to use PWM. End users interact with PWM signal from this class.
     *    It includes public functions to set and get properties of PWM.
     *
     * @par Example
      @verbatim
      EXAMPLE PROJECT FILE TREE:

         myPwmProject
         |-> src
             |-> BlackLib
                 |-> BlackDirectory
                     |-> BlackDirectory.cpp
                     |-> BlackDirectory.h
                 |-> BlackThread
                     |-> BlackThread.cpp
                     |-> BlackThread.h
                 |-> BlackMutex
                     |-> BlackMutex.cpp
                     |-> BlackMutex.h
                 |-> BlackTime
                     |-> BlackTime.cpp
                     |-> BlackTime.h
                 |-> BlackADC
                     |-> BlackADC.cpp
                     |-> BlackADC.h
                 |-> BlackGPIO
                     |-> BlackGPIO.cpp
                     |-> BlackGPIO.h
                 |-> BlackI2C
                     |-> BlackI2C.cpp
                     |-> BlackI2C.h
                 |-> BlackPWM
                     |-> BlackPWM.cpp
                     |-> BlackPWM.h
                 |-> BlackSPI
                     |-> BlackSPI.cpp
                     |-> BlackSPI.h
                 |-> BlackUART
                     |-> BlackUART.cpp
                     |-> BlackUART.h
                 |-> BlackCore.cpp
                 |-> BlackCore.h
                 |-> BlackDef.h
                 |-> BlackErr.h
                 |-> BlackLib.h
             |-> myPwmProject.cpp
      @endverbatim
     *  @n@n If BlackLib source files are located in your project like above example project file tree, you have to
     *  include BlackPWM.h or another source files with adding this line to your project file (myPwmProject.cpp at
     *  the example):
     *  @code{.cpp}
     *      #include "BlackLib/BlackPWM/BlackPWM.h"
     *  @endcode
     *  @n@n If BlackLib source files are located at same level with your project file (myPwmProject.cpp at the
     *  example), you have to include BlackPWM.h or another source files with adding this line to your project file:
     *  @code{.cpp}
     *      #include "BlackPWM/BlackPWM.h"
     *  @endcode
     *  @n @n
     *  @code{.cpp}
     *  // Filename: myPwmProject.cpp
     *  // Author:   Yiğit Yüce - ygtyce@gmail.com
     *
     *  #include <iostream>
     *  #include "BlackLib/BlackPWM/BlackPWM.h"
     *
     *  int main()
     *  {
     *      BlackLib::BlackPWM  myPwm(BlackLib::P8_19);
     *      std::cout << myPwm.getValue();
     *
     *      return 0;
     *  }
     * @endcode
     * @n @n
     * You can use "using namespace BlackLib" also. You can get rid of writing "BlackLib::", with using this method.
     * @code{.cpp}
     *  // Filename: myPwmProject.cpp
     *  // Author:   Yiğit Yüce - ygtyce@gmail.com
     *
     *  #include <iostream>
     *  #include "BlackLib/BlackPWM/BlackPWM.h"
     *  using namespace BlackLib;
     *
     *  int main()
     *  {
     *      BlackPWM  myPwm(EHRPWM2A);
     *      std::cout << myPwm.getValue();
     *
     *      return 0;
     *  }
     *
     * @endcode
     *
     * @n @n
     * @par Pwm Definition
       @verbatim
        1 ...............................________............................________
             :        :        :        |        |        :        :        |        |
             :   (1)  :   (2)  :   (3)  |   (4)  |        :        :        |        |
        0 ...:________:________:________|        |________:________:________|        |
             :        :        :        :        :        :        :        :        :
             <-----------t1------------> <--t2--> <-----------t1-----------> <--t2-->
       @endverbatim
     * @n @n @b t1 represents "space time" ==> @b 3 units at the figure
     * @n @b t2 represents "load time"     ==> @b 1 unit at the figure
     * @n <b> (t1+t2) </b> represents "period time" ==> @b 4 units at the figure
     * @n <b> (t2/(t1+t2)) </b> represents "duty ratio" ==> @b 0.25 at the figure
     */
    class BlackPWM : virtual private BlackCorePWM
    {
        private:
            errorPWM        *pwmErrors;                 /*!< @brief is used to hold the errors of BlackPWM class */
            std::string     periodPath;                 /*!< @brief is used to hold the @a period file path */
            std::string     dutyPath;                   /*!< @brief is used to hold the @a duty file path */
            std::string     runPath;                    /*!< @brief is used to hold the @a run file path */
            std::string     polarityPath;               /*!< @brief is used to hold the @a polarity file path */


        public:
            /*!
            * This enum is used to define PWM debugging flags.
            */
            enum flags      {   periodFileErr   = 0,    /*!< enumeration for @a errorPWM::periodFileError status */
                                dutyFileErr     = 1,    /*!< enumeration for @a errorPWM::dutyFileError status */
                                runFileErr      = 2,    /*!< enumeration for @a errorPWM::runFileError status */
                                polarityFileErr = 3,    /*!< enumeration for @a errorPWM::polarityFileError status */
                                outOfRangeErr   = 4,    /*!< enumeration for @a errorPWM::outOfRange status */
                                dtErr           = 5,    /*!< enumeration for @a errorCorePWM::dtError status */
                                dtSubSystemErr  = 6,    /*!< enumeration for @a errorCorePWM::dtSsError status */
                                pwmTestErr      = 7,    /*!< enumeration for @a errorCorePWM::pwmTestError status */
                                cpmgrErr        = 9,    /*!< enumeration for @a errorCore::capeMgrError status */
                                ocpErr          = 10    /*!< enumeration for @a errorCore::ocpError status */
                            };

            /*! @brief Constructor of BlackPWM class.
            *
            * This function initializes BlackCorePWM class with entered parameter and errorPWM struct.
            * Then it sets file paths of period, duty, polarity and run files.
            * @param [in] pwm        pwm name (enum)
            *
            * @par Example
            *  @code{.cpp}
            *   BlackLib::BlackPWM  myPwm(BlackLib::P8_19);
            *   BlackLib::BlackPWM *myPwmPtr = new BlackLib::BlackPWM(BlackLib::EHRPWM2B);
            *
            *   myPwm.getValue();
            *   myPwmPtr->getValue();
            *
            * @endcode
            *
            * @sa pwmName
            */
                            BlackPWM(pwmName pwm);

            /*! @brief Destructor of BlackPWM class.
            *
            * This function deletes errorPWM struct pointer.
            */
            virtual         ~BlackPWM();

            /*! @brief Reads percentage value of duty cycle.
            *
            * This function calls getNumericPeriodValue() and getNumericDutyValue() inline functions for
            * finding out period and duty values of pwm. After do that, it calculates percentage value of
            * time that stays at "1".
            *  @return @a String type percentage value.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(21.0);
            *   std::string val = myPwm.getValue();
            *   std::cout << "Pwm duty ratio: " << val << "%";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm duty ratio: 21%
            * @endcode
            *
            *  @sa getNumericPeriodValue()
            *  @sa getNumericDutyValue()
            */
            std::string     getValue();

            /*! @brief Reads period value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::periodPath variable.
            * This file holds pwm period value at nanosecond (ns) level.
            *  @return @a string type period value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_STRING.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   // if new period value is less than the current duty value, the new period value setting
            *   // operation couldn't execute. So firstly duty value is set to zero for safe steps.
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(214000);
            *
            *   std::string val = myPwm.getPeriodValue();
            *   std::cout << "Pwm period time: " << val << " nanoseconds";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 214000 nanoseconds
            * @endcode
            */
            std::string     getPeriodValue();

            /*! @brief Reads duty value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::dutyPath variable.
            * This file holds pwm duty value at nanosecond (ns) level.
            *  @return @a string type duty value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_STRING.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   // if new period value is less than the current duty value, the new period value setting
            *   // operation couldn't execute. So firstly duty value is set to zero for safe steps.
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(900000);
            *   myPwm.setDutyPercent(50.0);
            *
            *   std::string val = myPwm.getDutyValue();
            *   std::cout << "Pwm duty time: " << val << " nanoseconds";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm duty time: 450000 nanoseconds
            * @endcode
            */
            std::string     getDutyValue();

            /*! @brief Reads run value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::runPath variable.
            * This file holds pwm run value.
            *  @return @a string type run value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_STRING.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setRunState(BlackLib::run);
            *   std::string val1 = myPwm.getRunValue();
            *
            *   myPwm.setRunState(BlackLib::stop);
            *   std::string val2 = myPwm.getRunValue();
            *
            *   std::cout << "Run value 1: " << val1 << std::endl;
            *   std::cout << "Run value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Run value 1: 1
            *   // Run value 2: 0
            * @endcode
            */
            std::string     getRunValue();

            /*! @brief Reads polarity value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::polarityPath variable.
            * This file holds pwm polarity value.
            *  @return @a String type polarity value. If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_STRING.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setPolarity(BlackLib::straight);
            *   std::string val1 = myPwm.getPolarityValue();
            *
            *   myPwm.setPolarity(BlackLib::reverse);
            *   std::string val2 = myPwm.getPolarityValue();
            *
            *   std::cout << "Polarity value 1: " << val1 << std::endl;
            *   std::cout << "Polarity value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Polarity value 1: 0
            *   // Polarity value 2: 1
            * @endcode
            */
            std::string     getPolarityValue();

            /*! @brief Reads numeric percentage value of duty cycle.
            *
            * This function calls getNumericPeriodValue() and getNumericDutyValue() inline functions, for finding
            * out the period and duty values of pwm. After do that, it calculates percentage value of
            * time that stays at "1".
            *  @return @a Float type percentage value.
            *
            *  @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(21.0);
            *   float val = myPwm.getNumericValue();
            *   std::cout << "Pwm duty ratio: " << val << "%";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm duty ratio: 21.0%
            * @endcode
            *
            *  @sa getNumericPeriodValue()
            *  @sa getNumericDutyValue()
            */
            float           getNumericValue();

            /*! @brief Reads numeric period value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::periodPath variable.
            * This file holds pwm period value at nanosecond (ns) level.
            * @return @a int64_t (long int) type period value.  If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_INT.
            * @warning Any alphabetic character existence in period file can crash your application.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   // if new period value is less than the current duty value, the new period value setting
            *   // operation couldn't execute. So firstly duty value is set to zero for safe steps.
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(214000);
            *
            *   long int val = myPwm.getNumericPeriodValue();
            *   std::cout << "Pwm period time: " << val << " nanoseconds";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 214000 nanoseconds
            * @endcode
            */
            int64_t         getNumericPeriodValue();

            /*! @brief Reads numeric duty value of pwm signal.
            *
            * This function reads specified file from path, where defined at BlackPWM::dutyPath variable.
            * This file holds pwm duty value at nanosecond (ns) level.
            * @return @a int64_t (long int) type duty value.  If file opening fails, it returns BlackLib::FILE_COULD_NOT_OPEN_INT.
            * @warning Any alphabetic character existence in period file can crash your application.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   // if new period value is less than the current duty value, the new period value setting
            *   // operation couldn't execute. So firstly duty value is set to zero for safe steps.
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(214000);
            *
            *   // space ratio time = duty value, also duty value must be less than current period time
            *   myPwm.setSpaceRatioTime(200000, BlackLib::nanosecond);
            *
            *   long int val = myPwm.getNumericDutyValue();
            *   std::cout << "Pwm duty time: " << val << " nanoseconds";
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 200000 nanoseconds
            * @endcode
            */
            int64_t         getNumericDutyValue();


            /*! @brief Sets percentage value of duty cycle.
            *
            * If input parameter is in range (from 0.0 to 100.0), this function changes duty value
            * without changing period value. For calculating new duty value, the current period
            * multiplies by (1 - entered percentage/100) value. After do that, this calculated value
            * is saved to duty file.
            * @param [in] percentage new percantage value(float)
            * @return True if setting new value is successful, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(500000, BlackLib::nanosecond);
            *   myPwm.setDutyPercent(20.0);
            *
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty time  : " << myPwm.getDutyValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty ratio : " << myPwm.getValue() << "%";
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 500000 nanoseconds
            *   // Pwm duty time  : 400000 nanoseconds
            *   // Pwm duty ratio : 20.0%
            * @endcode
            *
            * @sa getNumericPeriodValue()
            * @note This function can do very little rounding error at new duty value. For example if
            * your current duty value is \f$ 10^6 \f$, this rounding error equals maximum \f$ 0.5 / 10^6 = 5*10^-7 \f$
            */
            bool            setDutyPercent(float percentage);

            /*! @brief Sets period value of pwm signal.
            *
            * If input parameter's nanosecond equivalent is in range (from 0 to 10^9), this function changes period value
            * by saving calculated input parameter to period file. Users can select time type of entered period
            * value like picosecond, nanosecond, microsecond, milisecond and second. This parameter's
            * default value is nanosecond.
            * @param [in] period new period value
            * @param [in] tType time type of your new period value(enum)
            * @return True if setting new period value is successful, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(100.0);
            *
            *   myPwm.setPeriodTime(300000000, BlackLib::picosecond);
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *
            *   myPwm.setPeriodTime(500000, BlackLib::nanosecond);
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *
            *   myPwm.setPeriodTime(700, BlackLib::microsecond);
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *
            *   myPwm.setPeriodTime(1, BlackLib::milisecond);
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 300000 nanoseconds
            *   // Pwm period time: 500000 nanoseconds
            *   // Pwm period time: 700000 nanoseconds
            *   // Pwm period time: 1000000 nanoseconds
            * @endcode
            *
            * @sa BlackLib::timeType
            */
            bool            setPeriodTime(uint64_t period, timeType tType = nanosecond);

            /*! @brief Sets space time value of pwm signal.
            *
            * If input parameter's nanosecond equivalent is in range (from 0 to 10^9), this function changes duty value
            * by saving calculated input parameter to duty file. Users can select time type of entered space time value
            * like picosecond, nanosecond, microsecond, milisecond and second. This parameter's default value is nanosecond.
            * @param [in] space new space time
            * @param [in] tType time type of your new period value(enum)
            * @return True if setting new value is successful, else false.
            * @sa BlackLib::timeType
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(500000, BlackLib::nanosecond);
            *   myPwm.setSpaceRatioTime(125000);
            *
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty time  : " << myPwm.getDutyValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty ratio : " << myPwm.getValue() << "%";
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 500000 nanoseconds
            *   // Pwm duty time  : 125000 nanoseconds
            *   // Pwm duty ratio : 75.0%
            * @endcode
            *
            * @warning If your entered space time's nanosecond equivalent value is greater than the current period time,
            * the new value could not set. This feature is Beaglebone's default.
            */
            bool            setSpaceRatioTime(uint64_t space, timeType tType = nanosecond);

            /*! @brief Sets load time value of pwm signal.
            *
            * If input parameter's nanosecond equivalent is in range (from 0 to 10^9), this function changes duty value
            * by saving (current period value - calculated input value) to duty file. Users can select time type of entered
            * space time value like picosecond, nanosecond, microsecond, milisecond and second. This parameter's default
            * value is nanosecond.
            * @param [in] load new load time
            * @param [in] tType time type of your new period value(enum)
            * @return True if setting new value is successful, else false.
            * @sa BlackLib::timeType
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setDutyPercent(100.0);
            *   myPwm.setPeriodTime(500000, BlackLib::nanosecond);
            *   myPwm.setLoadRatioTime(125000);
            *
            *   std::cout << "Pwm period time: " << myPwm.getPeriodValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty time  : " << myPwm.getDutyValue() << " nanoseconds \n";
            *   std::cout << "Pwm duty ratio : " << myPwm.getValue() << "%";
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Pwm period time: 500000 nanoseconds
            *   // Pwm duty time  : 375000 nanoseconds
            *   // Pwm duty ratio : 25.0%
            * @endcode
            *
            * @warning If your entered load time's nanosecond equivalent value is greater than the current period time,
            * the new value could not set. This feature is Beaglebone's default.
            */
            bool            setLoadRatioTime(uint64_t load, timeType tType = nanosecond);

            /*! @brief Sets polarity of pwm signal.
            *
            * The input parameter is converted to 1 or 0 and this value is saved to polarity file.
            * @param [in] polarity new polarity value(enum)
            * @return True if setting new polarity is successful, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setPolarity(BlackLib::straight);
            *   std::string val1 = myPwm.getPolarityValue();
            *
            *   myPwm.setPolarity(BlackLib::reverse);
            *   std::string val2 = myPwm.getPolarityValue();
            *
            *   std::cout << "Polarity value 1: " << val1 << std::endl;
            *   std::cout << "Polarity value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Polarity value 1: 0
            *   // Polarity value 2: 1
            * @endcode
            *
            * @sa polarityType
            */
            bool            setPolarity(polarityType polarity);

            /*! @brief Sets run value of pwm signal.
            *
            * The input parameter is converted to 1 or 0 and this value is saved to run file.
            * @param [in] state new run value(enum)
            * @return True if setting new run value is successful, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setRunState(BlackLib::run);
            *   std::string val1 = myPwm.getRunValue();
            *
            *   myPwm.setRunState(BlackLib::stop);
            *   std::string val2 = myPwm.getRunValue();
            *
            *   std::cout << "Run value 1: " << val1 << std::endl;
            *   std::cout << "Run value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Run value 1: 1
            *   // Run value 2: 0
            * @endcode
            *
            * @sa runValue
            */
            bool            setRunState(runValue state);

            /*! @brief Toggles run state of pwm signal.
            *
            * This function sets run value to 1 or 0, by value of current state. This new value is
            * saved to run file.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setRunState(BlackLib::run);
            *   std::string val1 = myPwm.getRunValue();
            *
            *   myPwm.toggleRunState();
            *   std::string val2 = myPwm.getRunValue();
            *
            *   std::cout << "Run value 1: " << val1 << std::endl;
            *   std::cout << "Run value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Run value 1: 1
            *   // Run value 2: 0
            * @endcode
            */
            void            toggleRunState();

            /*! @brief Toggles polarity type of pwm signal.
            *
            * This function sets polarity value to 1 or 0, by value of current polarity. This new
            * value is saved to polarity file.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setPolarity(BlackLib::straight);
            *   std::string val1 = myPwm.getPolarityValue();
            *
            *   myPwm.tooglePolarity();
            *   std::string val2 = myPwm.getPolarityValue();
            *
            *   std::cout << "Polarity value 1: " << val1 << std::endl;
            *   std::cout << "Polarity value 2: " << val2 << std::endl;
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Polarity value 1: 0
            *   // Polarity value 2: 1
            * @endcode
            */
            void            tooglePolarity();

            /*! @brief Checks run state of pwm signal.
            *
            * This function calls getRunValue() function and evaluates return value.
            * @return True if return value equals to 1, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setRunState(BlackLib::run);
            *   std::cout << "Run value 1: " << std::boolalpha << myPwm.isRunning() << std::endl;
            *
            *   myPwm.setRunState(BlackLib::stop);
            *   std::cout << "Run value 2: " << std::boolalpha << myPwm.isRunning() << std::endl;
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Run value 1: true
            *   // Run value 2: false
            * @endcode
            *
            * @sa getRunValue()
            */
            bool            isRunning();

            /*! @brief Checks polarity of pwm signal.
            *
            * This function calls getPolarityValue() function and evaluates return value.
            * @return True if return value not equals to 1, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setPolarity(BlackLib::straight);
            *   std::cout << "Polarity value 1: " << std::boolalpha << myPwm.isPolarityStraight() << std::endl;
            *
            *   myPwm.setPolarity(BlackLib::reverse);
            *   std::cout << "Polarity value 2: " << std::boolalpha << myPwm.isPolarityStraight() << std::endl;
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Polarity value 1: true
            *   // Polarity value 2: false
            * @endcode
            *
            * @sa getPolarityValue()
            */
            bool            isPolarityStraight();

            /*! @brief Checks polarity of pwm signal.
            *
            * This function calls getPolarityValue() function and evaluates return value.
            * @return True if return value equals to 1, else false.
            *
            * @par Example
            * @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.setPolarity(BlackLib::straight);
            *   std::cout << "Polarity value 1: " << std::boolalpha << myPwm.isPolarityReverse() << std::endl;
            *
            *   myPwm.setPolarity(BlackLib::reverse);
            *   std::cout << "Polarity value 2: " << std::boolalpha << myPwm.isPolarityReverse() << std::endl;
            *
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // Polarity value 1: false
            *   // Polarity value 2: true
            * @endcode
            *
            * @sa getPolarityValue()
            */
            bool            isPolarityReverse();

            /*! @brief Is used for general debugging.
            *
            * @return True if any error occured, else false.
            *
            * @par Example
            *  @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   myPwm.getValue();
            *
            *   if( myPwm.fail() )
            *   {
            *       std::cout << "ERROR OCCURED" << std::endl;
            *   }
            *   else
            *   {
            *       std::cout << "EVERYTHING IS OK" << std::endl;
            *   }
            * @endcode
            * @code{.cpp}
            *   // Possible Output:
            *   // EVERYTHING IS OK
            * @endcode
            *
            * @sa errorPWM
            */
            bool            fail();

            /*! @brief Is used for specific debugging.
            *
            * You can use this function, after call BlackPWM member functions in your code. The
            * input parameter is used for finding out status of selected error.
            * @param [in] f specific error type (enum)
            * @return Value of @a selected error.
            *
            * @par Example
            *  @code{.cpp}
            *   BlackLib::BlackPWM myPwm(BlackLib::P8_19);
            *
            *   if( myPwm.fail(BlackLib::BlackPWM::dtErr) or
            *       myPwm.fail(BlackLib::BlackPWM::pwmTestErr) or
            *       myPwm.fail(BlackLib::BlackPWM::dtSubSystemErr) )
            *   {
            *        std::cout << "BlackPWM INITIALIZATION FAILED" << std::endl;
            *   }
            *
            *   myPwm.getValue();
            *   if( myPwm.fail(BlackLib::BlackPWM::dutyFileErr) or
            *       myPwm.fail(BlackLib::BlackPWM::periodFileErr) )
            *   {
            *        std::cout << "Duty and/or period file reading is failed." << std::endl;
            *   }
            *
            *
            *   myPwm.setDutyPercent(110.5);
            *   if( myPwm.fail(BlackLib::BlackPWM::outOfRangeErr) )
            *   {
            *        std::cout << "Your percentage is huge." << std::endl;
            *   }
            *
            *   if( myPwm.fail(BlackLib::BlackPWM::dutyFileErr) )
            *   {
            *        std::cout << "Duty file reading or range value is failed." << std::endl;
            *   }
            *
            *   if( myPwm.fail(BlackLib::BlackPWM::periodFileErr) )
            *   {
            *        std::cout << "Period file reading or range value is failed." << std::endl;
            *   }
            *
            *
            *  @endcode
            *  @code{.cpp}
            *   // Possible Output:
            *   // Your percentage is huge.
            *   // Duty file reading or range value is failed.
            *   // Period file reading or range value is failed.
            *  @endcode
            *
            * @sa errorPWM
            */
            bool            fail(BlackPWM::flags f);
    };
    // ########################################### BLACKPWM DECLARATION STARTS ############################################ //


    // ######################################### BLACKCOREPWM DEFINITION STARTS ########################################## //
    BlackCorePWM::BlackCorePWM(pwmName pwm)
    {
        this->pwmPinName    = pwm;
        this->pwmCoreErrors = new errorCorePWM( this->getErrorsFromCore() );

        this->loadDeviceTree();

        this->pwmTestPath   = "/sys/devices/" + this->getOcpName() + "/" + this->findPwmTestName( this->pwmPinName );
    }


    BlackCorePWM::~BlackCorePWM()
    {
        delete this->pwmCoreErrors;
    }


    bool        BlackCorePWM::loadDeviceTree()
    {
        std::string file    = this->getSlotsFilePath();
        std::ofstream slotsFile;

        slotsFile.open(file.c_str(), std::ios::out);
        if(slotsFile.fail())
        {
            slotsFile.close();
            this->pwmCoreErrors->dtSsError  = true;
            this->pwmCoreErrors->dtError    = true;
            return false;
        }
        else
        {
            slotsFile << "am33xx_pwm";
            slotsFile.close();
            this->pwmCoreErrors->dtSsError  = false;
        }


        slotsFile.open(file.c_str(), std::ios::out);
        if(slotsFile.fail())
        {
            slotsFile.close();
            this->pwmCoreErrors->dtError    = true;
            return false;
        }
        else
        {
            slotsFile << ("bone_pwm_" + pwmNameMap[this->pwmPinName]);
            slotsFile.close();
            this->pwmCoreErrors->dtError    = false;
            return true;
        }
    }

    std::string BlackCorePWM::findPwmTestName(pwmName pwm)
    {
        std::string searchResult = SEARCH_DIR_NOT_FOUND;
        switch (pwm)
        {
            case P8_13:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P8_13);
                break;
            }

            case P8_19:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P8_19);
                break;
            }

            case P9_14:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P9_14);
                break;
            }

            case P9_16:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P9_16);
                break;
            }

            case P9_21:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P9_21);
                break;
            }

            case P9_22:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P9_22);
                break;
            }

            case P9_42:
            {
                searchResult = this->searchDirectoryOcp(BlackCore::PWM_P9_42);
                break;
            }
        };


        if( searchResult == SEARCH_DIR_NOT_FOUND )
        {
            this->pwmCoreErrors->pwmTestError = true;
            return PWM_TEST_NAME_NOT_FOUND;
        }
        else
        {
            this->pwmCoreErrors->pwmTestError = false;
            return searchResult;
        }
    }



    std::string BlackCorePWM::getPeriodFilePath()
    {
        return (this->pwmTestPath + "/period");
    }

    std::string BlackCorePWM::getDutyFilePath()
    {
        return (this->pwmTestPath + "/duty");
    }

    std::string BlackCorePWM::getRunFilePath()
    {
        return (this->pwmTestPath + "/run");
    }

    std::string BlackCorePWM::getPolarityFilePath()
    {
        return (this->pwmTestPath + "/polarity");
    }

    errorCorePWM *BlackCorePWM::getErrorsFromCorePWM()
    {
        return (this->pwmCoreErrors);
    }
    // ########################################## BLACKCOREPWM DEFINITION ENDS ########################################### //












    // ########################################### BLACKPWM DEFINITION STARTS ############################################ //
    BlackPWM::BlackPWM(pwmName pwm) : BlackCorePWM(pwm)
    {
        this->pwmErrors     = new errorPWM( this->getErrorsFromCorePWM() );

        this->periodPath    = this->getPeriodFilePath();
        this->dutyPath      = this->getDutyFilePath();
        this->runPath       = this->getRunFilePath();
        this->polarityPath  = this->getPolarityFilePath();
    }

    BlackPWM::~BlackPWM()
    {
        delete this->pwmErrors;
    }

    std::string BlackPWM::getValue()
    {
        double period   = static_cast<long double>( this->getNumericPeriodValue() );
        double duty     = static_cast<long double>( this->getNumericDutyValue() );

        return tostr(static_cast<float>( (1.0 - (duty / period )) * 100 ));
    }

    std::string BlackPWM::getPeriodValue()
    {
        std::ifstream periodValueFile;

        periodValueFile.open(this->periodPath.c_str(),std::ios::in);
        if(periodValueFile.fail())
        {
            periodValueFile.close();
            this->pwmErrors->periodFileError = true;
            return FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            std::string readValue;
            periodValueFile >> readValue;

            periodValueFile.close();
            this->pwmErrors->periodFileError = false;
            return readValue;
        }
    }

    std::string BlackPWM::getDutyValue()
    {
        std::ifstream dutyValueFile;

        dutyValueFile.open(this->dutyPath.c_str(),std::ios::in);
        if(dutyValueFile.fail())
        {
            dutyValueFile.close();
            this->pwmErrors->dutyFileError = true;
            return FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            std::string readValue;
            dutyValueFile >> readValue;

            dutyValueFile.close();
            this->pwmErrors->dutyFileError = false;
            return readValue;
        }
    }

    std::string BlackPWM::getRunValue()
    {
        std::ifstream runValueFile;

        runValueFile.open(this->runPath.c_str(),std::ios::in);
        if(runValueFile.fail())
        {
            runValueFile.close();
            this->pwmErrors->runFileError = true;
            return FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            std::string readValue;
            runValueFile >> readValue;

            runValueFile.close();
            this->pwmErrors->runFileError = false;
            return readValue;
        }
    }

    std::string BlackPWM::getPolarityValue()
    {
        std::ifstream polarityValueFile;

        polarityValueFile.open(this->polarityPath.c_str(),std::ios::in);
        if(polarityValueFile.fail())
        {
            polarityValueFile.close();
            this->pwmErrors->polarityFileError = true;
            return FILE_COULD_NOT_OPEN_STRING;
        }
        else
        {
            std::string readValue;
            polarityValueFile >> readValue;

            polarityValueFile.close();
            this->pwmErrors->polarityFileError = false;
            return readValue;
        }
    }

    float       BlackPWM::getNumericValue()
    {
        double period   = static_cast<long double>( this->getNumericPeriodValue() );
        double duty     = static_cast<long double>( this->getNumericDutyValue() );

        return static_cast<float>( (1.0 - (duty / period )) * 100 );
    }

    inline int64_t    BlackPWM::getNumericPeriodValue()
    {
        int64_t readValue = FILE_COULD_NOT_OPEN_INT;

        std::ifstream periodValueFile;

        periodValueFile.open(this->periodPath.c_str(),std::ios::in);
        if(periodValueFile.fail())
        {
            periodValueFile.close();
            this->pwmErrors->periodFileError = true;
        }
        else
        {
            periodValueFile >> readValue;

            periodValueFile.close();
            this->pwmErrors->periodFileError = false;
        }
        return readValue;
    }

    inline int64_t    BlackPWM::getNumericDutyValue()
    {
        int64_t readValue = FILE_COULD_NOT_OPEN_INT;
        std::ifstream dutyValueFile;

        dutyValueFile.open(this->dutyPath.c_str(),std::ios::in);
        if(dutyValueFile.fail())
        {
            dutyValueFile.close();
            this->pwmErrors->dutyFileError = true;
        }
        else
        {
            dutyValueFile >> readValue;

            dutyValueFile.close();
            this->pwmErrors->dutyFileError = false;
        }
        return readValue;
    }


    bool        BlackPWM::setDutyPercent(float percantage)
    {
        if( percantage > 100.0 or percantage < 0.0 )
        {
            this->pwmErrors->outOfRange      = true;
            this->pwmErrors->dutyFileError   = true;
            this->pwmErrors->periodFileError = true;
            return false;
        }

        this->pwmErrors->outOfRange = false;

        std::ofstream dutyFile;
        dutyFile.open(this->dutyPath.c_str(),std::ios::out);
        if(dutyFile.fail())
        {
            dutyFile.close();
            this->pwmErrors->dutyFileError = true;
            return false;
        }
        else
        {
            dutyFile << static_cast<int64_t>(round((this->getNumericPeriodValue()) * (1.0 - (percantage/100))));
            dutyFile.close();
            this->pwmErrors->dutyFileError = false;
            return true;
        }

    }

    bool        BlackPWM::setPeriodTime(uint64_t period, timeType tType)
    {
        uint64_t writeThis = static_cast<uint64_t>(period * static_cast<double>(pow( 10, static_cast<int>(tType)+9) ));

        if( writeThis > 1000000000)
        {
            this->pwmErrors->outOfRange = true;
            return false;
        }
        else
        {
            this->pwmErrors->outOfRange = false;
            std::ofstream periodFile;
            periodFile.open(this->periodPath.c_str(),std::ios::out);
            if(periodFile.fail())
            {
                periodFile.close();
                this->pwmErrors->periodFileError = true;
                return false;
            }
            else
            {
                periodFile << writeThis;
                periodFile.close();
                this->pwmErrors->periodFileError = false;
                return true;
            }
        }

    }

    bool        BlackPWM::setSpaceRatioTime(uint64_t space, timeType tType)
    {
        uint64_t writeThis = static_cast<int64_t>(space * static_cast<double>(pow( 10, static_cast<int>(tType)+9) ));

        if( writeThis > 1000000000)
        {
            this->pwmErrors->outOfRange = true;
            return false;
        }
        else
        {
            std::ofstream dutyFile;
            dutyFile.open(this->dutyPath.c_str(),std::ios::out);
            if(dutyFile.fail())
            {
                dutyFile.close();
                this->pwmErrors->dutyFileError = true;
                return false;
            }
            else
            {
                dutyFile << writeThis;
                dutyFile.close();
                this->pwmErrors->dutyFileError = false;
                return true;
            }
        }
    }

    bool        BlackPWM::setLoadRatioTime(uint64_t load, timeType tType)
    {
        uint64_t writeThis = (this->getNumericPeriodValue() - static_cast<int64_t>(load * static_cast<double>(pow( 10, static_cast<int>(tType)+9) )));

        if( writeThis > 1000000000)
        {
            this->pwmErrors->outOfRange = true;
            return false;
        }
        else
        {
            std::ofstream dutyFile;
            dutyFile.open(this->dutyPath.c_str(),std::ios::out);
            if(dutyFile.fail())
            {
                dutyFile.close();
                this->pwmErrors->dutyFileError = true;
                return false;
            }
            else
            {
                dutyFile << writeThis;
                dutyFile.close();
                this->pwmErrors->dutyFileError = false;
                return true;
            }
        }
    }

    bool        BlackPWM::setPolarity(polarityType polarity)
    {
        std::ofstream polarityFile;
        polarityFile.open(this->polarityPath.c_str(),std::ios::out);
        if(polarityFile.fail())
        {
            polarityFile.close();
            this->pwmErrors->polarityFileError = true;
            return false;
        }
        else
        {
            polarityFile << static_cast<int>(polarity);
            polarityFile.close();
            this->pwmErrors->polarityFileError = false;
            return true;
        }
    }

    bool        BlackPWM::setRunState(runValue state)
    {
        std::ofstream runFile;
        runFile.open(this->runPath.c_str(),std::ios::out);
        if(runFile.fail())
        {
            runFile.close();
            this->pwmErrors->runFileError = true;
            return false;
        }
        else
        {
            runFile << static_cast<int>(state);
            runFile.close();
            this->pwmErrors->runFileError = false;
            return true;
        }
    }



    bool        BlackPWM::isRunning()
    {
        return (this->getRunValue() == "1");
    }

    bool        BlackPWM::isPolarityStraight()
    {
        return !(this->getPolarityValue() == "1");
    }

    bool        BlackPWM::isPolarityReverse()
    {
        return (this->getPolarityValue() == "1");
    }



    void        BlackPWM::toggleRunState()
    {
        if( this->getRunValue() == "1" )
        {
            this->setRunState(stop);
        }
        else
        {
            this->setRunState(run);
        }
    }

    void        BlackPWM::tooglePolarity()
    {
        if( this->getPolarityValue() == "0" )
        {
            this->setPolarity(reverse);
        }
        else
        {
            this->setPolarity(straight);
        }
    }

    bool        BlackPWM::fail()
    {
        return (this->pwmErrors->outOfRange or
                this->pwmErrors->runFileError or
                this->pwmErrors->dutyFileError or
                this->pwmErrors->periodFileError or
                this->pwmErrors->polarityFileError or
                this->pwmErrors->pwmCoreErrors->dtError or
                this->pwmErrors->pwmCoreErrors->dtSsError or
                this->pwmErrors->pwmCoreErrors->pwmTestError or
                this->pwmErrors->pwmCoreErrors->coreErrors->ocpError or
                this->pwmErrors->pwmCoreErrors->coreErrors->capeMgrError
                );
    }

    bool        BlackPWM::fail(BlackPWM::flags f)
    {
        if(f==outOfRangeErr)    { return this->pwmErrors->outOfRange;                               }
        if(f==runFileErr)       { return this->pwmErrors->runFileError;                             }
        if(f==dutyFileErr)      { return this->pwmErrors->dutyFileError;                            }
        if(f==periodFileErr)    { return this->pwmErrors->periodFileError;                          }
        if(f==polarityFileErr)  { return this->pwmErrors->polarityFileError;                        }
        if(f==dtErr)            { return this->pwmErrors->pwmCoreErrors->dtError;                   }
        if(f==dtSubSystemErr)   { return this->pwmErrors->pwmCoreErrors->dtSsError;                 }
        if(f==pwmTestErr)       { return this->pwmErrors->pwmCoreErrors->pwmTestError;              }
        if(f==ocpErr)           { return this->pwmErrors->pwmCoreErrors->coreErrors->ocpError;      }
        if(f==cpmgrErr)         { return this->pwmErrors->pwmCoreErrors->coreErrors->capeMgrError;  }

        return true;
    }

    // ########################################### BLACKPWM DEFINITION STARTS ############################################ //



} /* namespace BlackLib */

#endif /* BLACKPWM_H_ */
