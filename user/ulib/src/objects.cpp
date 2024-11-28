/* SPDX-License-Identifier: MIT */

/* StACSOS - userspace standard library
 *
 * Copyright (c) University of St Andrews 2024
 * Tom Spink <tcs6@st-andrews.ac.uk>
 */
#include <stacsos/objects.h>
#include <stacsos/user-syscall.h>

using namespace stacsos;

object *object::open(const char *path)
{
	auto result = syscalls::open(path);
	if (result.code != syscall_result_code::ok) {
		return nullptr;
	}

	return new object(result.id);
}

/**
 * @brief Object constructor which invokes a syscall
 * 
 * @param path - path of directory
 * @param is_l - toggle for -l flag
 * @param is_a - toggle for -a flag
 * @param is_U - toggle for -U flag
 * @return object* - containing information about the directory given in path.
 */
object* object::listdir_(const char* path, bool is_l, bool is_a, bool is_U) {
	auto result = syscalls::listdir_(path, is_l, is_a, is_U);
	if (result.code != syscall_result_code::ok) {
		return nullptr;
	}

	return new object(result.id);
}

object::~object() { syscalls::close(handle_); }

size_t object::read(void *buffer, size_t length) { return syscalls::read(handle_, buffer, length).length; }
size_t object::write(const void *buffer, size_t length) { return syscalls::write(handle_, buffer, length).length; }
size_t object::pwrite(const void *buffer, size_t length, size_t offset) { return syscalls::pwrite(handle_, buffer, length, offset).length; }
size_t object::pread(void *buffer, size_t length, size_t offset) { return syscalls::pread(handle_, buffer, length, offset).length; }
u64 object::ioctl(u64 cmd, void *buffer, size_t length) { return syscalls::ioctl(handle_, cmd, buffer, length).length; }
