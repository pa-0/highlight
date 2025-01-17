/***************************************************************************
                          main.ch  -  description
                             -------------------
    begin                : Die Apr 23 22:16:35 CEST 2002
    copyright            : (C) 2002-2021 by Andre Simon
    email                : a.simon@mailbox.org

 ***************************************************************************/

/*
This file is part of Highlight.

Highlight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Highlight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Highlight.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef HIGHLIGHT_APP
#define HIGHLIGHT_APP


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cassert>

#include "cmdlineoptions.h"
#include "codegenerator.h"
#include "help.h"
#include "datadir.h"
#include "version.h"
#include "platform_fs.h"

#define IO_ERROR_REPORT_LENGTH 5


/// Main application class of the command line interface

class HLCmdLineApp
{

public:

    HLCmdLineApp() {};
    ~HLCmdLineApp() {};

    /** Start application
      \param argc Number of command line arguments
      \param argv values of command line arguments
      \return EXIT_SUCCESS or EXIT_FAILURE
    */
    int run ( const int argc, const char *argv[] );

private:

    DataDir dataDir;

    bool serviceModeCheck(CmdLineOptions &options, highlight::CodeGenerator * generator, std::string &suffix, unsigned int &curFileIndex);
    std::string service_mode_tag;


    /** print version info*/
    void printVersionInfo(bool quietMode=false);

    /** print configuration info*/
    void printConfigInfo ();

    /** print error message*/
    void printBadInstallationInfo();

    /** print input and output errors */
    void printIOErrorReport ( unsigned int numberErrorFiles, std::vector<std::string> & fileList,
                              const std::string &action, const std::string &streamName );

    /** list installed  files
        \return true if files were found
    */
    int printInstalledFiles(const std::string& where, const std::string& wildcard, const std::string& kind,
                            const std::string&option, const std::string& categoryFilterList="");

    /** print debug information
        \param lang language definition
        \param langDefPath path to language definition
        \param level verbosity level

    */
    void printDebugInfo ( const highlight::SyntaxReader *lang,
                          const std::string &langDefPath, int level );

    void printPersistentStates ( const highlight::SyntaxReader *lang);

    int getNumDigits ( int i );

    void printProgressBar ( int total, int count );
    void printCurrentAction ( const std::string&outfilePath,
                              int total, int count, int countWidth );

    bool readInputFilePaths ( std::vector<std::string> &fileList, std::string wildcard,
                              bool recursiveSearch );

    void printInstalledFiles();

    std::vector <std::string> collectPluginPaths(const std::vector<std::string>& plugins);
};

#endif
