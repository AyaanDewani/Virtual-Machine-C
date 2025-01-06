#include "./bm.c"


int main(int argc, char **argv){

    if (argc < 2){
        fprintf(stderr, "Usage: ./bmi <input.bm>\n");
        fprintf(stderr, "ERROR: Expected input\n"); 
        exit(1);         
    }

    // bm_load_program_from_memory(&bm, program, ARRAY_SIZE(program));   
    bm_load_program_from_file(&bm, argv[1]); //works
    //code in fib.bm -> last 6 line of the program array in this file 
    //produces the same output (fibonacci sequence until F18)
    bm_dump_stack(stdout, &bm); 
    for (Word i = 0; i < 69 && !bm.halt; ++i) {
        // printf("%s\n", inst_type_as_cstr(program[bm.ip].type)); 
        Err err = bm_execute_inst(&bm); 
        if (err != ERR_OK) {
            fprintf(stderr, "Error activated: %s\n", err_as_cstr(err)); 
            exit(1); 
        }
    }; 
    
    bm_dump_stack(stdout, &bm); 


    return 0;
}

