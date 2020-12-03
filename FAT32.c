/////   all includes  
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

size_t strnlen(const char *str, size_t n)
{
    for (size_t size = 0; size < n; size++)
    {
        if (str[size] == '\0')
            return size;
    }
    return n;
}

char *strdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}

char *strndup(const char *s, size_t n) {
    char *p;
    size_t i;

    for (i = 0; i < n && s[i] != '\0'; i++)
        continue;
    p = malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, i);
        p[i] = '\0';
    }
    return p;
}

char *strsep (char **st, const char *de)
{
  char *i, *j;
  i = *st;
  if (i == NULL)
    return NULL;
  /* Find the end of the token.  */
  j = i + strcspn (i, de);
  if (*j)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *j++ = '\0';
      *st = j;
    }
  else
    /* No more delimiters; this is the last token.  */
    *st = NULL;
  return i;
}

void decimal_to_hexadecimal(int dec);                         // Converts decimal numbers to hex to be printed in info (see execute, line 82)
void stat(char *dir_name);                                    // Prints the attributes of the directory 
void vol();   
void INIT_ARGUMENTS();                                            // Receives input from the user that is parsed into tokens.
void get_dir_info();                                          // Prints directory info stored in struct above (line 67)
int32_t getCluster(char *dir_name);                           // Receives the cluster of information to be used in execute (line 82)
void ls();                                                    // Prints the current working directory (ls)
void cd(int32_t sector);                                      // Changes directory by user specification (cd)
void format_dir(char *dir_name);                              // Formats the directory to remove whitespace and concatenate a period between the name and extension.
void get();                                                   // Pulls file from the file system image into your cwd (current working directory)                                                // Prints the name of the volume in the fat32 file system image
void read_file(char *dir_name, int pos, int num_of_bytes);    // Reads the bytes specified by the user in the file of their choice
int32_t getSizeOfCluster(int32_t cluster);                    // Receives of the size of the cluster as an attribute
void INIT_RUNFAT();                                          // Main function of the program, acts as the shell receiving commands
void openImgFile(char file[]);                                // Opens a file system image to be used.
void closeImgFile();                                          // Closes the file system before exiting the program.

#define WHITESPACE " \t\n" 
// using whilte space as delimeter for command line spliting. token separation                           

#define MAX_COMSIZE 255         // The maximum command-line size

#define MAX_ARG 10              // Mav shell only supports five arguments

// buffer and cmd_buffer used for tokenizing user input

char *buffer[MAX_ARG];          // Parsed input string separated by white space
char cmd_buffer[MAX_COMSIZE];   // Entire string inputted by the user. It will be parsed into multiple tokens (47)
char BS_OEMName[8];
char formatted_dir[12];            // String to contain the fully formatted string
char BPB_Volume[11];               // String to store the volume of the fat32 file image


int8_t BPB_NumFATs , BPB_SecPerClus;     
// The amount of sectors per cluster of the fat32 file image

int16_t BPB_RootEntCnt, BPB_RsvdSecCnt, BPB_BytesPerSec;   
/*
this denotes amount of bytes in each secotr  
rootEntCnt is count of root entry
Reserverd sector count in iamge file 
*/

int32_t BPB_FATSz32, BPB_RootClus, FirstSectorofCluster = 0, FirstDataSector = 0,  RootDirSectors = 0, current_dir;       
/*
location of root cluster - rootlus
offset 0 denotes the location of first sector of cluster
root directory sectors - rootdirsectors
current working directory - 
*/


struct __attribute__((__packed__)) DirectoryEntry
{
    char dir_name[11];               // Name of the directory retrieved
    uint8_t dir_attribute;                // Attribute count of the directory retrieved
    uint16_t dir_first_cluster_high;
    uint16_t dir_first_cluster_low;
    uint32_t dir_file_size;           // Size of the directory (Always 0)
};
struct DirectoryEntry dir[16];       //Creation of the directory 

FILE *fp;


int main()
{
    for (;0 == 0;)
    {
        INIT_ARGUMENTS();
        INIT_RUNFAT();
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

void INIT_ARGUMENTS()    // patigyu
{
    printf("CMD> ");
    //console like application , here the commands are taken as input

    memset(cmd_buffer, '\0', MAX_COMSIZE);

    while (!fgets(cmd_buffer, MAX_COMSIZE, standard_input))
        ; //wait for the input. if yes store it in command buffer data structure

    int argument_cnt = 0;

    char *argument_pointer, *working_str = strdup(cmd_buffer), *working_root = working_str;

    memset(&buffer, '\0', MAX_ARG), memset(&buffer, '\0', sizeof(MAX_ARG));
    //filling the buffer with end marker

    while (((argument_pointer = strsep(&working_str, WHITESPACE)) != NULL) && (argument_cnt < MAX_ARG))
    {
        buffer[argument_cnt] = strndup(argument_pointer, MAX_COMSIZE); 
        // v[i] = string
        // as token count is initialized as zero so we count from 0. store every argument from the input in 
        // buffer character 2d array.
        if (strlen(buffer[argument_cnt]) == 0)                // size = 0 is not valid
            buffer[argument_cnt] = NULL;
        
        argument_cnt++;
    }
    free(working_root);
}
void arg_cmp_func(string s)
{
    if(s=="info")
    {
        printf("BPB_BytesPerSec: %d - ", BPB_BytesPerSec);
        decimal_to_hexadecimal(BPB_BytesPerSec);
        printf("\n");
        printf("BPB_SecPerClus: %d - ", BPB_SecPerClus);
        decimal_to_hexadecimal(BPB_SecPerClus);
        printf("\n");
        printf("BPB_RsvdSecCnt: %d - ", BPB_RsvdSecCnt);
        decimal_to_hexadecimal(BPB_RsvdSecCnt);
        printf("\n");
        printf("BPB_NumFATs: %d - ", BPB_NumFATs);
        decimal_to_hexadecimal(BPB_NumFATs);
        printf("\n");
        printf("BPB_FATSz32: %d - ", BPB_FATSz32);
        decimal_to_hexadecimal(BPB_FATSz32);
        printf("\n");
    }
    if(s=="get")
        get(buffer[1]);
    if(s=="volume")
        vol();
    if(s=="stat")
        get(buffer[1]);
    if(s=="ls")
        ls();
    if(s=="close")
        closeImgFile();
    if(s=="cd")
        cd(getCluster(buffer[1]));
    if(s=="read")
        read_file(buffer[1], atoi(buffer[2]), atoi(buffer[3]));
    
}
void INIT_RUNFAT()
{
    // after storing the argument or the input data in the buffer array, we now process it
    if (buffer[0]==NULL)   // If the user just hits enter, do nothing
        return;
    
    if (strcmp(buffer[0], "open") == 0)  // if command entered is "open"
    {
        if (fp!=NULL)    
        {
            // if already occupied , some image file is already open
            printf(" Image File already opended!!! \n");
            return;
        }
        if (buffer[1]==NULL)  
        {
            // file name not given
            printf(" The arguments are not provided. Error in opening file\n");
            
        }
        else if (buffer[1]!=NULL && fp==NULL) 
        {
            // ok condition
            openImgFile(buffer[1]);  // function to open image file
        }
        return;
    } // different commands to be implemented
    else if (strcmp(buffer[0], "info") == 0)
    {
        arg_cmp_func("info");
    }
    else if (strcmp(buffer[0], "get") == 0)
    {
        arg_cmp_func("get");
    }
    else if (strcmp(buffer[0], "read") == 0)
    {
        if (buffer[1] == NULL || buffer[2] == NULL || buffer[3] == NULL)
        {
            printf("Please valid input arguments.\n");
            return;
        }
        arg_cmp_func("read");    
    }
    else if (strcmp(buffer[0], "volume") == 0)
        arg_cmp_func("volume");
    else if (strcmp(buffer[0], "stat") == 0)
        arg_cmp_func("stat");    
    else if (strcmp(buffer[0], "ls") == 0)
        arg_cmp_func("ls");
    else if (strcmp(buffer[0], "cd") == 0)
    {
        if (buffer[1] == NULL)
        {
            printf("The directory to change not provided!! Err\n");
            return;
        }
        arg_cmp_func("cd");
    }
    else if (strcmp(buffer[0], "close") == 0)
        arg_cmp_func("close");
    
}

void openImgFile(char file[])
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
    current_dir = BPB_RootClus;    // contains cluster number of root dir

    int offset = LBAToOffset(current_dir);  // get offset no. of root dir
    fseek(fp, offset, SEEK_SET);                 // take fp to root dir pointer
    fread(&dir[0], 32, 16, fp);                  // 
}

void vol()
{
    fseek(fp, 71, SEEK_SET);
    fread(&BPB_Volume, 11, 1, fp);
    printf("Volume name: %s\n", BPB_Volume);
}



void get(char *dirname)   // ambiguous
{
    char *dirstring = (char *)malloc(strlen(dirname));
    strncpy(dirstring, dirname, strlen(dirname));
    int cluster = getCluster(dirstring);
    int size = getSizeOfCluster(cluster);
    FILE *newfp = fopen(buffer[1], "w");
    fseek(fp, LBAToOffset(cluster), SEEK_SET);
    unsigned char *ptr = malloc(size);
    fread(ptr, size, 1, fp);
    fwrite(ptr, size, 1, newfp);
    fclose(newfp);
}

void format_dir(char *dirname)  // converts to capital letters  microsoft - 8.3 filename syster where 8 is filename and 3 is for extension
{
    char expanded_name[12];
    memset(expanded_name, ' ', 12);   // initiliaze with whitespace

    char *buffer = strtok(dirname, "."); // separated by delimeter "." and stored in buffer array 

    if (buffer)
    {
        strncpy(expanded_name, buffer, strlen(buffer));  // copies buffer to expanded_name 

        buffer = strtok(NULL, ".");

        if (buffer)
        {
            strncpy((char *)(expanded_name + 8), buffer, strlen(buffer));  // take only first 8 characters of expanded_name
        }

        expanded_name[11] = '\0';

        int i;
        for (i = 0; i < 11; i++)
        {
            expanded_name[i] = toupper(expanded_name[i]);  // converting everything to uppercase
        }
    }
    else
    {
        strncpy(expanded_name, dirname, strlen(dirname));
        expanded_name[11] = '\0';
    }
    strncpy(formatted_dir, expanded_name, 12);    // ALL capital letters with 8 filename and 3 for extension length
}

int32_t getCluster(char *dirname)
{
    format_dir(dirname);  // converts all letter to capital
    int i;
    for (i = 0; i < 16; i++)
    {
        char *directory = malloc(11);   // allocate 11 bytes
        memset(directory, '\0', 11);    // set all to '\0'
        memcpy(directory, dir[i].dir_name, 11);   // copies dir name to variable

        if (strncmp(directory, formatted_dir, 11) == 0)   // compares original name and given name
        {
            int cluster = dir[i].dir_first_cluster_low;     // initial pointer of that dir
            return cluster;
        }
    }
    return -1;  // if no such file is present
}

int32_t getSizeOfCluster(int32_t cluster)           // returns size of provided cluster
{
    int i;
    for (i = 0; i < 16; i++)                        // traversing all dir
    {
        if (cluster == dir[i].dir_first_cluster_low)  // finds correct cluster
        {
            int size = dir[i].dir_file_size;         // gets size of that dir
            return size;
        }
    }
    return -1;
}

void stat(char *dirname)
{
    int cluster = getCluster(dirname);     // obtains cluster
    int size = getSizeOfCluster(cluster);  // gets size of cluster
    printf("Size: %d\n", size);            // print file size
    int i;
    for (i = 0; i < 16; i++)
    {
        if (cluster == dir[i].dir_first_cluster_low)    // finds its cluster
        {
            printf("Attr: %d\n", dir[i].dir_attribute);
            printf("Starting Cluster: %d\n", cluster);
            printf("Cluster High: %d\n", dir[i].dir_first_cluster_high);
        }
    }
}

void cd(int32_t cluster)                                   // "cd" implemented
{
    if (strcmp(buffer[1], "..") == 0)                                   // if it command is "cd .."
    {
        int i;
        for (i = 0; i < 16; i++)
        {
            if (strncmp(dir[i].dir_name, "..", 2) == 0)                  // finds cluster for ..
            {
                int offset = LBAToOffset(dir[i].dir_first_cluster_low);    // gets offset
                current_dir = dir[i].dir_first_cluster_low;                // stores in current dir
                fseek(fp, offset, SEEK_SET);                             // moves pointer to offset
                fread(&dir[0], 32, 16, fp);                              // read that content
                return;
            }
        }
    }
    int offset = LBAToOffset(cluster);    // gets offset
    current_dir = cluster;           // stores in current dir
    fseek(fp, offset, SEEK_SET);          // moves pointer to offset
    fread(&dir[0], 32, 16, fp);           // read that content
}

void read_file(char *dirname, int position, int numOfBytes)  // reads file starting from the given position upto the number of bytes mentioned
{
    int cluster = getCluster(dirname);        // gets value of cluster
    int offset = LBAToOffset(cluster);        // gets offset
    fseek(fp, offset + position, SEEK_SET);   // moves pointer from where we need to read the file
    char *bytes = malloc(numOfBytes);         // allocate given size
    fread(bytes, numOfBytes, 1, fp);          // reads the entire content till size numofbytes
    printf("%s\n", bytes);
}

void get_dir_info()    // prints information
{
    int i;
    for (i = 0; i < 16; i++)
    {
        fread(&dir[i], 32, 1, fp);
    }
}

void print(char *dir)
{
	int i=0;
	for(i=0; i<11; i++)
	{
		if((dir[i]>='A' && dir[i]<='Z') || (dir[i]>='0' && dir[i]<='9') || dir[i]==' ')
		{
			printf("%c", dir[i]);
		}	
	}
	printf("\n");
}

void ls()  // works as "ls" command
{
    if (fp == NULL) // image file not opened
    {
        printf("No image is opened\n");
        return;
    }

    int offset = LBAToOffset(current_dir);  // get offset for current dir
    fseek(fp, offset, SEEK_SET);  // moves pointer to offset

    int i;
    for (i = 0; i < 16; i++)  // traversing through all directories
    {
        fread(&dir[i], 32, 1, fp);  // reading ith directory

        if ((dir[i].dir_name[0] != (char)0xe5) && (dir[i].dir_attribute == 0x1 || dir[i].dir_attribute == 0x10 || dir[i].dir_attribute == 0x20))
        {
            char *directory = malloc(11);
            memset(directory, '\0', 11);   // initialize directory to '\0'
            memcpy(directory, dir[i].dir_name, 11); // copies directory name
            print(directory); // print
        }
    }
}

void decimal_to_hexadecimal(int dec)
{
    char hex[100];
    int i = 1;
    int j;
    int temp;
    while (dec != 0)
    {
        temp = dec % 16;
        if (temp < 10)
        {
            temp += 48;
        }
        else
        {
            temp += 55;
        }
        hex[i++] = temp;
        dec /= 16;
    }
    for (j = i - 1; j > 0; j--)
    {
        printf("%c", hex[j]);
    }
}

void closeImgFile()  // closes the currently opened image file
{
    if (fp == NULL)   // if file is not opened
    {
        printf("Error: File system not open.");
        return;
    }

    fclose(fp);   // closes the file
    fp = NULL;    // sets file pointer to NULL
}
