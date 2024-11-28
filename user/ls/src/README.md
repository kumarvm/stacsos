### Additional Features of ls implementation

# -a flag
The -a flag is supported - including this flag reveals any hidden files (files with names starting with '.').
Note that the implied ./ and ../ are not included since STACSOS has no concept of the current directory.

# -l flag
The -l flag is supported - including this flag produces a long listing of the given directory ie. revealing the file type and the file size along with the file name.

# -U flag
the -U is supported - including this flag produces an unsorted listing of the given directory. Note that the default listing is sorted by alphabetical order.

# Flexibility of ls command
The ls supports all combinations of flags:
- /usr/ls /path
- /usr/ls /path -l
- /usr/ls /path -a
- /usr/ls /path -U
- /usr/ls /path -al
- /usr/ls /path -la
- /usr/ls /path -aU
- /usr/ls /path -Ua
- /usr/ls /path -Ul
- /usr/ls /path -lU
- /usr/ls /path -alU
- /usr/ls /path -aUl
- /usr/ls /path -lUa
- /usr/ls /path -laU
- /usr/ls /path -Ual
- /usr/ls /path -Ula

- /usr/ls -l /path
- /usr/ls -a /path
- /usr/ls -U /path
- /usr/ls -al /path
- /usr/ls -la /path
- /usr/ls -aU /path
- /usr/ls -Ua /path
- /usr/ls -Ul /path
- /usr/ls -lU /path
- /usr/ls -alU /path 
- /usr/ls -aUl /path
- /usr/ls -lUa /path
- /usr/ls -laU /path
- /usr/ls -Ual /path
- /usr/ls -Ula /path

- You can also write each flag individually and in any order with the directory path
- eg. /usr/ls / -l -a -U
- eg. /usr/ls -l / -aU