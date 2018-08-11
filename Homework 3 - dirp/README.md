**Q: What is this program?**

A: This is a recreation of the ls system command with the -l flag included. It is also equivalent to the "dir" system command for Microsoft Windows' Command Prompt / CLI. It only tests for whether or not a file is a **dir**ectory or not (hence **dir**p). 

The ls system command opens a directory and writes out the contents of the directory to stdout. To explain, folders / directories for a computer are a file just as much as a .txt file is a file.

Directories specifically are files that contain records of other files. The way that the operating systems like Unix are able to determine what *type* of file a file is, is by the OS creating some meta-data for every file it creates. Part of this meta-data is a flag for the type of file. A 'd' flag indicates that a file is a directory and this 'd' is positioned in front of file permissions, which can be inspected via the "long" flag or "-l" (lowercase L) argument for the ls system command.

These file permissions are arranged in the following order:

**Root | Group | User**

rwx rwx rwx

r = read permissions are set (this user[s] can read this file)

w = write permissions are set (this user[s] can modify this file. Modification includes deletion!)

x = executable permissions are set (this user[s] can run this file. The contents of this file can be loaded into a CPU to be computed)

"Root" refers to the "root user", which is the first user account created by the Unix OS (Linux too). This user account is the equivalent to the "god" of the system, an account that has absolute power: the ability to access any file (therefore any directory too) and the ability to destroy any file. Some operating systems such as certain distributions of Linux (a distribution is a different OS. Linux is a family of operating systems that share the same kernel source code but their system calls and internal construction outside of the kernel may differ) utilize the terminology "Superuser" or "SU" to also refer to the root user and Microsoft Windows uses the terminology "Administrator" for the root user.*

"Group" refers to a collection of user accounts. For example, some websites have subscription content that is not accessible to individuals who are not subscribers. So the accounts that *are* subscribed would be placed into their own group.

Finally "user" refers to the user account that an individual is currently signed in as. 

In the case of directories, Unix appends a lowercase D or "d" to the front of the permissions to indicate if a file is a directory or not. Similarly, other files such as shortcuts (known as "symbolic links" in Unix/Linux) also have special file type identifiers in their meta-data. I'm not going to list out all of the file type identifiers but if you are interested, I will have a link at the bottom to see the various file type identifiers that Linux provides!

Because directories are not human-readable files like text files, Unix and Linux provide an alternative to the open() system call known as "opendir()". The opendir() system call takes the parameter of a DIR pointer or "directory pointer". If a DIR pointer is *not* passed into opendir() then opendir() will return a NULL pointer. Hopefully you are aware of pointers and null pointers but if not I will briefly explain them in the notes at the bottom.

A dedicated read system call for directories is also provided: readdir() (pronounced "reader"). readdir() takes in the parameter of a dirrent* pointer or "directory entry" pointer. This data type is utilized instead of the DIR* pointer for two reasons:

1. It allows us to logically separate directories from the *contents* of the directory.

2. It allows us to logically separate directory *meta-data* from the contents of the directory.

For a helpful analogy, you can think of DIR* variables as being like aisle signs that tell you what aisle you are currently on or currently standing in front of. A dirent* variable on the other hand is like the tag on an aisle shelf underneath an item that indicates both what the name of the item is and what the price of the item is.

Meta-data for directories is maintained through stat pointers where "stat" stands for "statistics". The "mode" of the file indicates the file type identifier. The reason for the use of the term "mode" for this identifier has to do with a concept known as the "inode" of a file. In Unix and Unix-like systems such as Linux distributions, files of a given type are usually given a definitive dimension in terms of how many bytes of memory the file contains. This is known as "paging" because like the pages of a book, the pages of memory are easier to traverse if they are definitive in size.

We don't have to check the size of the file when we want to traverse memory to find a file (or to load the contents of a file into RAM to be accessed by the CPU from storage memory like a hard drive or Solid State Drive (SSD)) if we use paging, although paging brings about a new kind of problem known as internal fragmentation (chunks of memory that we can't write any values to. We call this problem also a "memory leak") so another alternative exists known as segmentation which allows varying sizes. Files though are typically uniform in size, such as each file being 4096 bytes (or 4kB).

Like the pages of a book, if we overrun content from a memory page we can write the contents on a new page! But pages of information don't need to be sequential. There's no problem (aside from extra memory being used for pointers. Pointers are all also uniform in size. Every pointer is the same size as the system bus between RAM and the CPU. To know how programming languages such as C or C++ keep track of the data type, see the notes at the bottom) with doing this approach and for writing operations it's actually *better* to not use defined allocations like in the case of using defined sizes for memory pages.

Reading data from defined-sized allocations is an O(1) time complexity so things like arrays, memory pages, etc. are much better options whenever we're dealing with a reading operation (like outputing the contents of a file to a screen). Writing on the other hand is an O(1) time complexity when we can just check if memory is available and then write the contents to memory (along with a pointer to keep track of a page's location in memory).

Files contain several inodes that indicate the memory location of a file on a storage device like your hard drive or SSD. These inodes are also kept organized in an array of values the operating system creates via its kernel. There is a theoretical limit to the number of files that can be exist on a storage device regardless of there being any phyiscal free memory still being available if every inode is used however the *chances* of this happening are astronomically low due to the structure of the inode table.

FILE METADATA DIAGRAM (ext4 file system :: file metadata)
---------------------

[ Mode ][Owner info][Timestamps][File Size][Direct Blocks][Indirect Blocks][Double Indirect][Triple Indirect]

**Mode** - File permissions and file type identifier are all contained here.
**Owner info** - 2 owners are attached to every file in ext4: root user and a general user that asked for the file to be created with creat() system call.
**Timestamps** - 3 timestamps in total for ext4. These are used to help indicate when the file was created, when the file was most recently viewed, and when the file was most recently modified.
**File Size** - Total number of bytes actually used by the file (ext4's minimum file size is 4096 bytes or 4 KiB). 
**Direct Blocks** - A collection of 12 file pointers that point to where in a storage drive the file actually exists. 
**Single Indirect** - Pointer to an array of additional file pointers (NULL pointer if unused)
**Double Indirect** - Pointer to another array of additional file pointers (NULL pointer if unused)
**Triple Indirect** - Pointer to another array of additional file pointers (NULL pointer if unused)

NOTES
-----

\* - Note that file permissions (also typically called "privileges") can be modified. This is done through a system call known as "chmod" (Change mode). With chmod, one can add or revoke permissions as they so choose and the root user's permissions can be altered as well. However this is a bit redundant as the root user can access chmod and change permissions at any time. As such, it's important to never give access to the root user to anyone who you wouldn't trust! 

Also some Linux distributions such as Ubuntu provide some helpful system commands to prevent abuse of the root user. A "sudo" command or "super user do" (which is what it stands for) allows you to instruct the root user account to perform some action. On systems with sudo or similar system commands it is highly recommended you utilize these system commands whenever you need to invoke the root user rather than signing in as the root user. 

\* - *What's the point of pointers?*

In programming languages we have varying types of data we work with. Sometimes we want to crunch numbers, other times we want to read text, some times we want to check to see if something is true or not (like "is user X currently signed on?"), and much more. All of this needs to be stored in a binary format which means that at the machine level the concepts of "text" versus "truthness" (known as boolean values) does not exist. It's all 1s and 0s. More importantly the number of possibilities or "permutations" we want for something (like the number of characters that exist on a keyboard versus the amount of integers we would like to count up to) differ.

In order to keep our logic sound (you can't "add" a number to a letter or at least when you do "add" a number to a letter it's a different *kind* of adding we're doing. This kind of adding is known as "concatenation"), we have special data types known as pointers!

Pointers do two things for us:

1. They keep track of a memory location

2. They tell us how *many* bytes of memory a given data type is composed of.

For example, in the C programming language a char data type or "character data type" is one byte in size. On the other hand, the int data type or "integer data type" is typically (but not always) four bytes in size. So a char pointer or char\* would read one byte of memory into the CPU while an int pointer or int\* would read *four* bytes of memory into the CPU.

Pointers typically confuse students because the idea of a memory address containing a memory address sounds confusing but really pointers are just integer values underneath the hood. By using pointers as integers we can perform arithmetic operations on them although pointer arithmetic is slightly different because of the differing size of data types.

Example:

If I am reading an array of characters, I traverse memory byte by byte. So if char[] firstName = {'J', 'o', 'h', 'n'} and 'J' is located at the fictional memory address 1000, then 'o' is at memory address 1001 (because arrays are sequential in memory), 'h' is at memory address 1002, and 'n' is at memory address 1003.

However if I have an array of *integers* and our integers are 4 bytes in size, then the *first* byte of each integer is 4 bytes away from one another. So if int[] myScores = {100, 90, 87, 73} and 100 is at the fictional memory address 2000, then 90 is 2004, 87 is at 2008, and 73 is at 2012.

This means if I have a char\* readMyName initialized to firstName[0], then the readMyName contains the value 1000. Doing readMyName = readMyName + 1 will make it now contain 1001. On the other hand if I have an int\* readMyScores initialized to myScores[0], then readMyScores contains the value 2000. Doing readMyScores = readMyScores + 1 now contains the value 2003.

Although with readMyScores it appears as if I added the number 1 to it, in actuality I added sizeof(int) to it (and we stated that an int data type is typically [but not always!] four bytes in size).  

End of example.

This integer value contained in the pointer data type after the pointer arithmetic has been computed is then dispatched to the MMU or "Memory Management Unit". This device keeps a logical mapping of the system's RAM layout and the integer value is used to determine via the MMU where a given memory address is located.

There also exists the concept of a special type of pointer known as the "NULL pointer". To better understand the idea of null, you should take a course on linear algebra. In general though, most of the time the NULL pointer refers to the memory address 0x00000000 (however a null pointer can be located anywhere in RAM). Whenever a pointer is utilized, the operating system compares the integer value to a NULL value it has computed. Null pointers are always guaranteed to have an equivalent integer value (which is why zero is the typical convention, it's easy to check if a number is equal to zero) and so if the operating system determines that a pointer is equal to the NULL value, it will halt access to that memory location. This is to help ensure that memory cannot be accessed freely and that there can be some security built into the operating system. 

HELPFUL LINKS TO LEARN MORE
---------------------------

File Type Identifiers in Linux: https://linuxconfig.org/identifying-file-types-in-linux

Null pointers: https://en.wikipedia.org/wiki/Null_pointer

Inodes: https://en.wikipedia.org/wiki/Inode

File Systems Explored (SimpleFS): https://www3.nd.edu/~dthain/courses/cse30341/spring2017/project6/ (This one goes into further detail on the structure of inodes)
