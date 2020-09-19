#include "arguments.hpp"

uint8_t parseArguments(int argc, char* argv[], args_t* settings)
{
    uint8_t exit = 0;

    for (uint8_t i = 1; i < argc; i++)
    {
        std::string argument = std::string(argv[i]);

        if (argument == "--help")
        {
            exit = 1;
            std::cout << "Usage: CpuLogger [options]" << std::endl;
            std::cout << "Options: " << std::endl;
            std::cout << "  --help                      Display this information." << std::endl;
            std::cout << "  --version                   Display compiler version information." << std::endl;
            std::cout << "  -d=<duration>               Length of time to sample cpu frequency, in seconds." << std::endl;
            std::cout << "  -w=<wait_time>              Length of time to wait before sampling, in seconds." << std::endl;
        }
        else if (argument == "--version")
        {
            exit = 1;
            std::cout << "1.0.0.0" << std::endl;
        }
        else if (argument.substr(0, 3) == "-d=")
        {
            if ( ( argument.size() > 3 ) && CheckNumeric( argument.substr( 3, argument.size() ) ) )
            {
                uint64_t dur = std::stoi( argument.substr( 3, argument.size() ) );
                settings->durationTicks = dur / ((float)settings->evaluationInterval_us / 1000000);
            }
        }
        else if (argument.substr(0, 3) == "-w=")
        {
            if ( ( argument.size() > 3 ) && CheckNumeric( argument.substr( 3, argument.size() ) ) )
            {
                uint64_t wait = std::stoi( argument.substr( 3, argument.size() ) );
                settings->waitTicks = wait / ((float)settings->evaluationInterval_us / 1000000);
            }
        }
    }

    return exit;
}


bool CheckNumeric(std::string arg)
{
    bool numeric = true;
    for (uint8_t i = 0; i < arg.size(); i++)
    {
        if (arg.substr(i, i+1) < "0" || arg.substr(i, i+1) > "9")
        {
            numeric = false;
            break;
        }
    }

    return numeric;
}