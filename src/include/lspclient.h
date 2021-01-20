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
    bool supportsHover;
    bool supportsSemanticHighlighting;

    std::string executable, workspace;

    std::vector<std::string> options;

    pid_t pid;
    int inpipefd[2];
    int outpipefd[2];

    bool pipe_write(const std::string &message);

    std::string pipe_read();

public:
    /** Constructor */
    LSPClient();
    ~LSPClient();

    void setExecutable ( const std::string& exec );

    void setOptions (const std::vector<std::string>& o);

    void setWorkspace ( const std::string& ws );

    bool init();

    bool runInitialize();

};

}

#endif
