#include <stacsos/console.h>
#include <stacsos/memops.h>
#include <stacsos/objects.h>
#include <stacsos/user-syscall.h>
#include <stacsos/syscalls.h>
#include <stacsos/string.h>
#include <stacsos/list.h>

using namespace stacsos;

/**
 * @brief main function - checks what flags are included in the command line and 
 *                        calls the syscall with the appropriate boolean values and the path of the directory as arguments.
 * 
 * @param cmdline - command line arguments (flags and path of directory).
 * @return int - returns 0 if ls command is successful, if one of the flags entered is not one supported by this function or 
 * the path entered is incorrect/directory isn't found, then function prints error message and returns 1.
 */
int main(const char *cmdline)
{
	if (!cmdline || memops::strlen(cmdline) == 0) {
		console::get().write("error: usage: /usr/ls <-la> <path>\n");
		return 1;
	}

    //Argument parsing
    string cmdline_string = string::format(cmdline);
    list<string> args = cmdline_string.split(' ', true);

    bool is_a = false; //Reveals hidden files
    bool is_l = false; //Long listing
    bool is_U = false; //Unsorts directory listing
    bool path_ind = 0;
    for (int i = 0; i < args.count(); i++) {
        if (args.at(i).c_str()[0] == '-') {

            for (int j = 1; j < args.at(i).length(); j++) {
                if (args.at(i).c_str()[j] == 'l') {
                    is_l = true;
                }
                else if (args.at(i).c_str()[j] == 'a') {
                    is_a = true;
                }
                else if (args.at(i).c_str()[j] == 'U') {
                    is_U = true;
                }
            }
        }
        else if (args.at(i).c_str()[0] == '/') {
            path_ind = i;
        }
    }

    //Creates an object which invokes a syscall.
    object* dir = object::listdir_(args.at(path_ind).c_str(), is_l, is_a, is_U);
    if (!dir) {
        console::get().writef("ls: No such file or directory or flags incorrect\n");
        return 1;
    }

    char buffer[4096];
    int bytes_read = dir->read(buffer, sizeof(buffer) - 1);
    console::get().writef("%s", buffer);
    delete dir; //Want to save memory so object is deleted once ls is done.

    return 0;
}