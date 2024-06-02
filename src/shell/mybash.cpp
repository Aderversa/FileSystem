//
// Created by 24576 on 2024/5/31.
//
#include <sfs/FileSystemManager.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#define streq(a, b) (strcmp((a), (b)) == 0)

void do_mkdir(FileSystemManager& fsm, char* arg);
void do_cd(FileSystemManager& fsm, char* arg);
void do_pwd(FileSystemManager& fsm, char* arg);
void do_touch(FileSystemManager& fsm, char* arg);
void do_rm(FileSystemManager& fsm, char* arg);
void do_cat(FileSystemManager& fsm, char* arg);
void do_ls(FileSystemManager& fsm, char* arg);
void do_vim(FileSystemManager& fsm, char* arg);
void do_exec(FileSystemManager& fsm, char* arg);
void do_register(FileSystemManager& fsm, char* arg);
void do_unregister(FileSystemManager& fsm, char* arg);
void do_login(FileSystemManager& fsm, char* arg);
void do_help(FileSystemManager& fsm, char* arg);

int main() {
    std::string diskname("../data/image.20");
    FileSystemManager file_system_manager(diskname, 20);
    while(true) {
        char line[BUFSIZ];
        char cmd[BUFSIZ];
        char arg[BUFSIZ];
        fprintf(stderr, "$ ");
        fflush(stderr);
        if (fgets(line, BUFSIZ, stdin) == NULL) {
            break;
        }
        int args = sscanf(line, "%s %s", cmd, arg);
        if (arg == 0) {
            continue;
        }
        if (streq(cmd, "mkdir")) {
            do_mkdir(file_system_manager, arg);
        }
        else if (streq(cmd, "cd")) {
            do_cd(file_system_manager, arg);
        }
        else if (streq(cmd, "pwd")) {
            do_pwd(file_system_manager, arg);
        }
        else if (streq(cmd, "touch")) {
            do_touch(file_system_manager, arg);
        }
        else if (streq(cmd, "rm")) {
            do_rm(file_system_manager, arg);
        }
        else if (streq(cmd, "cat")) {
            do_cat(file_system_manager, arg);
        }
        else if (streq(cmd, "ls")) {
            do_ls(file_system_manager, arg);
        }
        else if (streq(cmd, "vim")) {
            do_vim(file_system_manager, arg);
        }
        else if (streq(cmd, "exec")) {
            do_exec(file_system_manager, arg);
        }
        else if (streq(cmd, "register")) {
            do_register(file_system_manager, arg);
        }
        else if (streq(cmd, "unregister")) {
            do_unregister(file_system_manager, arg);
        }
        else if (streq(cmd, "login")) {
            do_login(file_system_manager, arg);
        }
        else if (streq(cmd, "help")) {
            do_help(file_system_manager, arg);
        }
        else if (streq(cmd, "exit")) {
            break;
        }
        else {
            std::cout << "Unknown command: " << line;
            std::cout << "Type 'help' for a list of commands." << std::endl;
        }
    }
    return 0;
}

void do_mkdir(FileSystemManager &fsm, char *arg) {
    try {
        fsm.mkdir(arg);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void do_cd(FileSystemManager &fsm, char *arg) {
    try {
        fsm.cd(arg);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void do_pwd(FileSystemManager &fsm, char *arg) {
    try {
        fsm.pwd();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void do_touch(FileSystemManager &fsm, char *arg) {

}

void do_rm(FileSystemManager &fsm, char *arg) {

}

void do_cat(FileSystemManager &fsm, char *arg) {

}

void do_ls(FileSystemManager &fsm, char *arg) {
    try {
        fsm.ls();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void do_vim(FileSystemManager &fsm, char *arg) {

}

void do_exec(FileSystemManager &fsm, char *arg) {

}

void do_register(FileSystemManager &fsm, char *arg) {
    try {
        fsm.registerUser(arg);
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void do_unregister(FileSystemManager &fsm, char *arg) {
    try {
        fsm.unregisterUser(arg);
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void do_login(FileSystemManager &fsm, char *arg) {
    try {
        fsm.login(arg);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void do_help(FileSystemManager &fsm, char *arg) {
    using std::cout;
    using std::endl;
    cout << "mkdir <filename>       --- create a new directory in workspace" << endl;
    cout << "cd    <filename>       --- change workspace to <filename>" << endl;
    cout << "pwd                    --- print the path of the workspace" << endl;
    cout << "touch <filename>       --- create a normal file in workspace" << endl;
    cout << "ls                     --- print all files of the workspace" << endl;
    cout << "vim   <filename>       --- edit <filename>" << endl;
    cout << "exec  <filename>       --- execute <filename> if it has permission" << endl;
    cout << "register <username>    --- register a new user" << endl;
    cout << "unregister <username>  --- unregister a user if existing" << endl;
    cout << "login <username>       --- change workspace to /home/<username>" << endl;
    cout << "help                   --- print the usage of commands" << endl;
    cout << "exit                   --- exit the program" << endl;
}














