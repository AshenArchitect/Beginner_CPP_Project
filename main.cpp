#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>

// include the required functions for use in the program
#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
#include <windows.h>
#include <lmcons.h>
#else
#include <stdint.h>
#endif


// function prototypes
void slow_display(std::string input_string, int time_delay = 7);
void get_cpu_info(std::string& cpu_info);
void get_username(std::string& username);
void layout_information();

int main (){
    // Display the information gotten from the computer
    layout_information();

    return std::cin.get();
}


// function definitions
void slow_display(std::string input_string, int time_delay){
    /** This function will slowly display whatever is in the input_string*/
    for(auto counter = 0; counter < input_string.length(); counter++){
        std::cout << input_string.at(counter);
        auto timer = std::chrono::milliseconds(time_delay);
        std::this_thread::sleep_for(timer);
    }
}

void get_cpu_info(std::string& cpu_info){
    /** This function will obtain the CPU Information and return it as 
     * cpu_info*/
        cpu_info = "";
        uint32_t regs[4];
        int32_t EAX_VALUE = 0x80000002;

        // First part of CPU Information
        asm volatile ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
       : "a" (EAX_VALUE), "c" (0));

        cpu_info = std::string((const char *)&regs[0], 4) + 
                    std::string((const char *)&regs[1], 4) + 
                    std::string((const char *)&regs[3], 4) +
                    std::string((const char *)&regs[2], 4);

        // Second Part of CPU Information
        EAX_VALUE = 0x80000003;

        asm volatile ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
       : "a" (EAX_VALUE), "c" (0));

        cpu_info += std::string((const char *)&regs[0], 4) + 
                    std::string((const char *)&regs[1], 4) + 
                    std::string((const char *)&regs[3], 4) +
                    std::string((const char *)&regs[2], 4);

        // Third Part of CPU Information
        EAX_VALUE = 0x80000004;

        asm volatile ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
       : "a" (EAX_VALUE), "c" (0));

        cpu_info += std::string((const char *)&regs[0], 4) + 
                    std::string((const char *)&regs[1], 4) + 
                    std::string((const char *)&regs[3], 4) +
                    std::string((const char *)&regs[2], 4);

}

void get_username(std::string& username){
    /** This function will obtain the username stored on the OS is some form
     *  and return it for use.
    */

    #if defined(_WIN32) || defined(_WIN64)
        TCHAR name [UNLEN + 1];
        DWORD size = UNLEN + 1;
        GetUserName((TCHAR*)name, &size);
        username = name;
    #elif unix
        char user_name_array[32];
        cuserid(user_name_array);
        username = user_name_array;
    #else
        username = "Unknown";
        username += " (You're on Mac?) ";
    #endif
}

void layout_information() {
    std::string username = "";
    std::string cpu_info = "";

    get_username(username);
    get_cpu_info(cpu_info);

    slow_display("\n");
    slow_display(">>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<\n", 7);
    slow_display("Username: " + username + "\n", 12);
    slow_display("CPU Info: " + cpu_info + "\n", 12);
    slow_display(">>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<\n", 7);
    slow_display("\n");
}