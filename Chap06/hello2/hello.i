# 1 "hello.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 371 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "hello.c" 2
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 1 3 4
# 41 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/cdefs.h" 1 3 4
# 361 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/cdefs.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/android/versioning.h" 1 3 4
# 362 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/cdefs.h" 2 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/android/api-level.h" 1 3 4
# 176 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/android/api-level.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/get_device_api_level_inlines.h" 1 3 4
# 38 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/get_device_api_level_inlines.h" 3 4
int __system_property_get(const char* __name, char* __value);
int atoi(const char* __s) __attribute__((__pure__));

static __inline int android_get_device_api_level() {
  char value[92] = { 0 };
  if (__system_property_get("ro.build.version.sdk", value) < 1) return -1;
  int api_level = atoi(value);
  return (api_level > 0) ? api_level : -1;
}
# 177 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/android/api-level.h" 2 3 4
# 363 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/cdefs.h" 2 3 4

# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/android/ndk-version.h" 1 3 4
# 365 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/cdefs.h" 2 3 4
# 42 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 1 3 4
# 32 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 1 3 4
# 35 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 46 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 3 4
typedef unsigned int size_t;
# 74 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 3 4
typedef unsigned int wchar_t;
# 102 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/__stddef_max_align_t.h" 1 3 4
# 19 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/__stddef_max_align_t.h" 3 4
typedef struct {
  long long __clang_max_align_nonce1
      __attribute__((__aligned__(__alignof__(long long))));
  long double __clang_max_align_nonce2
      __attribute__((__aligned__(__alignof__(long double))));
} max_align_t;
# 103 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 2 3 4
# 33 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 2 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdint.h" 1 3 4
# 52 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdint.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdint.h" 1 3 4
# 32 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdint.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/wchar_limits.h" 1 3 4
# 33 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdint.h" 2 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 1 3 4
# 34 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdint.h" 2 3 4


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;




typedef long long __int64_t;
typedef unsigned long long __uint64_t;






typedef int __intptr_t;
typedef unsigned int __uintptr_t;


typedef __int8_t int8_t;
typedef __uint8_t uint8_t;

typedef __int16_t int16_t;
typedef __uint16_t uint16_t;

typedef __int32_t int32_t;
typedef __uint32_t uint32_t;

typedef __int64_t int64_t;
typedef __uint64_t uint64_t;

typedef __intptr_t intptr_t;
typedef __uintptr_t uintptr_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;

typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;

typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;

typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;

typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;







typedef int32_t int_fast16_t;
typedef uint32_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;


typedef uint64_t uintmax_t;
typedef int64_t intmax_t;
# 53 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdint.h" 2 3 4
# 34 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 2 3 4


# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/types.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/types.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/int-ll64.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/int-ll64.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/bitsperlong.h" 1 3 4
# 19 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/bitsperlong.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/bitsperlong.h" 1 3 4
# 20 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/bitsperlong.h" 2 3 4
# 22 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/int-ll64.h" 2 3 4

typedef __signed__ char __s8;
typedef unsigned char __u8;
typedef __signed__ short __s16;
typedef unsigned short __u16;
typedef __signed__ int __s32;
typedef unsigned int __u32;

__extension__ typedef __signed__ long long __s64;
__extension__ typedef unsigned long long __u64;
# 22 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/types.h" 2 3 4
# 22 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/types.h" 2 3 4

# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/posix_types.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/posix_types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/stddef.h" 1 3 4
# 19 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/stddef.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/compiler_types.h" 1 3 4




# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/compiler.h" 1 3 4
# 6 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/compiler_types.h" 2 3 4
# 20 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/stddef.h" 2 3 4
# 22 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/posix_types.h" 2 3 4


typedef struct {
  unsigned long fds_bits[1024 / (8 * sizeof(long))];
} __kernel_fd_set;
typedef void(* __kernel_sighandler_t) (int);
typedef int __kernel_key_t;
typedef int __kernel_mqd_t;

# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/posix_types.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/posix_types.h" 3 4
typedef unsigned short __kernel_mode_t;

typedef unsigned short __kernel_ipc_pid_t;

typedef unsigned short __kernel_uid_t;
typedef unsigned short __kernel_gid_t;

typedef unsigned short __kernel_old_dev_t;


# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/posix_types.h" 1 3 4
# 21 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/posix_types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/bitsperlong.h" 1 3 4
# 22 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/posix_types.h" 2 3 4

typedef long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;


typedef __kernel_ulong_t __kernel_ino_t;





typedef int __kernel_pid_t;
# 43 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/asm-generic/posix_types.h" 3 4
typedef __kernel_long_t __kernel_suseconds_t;


typedef int __kernel_daddr_t;


typedef unsigned int __kernel_uid32_t;
typedef unsigned int __kernel_gid32_t;


typedef __kernel_uid_t __kernel_old_uid_t;
typedef __kernel_gid_t __kernel_old_gid_t;






typedef unsigned int __kernel_size_t;
typedef int __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;







typedef struct {
  int val[2];
} __kernel_fsid_t;

typedef __kernel_long_t __kernel_off_t;
typedef long long __kernel_loff_t;
typedef __kernel_long_t __kernel_old_time_t;
typedef __kernel_long_t __kernel_time_t;
typedef long long __kernel_time64_t;
typedef __kernel_long_t __kernel_clock_t;
typedef int __kernel_timer_t;
typedef int __kernel_clockid_t;
typedef char * __kernel_caddr_t;
typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
# 31 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/arm-linux-androideabi/asm/posix_types.h" 2 3 4
# 31 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/posix_types.h" 2 3 4
# 24 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/linux/types.h" 2 3 4


typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;
typedef __u16 __sum16;
typedef __u32 __wsum;



typedef unsigned __poll_t;
# 37 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 2 3 4


# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/pthread_types.h" 1 3 4
# 32 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/pthread_types.h" 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 1 3 4
# 33 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/pthread_types.h" 2 3 4

typedef struct {
  uint32_t flags;
  void* stack_base;
  size_t stack_size;
  size_t guard_size;
  int32_t sched_policy;
  int32_t sched_priority;



} pthread_attr_t;
# 60 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/pthread_types.h" 3 4
typedef struct {



  int32_t __private[1];

} pthread_cond_t;

typedef long pthread_condattr_t;

typedef int pthread_key_t;

typedef struct {



  int32_t __private[1];

} pthread_mutex_t;

typedef long pthread_mutexattr_t;

typedef int pthread_once_t;

typedef struct {



  int32_t __private[10];

} pthread_rwlock_t;

typedef long pthread_rwlockattr_t;
# 104 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/pthread_types.h" 3 4
typedef long pthread_t;
# 40 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 2 3 4


typedef __kernel_gid32_t __gid_t;
typedef __gid_t gid_t;
typedef __kernel_uid32_t __uid_t;
typedef __uid_t uid_t;
typedef __kernel_pid_t __pid_t;
typedef __pid_t pid_t;
typedef uint32_t __id_t;
typedef __id_t id_t;

typedef unsigned long blkcnt_t;
typedef unsigned long blksize_t;
typedef __kernel_caddr_t caddr_t;
typedef __kernel_clock_t clock_t;

typedef __kernel_clockid_t __clockid_t;
typedef __clockid_t clockid_t;

typedef __kernel_daddr_t daddr_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;

typedef __kernel_mode_t __mode_t;
typedef __mode_t mode_t;

typedef __kernel_key_t __key_t;
typedef __key_t key_t;

typedef __kernel_ino_t __ino_t;
typedef __ino_t ino_t;

typedef uint64_t ino64_t;

typedef uint32_t __nlink_t;
typedef __nlink_t nlink_t;

typedef void* __timer_t;
typedef __timer_t timer_t;

typedef __kernel_suseconds_t __suseconds_t;
typedef __suseconds_t suseconds_t;


typedef uint32_t __useconds_t;
typedef __useconds_t useconds_t;



typedef uint32_t dev_t;





typedef __kernel_time_t __time_t;
typedef __time_t time_t;
# 105 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 3 4
typedef __kernel_off_t off_t;
typedef __kernel_loff_t loff_t;
typedef loff_t off64_t;




typedef int32_t __socklen_t;




typedef __socklen_t socklen_t;

typedef __builtin_va_list __va_list;
# 128 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/sys/types.h" 3 4
typedef __kernel_ssize_t ssize_t;


typedef unsigned int uint_t;
typedef unsigned int uint;


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef uint32_t u_int32_t;
typedef uint16_t u_int16_t;
typedef uint8_t u_int8_t;
typedef uint64_t u_int64_t;
# 43 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4

# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdarg.h" 1 3 4
# 14 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdarg.h" 3 4
typedef __builtin_va_list va_list;
# 32 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 45 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4
# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include/stddef.h" 1 3 4
# 46 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4

# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/seek_constants.h" 1 3 4
# 48 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4


# 1 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/struct_file.h" 1 3 4
# 36 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/bits/struct_file.h" 3 4
struct __sFILE {



  char __private[84];

} __attribute__((aligned(sizeof(void*))));
# 51 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 2 3 4




typedef off_t fpos_t;
typedef off64_t fpos64_t;

struct __sFILE;
typedef struct __sFILE FILE;
# 72 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
extern FILE __sF[] ;
# 106 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
void clearerr(FILE* __fp);
int fclose(FILE* __fp);
int feof(FILE* __fp);
int ferror(FILE* __fp);
int fflush(FILE* __fp);
int fgetc(FILE* __fp);
char* fgets(char* __buf, int __size, FILE* __fp);
int fprintf(FILE* __fp , const char* __fmt, ...) __attribute__((__format__(printf, 2, 3)));
int fputc(int __ch, FILE* __fp);
int fputs(const char* __s, FILE* __fp);
size_t fread(void* __buf, size_t __size, size_t __count, FILE* __fp);
int fscanf(FILE* __fp, const char* __fmt, ...) __attribute__((__format__(scanf, 2, 3)));
size_t fwrite(const void* __buf, size_t __size, size_t __count, FILE* __fp);
int getc(FILE* __fp);
int getchar(void);


ssize_t getdelim(char** __line_ptr, size_t* __line_length_ptr, int __delimiter, FILE* __fp) ;
ssize_t getline(char** __line_ptr, size_t* __line_length_ptr, FILE* __fp) ;



void perror(const char* __msg);
int printf(const char* __fmt, ...) __attribute__((__format__(printf, 1, 2)));
int putc(int __ch, FILE* __fp);
int putchar(int __ch);
int puts(const char* __s);
int remove(const char* __path);
void rewind(FILE* __fp);
int scanf(const char* __fmt, ...) __attribute__((__format__(scanf, 1, 2)));
void setbuf(FILE* __fp, char* __buf);
int setvbuf(FILE* __fp, char* __buf, int __mode, size_t __size);
int sscanf(const char* __s, const char* __fmt, ...) __attribute__((__format__(scanf, 2, 3)));
int ungetc(int __ch, FILE* __fp);
int vfprintf(FILE* __fp, const char* __fmt, va_list __args) __attribute__((__format__(printf, 2, 0)));
int vprintf(const char* __fp, va_list __args) __attribute__((__format__(printf, 1, 0)));


int dprintf(int __fd, const char* __fmt, ...) __attribute__((__format__(printf, 2, 3))) ;
int vdprintf(int __fd, const char* __fmt, va_list __args) __attribute__((__format__(printf, 2, 0))) ;
# 162 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
int sprintf(char* __s, const char* __fmt, ...)
    __attribute__((__format__(printf, 2, 3))) ;
int vsprintf(char* __s, const char* __fmt, va_list __args)
    __attribute__((__format__(printf, 2, 0))) ;
char* tmpnam(char* __s)
    __attribute__((deprecated("tempnam is unsafe, use mkstemp or tmpfile instead")));

char* tempnam(const char* __dir, const char* __prefix)
    __attribute__((deprecated("tempnam is unsafe, use mkstemp or tmpfile instead")));







int rename(const char* __old_path, const char* __new_path);







int renameat(int __old_dir_fd, const char* __old_path, int __new_dir_fd, const char* __new_path);
# 223 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
int fseek(FILE* __fp, long __offset, int __whence);
long ftell(FILE* __fp);
# 248 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
int fgetpos(FILE* __fp, fpos_t* __pos);
int fsetpos(FILE* __fp, const fpos_t* __pos);
int fseeko(FILE* __fp, off_t __offset, int __whence);
off_t ftello(FILE* __fp);
# 280 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
FILE* fopen(const char* __path, const char* __mode);





FILE* freopen(const char* __path, const char* __mode, FILE* __fp);





FILE* tmpfile(void);






int snprintf(char* __buf, size_t __size, const char* __fmt, ...) __attribute__((__format__(printf, 3, 4)));
int vfscanf(FILE* __fp, const char* __fmt, va_list __args) __attribute__((__format__(scanf, 2, 0)));
int vscanf(const char* __fmt , va_list __args) __attribute__((__format__(scanf, 1, 0)));
int vsnprintf(char* __buf, size_t __size, const char* __fmt, va_list __args) __attribute__((__format__(printf, 3, 0)));
int vsscanf(const char* __s, const char* __fmt, va_list __args) __attribute__((__format__(scanf, 2, 0)));
# 312 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
FILE* fdopen(int __fd, const char* __mode);
int fileno(FILE* __fp);
int pclose(FILE* __fp);
FILE* popen(const char* __command, const char* __mode);
void flockfile(FILE* __fp);
int ftrylockfile(FILE* __fp);
void funlockfile(FILE* __fp);
int getc_unlocked(FILE* __fp);
int getchar_unlocked(void);
int putc_unlocked(int __ch, FILE* __fp);
int putchar_unlocked(int __ch);
# 332 "/root/Android/Sdk/ndk/22.0.7026061/toolchains/llvm/prebuilt/linux-x86_64/bin/../sysroot/usr/include/stdio.h" 3 4
int asprintf(char** __s_ptr, const char* __fmt, ...) __attribute__((__format__(printf, 2, 3)));
char* fgetln(FILE* __fp, size_t* __length_ptr);
int fpurge(FILE* __fp);
void setbuffer(FILE* __fp, char* __buf, int __size);
int setlinebuf(FILE* __fp);
int vasprintf(char** __s_ptr, const char* __fmt, va_list __args) __attribute__((__format__(printf, 2, 0)));
# 2 "hello.c" 2

int main()
{

    printf("Hello, World! \n");

    return 0;
}
