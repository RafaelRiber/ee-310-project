#include <nds.h>
#include <stdio.h>
#include "message_protocol.h"
#include "battleships.h"

int main(void) {

    consoleDemoInit();
    int a = 1;

    while(1){
        printf("\nHello Times:%d!\n",a++);
        swiWaitForVBlank();
    }
}
