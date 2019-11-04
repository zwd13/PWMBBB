#include "BlackPWM.h"
#include <string>
#include <iostream>

void example_PWM()
{

    float percent = 5.0;
    std::string currentDuty;
    std::string currentPeriod;
    std::string currentPercentValue;
    std::string currentPolarity;
    std::string currentRun;

    int64_t currentDuty_numeric;
    int64_t currentPeriod_numeric;
    float currentPercentValue_numeric;

    BlackLib::BlackPWM    pwmLed(BlackLib::EHRPWM2A);

    // if new period value is less than the current duty value, the new period value setting
    // operation couldn't execute. So firstly duty value is set to zero for safe steps.
    pwmLed.setDutyPercent(0.0);
    pwmLed.setPeriodTime(100000);
    sleep(1);

    pwmLed.setSpaceRatioTime(67, BlackLib::microsecond);

    std::cout << "DUTY after setting space time: \t\t" << pwmLed.getDutyValue() << std::endl;


    pwmLed.setPeriodTime(300000000, BlackLib::picosecond);

    std::cout << "PERIOD after setting period time: \t" << pwmLed.getPeriodValue() << std::endl;


    pwmLed.setLoadRatioTime(15000, BlackLib::nanosecond);

    std::cout << "DUTY after setting load time: \t\t" << pwmLed.getDutyValue() << std::endl;


    pwmLed.setDutyPercent(11.75);

    std::cout << "DUTY after setting percent: \t\t" << pwmLed.getDutyValue() << std::endl;


    currentDuty = pwmLed.getDutyValue();

    currentDuty_numeric = pwmLed.getNumericDutyValue();

    currentPeriod = pwmLed.getPeriodValue();

    currentPeriod_numeric = pwmLed.getNumericPeriodValue();


    currentPercentValue = pwmLed.getValue();

    currentPercentValue_numeric = pwmLed.getNumericValue();

    currentPolarity = pwmLed.getPolarityValue();

    currentRun = pwmLed.getRunValue();

    std::cout << "Duty_str: \t"     << currentDuty << std::endl;
    std::cout << "Duty_num: \t"     << currentDuty_numeric << std::endl;
    std::cout << "Period_str: \t"   << currentPeriod << std::endl;
    std::cout << "Period_num: \t"   << currentPeriod_numeric << std::endl;
    std::cout << "Percent_str: \t"  << currentPercentValue << std::endl;
    std::cout << "Percent_num: \t"  << currentPercentValue_numeric << std::endl;
    std::cout << "Polarity_str: \t" << currentPolarity << std::endl;
    std::cout << "Run_str: \t"      << currentRun << std::endl;


    while( ! pwmLed.fail(BlackLib::BlackPWM::outOfRangeErr) )
    {
        pwmLed.setDutyPercent(percent);

        std::cout << std::endl << "New percent value: " << percent << std::endl;

        percent += 25.0;
        usleep(500000);
    }

    std::cout << std::endl << "Percent value is out of range." << std::endl;


}
