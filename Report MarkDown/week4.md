**Operating System Lab - CSE 341**

**Faculty**: *Prof. Mansukh Savaliya*

**Project Progress Report**: *Week 4*

**Group Number**: *3*

**Group Members**:

|     Name      | Enrolment Number |
| :-----------: | :--------------: |
| Yashil Depani |    AU1841005     |
| Vidish Joshi  |    AU1841019     |
|  Manav Patel  |    AU1841037     |



## OSProject - toaruOS

An operating systems project on understanding and solving issues for the open source OS - [toaruOS](https://github.com/klange/toaruos).



During this week, our main focus was to go head with the coding part and develop the program which allows us to open the image file and also gives us the pointers to the root directory location in the File Allocation Table to be able to operate on it.



### Necessary relationships between fields to access the regions of the FAT32 configured image file:

1. FAT #1 starts at address BPB_RsvdSecCnt * BPB_BytsPerSec

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/1.PNG)

2. Each FAT has 1 32-bit word for every cluster. Each entry is the logical block of the next
   block in the file.

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/2.PNG)

4. Clusters start at address (BPB_NumFATs * BPB_FATSz32 * BPB_BytsPerSec) + (BPB_RsvdSecCnt * BPB_BytsPerSec)

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/3.PNG)

We use these relationships to create functions which allow us to open the `.img` file and store all the fields and necessary offsets with their appropriate size in variables.



### Created Functions:

This the structure of a directory. We store all the information of a *dir* in a variable of this structure to access the its properties such as FileSize, DIrName, FirstCluster properties, etc. as shown in the image below,

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code1.jpeg)

When we retrieve the location of directories from the Allocation Table, the location address follows *Logical Block Addressing* method which is useful for the machine to locate directories. We convert it into readable form of memory offset.

This conversion can be understood and achieved from the relationships shown in the first section and the *documentation*.

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code2.jpeg)



This is the `openImage` function that allows us to open the `.img` file containing the FAT configured drive. As described in the previous weekly reports regarding the offsets in the Allocation table, their sizes and their importance, we store these necessary values after opening the file in appropriate data structures using *file pointer* and related functions such `fseek` and `fread`.

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code3.jpeg)





This function is used to implement the `info` command on an already opened `.img` file. This function prints all the important fields as mentioned in the documentation that we stored in variable after opening the image file.

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code5.PNG)



This code is maintained in [this](https://github.com/VidishJoshi/OSProject-toaruOS) GitHub repository.

### Plan for next week

To summarize, with the help of the above code, we are able to open a *FAT32* configured image file and access the directory structure stored in the File Allocation Table inside it. Our next aim is to open, read and print this directory structure with its directories, sub-directories and files.



## Contribution

Writing and implementing above code and functionalities - All 3 members