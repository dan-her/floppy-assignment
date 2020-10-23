This project creates an environment for the user to mount and analyze a floppy disk.
Contribution:
    Peter Bernacki (2718581):
        Orginal basic program strcuture
        help command
        fmount
        fumount
        exit
        small debugging progress on traverse
        showfat
        majority of readme
        learning from Dan because he's really smart:)

    Dan Hercik :



To compile and execute:
    make
    ./floppyBrowser
Test runs

fmount/fumount:
    p@p-X510UAR:~/CLionProjects/floppy-assignment$ ./floppyBrowser
    enter floppy command: fmount /home/p/Downloads/imagefile.img
    Image at /home/p/Downloads/imagefile.img mounted.

showsector:
enter floppy command: showsector 1
         0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
    0    F0  FF  FF  0   40  0   FF  6F  0   FF  FF  FF  9   F0  FF  FF
    10   CF  0   FF  FF  FF  FF  F   1   11  F0  FF  FF  FF  FF  15  F0
    20   FF  17  F0  FF  FF  FF  FF  FF  FF  FF  1D  F0  FF  1F  0   2
    30   FF  FF  FF  0   0   0   0   0   0   0   0   0   0   0   0   0
    40   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    50   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    60   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    70   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    80   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    90   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    A0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    B0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    C0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    D0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    E0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    F0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    100  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    110  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    120  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    130  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    140  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    150  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    160  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    170  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    180  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    190  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1A0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1B0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1C0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1D0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1E0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
    1F0  0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0

help:
    enter floppy command: help
    fmount                          mounts a local floppy disk from the specified .img file
    fumount                         unmount the mounted floppy disk
    structure                       lists the structure of the floppy disk
    traverse [-l]                   lists the content in the root directory. switch l shows all detailed information about a file
    showsector [sector number]      show the content of the specified sector (512 bytes of hex dump)
    showfat                         shows the content of the first 256 entries of the FAT table (as a hex dump)
    showfile [filename]             shows the content of the file (as a hex dump)
    quit                            quits the Floppy Disk Browser shell


structure:
    enter floppy command: structure
    number of FAT:                      2
    number of sectors used by FAT:      9
    number of sectors per cluster:      1
    number of ROOT Entries:           224
    number of bytes per sector:       512
    ---Sector #---     ---Sector Types---
    0                   BOOT
    1 -- 9                          FAT1
    10 -- 18                        FAT2
    19 -- 32                        ROOT DIRECTORY

traverse:
    enter floppy command: traverse
    /A.CLA
    /ALOOP1.CLA
    /ALOOP1.JAV
    /ALOOP2.CLA
    /ALOOP2.JAV
    /ALOOP3.CLA
    /ALOOP3.JAV
    /B.CLA
    /CONDIT.CLA
    /CONDIT.JAV
    /D.CLA
    /DEMO.CLA
    /DEMO.JAV
    /DIR1           <DIR>
    /DIR1/.         <DIR>
    /DIR1/..                <DIR>
    /DIR1/DIR2              <DIR>
    /DIR1/DIR2/.            <DIR>
    /DIR1/DIR2/..           <DIR>
    /DIR1/DIR2/FILE3
    /POLYTEST.CLA
    /POLYTEST.JAV


showfat:
    enter floppy command: showfat
        0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
    0   FREE ffffffff  ffffffff  FREE FREE FREE ffffffff  6f  FREE ffffffff  ffffffff  ffffffff  9  FREE ffffffff  ffffffff
    10  ffffffcf  FREE ffffffff  ffffffff  ffffffff  ffffffff  f  1  11  FREE ffffffff  ffffffff  ffffffff  ffffffff  15  FREE
    20  ffffffff  17  FREE ffffffff  ffffffff  ffffffff  ffffffff  ffffffff  ffffffff  ffffffff  1d  FREE ffffffff  1f  FREE 2
    30  ffffffff  ffffffff  ffffffff  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    40  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    50  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    60  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    70  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    80  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    90  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    A0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    B0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    C0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    D0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    E0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    F0  FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE



bugs:
    Showfat:
    couldn't get showfat to only print 3 digits, had to do with bit shifting, ran out of time to work on it.
    Showfile:
    no progress at time of writing readme

