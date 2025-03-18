/* birchvm.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

// There is a separate installation step for this utility header.
// Find : https://youtu.be/0OcmaLP-tT8?si=4yxgNJQLJqfLBtYd
#include <birchutils.h>

// Aliased types for ease of handling
typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int64;

// Global Typecasts
#define $1 (int8 *) // named after bytes
#define $2 (int16)  // named after bytes
#define $4 (int32)  // named after bytes
#define $8 (int64)  // named after bytes
#define $c (char *)
#define $i (int)

/*
    16 bit virtual machine
    65536 different memory addresses (possibilities)
    65 kB memory
    Serial COM port (optional)
    Floppy drive (optional)

    Registers:
        AX
        BX
        CX
        DX
        SP (Stack Pointer)
        IP (Instruction Pointer)
*/

typedef int16 Reg;

// This is going to be our set of registers that powers the virtual machine
struct s_registers {
    Reg ax;
    Reg bx;
    Reg cx;
    Reg dx;
    Reg sp;
    Reg ip;
};
typedef struct s_registers Registers;

// This is going to be our set of CPUs
struct s_cpu {
    Registers r;
};
typedef struct s_cpu CPU;

/*
    mov ax, 0x05 // (0x01 OR ax)
                 // 0000 0001 = trans. "mov"
    From the 4-least significant bytes, the LSB defines the move instruction
    and the remaining 3 bytes decide the register associated with it. So
    the complete instruction can be determined by using an OR operation
    between the "register-address" and the "mov"
                 // 0000 0011 -> (0x01 OR 0x02) -> mov ax
    (similarly)
                 // 0000 0101 -> mov bx
                 // 0000 0111 -> mov cx
                 // 0000 1001 -> mov dx
                 // 0000 1011 -> mov sp
                 // -> ip (we cannot move anything to the instruction pointer)
    Now, we have the 4 MSB which are left untouched. Which created
    2^4 = 16 combinations. This will not suffice as we need to support a lot
    more numbers.

    // 0000 0000
    // 0000 0101 = 0x05 (our range is from 0 - 65535) which makes it 2 bytes

    // Hence, the instruction will look like
    // -------------------------------------
    // 0000 0011 = mov ax
    // 0000 0000
    // 0000 0101 = 0x05
*/


enum e_opcode {
    mov = 0x01, // Move operation
    nop = 0x02  // No operation
};
typedef enum e_opcode Opcode;

struct s_instrmap {
    Opcode o;
    int8 size;
};
typedef struct s_instrmap IM;

// Args range from 0 to 2 bytes which means it can by represented by 8 bits
// Which is the character length in C (256 utf-8 characters)
typedef int8 Args;

struct s_instruction {
    /*
        Each instruction has an op-code that can be determined from
        the instruction-set table
    */
    Opcode o;
    Args a[]; /* 0 - 2 bytes */
};
typedef struct s_instruction Instruction;
typedef int8 Stack[((unsigned int)(-1))];
typedef Instruction Program;

struct s_vm {
    CPU c;
    Stack s;
    Program *p;
};
typedef struct s_vm VM;

static IM instrmap[] = {
    { mov, 0x03 },
    { nop, 0x01 }

};

int main(int, char**);
