##  Index

1. [Understanding FAT32 File Systems](#Understanding-FAT32-File-Systems)
2. [Understanding File Allocation Table](#Understanding-File-Allocation-Table)
3. [Steps to read from a FAT32 image]
(#Steps-to-read-from-a-FAT32-image)  
## Understanding FAT32 File Systems

**FAT32 Disk Layout**

FAT is a relatively simple and unsophisticated filesystem that is understood by nearly all operating systems, including Linux and MacOS, so it's usually a common choice for firmware-based projects that need to access hard drives.

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/img2/3.PNG)

*<u>Reserved Region</u>* – Includes the boot
sector, the extended boot sector, the file
system information sector, and a few other
reserved sectors

*<u>FAT Region</u>* – A map used to traverse the
data region. Contains mappings from cluster
locations to cluster locations

*<u>Data Region</u>* – Using the addresses from
the FAT region, contains actual file/directory
data

The first step to reading the FAT32 filesystem is the read its first sector, called the Volume ID. The Volume ID is read using the LBA Begin address found from the partition table. LBA is Logical block addressing is a common scheme used for specifying the location of blocks of data stored on computer storage devices. We can read it with LBA = 0.  LBA just numbers the sectors (512 Bytes per sector) sequentially starting at zero.

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/img2/1.PNG)

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

The File Allocation Table is a big array of 32 bit integers, Here is a visual example of three small files and a root directory, 

![image](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/img2/2.PNG)

We installed **Bless** editor to view Raw FAT image file.  **Here goes the explanation of img file, i was not able to find the proper explanation.**

It looks something like this . **here goes the image of raw FAT table**. 

## Steps to read from a FAT32 image
1. Locate, read, and extract important info from
the Boot Sector
2. Locate the Root Directory, get the list of files
and folders
3. Access the files and directories using
information from the Root Directory and the
FAT32 table

The [Microsoft Documnetation of FAT32](http://read.pudn.com/downloads77/ebook/294884/FAT32%20Spec%20%28SDA%20Contribution%29.pdf) was very helpful reagrding this. 
