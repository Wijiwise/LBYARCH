# x86-to-C interface programming project (S30G)

## Group 3 members

* Carlos, Luis Guiller Alberto
* Esparas, Jayhan Charlizze

## How to Compile and Run

### Prerequisites
* **OS:** Windows (64-bit / x86-64)
* **Assembler:** [NASM](https://www.nasm.us/)
* **C Compiler:** [MinGW-w64 (gcc)](https://www.mingw-w64.org/)

### Build & Execute (Command Prompt / PowerShell)

1. **Assemble the x86-64 source file into a Windows 64-bit object file:**
   ```bash
   nasm -f win64 imgCvtGrayDoubleToInt.asm -o imgCvtGrayDoubleToInt.obj
   ```

2. **Compile the C driver and link it with the assembly object file:**
   ```bash
   gcc -O2 main.c imgCvtGrayDoubleToInt.obj -o grayscale.exe
   ```

3. **Run the executable:**
   ```bash
   grayscale
   ```

### Usage Walkthrough
1. Enter the image dimensions (`height` and `width`, e.g., `3 4`).
2. Select the input mode:
   * `1` — Manually enter `height * width` double-precision pixel values.
   * `2` — Automatically generate random pixel values.
3. After displaying the converted integer matrix, single-run execution time, and correctness result, the program automatically runs the required 30-iteration benchmarks for $10 \times 10$, $100 \times 100$, and $1000 \times 1000$ images.

## Execution Time & Performance Analysis

### Execution Time (asm function only)

| Image Dimensions ($H \times W$) | Total Pixels | Number of Runs | Average Execution Time (ms) | Correctness Check |
| :------------------------------ | :----------: | :------------: | :-------------------------: | :---------------: |
| $10 \times 10$                  |    $100$     |      $30$      |        0.000063 ms          |       PASS        |
| $100 \times 100$                |   $10,000$   |      $30$      |        0.007207 ms          |       PASS        |
| $1000 \times 1000$              | $1,000,000$  |      $30$      |        0.809773 ms          |       PASS        |

### Short Performance Analysis
* `[Discuss how average execution time scales as pixel count increases by 100x from 10x10 to 100x100 and 1000x1000.]`
* `[Explain the effect of scalar SIMD processing (one double pixel per loop iteration using XMM0–XMM2) and memory access patterns / CPU cache behavior on larger arrays.]`
* `[Note any observations about loop overhead on small inputs (10x10) versus linear scaling on large inputs (1000x1000).]`

## Program Output & Correctness Check Screenshot

<img width="1522" height="746" alt="image" src="https://github.com/user-attachments/assets/4daa3f74-519b-46e4-bbb8-ffdc994ee533" />

## Demo Video (5–10 Minutes)

> Contents to cover: C and x86-64 source code walkthrough, compilation process, and live execution with correctness checks.

`[Insert Google Drive / YouTube / OneDrive link here — ensure sharing permissions are set to public/anyone with the link]`
