#ifndef lib_h
#define lib_h

void hello();

struct Block{
    int op_num;
    char* operations[];
};

struct Block** MainArray;

char* compareTwoFiles(char* file1, char*file2);

#endif //lib_h