## School of Engineering and Applied Science, Ahmedabad University

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

This repository and read me contains the work done, issues faced and implementation done by us on the original OS. These steps will be
similar in some cases to that of the original OS and different in some others.

##  Index

1. [Understanding FAT32 File Systems](#Understanding-FAT32-File-Systems)
2. [Understanding File Allocation Table](#Understanding-File-Allocation-Table)
3. [Staring the coding part](#Starting-the-Coding-Part)  
4. [Plan for next week](#Plan-for-next-week)
5. [Contribution](#Contribution)
## Understanding FAT32 File Systems

**FAT32 Disk Layout**

FAT is a relatively simple and unsophisticated filesystem that is understood by nearly all operating systems, including Linux and MacOS, so it's usually a common choice for firmware-based projects that need to access hard drives.

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/3.PNG)

*<u>Reserved Region</u>* – Includes the boot sector, the extended boot sector, the file system information sector, and a few other reserved sectors

*<u>FAT Region</u>* – A map used to traverse the data region. Contains mappings from cluster locations to cluster locations. 

*<u>Data Region</u>* – Using the addresses from the FAT region, contains actual file/directory data.

The first step to reading the FAT32 filesystem is the read its first sector, called the Volume ID. The Volume ID is read using the LBA Begin address found from the partition table. LBA is Logical block addressing is a common scheme used for specifying the location of blocks of data stored on computer storage devices. We can read it with LBA = 0.  LBA just numbers the sectors (512 Bytes per sector) sequentially starting at zero.

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/1.png)

Simple C code for above image:
```
fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;

cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FAT);

sectors_per_cluster = BPB_SecPerClus; 

root_dir_first_cluster = BPB_RootClus;

lba_addr = cluster_begin_lba + (cluster_number - 2) * sectors_per_cluster;
```



## Understanding File Allocation Table

The directory entries tell us where the first cluster of each file (or subdirectory) is located on the disk, and of course you find the first cluster of the root directory from the volume ID sector. To access all the other clusters of a file beyond the first one, we need to use the File Allocation Table. 

The File Allocation Table is a big array of 32 bit integers. We want to understand what the entries in this table actually mean. Each cell in this table contains *memory address pointer* to the current directory and and the files which are present inside the directories.

Here are the possible entries in a FAT cell:

* 0 - unallocated
* FF7, FFF7 or FFF FFF7 (12, 16, 32) - bad cluster
* FF8, FFF8, FFF FFF8 (12, 16, 32) - EOF
* All other values mean the cluster is allocated.

Let us understand the following File Allocation Table and the entries inside them:

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/2.PNG)



*Here, all the files and directories shown in the image are present inside the **root** directory.*

*The cells in the table are numbered starting from **0** .  Here in the above table the root directory starts at memory address 2. This  directory has memory link to address 9. This cell has a link to address 10 which has a link to address 11. The cell number 11 end-of-file.*

*From memory address 3 starts File #1. Cell 3 has link to 4, which links to cell 5. Cell 5 has link to cell 7. Notice that cell number 6 is skipped and it contains 000000 as entry. This shows that cell 6 is unallocated. Cell 7 links to cell 8. Cell 8 is the end-of-file. Similarly we can observe for the rest of the files.*



We installed [**Bless**](https://github.com/afrantzis/bless) Hex editor to read binary files in a FAT32 image to read the file structure maintained in the File Allocation Table in that image.

Below is the image when a FAT image file is viewd in *Bless* editor:

![fatImageBless](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/4.jpeg)



## Starting the Coding Part

One of our main aim for this week was to start the coding part *or* at-least understand the concepts necessary to start the coding for the project.

FAT32 file system, which was developed by Microsoft for the DOS OS, has a set of standards which are followed while using this FS. The File Allocation Table is created and maintained according to particular set of guidelines which have been found to maximize the performance of this FS.

All this standards have been written in the [Microsoft Documnetation of FAT32](http://read.pudn.com/downloads77/ebook/294884/FAT32%20Spec%20%28SDA%20Contribution%29.pdf). 

**Steps to read from a FAT32 image:**

1. Locate, read, and extract important info from the Boot Sector
2. Locate the Root Directory, get the list of files and folders
3. Access the files and directories using information from the Root Directory and the FAT32 table

Various attributes that are required to maintain the table for each region of the memory are defined and documented in detail in this manual. Below are images from the manual the fields used in the *BIOS Parameter Block(BPB)* located in the first sector of the volume in the *Reserved Region.* 

![BPB](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/5.PNG)

![BPB2](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img2/6.PNG)

The code should work along with these attributes and fields since these fields will hold the information about the file structure.

## 

## Plan for next week

Read and understand the Microsoft documentation for FAT32 system.

Along with it, start coding the blocks that are understood. Refer to the documentation whenever necessary. 

The following link is also useful along with the documentation as it explains concepts very clearly:

* [https://www.pjrc.com/tech/8051/ide/fat32.html](https://www.pjrc.com/tech/8051/ide/fat32.html)

This YouTube lecture series is good. Great explanation + implementation is done in this series. Refer this in the coming week.

* [https://www.youtube.com/playlist?list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M](https://www.youtube.com/playlist?list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M)



## Contribution

Reading/understanding the Microsoft documentation - All 3 members

Starting to write the code - All 3 members

Understanding the working of a FAT table - All 3 members

Working around creating and manipulating .img file - All 3 members
