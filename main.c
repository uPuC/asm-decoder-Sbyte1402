/******************************************************************************
Prac 2 - AVR ASM OpCode Decoder
*******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

const uint8_t flash_mem[] ={ 
    0x00, 0x24, 0xA0, 0xE0, 0xB2, 0xE0, 0x0D, 0x91, 0x00, 0x30, 0xE9, 0xF7, 0x11, 0x97, 0xC0, 0xE0, 0xC4, 
    0xD2, 0xE0, 0x09, 0x91, 0x1E, 0x91, 0x01, 0x17, 0x51, 0xF4, 0x0A, 0x2F, 0x0A, 0x95, 0x1C, 0x2F, 0x65, 
    0x01, 0x17, 0xB9, 0xF7, 0x0B, 0x2F, 0x1D, 0x2F, 0x01, 0x17, 0x99, 0xF7, 0x03, 0x94, 0x00, 0x00 };

const uint16_t inst16_table[] = {
    0x0,    // NOP
    0x9,    // EOR, CLR
    0xE,    // LDI
    0x4B    // LD_X
};

enum{
    e_NOP,
    e_EOR,
    e_LDI,
    e_LDX
};


// Op Code struct
typedef union{
    uint16_t op16; // e.g.: watchdog, nop

    struct{
        uint16_t op4:4;
        uint16_t d5:5;
        uint16_t op7:7;
    }type1; // e.g: LSR, LD_X

    struct{
        uint16_t r4:4;
        uint16_t d5:5;
        uint16_t r1:1;
        uint16_t op6:6;
    }type2; // e.g.: MOV,MUL,ADC,ADD,AND,
    
    struct{
        uint16_t kl4: 4;
        uint16_t d4: 4;
        uint16_t kh4: 4;
        uint16_t op4: 4;
    }type3; // e.g.: LDI
}Op_Code_t;


int main(){
    Op_Code_t *instruction;

    printf("- Practica 2: AVR OpCode -\n");
    // Decode the instructions by cycling through the array
    //
    for (uint8_t idx = 0; idx < sizeof(flash_mem); idx += 2){
        instruction = (Op_Code_t*)&flash_mem[idx];

        if (instruction -> op16 == inst16_table[e_NOP]) {
            printf("NOP\n");
        } else if (instruction -> type2.op6 == inst16_table[e_EOR]) {
            uint8_t Rd = instruction -> type2.d5;
            uint8_t Rr = (instruction -> type2.r1 << 4) | (instruction ->type2.r4);

            if(Rd == Rr){
                printf("CLR R%d\n", Rd);
            }else{
                printf("EOR R%d, R%d\n", Rd, Rr);
            }
        } else if (instruction -> type3.op4 == inst16_table[e_LDI]) {
            uint8_t k = (instruction -> type3.kh4 << 4) | (instruction -> type3.kl4);
            uint8_t Rd = instruction -> type3.d4;

            printf("LDI R%d, %d\n", Rd, k);
        } else if ((instruction -> type1.op7 | (instruction -> type1.op4 >> 2)) == inst16_table[e_LDX]) {
            uint8_t Rd = instruction -> type1.d5;
            uint8_t op = instruction -> type1.op4;

            if (op == 0xC) {
                printf("LD R%d, X\n", Rd);
            } else if (op == 0xD) {
                printf("LD R%d, X+\n", Rd);
            } else {
                printf("LD R%d, -X\n", Rd);
            }
        } else {
            printf("unknown\n");
        }
    }

    return 0;
}
