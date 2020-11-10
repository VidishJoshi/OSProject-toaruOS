/////   all includes   
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WHITESPACE " \t\n" // We want to split our command line up into tokens \
                           // so we need to define what delimits our tokens.   \
                           // In this case  white space                        \
                           // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

#define MAX_NUM_ARGUMENTS 10 // Mav shell only supports five arguments

// token and cmd_str used for tokenizing user input

char *token[MAX_NUM_ARGUMENTS];   // Parsed input string separated by white space
char cmd_str[MAX_COMMAND_SIZE];   // Entire string inputted by the user. It will be parsed into multiple tokens (47)

char BS_OEMName[8];
int16_t BPB_BytesPerSec;   // The amount of bytes in each sector of the fat32 file image
int8_t BPB_SecPerClus;     // The amount of sectors per cluster of the fat32 file image
int16_t BPB_RsvdSecCnt;    // Amount of reserved sectors in the fat32 image
int8_t BPB_NumFATs;
int16_t BPB_RootEntCnt;    // Root entry count
int32_t BPB_FATSz32;
int32_t BPB_RootClus;      // Rootcluster location in the fat32 image

int32_t RootDirSectors = 0;        // Amount of root directory sectors
int32_t FirstDataSector = 0;       // Where the first data sector exists in the fat32 file image.
int32_t FirstSectorofCluster = 0;  // First sector of the data cluster exists at point 0 in the fat32 file image.

int32_t currentDirectory;          // Current working directory
char formattedDirectory[12];       // String to contain the fully formatted string
char BPB_Volume[11];               // String to store the volume of the fat32 file image

struct __attribute__((__packed__)) DirectoryEntry
{
    char DIR_Name[11];               // Name of the directory retrieved
    uint8_t DIR_Attr;                // Attribute count of the directory retrieved
    uint8_t Unused1[8];
    uint16_t DIR_FirstClusterHigh;
    uint8_t Unused2[4];
    uint16_t DIR_FirstClusterLow;
    uint32_t DIR_FileSize;           // Size of the directory (Always 0)
};
struct DirectoryEntry dir[16];       //Creation of the directory 

FILE *fp;

int main()
{
    while (1)
    {
        getInput();
        execute();
    }
    return 0;
}


int LBAToOffset(int32_t sector)
{
    if (sector == 0)  // want offset for root dir
        sector = 2;
    // FAT #1 starts at address BPB_RsvdSecCnt * BPB_BytsPerSec
    // BPB_NumFATs * BPB_FATSz32 * BPB_BytsPerSec  ==> total FAT size
    // Clusters are each (BPB_SecPerClus * BPB_BytsPerSec) in bytes
    // Clusters start at address (BPB_NumFATs * BPB_FATSz32 * BPB_BytsPerSec) + (BPB_RsvdSecCnt * BPB_BytsPerSec)  ==> location of root dir
    return ((sector - 2) * BPB_BytesPerSec) + (BPB_BytesPerSec * BPB_RsvdSecCnt) + (BPB_NumFATs * BPB_FATSz32 * BPB_BytesPerSec);
}

void getInput()    // patigyu
{
    printf("CMD> ");

    memset(cmd_str, '\0', MAX_COMMAND_SIZE);

    while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin))
        ;

    int token_count = 0;

    char *arg_ptr;

    char *working_str = strdup(cmd_str);

    char *working_root = working_str;

    memset(&token, '\0', MAX_NUM_ARGUMENTS);

    memset(&token, '\0', sizeof(MAX_NUM_ARGUMENTS));
    while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) && (token_count < MAX_NUM_ARGUMENTS))
    {
        token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE); // v[i] = string
        if (strlen(token[token_count]) == 0)  // size = 0 is not valid
        {
            token[token_count] = NULL;
        }
        token_count++;
    }
    free(working_root);
}

void execute()
{
    
    if (token[0] == NULL)   // If the user just hits enter, do nothing
    {
        return;
    }
    if (strcmp(token[0], "open") == 0)  // if command entered is "open"
    {
        if (fp != NULL)    // if already occupied , some image file is already open
        {
            printf("Error: File system image already open.\n");
            return;
        }
        if (token[1] != NULL && fp == NULL)  // ok condition
        {
            openImage(token[1]);  // function to open image file
        }
        else if (token[1] == NULL) // file name not given
        {
            printf("ERR: Must give argument of file to open\n");
        }
        return;
    }
    else if (fp == NULL)  // if open not execute yet then first do it
    {
        printf("Error: File system image must be opened first.\n");
        return;
    }
    // different commands to be implemented
    else if (strcmp(token[0], "info") == 0)
    {
        printf("BPB_BytesPerSec: %d - ", BPB_BytesPerSec);
        decToHex(BPB_BytesPerSec);
        printf("\n");
        printf("BPB_SecPerClus: %d - ", BPB_SecPerClus);
        decToHex(BPB_SecPerClus);
        printf("\n");
        printf("BPB_RsvdSecCnt: %d - ", BPB_RsvdSecCnt);
        decToHex(BPB_RsvdSecCnt);
        printf("\n");
        printf("BPB_NumFATs: %d - ", BPB_NumFATs);
        decToHex(BPB_NumFATs);
        printf("\n");
        printf("BPB_FATSz32: %d - ", BPB_FATSz32);
        decToHex(BPB_FATSz32);
        printf("\n");
    }
    else if (strcmp(token[0], "ls") == 0)
    {
        printDirectory();
    }
    else if (strcmp(token[0], "volume") == 0)
    {
        volume();
    }
    else if (strcmp(token[0], "close") == 0)
    {
        closeImage();
    }
}

void getInput()
{
    printf("CMD> ");

    memset(cmd_str, '\0', MAX_COMMAND_SIZE);

    while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin));

    int token_count = 0;

    char *arg_ptr;

    char *working_str = strdup(cmd_str);

    char *working_root = working_str;

    memset(&token, '\0', MAX_NUM_ARGUMENTS);

    memset(&token, '\0', sizeof(MAX_NUM_ARGUMENTS));
    while (((arg_ptr = strsep(&working_str, WHITESPACE)) != NULL) && (token_count < MAX_NUM_ARGUMENTS))
    {
        token[token_count] = strndup(arg_ptr, MAX_COMMAND_SIZE); // v[i] = string
        if (strlen(token[token_count]) == 0)  // size = 0 is not valid
        {
            token[token_count] = NULL;
        }
        token_count++;
    }
    free(working_root);
}

void openImage(char file[])
{
    fp = fopen(file, "r");  // open image file in read mode

    if (fp == NULL)  // no such file exist
    {
        printf("Image does not exist\n");
        return;
    }
    printf("%s opened.\n", file);
    fseek(fp, 3, SEEK_SET);             // BS_jmpBoot ==> size - 3 bytes, offest - 0   // not interested in 0 to 3 bytes
    fread(&BS_OEMName, 8, 1, fp);       // BS_OEMName ==>  siZe 8 bytes, offest - 3 // stored in char array

    fseek(fp, 11, SEEK_SET);            // take file pointer to 11th byte
    fread(&BPB_BytesPerSec, 2, 1, fp);  // BPB_BytesPerSec ==>  size 2 byte, offest - 11  // Count of bytes per sector
    fread(&BPB_SecPerClus, 1, 1, fp);   // BPB_SecPerClus ==> size 1 byte, offest - 13  // Number of sectors per allocation unit.
    fread(&BPB_RsvdSecCnt, 2, 1, fp);   // BPB_RsvdSecCnt ==> size 2 byte, offset - 14 // Number of reserved sectors in the Reserved region of the volume starting
    fread(&BPB_NumFATs, 1, 1, fp);      // BPB_NumFATs ==> size 1 byte, offset - 16 // Count of FAT data structures on the volume.
    fread(&BPB_RootEntCnt, 2, 1, fp);   // BPB_RootEntCnt ==> size 2 byte, offset - 17 // contains the count of 32-byte directory entries in the root directory

    // now table changes in documentation go to table 3 page 12

    fseek(fp, 36, SEEK_SET);            // take file pointer to 36th byte
    fread(&BPB_FATSz32, 4, 1, fp);      // BPB_FATSz32 ==> size 4 byte, offset - 36 // count of sectors occupied by ONE FAT

    fseek(fp, 44, SEEK_SET);            // take file pointer to 44th byte
    fread(&BPB_RootClus, 4, 1, fp);     // BPB_RootClus ==> 4 byte, offset - 44 //  set to the cluster number of the first cluster of the root directory
    currentDirectory = BPB_RootClus;    // contains cluster number of root dir

    int offset = LBAToOffset(currentDirectory);  // get offset no. of root dir
    fseek(fp, offset, SEEK_SET);                 // take fp to root dir pointer
    fread(&dir[0], 32, 16, fp);                  // 
}

void volume()
{
    fseek(fp, 71, SEEK_SET);
    fread(&BPB_Volume, 11, 1, fp);
    printf("Volume name: %s\n", BPB_Volume);
}

void getDirectoryInfo()
{
    int i;
    for (i = 0; i < 16; i++)
    {
        fread(&dir[i], 32, 1, fp);
    }
}

void printDirectory()
{
    if (fp == NULL)
    {
        printf("No image is opened\n");
        return;
    }

    int offset = LBAToOffset(currentDirectory);
    fseek(fp, offset, SEEK_SET);

    int i;
    for (i = 0; i < 16; i++)
    {
        fread(&dir[i], 32, 1, fp);

        if ((dir[i].DIR_Name[0] != (char)0xe5) && (dir[i].DIR_Attr == 0x1 || dir[i].DIR_Attr == 0x10 || dir[i].DIR_Attr == 0x20))
        {
            char *directory = malloc(11);
            memset(directory, '\0', 11);
            memcpy(directory, dir[i].DIR_Name, 11);
            printf("%s\n", directory);
        }
    }
}

void closeImage()
{
    if (fp == NULL)
    {
        printf("Error: File system not open.");
        return;
    }

    fclose(fp);
    fp = NULL;
}
