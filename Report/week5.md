**Operating System Lab - CSE 341**

**Faculty**: *Prof. Mansukh Savaliya*

**Project Progress Report**: *Week 5*

**Group Number**: *3*

**Group Members**:

|     Name      | Enrolment Number |
| :-----------: | :--------------: |
| Yashil Depani |    AU1841005     |
| Vidish Joshi  |    AU1841019     |
|  Manav Patel  |    AU1841037     |



## OSProject - toaruOS

An operating systems project on understanding and solving issues for the open source OS - [toaruOS](https://github.com/klange/toaruos)


##  Index

1. [Printing Directories](#Printing-Directoryies)
2. [Formating/(proper naming convention) Directories](#Formating-Directories)
3. [Change the directory](#change-the-directory)
4. [Reading File](#Reading-File)

## Printing Directories

This function 

## Formating Directories

This function formats the proper naming convention for the file directories in order to store it. This should be in capital letters (max 8 characters) and followed by 3 characters of extension. 

This is based on fact that name of a file/directory actually consists of 2 names: The old (short) 8.3 convention and the new (long) 255-character name. This function updates both these names whenever you give a name to the file. The short name is created automatically, based on the first characters of the long name (e.g. SyncToy_4264be.dat becomes SYNCTOY~1.DAT). If the name fits into the old naming convention, the long name is discarded.
Hence through this function, it converts all characters to uppercase.

## Change the directory

This function does the work of " cd .. " command. For every directories in img file, there are two pointers like ".." and ".". " .." is associated with parent directory of that current directory. So whenever user writes "cd .." , the function after validating the input, gets the offset of parent directory. Changes the pointer to that diectory through fseek and reads that pointer so now we are in that parent directory.

## Reading File

Thsi function does the work of reading the file. For this we pass the filename as the argument. We call the getCluster function to know the cluster of that file. With the help of this cluster we get the offset. We move the pointer to that file through that offset with the help of that fseek function. Now we read the data of the file and store it in a character array and finally print this array.