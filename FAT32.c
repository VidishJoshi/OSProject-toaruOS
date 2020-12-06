

/////   all includes  
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define i_32 int32_t

#define i64 int

#define varX size_t

void decimal_to_hexadecimal(i64 dec);                         // Converts decimal numbers to hex to be printed in info (see execute, line 82)
void statistics(char *dir_name);                                    // Prints the attributes of the DIR 
void vol();   
void INIT_ARGUMENTS();                                            // Receives input from the user that is parsed into tokens.
void get_dir_info();                                          // Prints DIR info stored in struct above (line 67)
i_32 obtain_clus(char *dir_name);                           // Receives the cluss of information to be used in execute (line 82)
void ls();                                                    // Prints the current working DIR (ls)
void cd(i_32 SEC);                                      // Changes DIR by user specification (cd)
void format_dir(char *dir_name);                              // Formats the DIR to remove whitespace and concatenate a period between the name and extension.
void read_file(char *dir_name, i64 pos, i64 num_of_bytes);    // Reads the bytes specified by the user in the file of their choice
i_32 find_clus_size(i_32 cluss);                    // Receives of the size of the cluss as an attribute
void INIT_RUNFAT();                                          // Main function of the program, acts as the shell receiving commands
void openImgFile(char file[]);                                // Opens a file system image to be used.
void closeImgFile();                                          // Closes the file system before exiting the program.

#define white_space " \t\n" 
// using whilte space as delimeter for command line spliting. token separation   

char *str_n_duplicates(const char *s, varX n) {
    char *p;
    varX i;

    for (i = 0; i < n && s[i] != '\0'; i++)
        continue;
    p = malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, i);
        p[i] = '\0';
    }
    return p;
}

char *str_separate (char **st, const char *de)
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

#define MAX_COMSIZE 255         // The maximum command-line size

#define MAX_ARG 10              // Mav shell only supports five arguments

// BUFER and cmdbuf used for tokenizing user input

char *BUFER[MAX_ARG];          // Parsed input string separated by white space
char cmdbuf[MAX_COMSIZE];   // Entire string inputted by the user. It will be parsed into multiple tokens (47)
char name_bs[8];
char final_modified_dir[12];            // String to contain the fully formatted string
char vol_bpb[11];               // String to store the volume of the fat32 file image


int8_t Number_of_FAT_bpb , Sector_pre_Cluster_bpb;     
// The amount of sectors per cluss of the fat32 file image

int16_t Root_Entry_Count_bpb, Reserved_sector_count_bpb, bytes_per_sector_bpb;   
/*
this denotes amount of bytes in each secotr  
rootEntCnt is count of root entry
Reserverd SEC count in iamge file 
*/
char *str_duplicates(const char *s) {
    varX size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}

i_32 FAT_size_32_bpb, Root_Cluster_bpb, current_dir;       
/*
location of root cluss - rootlus
phy_addr 0 denotes the location of first sector of cluss
root DIR sectors - rootdirsectors
current working DIR - 
*/


struct __attribute__((__packed__)) dir_etry
{
    char dir_name[11];               // Name of the DIR retrieved
    uint8_t dir_attribute;                // Attribute count of the DIR retrieved
    uint16_t dir_first_cluster_high, dir_first_cluster_low;
    uint32_t dir_file_size;           // Size of the DIR (Always 0)
};
struct dir_etry dir[16];       //Creation of the DIR 
/*
This the structure of a directory. We store all the information of a *dir* in a variable of this structure to access 
the its properties such as FileSize, DIrName, FirstCluster properties, etc. as shown in the image below,
*/
varX str_n_length(const char *str, varX n)
{
    for (varX size = 0; size < n; size++)
    {
        if (str[size] == '\0')
            return size;
    }
    return n;
}

FILE *fileptrr;


i64 main()
{
    for (;0 == 0;)
    {
        INIT_ARGUMENTS();
        INIT_RUNFAT();
    }
    return 0;
}


i64 logical_to_physical(i_32 SEC)
{
    if (SEC == 0)  // want phy_addr for root dir
        SEC = 2;
    // FAT #1 starts at address Reserved_sector_count_bpb * BPB_BytsPerSec
    // Number_of_FAT_bpb * FAT_size_32_bpb * BPB_BytsPerSec  ==> total FAT size
    // Clusters are each (Sector_pre_Cluster_bpb * BPB_BytsPerSec) in bytes
    // Clusters start at address (Number_of_FAT_bpb * FAT_size_32_bpb * BPB_BytsPerSec) + (Reserved_sector_count_bpb * BPB_BytsPerSec)  ==> location of root dir
    return ((SEC - 2) * bytes_per_sector_bpb) + (bytes_per_sector_bpb * Reserved_sector_count_bpb) + (Number_of_FAT_bpb * FAT_size_32_bpb * bytes_per_sector_bpb);
}

void INIT_ARGUMENTS()    // patigyu
{
    printf("CMD> ");
    memset(cmdbuf, '\0', MAX_COMSIZE);

    while (!fgets(cmdbuf, MAX_COMSIZE, standard_input))        ;

    i64 argument_cnt = 0;

    char *arg_ptr, *wrk_st = str_duplicates(cmdbuf), *wrkroot = wrk_st;

    memset(&BUFER, '\0', MAX_ARG), memset(&BUFER, '\0', sizeof(MAX_ARG));

    while (((arg_ptr = str_separate(&wrk_st, white_space)) != NULL) && (argument_cnt < MAX_ARG))
    {
        BUFER[argument_cnt] = str_n_duplicates(arg_ptr, MAX_COMSIZE); 
        // v[i] = string
        // BUFER character 2d array.
        if (strlen(BUFER[argument_cnt]) == 0)                // size = 0 is not valid
            BUFER[argument_cnt] = NULL;
        
        argument_cnt++;
    }
}
/*
This is the `openImage` function that allows us to open the `.img` file containing the FAT configured drive.
*/
void arg_cmp_func(char *s)
{
    if(strcmp(s,"info") == 0)
    {
        printf("bytes_per_sector_bpb: %d - ", bytes_per_sector_bpb);
        decimal_to_hexadecimal(bytes_per_sector_bpb);
        printf("\n");
        printf("Sector_pre_Cluster_bpb: %d - ", Sector_pre_Cluster_bpb);
        decimal_to_hexadecimal(Sector_pre_Cluster_bpb);
        printf("\n");
        printf("Reserved_sector_count_bpb: %d - ", Reserved_sector_count_bpb);
        decimal_to_hexadecimal(Reserved_sector_count_bpb);
        printf("\n");
        printf("Number_of_FAT_bpb: %d - ", Number_of_FAT_bpb);
        decimal_to_hexadecimal(Number_of_FAT_bpb);
        printf("\n");
        printf("FAT_size_32_bpb: %d - ", FAT_size_32_bpb);
        decimal_to_hexadecimal(FAT_size_32_bpb);
        printf("\n");
    }
    if(strcmp(s,"get") == 0)
        get(BUFER[1]);
    if(strcmp(s,"volume") == 0)
        vol();
    if(strcmp(s,"stat") == 0)
        statistics(BUFER[1]);
    if(strcmp(s,"ls") == 0)
        ls();
    if(strcmp(s,"close") == 0)
        closeImgFile();
    if(strcmp(s,"cd") == 0)
        cd(obtain_clus(BUFER[1]));
    if(strcmp(s,"read") == 0)
        read_file(BUFER[1], atoi(BUFER[2]), atoi(BUFER[3]));
    
}
void INIT_RUNFAT()
{
    // after storing the argument or the input data in the BUFER array, we now process it
    if (BUFER[0]==NULL)   // If the user just hits enter, do nothing
        return;
    
    if (strcmp(BUFER[0], "open") == 0)  // if command entered is "open"
    {
        if (fileptrr!=NULL)    
        {
            // if already occupied , some image file is already open
            printf(" Image File already opended!!! \n");
            return;
        }
        if (BUFER[1]==NULL)  
        {
            // file name not given
            printf("args insufficient!!!\n");
            
        }
        else if (BUFER[1]!=NULL && fileptrr==NULL) 
        {
            // ok condition
            openImgFile(BUFER[1]);  // function to open image file
        }
        return;
    } // different commands to be implemented
    else if (strcmp(BUFER[0], "info") == 0)
    {
        arg_cmp_func("info");
    }
    else if (strcmp(BUFER[0], "get") == 0)
    {
        arg_cmp_func("get");
    }
    else if (strcmp(BUFER[0], "read") == 0)
    {
        if (BUFER[1] == NULL || BUFER[2] == NULL || BUFER[3] == NULL)
        {
            printf("Please valid input arguments.\n");
            return;
        }
        arg_cmp_func("read");    
    }
    else if (strcmp(BUFER[0], "volume") == 0)
        arg_cmp_func("volume");
    else if (strcmp(BUFER[0], "stat") == 0)
        arg_cmp_func("stat");    
    else if (strcmp(BUFER[0], "ls") == 0)
        arg_cmp_func("ls");
    else if (strcmp(BUFER[0], "cd") == 0)
    {
        if (BUFER[1] == NULL)
        {
            printf("The DIR to change not provided!! Err\n");
            return;
        }
        arg_cmp_func("cd");
    }
    else if (strcmp(BUFER[0], "close") == 0)
        arg_cmp_func("close");
    
}

void openImgFile(char file[])
{
    fileptrr = fopen(file, "r");  // open image file in read mode

    if (fileptrr == NULL)  // no such file exist
    {
        printf("Image does not exist\n");
        return;
    }
    printf("%s opened.\n", file);
    fseek(fileptrr, 3, SEEK_SET);             // jump_boot_bpb ==> size - 3 bytes, offest - 0   // not interested in 0 to 3 bytes
    fread(&name_bs, 8, 1, fileptrr);       // name_bs ==>  siZe 8 bytes, offest - 3 // stored in char array

    fseek(fileptrr, 11, SEEK_SET);            // take file pointer to 11th byte
    fread(&bytes_per_sector_bpb, 2, 1, fileptrr);  // bytes_per_sector_bpb ==>  size 2 byte, offest - 11  // Count of bytes per sector
    fread(&Sector_pre_Cluster_bpb, 1, 1, fileptrr);   // Sector_pre_Cluster_bpb ==> size 1 byte, offest - 13  // Number of sectors per allocation unit.
    fread(&Reserved_sector_count_bpb, 2, 1, fileptrr);   // Reserved_sector_count_bpb ==> size 2 byte, phy_addr - 14 // Number of reserved sectors in the Reserved region of the volume starting
    fread(&Number_of_FAT_bpb, 1, 1, fileptrr);      // Number_of_FAT_bpb ==> size 1 byte, phy_addr - 16 // Count of FAT data structures on the volume.
    fread(&Root_Entry_Count_bpb, 2, 1, fileptrr);   // Root_Entry_Count_bpb ==> size 2 byte, phy_addr - 17 // contains the count of 32-byte DIR entries in the root DIR

    // now table changes in documentation go to table 3 page 12

    fseek(fileptrr, 36, SEEK_SET);            // take file pointer to 36th byte
    fread(&FAT_size_32_bpb, 4, 1, fileptrr);      // FAT_size_32_bpb ==> size 4 byte, phy_addr - 36 // count of sectors occupied by ONE FAT

    fseek(fileptrr, 44, SEEK_SET);            // take file pointer to 44th byte
    fread(&Root_Cluster_bpb, 4, 1, fileptrr);     // Root_Cluster_bpb ==> 4 byte, phy_addr - 44 //  set to the cluss number of the first cluss of the root DIR
    current_dir = Root_Cluster_bpb;    // contains cluss number of root dir

    i64 phy_addr = logical_to_physical(current_dir);  // get phy_addr no. of root dir
    fseek(fileptrr, phy_addr, SEEK_SET);                 // take fileptrr to root dir pointer
    fread(&dir[0], 32, 16, fileptrr);                  // 
}

void closeImgFile()  // closes the currently opened image file
{
    if (fileptrr == NULL)   // if file is not opened
    {
        printf("file is close !!");
        return;
    }
    //now close the file pointer
    fclose(fileptrr);
}

void vol()
{
    fseek(fileptrr, 71, SEEK_SET);
    fread(&vol_bpb, 11, 1, fileptrr);
    printf("name of volume: %s\n", vol_bpb);
}

void format_dir(char *directory_name)  // converts to capital letters  microsoft - 8.3 filename syster where 8 is filename and 3 is for extension
{
    char modified_name[12];
    memset(modified_name, ' ', 12);   // initiliaze with whitespace

    char *BUFER = strtok(directory_name, "."); // separated by delimeter "." and stored in BUFER array 

    if (BUFER)
    {
        strncpy(modified_name, BUFER, strlen(BUFER));  // copies BUFER to modified_name 

        BUFER = strtok(NULL, ".");

        if (BUFER)
        {
            strncpy((char *)(modified_name + 8), BUFER, strlen(BUFER));  // take only first 8 characters of modified_name
        }

        modified_name[11] = '\0';

        i64 i = -1;
        while(++i < 11)
        {
            modified_name[i] = toupper(modified_name[i]);  // converting everything to uppercase
        }
    }
    else
    {
        strncpy(modified_name, directory_name, strlen(directory_name));
        modified_name[11] = '\0';
    }
    strncpy(final_modified_dir, modified_name, 12);    // ALL capital letters with 8 filename and 3 for extension length
}
/*
This function formats the proper naming convention for the file directories in order to store it. 
This should be in capital letters (max 8 characters) and followed by 3 characters of extension. 
*/

i_32 obtain_clus(char *dir_name)
{
    format_dir(dir_name);  // converts all letter to capital
    i64 dirs = -1;
    while(++dirs < 16)
    {
        char *DIR = malloc(11);   // allocate 11 bytes
        memset(DIR, '\0', 11);    // set all to '\0'

        memcpy(DIR, dir[dirs].dir_name, 11);   // copies dir name to variable

        if (strncmp(DIR, final_modified_dir, 11) == 0)   // compares original name and given name
        {
            i64 Cls = dir[dirs].dir_first_cluster_low;     // initial pointer of that dir
            return Cls;
        }
    }
    return -1;  // if no such file is present
}
/*
we first get the formatted name of the directory. Then  traverse, and get the formatted name as well. 
If it formatted name matches with the formatted name of the directory , then we have got a match.
*/

void statistics(char *directory_name)
{
    i64 cluss = obtain_clus(directory_name);         // obtains cluss
    printf("obtained size: %d\n", find_clus_size);    // gets size of cluss
    i64 dirs=0;
    while(1)
    {
        i64 x = cluss;
        i64 y =  dir[dirs].dir_first_cluster_low;
        if (x == y)    // finds its cluss
        {
            printf("dispalying attrib: %d\n", dir[dirs].dir_attribute);
            printf("clust. start: %d\n", cluss);
            printf("clust. end: %d\n", dir[dirs].dir_first_cluster_high);
        }
        dirs++;
        if(dirs==16)
            break;
    }
}
/*
the cluster is obtained from the given directory name and then obtain information from the given statistics like 
cluster low, attribute, cluster high and everything that is stored as the property of the directory.
*/

i_32 find_clus_size(i_32 cluss)           // returns size of provided cluss
{
    i64 dirs = -1;
    while(++dirs < 16)                        // traversing all dir
    {
        if (cluss == dir[dirs].dir_first_cluster_low)  // finds correct cluss
        {
            i64 size = dir[dirs].dir_file_size;         // gets size of that dir
            return size;
        }
    }
    return -1;
}
/*
if the cluster number of directory under consideration matches with the cluster number passed as argument,
then we have gotten a match. We return the size of the cluster
*/

void cd(i_32 cluss)                                   // "cd" implemented
{
    char *dotdot = "..";
    i64 x = strcmp(BUFER[1], dotdot);
    i64 flag = 0;
    if (x == 0)                                   // if it command is "cd .."
    {
        i64 dirs=0;
        while(1)
        {
            i64 y = 2; 
            x=strncmp(dir[dirs].dir_name,dotdot,y);
            if (x == 0)                  // finds cluss for ..
            {
                fseek(fileptrr, logical_to_physical(dir[dirs].dir_first_cluster_low), SEEK_SET);     // moves pointer to phy_addr
                fread(&dir[0], 32, 16, fileptrr);                                         // read that content
                flag=1;
                break;
            }
            dirs++;
            if(dirs==16)
                break;
        }
    }
    if(flag==0)
    {
        fseek(fileptrr, logical_to_physical(cluss), SEEK_SET);          // moves pointer to phy_addr
        fread(&dir[0], 32, 16, fileptrr);           // read that content
    }
    else
        return;
}
/*
there are two pointers like ".." and ".". " .." is associated with parent directory
So the function after validating the input, gets the offset of parent directory. Changes the pointer and then read
*/

void read_file(char *directory_name, i64 pos, i64 number_of_bytes)  // reads file starting from the given pos upto the number of bytes mentioned
{
    fseek(fileptrr,  logical_to_physical(obtain_clus(directory_name)) + pos, SEEK_SET);   // moves pointer from where we need to read the file
    char DATA[14];
    fread(DATA, number_of_bytes, 1, fileptrr);          // reads the entire content till size numofbytes
    printf("%s\n", DATA);
}
/*
We call the getCluster function. So we get the offset. We move the pointer to that file 
help of that fseek function. Now read the data of the file and store and print this array.
 */

void get_dir_info()    // prints information
{
    i64 dirs=0;

    while(1)
    {
        fread(&dir[dirs], 32, 1, fileptrr);
        dirs++;
        if(dirs==16)
            break;
    }
}
/*
This function is used to implement the `info` command on an already opened `.img` file.
*/

void print(char *dir)
{
	i64 itr=0;
    i64 last_pointer = 11;
	for(itr=0; itr<last_pointer; itr++)
	{
		if((dir[itr]>='A' && dir[itr]<='Z'))
			printf("%c", dir[itr]);	
        if ( (dir[itr]>='0' && dir[itr]<='9') )
            printf("%c", dir[itr]);
        if(dir[itr]==' ')
            printf("%c",dir[itr]);
        else
        {
            continue;
        }
	}
	printf("\n");
}
void printing_dir(i64 dirs)
{
    char *DIR = malloc(11); // end the DIR with ending symbol
    DIR[11] = '\0';
    i64 itr = -1;
    while(++itr < 11)
        *DIR[itr] = dir[itr].dirs_name; //copy the DIR name
    
    print(DIR); //pint
}
void ls()  // works as "ls" command
{    
    i64 phy_addr = logical_to_physical(current_dir);  // get phy_addr for current dir
    fseek(fileptrr, phy_addr, SEEK_SET);  // moves pointer to phy_addr

    i64 dirs = -1;
    while(++dirs < 16)  // traversing through all directories
    {
        fread(&dir[dirs], 32, 1, fileptrr);  // reading ith DIR

        if ((dir[dirs].dir_name[0] != (char)0xe5))
        {
            if((dir[dirs].dir_attribute == 0x1 || 
                dir[dirs].dir_attribute == 0x10 || 
                dir[dirs].dir_attribute == 0x20 )
            {
                printing_dir(dirs);
            }  
        }
    }
}
/*
we need offset and then we are pointing the file pointer to and along with this, directory attributes are also required 
for the validation purpose.
*/

void decimal_to_hexadecimal(i64 dec)
{
    i64 n=dec,i,itr = 0, adder;
    char output[50];

    while(1) 
    {
        adder = 0;
        if(n<=0)
            break;  
        adder=n%16; 
        if(temp >= 10) 
            output[itr] = adder + 55;
        else
            output[itr] = adder + 48;
        itr++;  
        n = n/16; 
    }  
    for(i=itr-1; i>=0; i--) 
        printf("%c",output[i]); 
}