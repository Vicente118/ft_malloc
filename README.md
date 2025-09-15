<div id="top">

<!-- HEADER STYLE: MODERN -->
<div align="left" style="position: relative; width: 100%; height: 100%; ">
	
# FT_MALLOC

<em><em>

<!-- BADGES -->
<img src="https://img.shields.io/github/license/Vicente118/ft_malloc?style=for-the-badge&logo=opensourceinitiative&logoColor=white&color=00ADD8" alt="license">
<img src="https://img.shields.io/github/last-commit/Vicente118/ft_malloc?style=for-the-badge&logo=git&logoColor=white&color=00ADD8" alt="last-commit">
<img src="https://img.shields.io/github/languages/top/Vicente118/ft_malloc?style=for-the-badge&color=00ADD8" alt="repo-top-language">
<img src="https://img.shields.io/github/languages/count/Vicente118/ft_malloc?style=for-the-badge&color=00ADD8" alt="repo-language-count">

<em>Built with the tools and technologies:</em>

<img src="https://img.shields.io/badge/GNU%20Bash-4EAA25.svg?style=for-the-badge&logo=GNU-Bash&logoColor=white" alt="GNU%20Bash">
<img src="https://img.shields.io/badge/C-A8B9CC.svg?style=for-the-badge&logo=C&logoColor=black" alt="C">

</div>
</div>
<br clear="right">

---

## Table of Contents

I. [Table of Contents](#table-of-contents)<br>
II. [Overview](#overview)<br>
III. [Features](#features)<br>
IV. [Project Structure](#project-structure)<br>
V. [Getting Started](#getting-started)<br>
&nbsp;&nbsp;&nbsp;&nbsp;V.a. [Prerequisites](#prerequisites)<br>
&nbsp;&nbsp;&nbsp;&nbsp;V.b. [Installation](#installation)<br>
&nbsp;&nbsp;&nbsp;&nbsp;V.c. [Usage](#usage)<br>

---

## Overview

ft_malloc is a custom heap allocator written in C that re-implements the standard allocation API: malloc, realloc, and free. Instead of relying on the system allocator, it manages memory directly using the mmap and munmap system calls and exposes debugging helpers to visualize the heap.

Design at a glance:
- Memory is grouped into zones of three classes:
  - TINY: allocations up to 1024 bytes
  - SMALL: allocations up to 16 KiB
  - LARGE: allocations larger than SMALL
- Zones are obtained from the kernel with mmap. TINY and SMALL zones contain many blocks; LARGE allocations get their own dedicated mapping.
- A global doubly linked list chains all zones. A single global mutex protects all allocator operations to provide thread safety.
- All returned pointers are 16-byte aligned, matching common ABI requirements on 64-bit systems.

Core data structures:
- struct s_zone (type, total_size, blocks, next/prev): describes a mapped region and its block list.
- struct s_block (size, allocated, next/prev): describes a single allocation payload.
- Global symbols: g_zones (head of all zones), g_mutex (allocator-wide lock).

Allocation strategy:
- First-fit search within the appropriate zone class.
- Blocks are split when a free block is larger than needed.
- On free, adjacent free blocks are coalesced to reduce fragmentation.
- LARGE allocations are unmapped immediately on free, returning memory to the OS.

Debugging and visibility:
- show_alloc_mem and show_alloc_mem_ex inspect the allocator state and print a human-readable summary of zones and blocks.
- print_memory_hex provides a hex-dump utility for inspecting memory contents.

---

## Features

- Drop-in replacements for:
  - malloc(size_t size)
  - realloc(void* ptr, size_t size)
  - free(void* ptr)
- Backed by mmap/munmap with no dependency on the system malloc.
- Size classes for better locality and reduced overhead:
  - TINY (<= 1024 bytes), SMALL (<= 16 KiB), LARGE (> 16 KiB).
- 16-byte alignment for all returned pointers on 64-bit systems.
- Block splitting and coalescing to mitigate fragmentation.
- Dedicated mappings for LARGE allocations to reduce waste and speed up free.
- Thread-safe with a single global pthread mutex.
- Introspection utilities:
  - show_alloc_mem and show_alloc_mem_ex to print allocator state.
  - print_memory_hex for hex dumps of memory regions.
    
---

## Project Structure

```sh
└── ft_malloc/
    ├── Makefile
    ├── Ressources
    │   ├── Cours.md
    │   └── GNU_Memory.md
    ├── diagram
    │   ├── ZonesAndBlocks.jpg
    │   ├── diagram.png
    │   ├── malloc_detailed_visualization.png
    │   └── memory.png
    ├── libft
    │   ├── Makefile
    │   ├── ft_atoi.c
    │   ├── ft_bzero.c
    │   ├── ft_calloc.c
    │   ├── ft_isalnum.c
    │   ├── ft_isalpha.c
    │   ├── ft_isascii.c
    │   ├── ft_isdigit.c
    │   ├── ft_isprint.c
    │   ├── ft_itoa.c
    │   ├── ft_lstadd_back_bonus.c
    │   ├── ft_lstadd_front_bonus.c
    │   ├── ft_lstclear_bonus.c
    │   ├── ft_lstdelone_bonus.c
    │   ├── ft_lstiter_bonus.c
    │   ├── ft_lstlast_bonus.c
    │   ├── ft_lstmap_bonus.c
    │   ├── ft_lstnew_bonus.c
    │   ├── ft_lstsize_bonus.c
    │   ├── ft_memchr.c
    │   ├── ft_memcmp.c
    │   ├── ft_memcpy.c
    │   ├── ft_memmove.c
    │   ├── ft_memset.c
    │   ├── ft_putchar_fd.c
    │   ├── ft_putendl_fd.c
    │   ├── ft_putnbr_fd.c
    │   ├── ft_putstr_fd.c
    │   ├── ft_split.c
    │   ├── ft_strchr.c
    │   ├── ft_strdup.c
    │   ├── ft_striteri.c
    │   ├── ft_strjoin.c
    │   ├── ft_strlcat.c
    │   ├── ft_strlcpy.c
    │   ├── ft_strlen.c
    │   ├── ft_strmapi.c
    │   ├── ft_strncmp.c
    │   ├── ft_strnstr.c
    │   ├── ft_strrchr.c
    │   ├── ft_strtrim.c
    │   ├── ft_substr.c
    │   ├── ft_tolower.c
    │   ├── ft_toupper.c
    │   └── libft.h
    ├── run.sh
    ├── src
    │   ├── free.c
    │   ├── malloc.c
    │   ├── malloc.h
    │   ├── realloc.c
    │   └── utils.c
    ├── test_fake.sh
    └── test_true.sh
```
---

## Getting Started

### Prerequisites

This project requires the following dependencies:

- Programming Language: C

### Installation

Build ft_malloc from source:

1. Clone the repository:

    ```sh
    ❯ git clone https://github.com/Vicente118/ft_malloc
    ```

2. Navigate to the project directory:

    ```sh
    ❯ cd ft_malloc
    ```

3. Compile the project

	```sh
 	❯ make
 	```

The shared library containing my own heap allocator is ready.

### Usage

Run the project with:

	```sh
 	❯ ./run.sh
  	❯ ./ProgramThatUsesMalloc
 	```
---

<div align="right">

[![][back-to-top]](#top)

</div>

[back-to-top]: https://img.shields.io/badge/-BACK_TO_TOP-151515?style=flat-square

---
