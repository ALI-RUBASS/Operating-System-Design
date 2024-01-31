#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include<fstream>
#include<vector>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

using namespace std;

mutex mtx;

void *calculator(void *p) {
    system("gnome-terminal --disable-factory -- ./calc"); // Replace "calc" with the name of your calculator program executable
    return NULL;
}


void *calendar(void *p) {
    system("gnome-terminal --disable-factory -- ./calendar"); // Replace "calendar" with the name of your calendar program executable
    return NULL;
}

void dis() {
    DIR* dir = opendir("/proc");
    if (!dir) {
        cerr << "Failed to open directory /proc.\n";
        return;
    }

    struct dirent* ent;
    vector<pid_t> pids;
    while ((ent = readdir(dir)) != nullptr) {
        if (ent->d_type != DT_DIR) continue;
        const string pid_str = ent->d_name;
        if (pid_str.find_first_not_of("0123456789") != string::npos) continue;
        const pid_t pid = stoi(pid_str);
        char cmdline_path[64];
        sprintf(cmdline_path, "/proc/%d/cmdline", pid);
        FILE* cmdline_file = fopen(cmdline_path, "r");
        if (!cmdline_file) continue;
        char cmdline[1024];
        const size_t len = fread(cmdline, 1, sizeof(cmdline), cmdline_file);
        fclose(cmdline_file);
        if (len == 0) continue;
        cmdline[len] = '\0';
        if (strstr(cmdline, "./") == cmdline) {
            pids.push_back(pid);
            cout << pid << " " << cmdline << endl;
        }
    }
    closedir(dir);

    if (pids.empty()) {
        cout << "No processes found.\n";
        return;
    }

    int pid;
    cout << "Enter the PID of the process you want to terminate: ";
    cin >> pid;

    int ret = kill(pid, SIGKILL);
    if (ret == -1) {
        perror("Failed to send signal to process");
        return;
    }

    cout << "Signal sent to process.\n";
}

int main() {
    int choice;
    pthread_t calc_thread, calendar_thread;
    while (choice != -1) {
        cout << "Choose an option:\n";
        cout << "1. Calculator\n";
        cout << "2. Calendar\n";
        cout << "-1. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                {
                    pid_t pid = fork();
                    if (pid == 0) {
                        // Child process
                        pthread_create(&calc_thread, NULL, calculator, NULL);
                        //pthread_join(calc_thread, NULL);
                        exit(0);
                    } else if (pid > 0) {
                        // Parent process
                        wait(NULL);
                    } else {
                        cerr << "Error creating process.\n";
                    }
                    break;
                }
            case 2:
                {
                    pid_t pid = fork();
                    if (pid == 0) {
                        // Child process
                        pthread_create(&calendar_thread, NULL, calendar, NULL);
                       // pthread_join(calendar_thread, NULL);
                        exit(0);
                    } else if (pid > 0) {
                        // Parent process
                        wait(NULL);
                    } else {
                        cerr << "Error creating process.\n";
                    }
                    break;
                }
            case -1:
                break;
            default:
		dis();
                break;
        }
    }
 pthread_join(calc_thread, NULL);
                        pthread_join(calendar_thread, NULL);
return 0;
}
