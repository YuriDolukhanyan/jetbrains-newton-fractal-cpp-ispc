# Newton Fractal (ISPC + C++)

This project generates a **Newton Fractal** image using the **ISPC (Intel SPMD Program Compiler)** for parallel computation and C++ for orchestration and image output.

---

## Overview

The Newton fractal visualizes the convergence behavior of the **Newton–Raphson method** applied to the function:

\[
\[f(z) = z^n - 1\]
\]

Each pixel corresponds to a complex number \( z \). The program iteratively applies:

\[
\[z_{k+1} = z_k - \frac{f(z_k)}{f'(z_k)}\]
\]

and colors the pixel based on which root \( z \) converges to and how many iterations it takes.

---

## Requirements

Make sure the following are installed and available in your system **PATH**:

- [ISPC Compiler](https://ispc.github.io/downloads.html)
- **GCC / G++** (MinGW or Linux toolchain)
- Any text editor or IDE (VS Code, CLion, etc.)

---

## Project Structure

```
.
├── main.cpp            # C++ code for image creation and ISPC call
├── newton.ispc         # ISPC kernel for computing the fractal
├── newton_ispc.h       # Generated ISPC header (auto-created)
├── newton_ispc.obj     # Generated ISPC object file (auto-created)
├── images (folder)     # Output fractal images (PPM format (also added the PNG format))
└── README.md
```

---

## Build & Run Instructions

### Compile the ISPC code

Run the following command in the project directory:

```bash
ispc\bin\ispc.exe newton.ispc -o newton_ispc.obj -h newton_ispc.h
```

- `-o` : output object file for linking
- `-h` : output header file for C++ to include

---

### Compile and link with G++

```bash
g++ main.cpp newton_ispc.obj -o newton.exe
```

Make sure `newton_ispc.obj` and `newton_ispc.h` are in the same directory as `main.cpp`.

---

### Run the program

You can specify the power `n` (number of roots) as a command-line argument:

Example:

```bash
newton.exe 7
```

If `n` is omitted, the default value is `n = 5`.

---

### Output

After execution, the program generates a file named:

```
newton_n.ppm
```

This is a **PPM image** that can be opened with many image viewers, or you can convert it to PNG/JPG using tools like ImageMagick:

```bash
magick convert newton.ppm newton.png
```

---

## Notes

- The ISPC compiler performs the heavy numerical work in parallel.
- The C++ code handles image generation, file writing, and root setup.
- Adjust parameters (like `width`, `height`, and `max_iter`) in `main.cpp` to control image resolution and detail.
- Typical runtime: a few seconds for 800×800 images.

---

## Example

```bash
ispc\bin\ispc.exe newton.ispc -o newton_ispc.obj -h newton_ispc.h
g++ main.cpp newton_ispc.obj -o newton.exe
newton.exe 5
```

Output:

```
Newton fractal saved to newton.ppm
```

## Tested Environment

- **Windows 10 / 11**
- **ISPC 1.23.0**
- **G++ (MinGW-w64) 12.2.0**
- **C++17**

---
