#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define flag "\x5d\x59\x25\x0c\x04\x16\x4f\x16\x08\x66\x51\x1b\x6e\x0d\x31\x41\x1b\x2b\x64\x52\x1e\x53\x68\x42\x18\x20"

static char *rand_string(char *str, int size) {
    if (size) {
        --size;
        for (int n = 0; n < size; n++) {
            str[n] =  rand() %  (122 - 33 + 1) + 33;
        }
        str[size] = '\0';
    }
    return str;
}

// SECRET XOR FUNCTION
char* encryption(char* data, char* key, int dataLen, int keyLen) {
	char* output = (char*)malloc(sizeof(char) * dataLen);
	for (int i = 0; i < dataLen; ++i) {
		output[i] = data[i] ^ key[i % keyLen];
	}
	return output;
}

int main() {
    char* ascii_art =
    "                                                    @@@@@@@@@@@@@      \n"
    "                                               *@@@@@@@@@@@@@@@@@@@@   \n"
    "   __________  ____ _       _______         .@@@@.@.@@@@@@@@@@@@@@@@@  \n"
    "  / ____/ __ \\/ __ \\ |     / /__  /        @@@@@../....,@@@@@@@@@@@@@@ \n"
    " / /   / /_/ / / / / | /| / / /_ <       @@@@@@@..........@@@@@@@@@@/  \n"
    "/ /___/ _, _/ /_/ /| |/ |/ /___/ /      @@@@@@@@@@.........@^@#@@@@@@  \n"
    "\\____/_/ |_|\\____/ |__/|__//____/      @@@@....................@@@@    \n"
    "                                      @@@@.......@@@@@@@@@@@@@@@@      \n"
    "                                      @@@@@@@@@@@@@@@...\033[31mINC.\033[0m@@@@       \n"
    "                                       @@@@@@@@@@@@@@@@@@@@@           \n"
    "                                         /@@@@@@@@@@@@@&               \n";

    char* text = 
    "       =[ \033[33m[REDACTED] v3.6.3-release [core:3.3 api:1.0]\033[0m\n"
    "+ -- --=[ 481 exploits  -  310 generations\n"
    "+ -- --=[ 176 flags - 21 encoders - 8 nops\n"
    "       =[ svn r7957 updated \033[31m298 days ago\033[0m (2023.12.23)\n"
    "\n"
    "WARNING: This copy of the [REDACTED] Framework was last updated \033[31m298 days ago\033[0m.\n"
    "	 We recommend that you update the framework at least every other day.\n"
    "	 For information on updating your copy of [[REDACTED]]. please see:\n"
    "		\033[33mhttps://github.com/[REDACTED]\033[0m \n"
    "\n"
    "|---------------------------------|\n"
    "> The Elite generator for flag cheating\n";
    char sec[49] = "> Give us an input and we will give you the flag";

    printf("%s\n", ascii_art);
    printf("%s%s\n", text,sec);

    int keyLen = strlen(text);
    srand(time(NULL));
    char string[5], string2[5],string3[5];

    int count = 0;
    while(1){
        printf("c̲r̲3 >");
        while(getchar()!='\n');
        if (count != 10000000){
            printf("\nFlag: cybersoc{%s_%s_%s}\n\n", rand_string(string,5),rand_string(string2,5),rand_string(string3,5));
        }else {printf("Real Flag: %s\n", encryption(flag, sec, 26, keyLen)); exit(0);}
        count++;
    }

    return 0;
}