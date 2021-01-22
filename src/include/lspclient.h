/***************************************************************************
                          lspclient.h  -  description
                             -------------------
    begin                : Wed Feb 20 2021
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


#ifndef LSPCLIENT_H
#define LSPCLIENT_H


#include <vector>
#include <string>

#include "picojson/picojson.h"

namespace highlight
{

class LSPClient
{

private:

    bool initialized;
    bool hoverRequests;
    bool semanticRequests;
    bool logRequests;

    std::string executable, workspace;
    std::string serverName, serverVersion;
    std::string triggerSyntax;


    std::vector<std::string> options;

    pid_t pid;
    int inpipefd[2];
    int outpipefd[2];
    float msgId;

    bool pipe_write_jsonrpc(const std::string &message);

    std::string pipe_read_jsonrpc();

    bool runSimpleAction(const std::string action);

    std::string getNestedString(picojson::value &json, const std::string &jpath);

public:
    /** Constructor */
    LSPClient();
    ~LSPClient();

    void setExecutable ( const std::string& exec );

    void setOptions (const std::vector<std::string>& o);

    void setWorkspace ( const std::string& ws );

    void setSyntax ( const std::string& syntax );


    bool init();

    bool runInitialize();

    bool runInitialized();

    bool runShutdown();

    bool runExit();


    bool isInitialized();

    bool supportsHoverRequests();

    bool supportsSemanticRequests();

    void setLogging(bool flag);

    std::string getServerName();
    std::string getServerVersion();
};

}

#endif
