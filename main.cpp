#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include "winsock.h"
#else
#include <netdb.h>
#include <arpa/inet.h>
#endif

using namespace std;
#ifdef _WIN32
string cmd_prefix="> ";
#else
string cmd_prefix="$ ";
#endif

class SL_Client {
    public: bool isConnecting;
    public: bool isConnected;
    public: bool isInitialized;
    public: bool isCLIInitialized;
    public: string currentUrl;
    public: FILE* sshConnection;
    public: bool isSSHConnected;
    unordered_map<string,unordered_map<string,string>>commands={
        {"connect", {
            {"desc", "initiates a SSH connection to provided hostname"},
            {"valid", "connect"},
            {"arguments", "[hostname] [port (opt)] <credentials (-cred)(opt)>"}
        }},
        {"config", {
            {"desc", "open the configuration menu"},
            {"valid", "cfg,config,configuration"},
            {"arguments", "None"}
        }},
        {"cinfo",{
            {"desc", "displays information about the current connection"},
            {"valid", "cinfo"},
            {"arguments", "None"}
        }},
        {"info", {
            {"desc", "displays information about the client"},
            {"valid", "info"},
            {"arguments", "None"}
        }},
        {"exit", {
            {"desc", "exits the CLI"},
            {"valid", "exit,quit,leave"},
            {"arguments", "None"}
        }}
    };
    public: string ip;
    public: int port;
    public: SL_Client() {
        this->isConnecting=false;
        this->isConnected=false;
        this->isInitialized=false;
        this->isCLIInitialized=false;
        this->currentUrl="";
        this->isSSHConnected=false;
    };
    public: void connect(string hostname,int port,string credentials){
        if(isConnected){
            this->print_err("Already connected to a server");
        };
        if(!isInitialized){
            this->print_err("Serverlink not initialized");
        };
        if (!isConnecting){
            this->isConnecting=true;
            this->print_info("Connecting to https://"+hostname+" on port "+to_string(port)+". Please wait...");
            this_thread::sleep_for(chrono::milliseconds(1000));
            if (this->hostname_resolves(hostname)){
                this->print_info("Connected to https://"+hostname+".");
                this->isConnecting=false;
                this->isConnected=true;
                this->ip=hostname;
                this->port=port;
                this->currentUrl="https://"+hostname+":"+to_string(port);
                this->initialize_cli(hostname);
            }else{
                this->print_err("Failed to connect to https://"+hostname+".\nHostname does not resolve.");
                this->isConnecting=false;
            };
        }else{
            this->print_err("Already connecting to a server");
        };
    };
    public: int initialize_ssh(string hostname,int port,string credentials){
        string usr=credentials.substr(0,credentials.find(":"));
        string pw=credentials.substr(credentials.find(":")+1,credentials.length());
        string precmd="ssh "+usr+"@"+hostname;
        const char* cmd=precmd.c_str();
        try{
            this->sshConnection=popen(cmd,"w");

        }catch(system_error& e){
            this->print_err("Failed to initialize SSH connection");
            return 1;
        };
        return 0;
    };
    public: void ssh_communicate(string command){
        if(!isConnected){
            this->print_err("Not connected to a server");
        }else if (this->isSSHConnected){
            fprintf(this->sshConnection,"%s",command.c_str());
        };
    };
    public: void ssh_close(){
        if(!isConnected){
            this->print_err("Not connected to a server");
        }else if(this->isSSHConnected){
            this->isSSHConnected=false;
            pclose(this->sshConnection);
            this->print_info("SSH connection closed");
        };
    };
    public: void initialize_cli(string hostname){
        if (!isCLIInitialized){
            this->print_info("Serverlink CLI initializing. Please wait...");
            this->isCLIInitialized=true;
            this_thread::sleep_for(chrono::milliseconds(100));
            this->print_info("<SHELL STARTING>");
            this_thread::sleep_for(chrono::milliseconds(900));
        };
    };
    public: void disconnect(){
        if(isConnected){
            this->print_info("Saving and disconnecting from https://"+this->ip+".");
            this_thread::sleep_for(chrono::milliseconds(1000));
            this->print_info("Disconnected from https://"+this->ip+".");
            this->isConnected=false;
        };
    };
    public: auto get_command(string cmd){
        string result="";
        for(auto i=this->commands.begin();i!=this->commands.end();i++){
            string cmd_name=i->first;
            if(i->second["valid"].find(cmd)!=string::npos){
                result=cmd_name;
                break;
            };
        };
        return result;
    };
    public: void initialize(){
        if(!isInitialized){
            this->print_info("Serverlink initializing.");
            this->isInitialized=true;
            this->isConnected=false;
            this->isConnecting=false;
            this->currentUrl="";
        }else{
            this->print_err("Serverlink already initialized.");
        };
    };
    public: bool hostname_resolves(const std::string& hostname) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            cerr << "WSAStartup failed." << endl;
            return false;
        };
        #endif
        struct addrinfo hints, *res;
        int status;
        memset(&hints,0,sizeof hints);
        hints.ai_family=AF_UNSPEC; // AF_INET or AF_INET6 to force version
        hints.ai_socktype=SOCK_STREAM;    
        if ((status=getaddrinfo(hostname.c_str(), NULL, &hints, &res))!=0){
            #ifdef _WIN32
            this->print_err("Unable to get address info: "+WSAGetLastError());
            #else
            cerr << "SL Error: Unable to get address info: " << gai_strerror(status) << endl;
            #endif
            #ifdef _WIN32
            WSACleanup();
            #endif
            return false;
        };
        freeaddrinfo(res); // free the linked list
        #ifdef _WIN32
        WSACleanup();
        #endif
        return true;
    };
    public: void run_cmd(string cmd, vector<string> args){
        if (cmd=="connect"){
            if(args.size()<1){
                this->print_err("No hostname provided, process exited.");
            }else if(args.size()<2){
                this->connect(args[0],22,"");
            }else if(args.size()<3){
                this->connect(args[0],stoi(args[1]),"");
            }else if (args.size()==3){
                if (args[2].find("-cred=")!=string::npos){
                    this->connect(args[0],stoi(args[1]),args[2].substr(6));
                }else{
                    this->print_err("Invalid credentials provided, process exited.");
                };
            }else{
                this->print_err("Too many arguments provided, process exited.");
            };
        };
    };
    public: void print_info(string output){
        cout << "SL: " << output << endl;
    };
    public: void print_err(string output){
        cout << "SL Error: " << output << endl;
    };
};

vector<string> split(const string &s) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (std::getline(tokenStream,token,' ')){
        tokens.push_back(token);
    };
    return tokens;
};

int main(int argc, char** argv){
    SL_Client slc=SL_Client();
    slc.initialize();
    bool magic_exit_code=false;
    if (argc<2){
        cout << "Welcome to Serverlink, what would you like to do?" << endl;
        cout << "Type 'help' for a list of commands" << endl;
        while(!magic_exit_code){
            string input;
            cout << cmd_prefix;
            getline(cin,input);
            vector<string> args=split(input);
            if(input==""){
                continue;
            }else if(args[0]=="help"){
                cout << "Commands:" << endl;
                for(auto i=slc.commands.begin();i!=slc.commands.end();i++){
                    cout << "  " << i->first << " - " << i->second["desc"] << endl;
                    cout << "Arguments: " << i->second["arguments"] << endl;
                };
            }else if(slc.get_command(args[0])!=""){
                string cmdname=slc.get_command(args[0]);
                if (cmdname=="exit"){
                    // Close the CLI if the user types exit
                    magic_exit_code=true;
                }else{
                    // Run the command
                    args.erase(args.begin());
                    slc.run_cmd(cmdname,args);
                };
            }else{
                cout << "Invalid command." << endl;
            };
        };
    }else if (argc>1){
        vector<string> args(argv,argv+argc);
        args.erase(args.begin());
        string cmd=args[0];
        if(cmd=="help"){
            cout << "Commands:" << endl;
            for(auto i=slc.commands.begin();i!=slc.commands.end();i++){
                cout << "  " << i->first << " - " << i->second["desc"] << endl;
                cout << "Arguments: " << i->second["arguments"] << endl;
            };
        }else if(slc.get_command(cmd)!=""){
            string cmdname=slc.get_command(cmd);
            // Run the command
            args.erase(args.begin());
            slc.run_cmd(cmdname,args);
        }else{
            cout << "Invalid command." << endl;
        };
    };
    return 0;
};