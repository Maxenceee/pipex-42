# Pipex 42

The purpose of this project is the discovery in detail UNIX mechanism - `pipe`.

## Mandatory part
The `pipex` program should repeat the following shell command behavior

```sh
$> < file1 cmd1 | cmd2 > file2
```
and looks like this:
```sh
$> ./pipex file1 cmd1 cmd2 file2
```
All errors like wrong commands, permission to files, etc, must be handled.

## Bonus part
The program need to support multiple pipes

```sh
$> < file1 cmd1 | cmd2 | cmd3 | ... | cmdn > file2

$> ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2