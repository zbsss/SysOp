#ifndef lib_h
#define lib_h

void hello();

struct Block{
    int operationNumber;
    char** operations;
};

struct Block** compareFiles(char* files[],int size);
void compareTwoFiles(char* file1, char*file2);
struct Block* createBlock(char operationsFile[]);
int getNumberOfOperations(int index, struct Block** blocks);
void deleteBlock(int index, struct Block** blocks, int size);
void deleteOperation(int index, struct Block* block);

#endif //lib_h