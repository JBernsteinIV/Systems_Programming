**Q: What is this program?**
 
 A: This program is a recreation of the system command "head" which outputs the beginning of a file to stdout. 
 By default the head system command will output the first ten lines of a file. This program, mh, does the same thing.
 This exercise also introduced the class to the issues of switching between user space and kernel space.
 To help explain this context for the reader (i.e. you), please see the below illustration of RAM for the partitioning of user space and kernel space.

          RAM
     +-----------+ 0x00000000
     |   Kernel  |
     |   Space   |
     | (reserved |
     |   by OS   |
     |     at    |
     |    boot)  |
     |-----------| <- Instruction Pointer
     |   User    |
     |   Space   |
     |  (your    |
     | programs  |
     |   live    |
     |   here)   |
     +-----------+ 0xFFFFFFFF
 
 When the computer first boots up, your operating system is loaded into RAM. This is handled via a firmware program known as the "boot loader". 
 The boot loader basically determines which drive to read from to load into RAM as the system starts up. The OS's kernel and various programs the kernel uses (known as system calls) are loaded into the first bytes of RAM.
 Once the kernel and its system calls are all loaded in, the OS will then leave the remaining sections of memory for all of the various programs a user wishes to run (whether that's your web browser, your word processing program like Microsoft Word or Open Office, video games, etc).

 Because the OS does not know ahead of time what a user's program is going to ask for, the OS has to periodically utilize its system calls to determine whether or not it should allow a program to do something. For example, a program may try to modify a file. The file's permissions may indicate that programs/users are allowed to read the file but not to modify the file. As such, the OS needs to utilize one of its system calls to determine whether the current user has the appropriate permissions to modify a file (in this case, the file is set to only be read not modified, and as such, the OS would stop the program from accessing the file). 

 This "switching" between user space and kernel space is time consuming and CPU-intensive. The OS is designed to prioritize system programs (i.e. those system calls I mentioned), meaning the programs a user wishes to run will have less time in the CPU and therefore will take longer to do whatever it is that program is built to do (play a video, run a video game, blah blah blah). I won't get into details of how the OS does this prioritization but if you're curious to learn more then I would point you towards "scheduling algorithms". These algorithms are the program(s) an OS uses in order to determine which programs you have running should go into the CPU next. 

 Another area where this switching between kernel space and user space gets time consuming is in pointer arithmetic. RAM is separated into memory addresses that are eight bits in length each, known as "bytes". In order to load information from RAM into the CPU, the CPU has what's known as an "instruction pointer" (also at times called a "program counter" or "PC" for short. Not to be confused with "personal computer"!). You can think of the PC/instruction pointer as being like a bookmark or the blinking cursor you see when you're typing into a textbox: it tells us where we last left off. By updating the PC, we "move" some amount of bytes in RAM. That byte of RAM (and generally a few more bytes) is then sent to the CPU via a "bus".*

 This pointer arithmetic requires the CPU to take the difference between the LAST memory address it was reading from RAM and some new memory address.
 what_to_read_next = new_address - previous_address

 Instruction_Pointer = previous_address + what_to_read_next

 Example:
 
 //Pointer arithmetic
 
 what_to_read_next = 1500 - 1000
 
 what_to_read_next = 500
 
 //Update the instruction pointer / program counter
 
 Instruction_Pointer = 1000 + 500
 
 Instruction_Pointer = 1500

 So now the CPU will send a signal (binary sequence) across the system bus to RAM to read in the information from the fictional memory address "1500" (fictional because the convention is to denote memory addresses with 0x at the beginning to indicate "this is a memory address". I have chosen to drop this notation for the example to make this pointer arithmetic more digestible / more understandable for a wider audience regardless of knowledge of hardware). If you're wondering "how does the CPU know of where each memory address is physically located?", this is handled via a small chip known as the Memory Management Unit or "MMU" for short. When the computer first boots up, the OS determines what kind of RAM its working with (how many memory addresses exist, the type of RAM, etc) via firmware on the RAM. The MMU then creates a map of the layout of RAM to determine where on the RAM modules a memory address physically exists (it's just like looking at a map of a city to determine which road you need to go down to get to some building). 

 This pointer arithmetic becomes time consuming because negative numbers are the complement of positive numbers. The CPU's programming language, known as an "assembly language", needs to invert all the bits of a number (like "1000" in the above example) in order to determine the complement of a number. This is an O(1) operation which means the amount of time it takes to figure out the inverse of a binary value never changes (it never gets slower and it never gets faster. The amount of time to compute the inverse is a constant value. So O(1) is a notation that means "constant time operation").

 Although this is a constant time operation, it adds up over time. Especially when you factor in the responsibilities of the OS in terms of doing things like checking file permissions / privileges like I explained earlier, it should start to become clear and apparent how this swapping between user space and kernel space slows things down. This long explanation that I've provided is all to make sense of that "#define MAX_SIZE 4096" all the way at the top of this program. That MAX_SIZE definition tells the system how many bytes of memory to read in from a file for the system call known as "read()".

The read() system call has three parameters:
 * fd - file descriptor
 * buf - buffer (which is a char array)
 * MAX_SIZE - How many bytes to read in

A "file descriptor" is Unix terminology to mean a integer number that represents a file. 
For an analogy, imagine you're in a supermarket and you are trying to find where this supermarket stocks potato chips. So you ask a clerk "where are the potato chips?" and the clerk responds "potato chips are on aisle 5". So you would walk through the store looking for a sign that says "aisle 5" and you would go down that aisle to find potato chips.
Similarly, your operating system finds files in your hard drive by using a number. Files do have file names but this is to make files human readable (so you can determine which file you're looking for) but for the computer, it uses numbers. 

The character array "buf" is to determine where to copy the contents of the file. 

MAX_SIZE tells the read() system call how many bytes of the file to read before finishing the read() system call and returning the instruction pointer back into user space (the partition of RAM where your programs are loaded). 

Another important and pivotal piece of information about operating systems and files is we need to do the following steps in order to safely prevent data corruption:

1. Open the file (this is handled in Unix via the open() system call)
2. Read the file (see above for the description of read()).
3. Write to the file (if the file is going to be modified. This is determined based off of the flag we pass into the open() system call. For this particular program, "mh", files are being opened in read only mode, hence the O_RDONLY (the O means "open", RDONLY means "read only").
4. Close the file (via the close() system call).

And one last piece of information about Unix that might be confusing but is an elementary concept about the Unix operating system: everything's a file! Therefore the open(), read(), write(), and close() system calls are the foundational system calls for virtually every program that deals with file input / file output. 

I hope this information has been helpful and has helped you learn more about systems programming. Have a wonderful day!
 
\* - "Why's it called a 'bus'?". If you think about a bus for public transportation, the city bus has a number of seats in it and the bus can carry some finite amount of passengers. Let's say a city bus has 32 seats inside it so the bus can carry up to 32 people at time (we'll exclude the driver for this analogy). Whether or not 32 people get onto the bus, the bus will transfer people from one location (one bus stop) to another location (another bus stop). Similarly, in a computer, the system bus is a series of input channels that sends an electrical signal from RAM to the CPU and vice versa. If a certain voltage is registered (e.g. 0.5 volts), then we say the bit is "on" or 1. If that voltage level is not hit, then we say it is "off" or 0. Depending on the output of each of these channels, the CPU notes down in its registers the sequence of bits that it read from the system bus. Please see below for a helpful illustration if this wording is confusing to you. Also if you'd like to learn more about this and figure out why we need two separate hardware components (CPU and RAM), please go read up on "Von Neumann Architecture" to learn more.

SYSTEM BUS ILLUSTRATION
-----------------------
    +---------------+
    |    Central    |
    |   Processing  |
    |     Unit      |
    |    (CPU)      |
    +---------------+
    |0|1|1|0|1|1|0|1| //System
    | | | | | | | | | //  Bus
    | | | | | | | | |
    +----------------+ 0x00000000
    |    Random      |
    |    Access      |
    |    Memory      |
    |    (RAM)       |
    |    ....        |
    |    ....        |
    |   01101101     | <- Instruction Pointer
    |    ....        |
    |    ....        |
    +----------------+ 0xFFFFFFFF

HELPFUL LINKS TO LEARN MORE
---------------------------

Learning Unix / Linux: http://www.ee.surrey.ac.uk/Teaching/Unix/

Whats the "boot loader"?: https://en.wikipedia.org/wiki/Booting#Modern_boot_loaders

System Bus: https://en.wikipedia.org/wiki/System_bus

Twos Complement (negative numbers): https://en.wikipedia.org/wiki/Two%27s_complement

Von Neumann Architecture: https://en.wikipedia.org/wiki/Von_Neumann_architecture

Memory Management Unit (MMU): https://en.wikipedia.org/wiki/Memory_management_unit
