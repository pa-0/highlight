/***************************************************************************
                          lspclient.cpp  -  description
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

#include "lspclient.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <signal.h>
#endif

#include <sstream>
#include <iostream>

namespace highlight
{

LSPClient::LSPClient():
initialized(false),
hoverRequests(false),
semanticRequests(false),
logRequests(false),
pid(0)
{}

LSPClient::~LSPClient()
{
    int status=0;
    kill(pid, SIGKILL); //send SIGKILL signal to the child process
    waitpid(pid, &status, 0);
}

void LSPClient::setExecutable ( const std::string& exec ) {
    executable = exec;
}

void LSPClient::setOptions (const std::vector<std::string>& o){
    options = o;
}

void LSPClient::setWorkspace ( const std::string& ws ){
    workspace = ws;
}

bool LSPClient::init() {

    initialized=true;

    pipe(inpipefd);
    pipe(outpipefd);

    pid = fork();

    if (pid == 0)
    {

        // Child
        dup2(outpipefd[0], STDIN_FILENO);
        dup2(inpipefd[1], STDOUT_FILENO);
        dup2(inpipefd[1], STDERR_FILENO);

        //ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        //https://stackoverflow.com/questions/5797837/how-to-pass-a-vector-of-strings-to-execv
        std::vector<char*> commandVector;

        // do a push_back for the command, then each of the arguments


        commandVector.push_back(const_cast<char*>(executable.c_str()));

        for (auto& option : options) {
            commandVector.push_back(const_cast<char*>(option.c_str()));
        }

        commandVector.push_back(NULL);

        // pass the vector's internal array to execvp
        char **command = commandVector.data();

        execvp(executable.c_str(), command);


        // Nothing below this line should be executed by child process. If so,
        // it means that the execl function wasn't successfull, so lets exit:

        exit(1);
    }
    // The code below will be executed only by parent. You can write and read
    // from the child using pipefd descriptors, and you can send signals to
    // the process using its pid by kill() function. If the child process will
    // exit unexpectedly, the parent process will obtain SIGCHLD signal that
    // can be handled (e.g. you can respawn the child process).

    //close unused pipe ends
    close(outpipefd[0]);
    close(inpipefd[1]);
    return true;
}

bool LSPClient::pipe_write_jsonrpc(const std::string &payload){
    std::ostringstream os;

    os << "Content-Length: " << payload.size() << "\r\n\r\n" <<payload;

    std::string msg (os.str());

    ssize_t w = write(outpipefd[1], msg.c_str(), msg.size());

    if ((long unsigned int)w!=msg.size()) {
        return false;
    }
    return true;
}

std::string LSPClient::pipe_read_jsonrpc(){
    char buf[256] = {0};
    ssize_t r=0;
    std::string resultString;

    while ( (r=read(inpipefd[0], buf, sizeof buf)) > 0 ) {
        resultString.append(buf, r);
        if ((long unsigned int)r<sizeof buf) break;
    }

    std::string payLoad;

    if (resultString.find("Content-Length:")==0) {
        std::string payloadLenString = resultString.substr(16, resultString.find("\r\n")-16);
        unsigned int payloadLen = atoi(payloadLenString.c_str());

        if (payloadLen<resultString.size()) {
            payLoad = resultString.substr(resultString.size() - payloadLen);
        }
    }

    return payLoad;
}


bool LSPClient::runInitialize(){

    picojson::object request;
    picojson::object params;
    picojson::object capabilities;
    picojson::value nullValue;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(1.0);
    request["method"] = picojson::value("initialize");

    params["processId"] =  picojson::value((float)getpid());

    // TODO workspaceFolders
    if (workspace.empty()){
        params["rootUri"] =  picojson::value(nullValue);
    } else {
        params["rootUri"] =  picojson::value("file://" + workspace);
    }

    params["capabilities"] = picojson::value(capabilities);
    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    if (logRequests) {
        std::cerr<<"LSP REQ:\n"<< serialized<<"\n";
    }

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    if (logRequests) {
        std::cerr<<"LSP RSP:\n"<< response<<"\n";
    }


    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);

    if (! err.empty()) {
        return false;
    }

    if (! jsonResponse.is<picojson::object>()) {
        return false;
    }

    hoverRequests = jsonResponse.get("result").get("capabilities").get("hoverProvider").get<bool>();

    serverName= jsonResponse.get("result").get("serverInfo").get("name").get<std::string>();
    serverVersion= jsonResponse.get("result").get("serverInfo").get("version").get<std::string>();

    //semanticRequests = jsonResponse.get("result").get("capabilities").get("hoverProvider").get<bool>()


    return response.size();
}

bool LSPClient::runShutdown(){
    picojson::object request;
    picojson::value nullValue;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(1.0);
    request["method"] = picojson::value("shutdown");

    request["params"] =  nullValue;

    std::string serialized = picojson::value(request).serialize();

    if (logRequests) {
        std::cerr<<"LSP REQ:\n"<< serialized<<"\n";
    }

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    if (logRequests) {
        std::cerr<<"LSP RSP:\n"<< response<<"\n";
    }

    return true;
}

bool LSPClient::runExit(){
    picojson::object request;
    picojson::value nullValue;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(1.0);
    request["method"] = picojson::value("exit");

    request["params"] =  nullValue;

    std::string serialized = picojson::value(request).serialize();

    if (logRequests) {
        std::cerr<<"LSP REQ:\n"<< serialized<<"\n";
    }

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    if (logRequests) {
        std::cerr<<"LSP RSP:\n"<< response<<"\n";
    }

    return true;
}


bool LSPClient::isInitialized(){
    return initialized;
}

bool LSPClient::supportsHoverRequests(){
    return hoverRequests;
}

bool LSPClient::supportsSemanticRequests(){
    return semanticRequests;
}

void LSPClient::setLogging(bool flag){
    logRequests = flag;
}

std::string LSPClient::getServerName(){
    return serverName;
}
std::string LSPClient::getServerVersion(){
    return serverVersion;
}



}
