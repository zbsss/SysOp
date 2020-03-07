#ifndef lib_h
#define lib_h

void hello();

struct Block{
    int operationNumber;
    char** operations;
};

void compareFiles(char* files[],int size);
void compareTwoFiles(char* file1, char*file2);
struct Block* createBlocks(char operationsFile[]);

#endif //lib_h