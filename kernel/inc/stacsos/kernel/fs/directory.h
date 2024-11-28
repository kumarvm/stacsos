#include <stacsos/list.h>
#include <stacsos/string.h>
#include <stacsos/kernel/debug.h>
#include <stacsos/kernel/fs/fs-node.h>

#pragma once

/* Created to be an object that can be accessed in user space where the information of the object can be read into a buffer. */
/* Based off of file.h */

namespace stacsos::kernel::fs {
class filesystem;
class directory {
public:
	
	directory(list<string> names_, list<u64> sizes_, list<fs_node_kind> kinds_, u64 list_size_, bool dash_l_, bool dash_a_, bool dash_U_)
		: names(names_)
		, sizes(sizes_)
		, kinds(kinds_)
		, list_size(list_size_)
		, dash_l(dash_l_)
		, dash_a(dash_a_)
		, dash_U(dash_U_)
		, indices(new int[list_size])
	{
	}

	virtual ~directory() { }

	/**
	 * @brief - Comparator - uses indexes since list<string> is immutable.
	 * 
	 * @param a - index of name 1
	 * @param b - index of name 2
	 * @param names - list of file names
	 * @return int (-1, 0, 1) => (str1 < str2, str1 = str2, str1 > str2)
	 */
	static int compare_indices(int a, int b, list<string> names) {
		return native_memops::strcmp(names.at(a).c_str(), names.at(b).c_str());
	}

	/**
	 * @brief Sorts the indices of the directory listing by name. Utilises bubble sort algorithm.
	 * 
	 * @param names - list of file names
	 * @param sizes - list of file sizes
	 * @param kinds - list of file types
	 * @param indices - indexes array
	 */
	static void sort(list<string> names, list <u64> sizes, list<fs_node_kind> kinds, int* indices) {
		for (u64 i = 0; i < names.count(); i++) {
			indices[i] = i;
		}

		for (u64 i = 0; i < names.count() - 1; i++) {
			for (u64 j = 0; j < names.count() - 1 - i; j++) {
				if (compare_indices(indices[j], indices[j+1], names) > 0) {
					int temp = indices[j];
					indices[j] = indices[j+1];
					indices[j+1] = temp;
				}
			}
		}
	}

	/**
	 * @brief Reads object information into buffer which can be printed out in user space.
	 * 
	 * @param buffer 
	 * @param size_of_buffer 
	 * @return size_t 
	 */
	virtual size_t read(char* buffer, size_t size_of_buffer)
	{
		size_t ptr = 0;
		sort(names, sizes, kinds, indices);
		
		for (size_t i = 0; i < list_size; i++) {
			if (ptr > size_of_buffer) {
				break;
			}

			size_t line_len = 0;
			const char* name_str;
			size_t name_len = 0;
			
			if (dash_U) {
				name_str = names.at(i).c_str();
				name_len = names.at(i).length();
			}
			else {
				name_str = names.at(indices[i]).c_str();
				name_len = names.at(indices[i]).length();
			}
			
			//Skips hidden files if -a is not included in command line (hidden files defined to start with a dot).
			if (!dash_a && name_str[0] == '.') {
				continue;
			}

			//Gets file type information if -l is included in command line.
			if (dash_l) {
				fs_node_kind kind;
				if (dash_U) {
					kind = kinds.at(i);
				}
				else {
					kind = kinds.at(indices[i]);
				}

				buffer[ptr] = '[';
				ptr += 1;
				line_len += 1;
				if (kind == fs_node_kind::file) {
					buffer[ptr] = 'F';
					ptr += 1;
					line_len += 1;
				}
				else if (kind == fs_node_kind::directory) {
					buffer[ptr] = 'D';
					ptr += 1;
					line_len += 1;
				}
				buffer[ptr] = ']';
				ptr += 1;
				line_len += 1;
				for (int j = 0; j < 2; j++) {
					buffer[ptr] = ' ';
					ptr += 1;
					line_len += 1;
				}
			}

			//Names of directory contents
			size_t word_len = 0;
			while (word_len < name_len) {
				buffer[ptr] = name_str[word_len];
				ptr += 1;
				line_len += 1;
				word_len += 1;
			}

			//Below acts as some padding so that all the sizes are aligned in a column. The number is arbitrary and can be changed.
			while (line_len < 32) {
				buffer[ptr] = ' ';
				ptr += 1;
				line_len += 1;
			}

			bool is_file = false;
			if (dash_U) {
				if (kinds.at(i) == fs_node_kind::file) {
					is_file = true;
				}
			}
			else {
				if (kinds.at(indices[i]) == fs_node_kind::file) {
					is_file = true;
				}
			}

			//Gets file size information if -l is included in command line.
			if (dash_l && is_file) {
				buffer[ptr] = ' ';
				ptr += 1;
				line_len += 1;

				const char* size_str;
				size_t num_len = 0;
				if (dash_U) {
					size_str = string::to_string(sizes.at(i)).c_str();
					num_len = string::to_string(sizes.at(i)).length();
				}
				else {
					size_str = string::to_string(sizes.at(indices[i])).c_str();
					num_len = string::to_string(sizes.at(indices[i])).length();
				}
				
				size_t num_ptr = 0;
				while (num_ptr < num_len) {
					buffer[ptr] = size_str[num_ptr];
					ptr += 1;
					num_ptr += 1;
					line_len += 1;
				}
			}
			
			buffer[ptr] = '\n';
			ptr += 1;
		}

		buffer[ptr] = '\0';
		ptr += 1;
		return ptr;
	}

private:
	list<string> names;
	list<u64> sizes;
	list<fs_node_kind> kinds;
	u64 list_size;
	bool dash_l;
	bool dash_a;
	bool dash_U;
	int* indices;
};
} // namespace stacsos::kernel::fs
