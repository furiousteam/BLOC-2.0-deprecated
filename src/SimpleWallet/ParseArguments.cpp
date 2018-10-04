/*
Copyright (C) 2018, The TurtleCoin developers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////
#include <SimpleWallet/ParseArguments.h>
////////////////////////////////////////

#include <Logging/LoggerManager.h>

#include <algorithm>

#include "CryptoNoteConfig.h"

#include <iostream>

#include <iomanip>

#include <string>

#include "version.h"

/* Thanks to https://stackoverflow.com/users/85381/iain for this small command
   line parsing snippet! https://stackoverflow.com/a/868894/8737306 */
char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

Config parseArguments(int argc, char **argv)
{
    Config config;

    config.exit = false;
    config.walletGiven = false;
    config.passGiven = false;

    config.host = "127.0.0.1";
    config.port = CryptoNote::RPC_DEFAULT_PORT;

    config.walletFile = "";
    config.walletPass = "";

    config.rpcHost = "127.0.0.1";
    config.rpcPassword = "";
    config.rpcPort = 2053;
    config.legacySecurity = false;
    config.testnet = false;
    config.log_level = Logging::TRACE;
	config.walletCreate = false;

    if (cmdOptionExists(argv, argv+argc, "-h")
     || cmdOptionExists(argv, argv+argc, "--help"))
    {
        helpMessage();
        config.exit = true;
        return config;
    }

    if (cmdOptionExists(argv, argv+argc, "-v")
     || cmdOptionExists(argv, argv+argc, "--version"))
    {
        versionMessage();
        config.exit = true;
        return config;
    }

    if (cmdOptionExists(argv, argv+argc, "--testnet"))
    {
        config.testnet = true;
    }

    if (cmdOptionExists(argv, argv+argc, "--generate-new-wallet"))
    {
        char *wallet = getCmdOption(argv, argv+argc, "--generate-new-wallet");

        if (!wallet)
        {
            std::cout << "--generate-new-wallet was specified, but no wallet file "
                      << "was given!" << std::endl;

            helpMessage();
            config.exit = true;
            return config;
        }

        config.walletFile = std::string(wallet);
        config.walletGiven = true;
		config.walletCreate = true;
    }

    if (cmdOptionExists(argv, argv+argc, "--wallet-file"))
    {
		if(config.walletCreate)
		{
            std::cout << "--generate-new-wallet was specified, you cannot do both at the same time!" << std::endl;
            helpMessage();
            config.exit = true;
            return config;
        }

        char *wallet = getCmdOption(argv, argv+argc, "--wallet-file");

        if (!wallet)
        {
            std::cout << "--wallet-file was specified, but no wallet file "
                      << "was given!" << std::endl;

            helpMessage();
            config.exit = true;
            return config;
        }

        config.walletFile = std::string(wallet);
        config.walletGiven = true;
    }

    if (cmdOptionExists(argv, argv+argc, "--password"))
    {
        char *password = getCmdOption(argv, argv+argc, "--password");

        if (!password)
        {
            std::cout << "--password was specified, but no password was "
                      << "given!" << std::endl;

            helpMessage();
            config.exit = true;
            return config;
        }

        config.walletPass = std::string(password);
        config.passGiven = true;
    }

    if (cmdOptionExists(argv, argv+argc, "--remote-daemon"))
    {
        char *url = getCmdOption(argv, argv + argc, "--remote-daemon");

        /* No url following --remote-daemon */
        if (!url)
        {
            std::cout << "--remote-daemon was specified, but no daemon was "
                      << "given!" << std::endl;

            helpMessage();

            config.exit = true;
        }
        else
        {
            std::string urlString(url);

            /* Get the index of the ":" */
            size_t splitter = urlString.find_first_of(":");

            /* No ":" present */
            if (splitter == std::string::npos)
            {
                config.host = urlString;
            }
            else
            {
                /* Host is everything before ":" */
                config.host = urlString.substr(0, splitter);

                /* Port is everything after ":" */
                std::string port = urlString.substr(splitter + 1,   
                                                    std::string::npos);

                try
                {
                    config.port = std::stoi(port);
                }
                catch (const std::invalid_argument &)
                {
                    std::cout << "Failed to parse daemon port!" << std::endl;
                    config.exit = true;
                }
            }
        }
    }

    if (cmdOptionExists(argv, argv+argc, "--rpc-bind-ip")) {
        char *rpcHost = getCmdOption(argv, argv + argc, "--rpc-bind-ip");

        if (rpcHost) {
          config.rpcHost = rpcHost;
        }
    }

    if (cmdOptionExists(argv, argv+argc, "--rpc-bind-port")) {
        char *rpcPort = getCmdOption(argv, argv + argc, "--rpc-bind-port");

        try
        {
            config.rpcPort = std::stoi(rpcPort);
        }
        catch (const std::invalid_argument &)
        {
            std::cout << "Failed to parse daemon port!" << std::endl;
            config.exit = true;
        }
    }
    
	if (cmdOptionExists(argv, argv+argc, "--set_log")) {
        char *level = getCmdOption(argv, argv + argc, "--set_log");

        try
        {
            config.log_level = std::stoi(level);
        }
        catch (const std::invalid_argument &)
        {
            std::cout << "Failed to parse log level! Not an integer?" << std::endl;
            config.exit = true;
        }
        
        if(config.log_level > Logging::TRACE || config.log_level < 0)
		{
            std::cout << "Log level is not 0 to " << Logging::TRACE << std::endl;
            config.exit = true;
		}
    }

    if (cmdOptionExists(argv, argv+argc, "--rpc-password")) {
        char *rpcPassword = getCmdOption(argv, argv + argc, "--rpc-password");

        if (rpcPassword) {
          config.rpcPassword = rpcPassword;
        }
    }
    
    
    if (cmdOptionExists(argv, argv+argc, "--legacy-security")) {
        config.legacySecurity = true;
    }

    return config;
}

void versionMessage()
{
    std::cout << "BLOC v" << PROJECT_VERSION << " Simplewallet"
              << std::endl;
}

void helpMessage()
{
    versionMessage();

    std::cout << std::endl << "simplewallet [--help] [--version] "
              << "[--remote-daemon <url>] [--wallet-file <file>] "
              << "[--password <pass>] [--rpc-bind-ip <ipaddress>] [--rpc-bind-port <RPC Port>] [--rpc-password <RPC Password>]"
              << std::endl << std::endl
              << "Commands:" << std::endl << "  -h, " << std::left
              << std::setw(30) << "--help"
              << "Display this help message and exit"
              << std::endl << "  -v, " << std::left << std::setw(30)
              << "--version" << "Display the version information and exit"
              << std::endl << "      " << std::left << std::setw(30)
              << "--remote-daemon <url>" << "Connect to the remote daemon at "
              << "<url:port> using RPC Port"
              << std::endl << "      " << std::left << std::setw(30)
              << "--wallet-file <file>" << "Open the wallet <file>"
              << std::endl << "      " << std::left << std::setw(30)
              << "--generate-new-wallet <file>" << "Create new wallet <file>"
              << std::endl << "      " << std::left << std::setw(30)
              << "--password <pass>" << "Use the password <pass> to open the wallet"
              << std::endl << "      " << std::left << std::setw(30)
              << "--legacy-security" << "Enable legacy mode (no password for RPC). WARNING: INSECURE. USE ONLY AS A LAST RESORT"
              << std::endl << "      " << std::left << std::setw(30)
              << "--rpc-bind-ip" << "Specify ip to bind rpc server"
              << std::endl << "      " << std::left << std::setw(30)
              << "--rpc-bind-port" << "Starts wallet as rpc server for wallet operations, sets bind port for server"
              << std::endl << "      " << std::left << std::setw(30)
              << "--rpc-password" << "Specify the RPC password to connect to the wallet using RPC calls"
              << std::endl << "      " << std::left << std::setw(30)
              << "--set_log" << "Change current log level, <level> is a number 0-4"
              << std::endl << "      " << std::left << std::setw(30)
              << "--testnet" << "Used to deploy test nets. The daemon must be launched with --testnet flag"
              << std::endl << std::endl;
}
