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

##  Index

1. [Reading FAT Table](#Reading_FAT_Table)
2. [Creating/Getting IMG File](#Creating_IMG_FIle)
3. [Implementing IMG File](#Implementing_IMG_File)



## Working on FAT File system

We want to write a code that can operate on FAT File system such as listing files, directories, openining directories, files and also editing them.  

![FatFileSys](https://github.com/VidishJoshi/OSProject-toaruOS/blob/master/Report/img3/1.PNG)

As we can see from the images, it composed of four basic regions, which are laid out in this order on the volume:
	
    1 – Reserved Region
	2 – FAT Region
    3 – Root Directory Region (doesn’t exist on FAT32 volumes)
    4 – File and Directory Data Region

The image has been taken reference from [Microsoft Documnetation of FAT32](http://read.pudn.com/downloads77/ebook/294884/FAT32%20Spec%20%28SDA%20Contribution%29.pdf). 

This documentation contains important and required inforamtion for FAT file system. After understanding this we started our coding part through which we would operate on a File Structure following FAT32 System.
For this we require a directory struture following FAT32 System and so we started working on .img files. This .img files should follow FAT32 File System. Now, how to create/get such files ? 

## Creating/Getting IMG File

Creating .img files following FAT32 File system is not that straight forward. We surfed a lot on this for 2-3 days. One cannot simply convert a directory into .img file. We didn't get any slightest hint on the internet regarding how to create/convert such files. But we have managed to get some .img files online. We also tried with some available online tools such as (here goes tools example) to convert into .img files. It did got converted but the raw structure was not following the above image. For writing the code , we strictly need to stick with on how Mircosoft defines the files structure. Which bytes represent what regions and how to access it. (here goes the image of table of microsoft explaining which sectors represents what and how much Bytes have been allocated). Therefore, we agrred on going with  .img files which we have managed from internet.



## Implementing IMG File


















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