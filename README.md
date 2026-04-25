# MINISHELL  
A minimal Unix shell implementation written in C.

The goal of minishell is to understand how a shell works internally by recreating key features of a Unix command interpreter.

The program handles command parsing, environment variables, process creation, piping, and redirections, while respecting bash-like behavior.

---

## Features

Execution of external commands

Builtins implementation (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)

Environment variable management

Command path resolution (`PATH`)

Pipes (`|`) handling

Redirections (`>`, `<`, `>>`, `<<`)

Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

Tokenization and parsing of user input

Error handling with bash-like behavior

---

## Challenges & Learnings

Understanding how a shell interprets and executes commands

Managing processes with `fork`, `execve`, and `wait`

Implementing pipes and inter-process communication

Handling file descriptors and redirections

Designing a tokenizer and parser for command input

Managing environment variables dynamically

Handling signals correctly in interactive programs

Ensuring memory is properly allocated and freed

Building a modular and maintainable architecture

---

## Build & Run

```bash
git clone https://github.com/GauthierRohr/minishell.git minishell

cd minishell

make

./minishell
```

---

## Usage

```bash
ls -la
echo "Hello world"
cat file.txt | grep test
export VAR=value
echo $VAR
```

Keyboard:

```
Ctrl + C --> interrupt current command
Ctrl + D --> exit shell
Ctrl + \ --> quit signal (handled)
```

---

## Project Structure

```
├── inc
│   ├── builtins.h
│   ├── env.h
│   ├── exec.h
│   ├── piping.h
│   ├── redirect.h
│   ├── signals.h
│   └── tokenizer.h
│
├── src
│   ├── builtins/
│   ├── exec/
│   ├── piping/
│   ├── redirect/
│   ├── tokenizer/
│   ├── env.c
│   ├── signals.c
│   └── main.c
│
├── libft/
│   ├── ft_printf/
│   └── get_next_line/
│
├── tests/
│
├── Makefile
└── README.md
```

---

## 👥 Project

Project developed in collaboration with Christopher Jauregui as part of the École 42 curriculum.
