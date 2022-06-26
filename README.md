# libexceptions
## C++ exceptions on bare metal

### Usage
* Add this to your linker script:
```ld
.eh_frame : {
    __eh_frame_start = .;
    KEEP(*(.eh_frame))
    __eh_frame_end = .;
}

.eh_frame_hdr : {
    KEEP(*(.eh_frame_hdr))
}

__eh_frame_hdr_start = SIZEOF(.eh_frame_hdr) > 0 ? ADDR(.eh_frame_hdr) : 0;
__eh_frame_hdr_end = SIZEOF(.eh_frame_hdr) > 0 ? . : 0;
```
* Provide malloc, calloc, realloc and free in your elf file.
* Link provided static library (out/libexceptions.a) to your elf file.
* Optionally provide your own versions of functions declared as "weak" (src/main.cpp) in your elf file.\
For example:
```c
void abort(); // run asm volatile ("cli; hlt");
void __assert_fail(const char *message, const char *file, int line, const char *func); // print message, file, line and func in terminal
void __assert_func(const char *file, int line, const char *func, const char *message) // same as previous
int __snprintf_chk(char *str, size_t maxlen, int flag, size_t len, const char *format, ...);
int __fprintf_chk(void *fp, int flag, const char *format, ...);
int fflush(void *fp);
/* ... */
```

This project uses libunwind and libc++abi from llvm-project.\
They were built with this flags:
```
cmake -S llvm -B build -G 'Unix Makefiles' -DLLVM_ENABLE_PROJECTS="" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_ASSERTIONS=Off -DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-pc-elf -DLLVM_TARGET_ARCH=X86 -DLLVM_TARGETS_TO_BUILD=X86  -DLLVM_ENABLE_ASSERTIONS=Off -DLIBUNWIND_ENABLE_ASSERIONS=Off -DLIBUNWIND_WEAK_PTHREAD_LIB=On -DLIBUNWIND_IS_BAREMETAL=On -DLIBUNWIND_ENABLE_THREADS=Off -DLIBCXXABI_ENABLE_ASSERTIONS=Off -DLIBCXXABI_ENABLE_THREADS=Off -DLIBCXXABI_INCLUDE_TESTS=Off -DLIBCXXABI_BAREMETAL=On -DLIBCXXABI_ENABLE_STATIC_UNWINDER=On -DLIBCXXABI_SILENT_TERMINATE=On -DLIBCXXABI_NON_DEMANGLING_TERMINATE=On -DLIBCXX_LINK_TESTS_WITH_SHARED_LIBCXX=Off -DLIBCXX_ENABLE_ABI_LINKER_SCRIPT=Off -DLIBCXX_ENABLE_SHARED=Off -DLIBUNWIND_ENABLE_SHARED=Off -DLIBCXXABI_ENABLE_SHARED=Off
cd build && make -j$(nproc --all)
```