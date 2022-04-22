//
// main.cpp
//
// This sample demonstrates the WLApi class.
//
//
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include "WLApi.h"
#include "Poco/Exception.h"

using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;
using Poco::Exception;

int main(int argc, char** argv)
{
    extern char *optarg;
    int opt;
    std::string nip = "";

    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << ": -n nip" << std::endl;
        return 1;
    }

    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            nip = std::string(optarg);
            break;
        }
    }
    try
    {
        WLApi api;
        std::cout << "Searching contractor NIP: " << nip << std::endl;
        api.getContractorByNIP(nip);
    }
    catch (Poco::Exception &err)
    {
        std::cout << "Program exception: [" << err.displayText() << "]" << std::endl;
    }
    return 0;
}
