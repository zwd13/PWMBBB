#ifndef BLACKCORE_H_
#define BLACKCORE_H_


#include "BlackErr.h"       // Black Errors header file
#include "BlackDef.h"       // Black Definitions header file

#include <cstring>
#include <string>
#include <sstream>          // need for tostr() function
#include <cstdio>           // need for popen() function in BlackCore::executeShellCmd()
#include <dirent.h>         // need for dirent struct in BlackCore::searchDirectory()

namespace BlackLib
{

    /*! @brief Uses for converting the different types variable to string type.
    *
    * @tparam T variable which will convert.
    * @return String type value of T type variable.
    */
    template <typename T>
    inline std::string tostr(const T& t)
    {
        std::ostringstream os;
        os << t;
        return os.str();
    }





    // ########################################### BLACKCORE DECLARATION STARTS ########################################### //

    /*! @brief Base class of the other classes.
     *
     *    This class is core of the @b BlackLib Library.
     *    It includes private functions like communicating with operating system and their limited versions for
     *    using from derived class.
     */
    class BlackCore
    {
        private:
            errorCore       *coreErrors;            /*!< @brief is used to hold the errors of BlackCore class */
            std::string     capeMgrName;            /*!< @brief is used to hold the capemgr name */
            std::string     ocpName;                /*!< @brief is used to hold the ocp name */
            std::string     slotsFilePath;          /*!< @brief is used to hold the slots file path */

            /*! @brief Finds full name of capemgr directory.
            *
            *  This function searches @b "/sys/devices/" directory,
            *  to find directory which starts with @b "bone_capemgr."
            *  @return True if successful, else false.
            *  @sa BlackCore::searchDirectory()
            */
            bool            findCapeMgrName();

            /*! @brief Finds full name of ocp directory.
            *
            *  This function searches @b "/sys/devices/" directory,
            *  to find directory which starts with @b "ocp."
            *  @return True if successful, else false.
            *  @sa BlackCore::searchDirectory()
            */
            bool            findOcpName();

            /*! @brief Executes system call.
            *
            *  This function executes system commands with using popen() function.
            *  This example executes "ls" command with argument "la" and saves
            *  output to returnValue variable. @n @n
            *  <b> string returnValue = executeCommand("ls -la"); </b>
            */
            std::string     executeCommand(std::string command);

            /*! @brief Searches specified directory to find specified file/directory.
            *
            *  @param[in] searchIn searching directory
            *  @param[in] searchThis search file/directory
            *  @return Full name of searching file/directory.
            */
            std::string     searchDirectory(std::string searchIn, std::string searchThis);

            /*! @brief First declaration of this function.
            */
            virtual bool    loadDeviceTree() = 0;



        protected:

            /*!
             * This enum is used for defining ADC, PWM and SPI device driver names.
             */
            enum ocpSearch  {   ADC_helper  =   0,         /*!< enumeration for @a adc device driver name for using at BlackCore::searchDirectoryOcp() function parameter*/
                                PWM_P8_13   =   1,         /*!< enumeration for @a P8_13 pwm device driver name */
                                PWM_P8_19   =   2,         /*!< enumeration for @a P8_19 pwm device driver name */
                                PWM_P9_14   =   3,         /*!< enumeration for @a P9_14 pwm device driver name */
                                PWM_P9_16   =   4,         /*!< enumeration for @a P9_16 pwm device driver name */
                                PWM_P9_21   =   5,         /*!< enumeration for @a P9_21 pwm device driver name */
                                PWM_P9_22   =   6,         /*!< enumeration for @a P9_22 pwm device driver name */
                                PWM_P9_42   =   7,         /*!< enumeration for @a P9_42 pwm device driver name */
                                SPI0        =   8,         /*!< enumeration for @a SPI0 spi device driver name */
                                SPI1        =   9          /*!< enumeration for @a SPI1 spi device driver name */
                            };

            /*! @brief Searches ocp directory to find specified file/directory.
            *
            * This function searches ocp directory only. It can be used from derived class.
            *  @param[in] searchThis takes BlackCore::ocpSearch type variable(enum)
            *  @return Full name of searching file/directory.
            */
            std::string     searchDirectoryOcp(BlackCore::ocpSearch searchThis);

            /*! @brief Exports errorCore struct to derived class.
            *
            *  @return errorCore struct pointer.
            */
            errorCore       *getErrorsFromCore();

            /*! @brief Exports capemgr name to derived class.
            *
            *  @return BlackCore::capeMgrName variable.
            */
            std::string     getCapeMgrName();

            /*! @brief Exports ocp name to derived class.
            *
            *  @return BlackCore::ocpName variable.
            */
            std::string     getOcpName();

            /*! @brief Exports slots file path to derived class.
            *
            *  @return BlackCore::slotsFilePath variable.
            */
            std::string     getSlotsFilePath();



        public:
            /*! @brief Constructor of BlackCore class.
            *
            * This function initializes errorCore struct and runs these functions:
            * @li findCapeMgrName()
            * @li findOcpName()
            */
            BlackCore();

            /*! @brief Destructor of BlackCore class.
            *
            * This function deletes errorCore struct pointer.
            */
            virtual ~BlackCore();

    };
    // ############################################ BLACKCORE DECLARATION ENDS ############################################ //


    // ########################################### BLACKCORE DEFINITION STARTS ########################################### //
    BlackCore::BlackCore()
    {
        this->coreErrors = new errorCore();

        this->findCapeMgrName();
        this->findOcpName();
        this->slotsFilePath = "/sys/devices/" + this->capeMgrName + "/slots";
    }

    BlackCore::~BlackCore()
    {
        delete this->coreErrors;
    }

    std::string BlackCore::executeCommand(std::string command)
    {
        FILE* pipe = popen(command.c_str(), "r");
        if ( pipe==NULL )
        {
            return "ERROR";
        }

        char buffer[128];
        std::string result = "";

        while( !feof(pipe) )
        {
            if( fgets(buffer, 128, pipe) != NULL )
            {
                result += buffer;
            }
        }

        pclose(pipe);
        return result;
    }

    std::string BlackCore::searchDirectory(std::string seachIn, std::string searchThis)
    {
        std::string str;
        DIR *path;
        dirent *entry;

        path = opendir(seachIn.c_str());
        if( path != NULL  )
        {
            while( (entry = readdir(path)) != NULL)
            {
                if( entry->d_name[0] == '.')
                {
                    continue;
                }

                str = entry->d_name;
                if(strstr(entry->d_name,searchThis.c_str()) != NULL )
                {
                    closedir(path);
                    return str;
                }
            }
        }
        closedir(path);

        return SEARCH_DIR_NOT_FOUND;
    }

    bool        BlackCore::findCapeMgrName()
    {
        std::string searchResult = this->searchDirectory("/sys/devices/","bone_capemgr.");

        if(searchResult == SEARCH_DIR_NOT_FOUND)
        {
            this->capeMgrName = "bone_capemgr." + DEFAULT_CAPE_MGR_NUMBER;
            this->coreErrors->capeMgrError = true;
            return false;
        }
        else
        {
            this->capeMgrName = searchResult;
            this->coreErrors->capeMgrError = false;
            return true;
        }
    }

    bool        BlackCore::findOcpName()
    {
        std::string searchResult = this->searchDirectory("/sys/devices/","ocp.");

        if(searchResult == SEARCH_DIR_NOT_FOUND)
        {
            this->ocpName = "ocp." + DEFAULT_OCP_NUMBER;
            this->coreErrors->ocpError = true;
            return false;
        }
        else
        {
            this->ocpName = searchResult;
            this->coreErrors->ocpError = false;
            return true;
        }
    }




    std::string BlackCore::searchDirectoryOcp(BlackCore::ocpSearch searchThis)
    {
        std::string searchResult;
        std::string searchPath = "/sys/devices/" + this->getOcpName() + "/";

        if( searchThis == this->SPI0 )
        {
            searchPath += DEFAULT_SPI0_PINMUX + ".spi/spi_master/";
        }
        else if( searchThis == this->SPI1 )
        {
            searchPath += DEFAULT_SPI1_PINMUX + ".spi/spi_master/";
        }


        switch(searchThis)
        {
            case ADC_helper:
            {
                searchResult = this->searchDirectory(searchPath,"helper.");
                break;
            }

            case PWM_P8_13:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P8_13.");
                break;
            }

            case PWM_P8_19:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P8_19.");
                break;
            }

            case PWM_P9_14:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P9_14.");
                break;
            }

            case PWM_P9_16:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P9_16.");
                break;
            }

            case PWM_P9_21:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P9_21.");
                break;
            }

            case PWM_P9_22:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P9_22.");
                break;
            }

            case PWM_P9_42:
            {
                searchResult = this->searchDirectory(searchPath,"pwm_test_P9_42.");
                break;
            }

            case SPI0:
            {
                searchResult = this->searchDirectory(searchPath,"spi");
                break;
            }

            case SPI1:
            {
                searchResult = this->searchDirectory(searchPath,"spi");
                break;
            }
        }


        return searchResult;
    }

    errorCore   *BlackCore::getErrorsFromCore()
    {
        return (this->coreErrors);
    }



    std::string BlackCore::getCapeMgrName()
    {
        return this->capeMgrName;
    }

    std::string BlackCore::getOcpName()
    {
        return this->ocpName;
    }

    std::string BlackCore::getSlotsFilePath()
    {
        return this->slotsFilePath;
    }

    // ############################################ BLACKCORE DEFINITION ENDS ############################################ //

} /* namespace BlackLib */

#endif /* BLACKCORE_H_ */
