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
#include "stringtools.h"


#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#if __linux__
#include <sys/prctl.h>
#endif

#endif

#include <sstream>
#include <iostream>
#include <fstream>

namespace highlight
{

LSPClient::LSPClient():
initialized(false),
hoverProvider(false),
semanticTokensProvider(false),
logRequests(false),
pid(0),
msgId(1.0),
lastErrorCode(0)
{
    serverName=serverVersion="?";
}

LSPClient::~LSPClient()
{
    if (initialized) {
        int status=0;
        kill(pid, SIGKILL); //send SIGKILL signal to the child process
        waitpid(pid, &status, 0);
    }
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

void LSPClient::setSyntax ( const std::string& syntax ){
    triggerSyntax = syntax;
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

#if __linux__
        //ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);
#endif

        //https://stackoverflow.com/questions/5797837/how-to-pass-a-vector-of-strings-to-execv
        std::vector<char*> commandVector;

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

    if (logRequests) {
        std::cerr << "LSP REQ:\n" << msg << "\n";
    }

    ssize_t w = write(outpipefd[1], msg.c_str(), msg.size());

    if ((long unsigned int)w!=msg.size()) {
        return false;
    }
    return true;
}

std::string LSPClient::pipe_read_jsonrpc(){
    char buf[2048] = {0};
    ssize_t r=0;
    std::string resultString;

    while ( (r=read(inpipefd[0], buf, sizeof buf)) > 0 ) {
        resultString.append(buf, r);
        if ((long unsigned int)r<sizeof buf) break;
    }

    std::string payLoad;

    if (logRequests) {
        std::cerr << "LSP RES:\n" << resultString << "\n";
    }

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
    picojson::object textDocument, documentSymbol, publishDiagnostics, semanticHighlightingCapabilities;
    picojson::value nullValue;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value("initialize");

    params["processId"] =  picojson::value((float)getpid());

    // TODO workspaceFolders
    if (workspace.empty()){
        params["rootUri"] =  picojson::value(nullValue);
    } else {
        params["rootUri"] =  picojson::value("file://" + workspace);
    }

    publishDiagnostics["relatedInformation"] = picojson::value(true);
    textDocument["publishDiagnostics"] = picojson::value(publishDiagnostics);

   // documentSymbol["hierarchicalDocumentSymbolSupport"] = picojson::value(true);
   // textDocument["documentSymbol"] = picojson::value(documentSymbol);


    // triggers
    /*
     * {"jsonrpc":"2.0","method":"textDocument/publishDiagnostics","params":{"diagnostics":[],"uri":"file:///home/andre/Projekte/c/re2.cpp","version":0}}Content-Length: 352
     *
     { "jsonrp*c":"2.0","method":"textDocument/semanticHighlighting","params":{"lines":[{"isInactive":false,"line":3,"tokens":"AAAAEAAFAA4AAAAXAAkADg=="},{"isInactive":false,"line":5,"tokens":"AAAABAAEAAM="},{"isInactive":false,"line":7,"tokens":"AAAAAAADAA4AAAAFAAYACAAAAAwABgAB"}],"textDocument":{"uri":"file:///home/andre/Projekte/c/re2.cpp","version":0}}}
     */

  //  semanticHighlightingCapabilities["semanticHighlighting"] = picojson::value(true);
  //  textDocument["semanticHighlightingCapabilities"] = picojson::value(semanticHighlightingCapabilities);

    capabilities["textDocument"] = picojson::value(textDocument);

    params["capabilities"] = picojson::value(capabilities);
    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);

    if (!checkErrorResponse(jsonResponse, err)) {
        return false;
    }

    if (   !jsonResponse.get("result").is<picojson::object>()
        && !jsonResponse.get("result").get("capabilities").is<picojson::object>()) {
        return false;
    }

    if (jsonResponse.get("result").get("serverInfo").is<picojson::object>()) {
        serverName= jsonResponse.get("result").get("serverInfo").get("name").get<std::string>();
        serverVersion= jsonResponse.get("result").get("serverInfo").get("version").get<std::string>();
    }

    hoverProvider = jsonResponse.get("result").get("capabilities").get("hoverProvider").get<bool>();
    //semanticRequests = jsonResponse.get("result").get("capabilities").get("hoverProvider").get<bool>()

    return true;
}

std::string LSPClient::runHover(const std::string &document, int character, int line){

    if (document.empty())
        return "";

    picojson::object request;
    picojson::object params;
    picojson::object position;
    picojson::object textDocument;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value("textDocument/hover");

    std::string uri("file://");
    uri.append(document);
    textDocument["uri"] = picojson::value(uri);

    position["character"] = picojson::value((double)character);
    position["line"] = picojson::value((double)line);

    params["textDocument"] = picojson::value(textDocument);
    params["position"] = picojson::value(position);

    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    bool writeRes=pipe_write_jsonrpc(serialized);

    if (!writeRes) {
        return "";
    }

    while (true) {

        std::string response = pipe_read_jsonrpc();

        picojson::value jsonResponse;
        std::string err = picojson::parse(jsonResponse, response);

        if (!checkErrorResponse(jsonResponse, err)) {
            return "";
        }

        if (skipUnsupportedNotifications(jsonResponse)) {
            continue;
        }

        if (   !jsonResponse.get("result").is<picojson::object>()
        //    || !jsonResponse.get("result").get("contents").is<picojson::object>()
        ) {
            return "";
        }

        // Apparantly each server sends the Hover data slightly different:
        // maybe configure this in the lsp.conf

        std::string hoverList;

        if (jsonResponse.get("result").get("contents").is<std::string>()){
            //rls
            hoverList = jsonResponse.get("result").get("contents").get<std::string>();
        }
        else if (jsonResponse.get("result").get("contents").is<picojson::array>()){

            picojson::array list = jsonResponse.get("result").get("contents").get<picojson::array>();
            for (picojson::array::iterator iter = list.begin(); iter != list.end(); ++iter) {
                if ((*iter).is<std::string>()){
                    // pyls
                    hoverList.append( (*iter).get<std::string>());
                }
                else if ((*iter).is<picojson::object>()){
                    //ccls
                    if ((*iter).get("value").is<std::string>())
                        hoverList = (*iter).get("value").get<std::string>();
                }
            }
        }
        else if (jsonResponse.get("result").get("contents").is<picojson::object>()){
            //clangd
            hoverList = jsonResponse.get("result").get("contents").get("value").get<std::string>();
        }

        return hoverList;
    }

    return "";
}

bool LSPClient::runDidOpen(const std::string &document, const string& syntax){

    if (document.empty() || syntax !=triggerSyntax)
        return false;

    std::ifstream t(document.c_str());
    std::stringstream buffer;
    buffer << t.rdbuf();

    picojson::object request;
    picojson::object params;
    picojson::object textDocument;

    request["jsonrpc"] = picojson::value("2.0");
    //request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value("textDocument/didOpen");

    std::string uri("file://");
    uri.append(document);
    textDocument["uri"] = picojson::value(uri);
    textDocument["languageId"] =  picojson::value(triggerSyntax);
    textDocument["text"] = picojson::value(buffer.str());
    textDocument["version"] =  picojson::value(0.0);

    params["textDocument"] = picojson::value(textDocument);

    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);
    return checkErrorResponse(jsonResponse, err);
}

bool LSPClient::runDidClose(const std::string &document, const string& syntax){

    if (document.empty() || syntax !=triggerSyntax)
        return false;

    picojson::object request;
    picojson::object params;
    picojson::object textDocument;

    request["jsonrpc"] = picojson::value("2.0");
    //request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value("textDocument/didClose");

    std::string uri("file://");
    uri.append(document);
    textDocument["uri"] = picojson::value(uri);

    params["textDocument"] = picojson::value(textDocument);

    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);
    return checkErrorResponse(jsonResponse, err);
}


bool LSPClient::runDocumentSymbol(const std::string &document){

    if (document.empty())
        return false;

    picojson::object request;
    picojson::object params;
    picojson::object textDocument;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value("textDocument/documentSymbol");

    std::string uri("file://");
    uri.append(document);
    textDocument["uri"] = picojson::value(uri);

    params["textDocument"] = picojson::value(textDocument);

    request["params"] =  picojson::value(params);

    std::string serialized = picojson::value(request).serialize();

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);
    return checkErrorResponse(jsonResponse, err);
}

bool LSPClient::checkErrorResponse(const picojson::value &json, const string& picoError){
    lastErrorCode = 0;
    lastErrorMessage.clear();
    if (! picoError.empty()) {
        lastErrorCode = 1;
        lastErrorMessage = picoError;
        return false;
    }

    if (! json.is<picojson::object>()) {
        lastErrorCode = 2;
        lastErrorMessage = "could not instatiate PicoJSON";
        return false;
    }

    if (json.get("error").is<picojson::object>()) {
        lastErrorCode = (int)json.get("error").get("code").get<double>();
        lastErrorMessage = json.get("error").get("message").get<std::string>();
        return false;
    }
    return true;
}


/*
 *  "{\"jsonrpc\":\"2.0\",\"method\":\"window/progress\",\"params\":{\"done\":null,\"id\":\"progress_2\",\"message\":null,\"percentage\":null,\"title\":\"Indexing\"}}"
 * katelspclientplugin: discarding notification "window/progress"
 *
 * {"jsonrpc":"2.0","method":"textDocument/publishDiagnostics","params":{"diagnostics":[],"uri":"file:///home/andre/Projekte/cpp/min.c","version":0}}
 *
 * {"jsonrpc":"2.0","method":"client/registerCapability","id":1,"params":{"registrations":[{"id":"rls-watch","method":"workspace/didChangeWatchedFiles","registerOptions":{"watchers":[{"globPattern":"/home/andre/Projekte/rust/hello_world/Cargo.lock"},{"globPattern":"/home/andre/Projekte/rust/hello_world/target","kind":4},{"globPattern":"/home/andre/Projekte/rust/hello_world/Cargo.toml"}]}}]}}
 *
 * */
bool LSPClient::skipUnsupportedNotifications(picojson::value &json){

    if (json.get("method").is<std::string>()) {
        std::string method = json.get("method").get<std::string>();

  /*      if ( ( method=="window/progress" || method=="textDocument/publishDiagnostics" || method=="client/registerCapability")) {
            std::cerr<<"\nSKIP "<< method <<"\n";
        }
*/
        return method=="window/progress" || method=="textDocument/publishDiagnostics" || method=="client/registerCapability";
    }

    return false;
}

bool LSPClient::runSimpleAction(const std::string action){
    picojson::object request;
    //picojson::value nullValue;
    picojson::object emptyObject;

    request["jsonrpc"] = picojson::value("2.0");
    request["id"] = picojson::value(msgId++);
    request["method"] = picojson::value(action);

    request["params"] =  picojson::value(emptyObject);

    std::string serialized = picojson::value(request).serialize();

    pipe_write_jsonrpc(serialized);

    std::string response = pipe_read_jsonrpc();

    picojson::value jsonResponse;
    std::string err = picojson::parse(jsonResponse, response);

   // if (skipUnsupportedNotifications(jsonResponse)) {
  //      return false;
  //  }

    return checkErrorResponse(jsonResponse, err);
}


bool LSPClient::runInitialized(){
    return runSimpleAction("initialized");
}

bool LSPClient::runShutdown(){
    return runSimpleAction("shutdown");
}

bool LSPClient::runExit(){
    return runSimpleAction("exit");
}


bool LSPClient::isInitialized(){
    return initialized;
}

bool LSPClient::isHoverProvider(){
    return hoverProvider;
}

bool LSPClient::isSemanticTokensProvider(){
    return semanticTokensProvider;
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

std::string LSPClient::getErrorMessage(){
    return lastErrorMessage;
}

int LSPClient::getErrorCode(){
    return lastErrorCode;
}

}

/*

 //documentHighlightProvider, see https://github.com/microsoft/python-language-server/pull/1767
 //referencesProvider,
 //foldingRangeProvider

 //ccls and cquery -> semantic highlighting

 https://code.visualstudio.com/api/language-extensions/programmatic-language-features
 https://code.visualstudio.com/api/language-extensions/syntax-highlight-guide
 https://code.visualstudio.com/api/language-extensions/semantic-highlight-guide

 publishDiagnostics:
 Your language server sends the textDocument/publishDiagnostics message to the language client. The message carries an array of diagnostic items for a resource URI.

 Note: The client does not ask the server for diagnostics. The server pushes the diagnostic information to the client.
 */
