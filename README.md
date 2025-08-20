# Minishell

A powerful and educational Unix shell implemented in C, designed to mimic core functionality of standard shells.

## Project Description

Minishell is an advanced command-line shell built for learning purposes, providing hands-on experience with Unix system programming. It allows users to execute commands, manage built-in functions, handle environment variables, perform input/output redirection, and use pipes for inter-process communication.

The project demonstrates process control, file descriptor manipulation, signal handling, and parsing techniques necessary to understand how real-world shells operate.

## How It Works

* The shell continuously waits for user input, parses commands, and executes them.
* Built-in commands such as `cd`, `echo`, `export`, `unset`, `exit`, and `pwd` are executed directly without creating a new process.
* For external commands, the shell spawns child processes using `fork()` and runs commands using `execve()`.
* Supports input/output redirection (`>`, `<`, `>>`) to files.
* Supports pipelines (`|`) to chain multiple commands together.
* Handles signals gracefully (`CTRL+C` to interrupt current process, `CTRL+D` to exit shell) to maintain stable execution.

## Features

* Execute system commands with arguments seamlessly.
* Handle built-in commands efficiently within the shell process.
* Redirect input/output streams to and from files.
* Chain commands using pipes to allow complex workflows.
* Manage and manipulate environment variables.
* Graceful handling of user signals to prevent crashes.
* Educational tool for learning Unix programming concepts.

## Usage

### 1. Compile the project

```bash
make
```

### 2. Start the shell

```bash
./minishell
```

### 3. Run commands

* **External commands:**

```bash
ls
pwd
echo Hello World
```

* **Built-in commands:**

```bash
cd /path/to/directory
export VAR=value
unset VAR
exit
```

* **Pipes and redirection:**

```bash
ls -l | grep ".c" > files.txt
cat < files.txt
```

## Notes

* Ensure the shell is running before executing commands.
* Signal handling is implemented to maintain shell stability during interruptions.
* Supports ASCII input and basic shell functionality.
* Designed as a learning tool for Unix system programming and process management.

#by adzahrao(adam zahraoui | akira) && mlaidi(Mohammed Laidi)
