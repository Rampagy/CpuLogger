#include "temperature.hpp"

void GetTemperatures( std::map<std::string, float>* temperatureInfo )
{
    std::string hwmonPath = "/sys/class/hwmon";

    /** Get all of the files/directories within hwmonPath. */
    for ( const auto& entry : std::filesystem::directory_iterator( hwmonPath ) )
    {
        /** Search through the files/directories within entry that contain a number prefixed by "hwmon" */
        std::string hwmonStr;
        for (uint16_t i = hwmonPath.size()-1; i < entry.path().string().size(); i++)
        {
            if ( ( entry.path().string()[i] >= '0' ) &&
                 ( entry.path().string()[i] <= '9' ) &&
                 ( entry.path().string().substr( hwmonPath.size()-1 , i ).find( "hwmon" ) ) )
            {
                hwmonStr += entry.path().string()[i];
            }
        }

        if ( hwmonStr.size() > 0 )
        {
            /** Of the directories found that contained a number and are prefixed by "hwmon" search
             *  through their files/directories searching for files names that contain a number
             *  and are prefixed by "temp" and suffixed by "_input" */
            std::string newPath = hwmonPath + "/hwmon" + hwmonStr;
            for ( const auto& hwmonFile : std::filesystem::directory_iterator( newPath ) )
            {
                std::string temperatureInputStr;
                for (uint16_t i = newPath.size(); i < hwmonFile.path().string().size(); i++)
                {
                    if ( ( hwmonFile.path().string()[i] >= '0' ) &&
                         ( hwmonFile.path().string()[i] <= '9' ) &&
                         ( hwmonFile.path().string().substr( newPath.size()-1 , i ).find( "temp" ) ) &&
                         ( hwmonFile.path().string().substr( i, hwmonFile.path().string().size() ).find( "_input" ) ) )
                    {
                        temperatureInputStr += hwmonFile.path().string()[i];
                    }
                }

                /** Read the temperature and temperature name from the files. */
                std::string temperaturePath = newPath + "/temp" + temperatureInputStr + "_input";
                std::string temperatureNamePath = newPath + "/temp" + temperatureInputStr + "_label";

                std::ifstream temperatureFile( temperaturePath );
                std::ifstream temperatureNameFile( temperatureNamePath );

                if ( temperatureFile.is_open() && temperatureNameFile.is_open() )
                {
                    std::string temperatureLine;
                    std::string temperatureNameLine;

                    if ( getline( temperatureFile, temperatureLine ) && getline( temperatureNameFile, temperatureNameLine ) )
                    {
                        float temperatureInC = std::stof(temperatureLine) / 1000;
                        temperatureInfo->insert( std::pair<std::string, float>( temperatureNameLine, temperatureInC ) );
                    }
                }
            }
        }
    }
}