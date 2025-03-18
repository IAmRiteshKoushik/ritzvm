/* ritzvm.c */
#include "ritzvm.h"
#include <birchutils.h>
#include <stdlib.h>

VM *virtualmachine(Program *pr, int16 progsz) {
    VM *p;
    Program *pp;
    int16 size;

    // Setting up some sanity checks
    assert((pr) && (progsz));

    // Setting up int16 as the size to avoid implicit typecasting warning
    size = $2 sizeof(struct s_vm);
    p = (VM *)malloc($i size);
    if (!p) {
        errno = ErrMem;
        return (VM *)0;
    }
    zero($1 p, size);
    pp = (Program *)malloc($i progsz);
    if (!pp){
        free(p);
        errno = ErrMem;
        return (VM *)0;
    }

    // If we do have a Program pointer (pp), we would copy the programs
    copy(pp, pr, progsz);
    return p;
}

Program *exampleprogram(){
    static Program prog[] = {
        {
            .o = mov,
            .a = { 0x00, 0x05 }
        },
        {
            .o = nop,
            .a = NoArgs
        }
    };

    return prog;
}

int main(int argc, char *argv[]){
    VM *vm;
    Program prog;

    prog = exampleprogram();
    vm = virtualmachine(prog, int16 progsz);
}
