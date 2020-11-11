
**Operating System Lab - CSE 341**

**Faculty**: *Prof. Mansukh Savaliya*

**Project Progress Report**: *Week 2*

**Group Number**: *3*

**Group Members**:

|     Name      | Enrolment Number |
| :-----------: | :--------------: |
| Yashil Depani |    AU1841005     |
| Vidish Joshi  |    AU1841019     |
|  Manav Patel  |    AU1841037     |



## OSProject - toaruOS

An operating systems project on understanding and solving issues for the open source OS - [toaruOS](https://github.com/klange/toaruos).

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
