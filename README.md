# OSProject - toaruOS
An operating systems project on understanding and solving issues for the open source OS - [toaruOS](https://github.com/klange/toaruos).

This repository and read me contains the work done, issues faced and implementation done by us on the original OS. These steps will be similar in some cases to that of the original OS and different in some others.

## Installation guide
This is the installation guide





## Issues

### FAT Driver Issues 

ToaruOS has a number of running issues, the most significant of which is File System issues. There are various issues currently open regarding File Systems in it's GitHub repository such as [this](https://github.com/klange/toaruos/issues/174) , [this](https://github.com/klange/toaruos/issues/172) and [this](https://github.com/klange/toaruos/issues/171). 

In the initial stage, we have decided to solve this issue of File System drivers through FAT file system. For this, we first get an understanding of FAT file system.

#### Understanding FAT32 Drivers

We watched the following YouTube videos to understand FAT32 Drivers.

	* [https://www.youtube.com/watch?v=_h30HBYxtws](https://www.youtube.com/watch?v=_h30HBYxtws)
	* [https://www.youtube.com/watch?v=HjVktRd35G8](https://www.youtube.com/watch?v=HjVktRd35G8)



File Systems are the standards for organizing data on storage devices and they are applied when one formats a drive or partition. 

The choice of these drivers is based on the type of OS one is working on and the type of drive one is formatting.  *For example*, in windows if we want to format the drive, we can chose NTFS file system of exFAT. Whereas in Linux, if we want to format a drive, we can chose from FAT32, NTFS or EXT4 drivers.



***Differences between File Systems:***

**FAT12, FAT16 or FAT32:**

FS divide storage space of the drive into virtual compartments or *clusters* and maintain an *index* of where individual files are located and available free space. The 1st DOS windows file system was known as *<u>File Allocation Table = "FAT"</u>* 



|       | Max File Size         | Max Volume Size                                          |
| ----- | --------------------- | -------------------------------------------------------- |
| FAT12 | 32 MB (8 KB clusters) | 32 MB (8 KB cluster)                                     |
| FAT16 | 2GB / 4GB             | 16GB (256 KB cluster)                                    |
| FAT32 | 4 GB                  | 32 GB (Windows format), 2 TB other OS, 16 TB theoretical |



FAT32 is very popular due to its wide compatibility with various OS and is widely used for formatting removable media.

**NTFS**

Currently, the most popular windows file system is the *<u>New Technology File System - NTFS</u>*. File size limit of 16 exabytes. NTFS is a journalling file system. NTFS also allows file permissions unlike FAT32. Limited OS compatibility.

**exFAT**

*<u>Extended File Alliocation Table - exFAT</u>*

File system optimized for high capacity USB flash drives and memory cards. Maximum File Size = 16 exabytes. Wider different OS support than NTFS. 

**ext2, ext3, ext4**

*<u>Extended File System (ext))</u>* was launched for linux. ext3 introduced journalling. Maximum file size upto 16TB and maximum volume size of upto 1 EB. NO OS except Linux supported. 



***Choosing a FS:***

For a system drive, chose on the basis of OS. NTFS for windows, ext4 for Linux, etc.

For USB drives and removable drives under 32 GB, use FAT32 for cross-platform support.

exFAT for removable drives for capacity greater than 32 GB capacity or for storing files greater than 4 GB.