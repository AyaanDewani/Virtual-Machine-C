#ifndef BM_H_
#define BM_H_

#include <stdint.h>
#include <stdio.h>

#define ARRAY_SIZE(xs) (sizeof(xs)/sizeof((xs)[0]))
#define BM_STACK_CAPACITY 1024
#define BM_PROGRAM_CAPACITY 1024
#define LABEL_CAPACITY 1024
#define UNRESOLVED_JMPS_CAPACITY 1024


typedef int64_t Word;

typedef enum {
    ERR_OK = 0,          // everything is okay :)
    ERR_STACK_OVERFLOW, 
    ERR_STACK_UNDERFLOW, 
    ERR_ILLEGAL_INST,
    ERR_DIV_BY_ZERO, 
    ERR_ILLEGAL_INST_ACCESS, 
    ERR_ILLEGAL_OPERAND, 
} Err;

const char *err_as_cstr(Err err);

typedef enum {
    INST_NOP = 0, 
    INST_PUSH, 
    INST_DUP, 
    INST_PLUS,
    INST_MINUS,
    INST_MULT,
    INST_DIV, 
    INST_JMP,         // unconditional jmp for loops
    INST_JMP_IF, 
    INST_EQ, 
    INST_HALT, 
    INST_PRINT_DEBUG,  
} Inst_Type;

const char *inst_type_as_cstr(Inst_Type type);

typedef struct {
    Inst_Type type; 
    Word operand; 
} Inst;

typedef struct {
    Word stack[BM_STACK_CAPACITY]; 
    Word stack_size; 

    Inst program[BM_PROGRAM_CAPACITY]; 
    Word program_size; 
    Word ip; 

    int halt; 
} Bm;

#define MAKE_INST_PUSH(value)    ((Inst) {.type = INST_PUSH, .operand = (value)})
#define MAKE_INST_PLUS           ((Inst) {.type = INST_PLUS})
#define MAKE_INST_MINUS          ((Inst) {.type = INST_MINUS})
#define MAKE_INST_MULT           ((Inst) {.type = INST_MULT})
#define MAKE_INST_DIV            ((Inst) {.type = INST_DIV})
#define MAKE_INST_JMP(addr)      ((Inst) {.type = INST_JMP, .operand = (addr)})
#define MAKE_INST_HALT(addr)     ((Inst) {.type = INST_HALT, .operand = (addr)})
#define MAKE_INST_DUP(addr)      ((Inst) {.type = INST_DUP, .operand = (addr)})

Err bm_execute_inst(Bm *bm);
Err bm_execute_program(Bm *bm, int limit);
void bm_dump_stack(FILE *stream, const Bm *bm);
void bm_load_program_from_memory(Bm *bm, Inst *program, size_t program_size);
void bm_load_program_from_file(Bm *bm, const char *file_path);
void bm_save_program_to_file(const Bm *bm, const char *file_path);

typedef struct {
    size_t count;
    const char *data;
} String_View;

typedef struct {
    String_View name; 
    Word addr;
} Label; 

typedef struct {
    Word addr;
    String_View label; 
} Unresolved_Jmp; 

typedef struct {
    Label labels[LABEL_CAPACITY]; 
    size_t labels_size; 
    Unresolved_Jmp unresolved_jmps[UNRESOLVED_JMPS_CAPACITY]; 
    size_t unresolved_jmps_size; 
} Label_Table; 

String_View cstr_as_sv(const char *cstr);
String_View sv_trim_left(String_View sv);
String_View sv_trim_right(String_View sv);
String_View sv_trim(String_View sv);
String_View sv_chop_by_delim(String_View *sv, char delim);
int sv_eq(String_View a, String_View b);
int sv_to_int(String_View sv);

void bm_translate_source(String_View source, Bm *bm,  Label_Table *lt);
String_View slurp_file(const char *file_path);

#endif // BM_H_
