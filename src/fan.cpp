#include "fan.hpp"


void GetFans( std::map<std::string, float>* fanInfo )
{
    std::string line;
    line.reserve(2000);

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
             *  and are prefixed by "fan" and suffixed by "_input" */
            std::string newPath = hwmonPath + "/hwmon" + hwmonStr;
            for ( const auto& hwmonFile : std::filesystem::directory_iterator( newPath ) )
            {
                std::string fanInputStr;
                for (uint16_t i = newPath.size(); i < hwmonFile.path().string().size(); i++)
                {
                    if ( ( hwmonFile.path().string()[i] >= '0' ) &&
                         ( hwmonFile.path().string()[i] <= '9' ) &&
                         ( hwmonFile.path().string().substr( newPath.size()-1 , i ).find( "fan" ) ) &&
                         ( hwmonFile.path().string().substr( i, hwmonFile.path().string().size() ).find( "_input" ) ) )
                    {
                        fanInputStr += hwmonFile.path().string()[i];
                    }
                }

                /** Read the fan speed from the files. */
                std::string fanPath = newPath + "/fan" + fanInputStr + "_input";
                std::ifstream fanFile( fanPath );

                if ( fanFile.is_open() )
                {
                    std::string fanLine;

                    if ( getline( fanFile, fanLine ) )
                    {
                        float fanSpeedInRPM = std::stof(fanLine);
                        fanInfo->insert( std::pair<std::string, float>( "Fan " + fanInputStr, fanSpeedInRPM ) );
                    }
                }
            }
        }
    }
}