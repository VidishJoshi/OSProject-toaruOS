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
3. [The CODE](#The-CODE)
4. [Planning for next week](#Planning-for-next-week)
5. [Contribution](#Contribution)

## Printing Directories

This function 

## Formating Directories

This function formats the proper naming convention for the file directories in order to store it. This should be in capital letters (max 8 characters) and followed by 3 characters of extension. 

This is based on fact that name of a file/directory actually consists of 2 names: The old (short) 8.3 convention and the new (long) 255-character name. This function updates both these names whenever you give a name to the file. The short name is created automatically, based on the first characters of the long name (e.g. SyncToy_4264be.dat becomes SYNCTOY~1.DAT). If the name fits into the old naming convention, the long name is discarded.
Hence through this function, it converts all characters to uppercase.
