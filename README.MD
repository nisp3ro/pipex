# 🚰 Pipex

Pipex is a project that replicates basic shell piping and redirection functionalities. The goal is to recreate the behavior of a shell pipeline by connecting commands through pipes and managing file redirection. The project includes both a mandatory part and bonus features that enhance its capabilities.

---

## Features

### **Mandatory Part**

- **Piping & Redirection:**
  - Execute two commands chained by a pipe.
  - Read input from a file and redirect it as input to the first command.
  - Write the output of the second command to a file (using truncate mode).

- **Error Handling:**
  - Validates the environment and the existence of the `PATH` variable.
  - Handles errors for file operations and command execution.

### **Bonus Part**

- **Here-Document:**
  - Supports here-document functionality to read input until a given delimiter is encountered.
  - When in here-document mode, appends output to the specified file.

- **Multiple Commands:**
  - Allows the execution of multiple commands chained together.
  - Manages complex pipelines using recursive fork and pipe calls.

---

## Installation & Compilation

To compile the mandatory part of Pipex, simply run:

```sh
make
```

This will compile the mandatory version of the project.

Alternatively, to compile the bonus version with additional features, run:

```sh
make bonus
```

### Cleaning Up

- `make clean` – Removes object files.
- `make fclean` – Removes object files and the final executable.
- `make re` – Fully recompiles the mandatory part.
- `make rebonus` – Fully recompiles the bonus part.

---

## Usage

### **Mandatory Usage**

The mandatory part of Pipex expects exactly four arguments:

```sh
./pipex file1 "cmd1" "cmd2" file2
```

- **file1:** The input file from which data is read.
- **cmd1:** The first command to execute.
- **cmd2:** The second command that receives the output from `cmd1`.
- **file2:** The output file where the final result is written (file is truncated).

Example:

```sh
./pipex infile "grep hello" "wc -l" outfile
```

### **Bonus Usage**

When using the bonus features (here-document), the syntax is:

```sh
./pipex here_doc LIMITER "cmd1" "cmd2" ... "cmdN" file
```

- **here_doc:** Indicates the use of here-document mode.
- **LIMITER:** The delimiter string for the here-document input.
- **cmd1 ... cmdN:** One or more commands to be executed in sequence.
- **file:** The output file where the final result is appended.

Example:

```sh
./pipex here_doc EOF "cat" "grep hello" outfile
```

In this mode, Pipex reads from the standard input until the `LIMITER` string is encountered, then feeds the collected input to the first command.

---

## Implementation Details

- **Process Creation & Piping:**  
  Pipex creates child processes using `fork()` and sets up pipes with `pipe()` to connect the output of one command to the input of the next.

- **Redirection:**  
  Uses `dup2()` to redirect file descriptors for reading from and writing to files.

- **Command Execution:**  
  The `execve()` function is used to execute commands, searching for the correct binary using the `PATH` environment variable.

- **Error Handling:**  
  Implements robust error checking for system calls (e.g., `open()`, `fork()`, `pipe()`) and displays appropriate error messages.

---

## License

This project is part of the 42 School curriculum.
