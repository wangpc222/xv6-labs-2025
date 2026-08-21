# xv6 操作系统课程设计实验

本仓库为操作系统课程设计项目，基于 MIT 6.S081 的 `xv6-labs-2025` 教学操作系统完成。

xv6 是 Unix V6 的现代重实现，面向 RISC-V 多处理器，用 ANSI C 编写。本项目在其基础上完成了 9 个实验，覆盖用户态工具、系统调用、虚拟内存、陷阱机制、并发、设备驱动、文件系统与内存映射等操作系统核心主题，所有实验均已实现并通过官方测试。

## 目录

- [项目概述](#项目概述)
- [实验列表](#实验列表)
- [各实验说明](#各实验说明)
- [环境搭建](#环境搭建)
- [构建与运行](#构建与运行)
- [测试结果](#测试结果)
- [项目特点](#项目特点)

## 项目概述

本次课程设计以 MIT 6.S081 的 xv6 教学操作系统为平台，逐实验深入操作系统的各个核心模块。

| 模块 | 涉及实验 |
| --- | --- |
| 用户态编程与系统调用 | Unix utilities、system calls |
| 虚拟内存与页表 | page tables、Copy-on-Write fork、mmap |
| 陷阱与中断 | traps |
| 设备驱动与协议栈 | networking |
| 并发与锁 | locks |
| 文件系统 | file system |

每个实验均独立成分支，便于对照查看与提交。

## 实验列表

| # | 实验名称 | 分支 | 核心内容 | 状态 |
| --- | --- | --- | --- | --- |
| 1 | Unix utilities | `util` | sleep、sixfive、memdump、find 与 find-exec | 已完成 |
| 2 | system calls | `syscall` | interpose 沙箱、路径白名单、GDB 调试、攻击实验 | 已完成 |
| 3 | page tables | `pgtbl` | 页表查看、USYSCALL 加速、vmprint、大页 | 已完成 |
| 4 | traps | `traps` | RISC-V 汇编、backtrace、alarm | 已完成 |
| 5 | Copy-on-Write fork | `cow` | 写时复制 fork | 已完成 |
| 6 | networking | `net` | E1000 网卡驱动、UDP 接收栈 | 已完成 |
| 7 | locks | `lock` | 每 CPU 内存分配器、读写锁 | 已完成 |
| 8 | file system | `fs` | 双重间接块、符号链接 | 已完成 |
| 9 | mmap | `mmap` | 内存映射文件 | 已完成 |

## 各实验说明

### 1. Unix utilities（分支 `util`）

- **sleep**：实现 `user/sleep.c`，调用 `pause()` 系统调用使进程睡眠指定数量的 tick，并处理参数缺失的错误情况。
- **sixfive**：实现 `user/sixfive.c`，逐字符读取文件，识别由分隔符隔开的十进制数字序列，输出其中能被 5 或 6 整除的数，重点练习 C 字符串与 `strchr()`。
- **memdump**：实现 `user/memdump.c` 中的 `memdump(fmt, data)`，按格式字符串打印内存内容，支持 `i`（32 位整数）、`p`（64 位十六进制）、`h`（16 位整数）、`c`（字符）、`s`（指针指向的字符串）、`S`（C 字符串）等格式符，加深对指针与结构体内存布局的理解。
- **find 与 find-exec**：实现 `user/find.c`，参考 `ls` 递归遍历目录树查找同名文件，跳过 `.` 与 `..`；并扩展 `-exec cmd` 功能，用 `fork/exec/wait` 对每个匹配文件执行命令。

### 2. system calls（分支 `syscall`）

- **GDB 调试**：使用 `qemu-gdb` 单步调试内核，理解系统调用的执行流程、寄存器保存与特权级切换。
- **interpose 沙箱**：实现 `interpose(mask, path)` 系统调用，屏蔽 `mask` 中置位的系统调用；mask 通过 `fork` 继承给子进程；在 `syscall()` 分发前检查并拒绝被屏蔽的调用。
- **路径白名单**：扩展 `interpose`，当 `open/exec` 被屏蔽但访问路径与白名单一致时放行。
- **攻击实验**：利用内核故意删除 `memset` 清零所造成的内存残留漏洞，编写 `attack.c` 扫描新分配页面，从先前进程残留数据中窃取 secret，理解内核隔离边界的重要性。

### 3. page tables（分支 `pgtbl`）

- **页表查看**：通过 `print_pgtbl` 观察并解释用户进程页表，理解 PTE 权限位与 xv6 地址空间布局。
- **USYSCALL 加速系统调用**：在每个进程地址空间映射一个只读共享页 `USYSCALL`，页首存放 pid，使 `ugetpid()` 无需陷入内核即可读取，理解 Linux vDSO 的原理雏形。
- **vmprint**：实现 `vmprint()` 递归打印整棵 Sv39 三级页表，以缩进表示层级，只输出有效 PTE。
- **大页**：实现 2 MB 大页（superpage）的分配与释放，修改 `uvmcopy()` / `uvmunmap()` 支持大页的 fork 与释放，并在 `sbrk` 部分释放大页时将其降级为普通页。

### 4. traps（分支 `traps`）

- **RISC-V 汇编**：阅读 `call.asm`，理解函数调用约定、参数寄存器与字节序。
- **backtrace**：实现内核栈回溯 `backtrace()`，利用 `s0` 栈帧指针沿调用链遍历打印返回地址，并挂接到 `panic` 中辅助调试。
- **alarm**：实现 `sigalarm(interval, handler)` 与 `sigreturn()`，定时器中断时保存 `trapframe`、调用用户态处理器，`sigreturn()` 恢复现场继续执行；同时处理 handler 重入问题。

### 5. Copy-on-Write fork（分支 `cow`）

- 修改 `uvmcopy()`：fork 时不再复制物理页，而是让父子共享页，并清除 `PTE_W`、标记 `PTE_COW`。
- 实现 `cowfault()`：写入 COW 页时分配新页、复制内容，并更新 PTE 为可写。
- 为物理页维护引用计数，`kfree()` 仅在最后一个引用消失时真正回收页面。
- 修改 `copyout()` 以正确处理 COW 页，避免内核写用户空间时破坏共享数据。

### 6. networking（分支 `net`）

- **E1000 网卡驱动**：实现 `e1000_transmit()` 与 `e1000_recv()`，通过 DMA 描述符环收发以太网帧，管理环形缓冲区索引与缓冲区生命周期，并使用锁保护并发访问。
- **UDP 接收栈**：实现 `sys_bind()`、`sys_recv()`、`ip_rx()`，解析以太网/IP/UDP 头部，注意网络字节序转换；按端口将数据报排队供用户态 `recv()` 读取，并处理等待唤醒与队列上限。

### 7. locks（分支 `lock`）

- **内存分配器**：将单一全局空闲链表改为每 CPU 独立空闲链表，降低 `kmem` 锁竞争；本 CPU 链表为空时从其他 CPU 偷取页面。
- **读写锁**：实现写者优先的读写自旋锁，支持多个读者并发、写者独占，且写者等待期间新读者不得插队，防止写者饿死。

### 8. file system（分支 `fs`）

- **大文件**：在 inode 中增加双重间接块，将文件最大尺寸从 268 块提升至 65,803 块；修改 `bmap()` 支持两级间接索引，修改 `itrunc()` 递归释放所有间接块。
- **符号链接**：实现 `symlink(target, path)` 系统调用，新增 `T_SYMLINK` 文件类型与 `O_NOFOLLOW` 标志；`open()` 跟随符号链接，并限制跟随深度以检测环。

### 9. mmap（分支 `mmap`）

- 实现 `mmap()` 与 `munmap()` 系统调用，支持 `MAP_SHARED`（写回文件）与 `MAP_PRIVATE`。
- 为每个进程维护 VMA（虚拟内存区域）表，`mmap` 时只登记映射关系，不分配物理页。
- 缺页时从文件按偏移读入页面并映射，实现惰性分配。
- `munmap` 与进程退出时将 `MAP_SHARED` 修改写回文件，`fork` 时复制 VMA 并增加文件引用计数，`exec` 时清理旧映射。

## 环境搭建

| 组件 | 说明 |
| --- | --- |
| 操作系统 | Windows 11，通过 WSL2 运行 Ubuntu |
| 交叉编译工具链 | `riscv64-linux-gnu-gcc` |
| 模拟器 | `QEMU-system-riscv64` |
| 构建工具 | GNU Make、Python 3 |

## 构建与运行

切换到实验对应分支后，在仓库根目录执行：

运行 `make qemu` 即可编译内核与用户程序并启动 xv6。

在 xv6 shell 中，可以运行各实验对应的测试程序，例如：

- `mmaptest`
- `cowtest`
- `symlinktest`
- `bigfile`
- `usertests -q`

退出 QEMU 可以使用 `Ctrl-a x`。

## 测试结果

所有 9 个实验均已实现，并通过对应的官方测试。

| 实验 | 测试结果 |
| --- | --- |
| Unix utilities | 通过 |
| system calls | 通过 |
| page tables | 通过 |
| traps | 通过 |
| Copy-on-Write fork | 通过 |
| networking | 通过 |
| locks | 通过 |
| file system | 通过 |
| mmap | 通过 |

## 项目特点

- 基于 MIT 6.S081 `xv6-labs-2025`
- 覆盖操作系统核心模块
- 每个实验使用独立 Git 分支管理
- 包含用户态程序、系统调用、虚拟内存、COW、网络驱动、并发、文件系统和 mmap 等内容
- 所有实验均通过官方测试
