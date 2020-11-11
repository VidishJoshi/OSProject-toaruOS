
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

1. FAT #1 starts at address BPB_RsvdSecCnt * BPB_BytsPerSec
2. Each FAT has 1 32-bit word for every cluster. Each entry is the logical block of the next
block in the file.
3. Clusters start at address (BPB_NumFATs * BPB_FATSz32 * BPB_BytsPerSec) +
(BPB_RsvdSecCnt * BPB_BytsPerSec)


![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/1.PNG)
![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/2.PNG)
![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/3.PNG)
![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code1.jpeg)
![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code2.jpeg)
![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img4/code3.jpeg)


## First character of the filename:

0x00 - Filename never used.
0xe5 - The filename has been used, but the file has been deleted.
0x05 - The first character of the filename is actually 0xe5.
0x2e - The entry is for a directory, not a normal file. If the second byte is
also 0x2e, the cluster field contains the cluster number of this directory's
parent directory. If the parent directory is the root directory (which is
statically allocated and doesn't have a cluster number), cluster number
0x0000 is specified here
