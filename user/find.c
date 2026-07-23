#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* path, char* target, char* exec_cmd, char* exec_arg)
{
  char buf[512], * p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    return;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_FILE && strcmp(de.name, target) == 0) {
      if (exec_cmd) {
        int pid = fork();
        if (pid == 0) {
          char* exec_argv[] = { exec_cmd, exec_arg, buf, 0 };
          exec(exec_cmd, exec_argv);
          fprintf(2, "find: exec %s failed\n", exec_cmd);
          exit(1);
        }
        wait(0);
      }
      else {
        printf("%s\n", buf);
      }
    }
    else if (st.type == T_DIR) {
      find(buf, target, exec_cmd, exec_arg);
    }
  }

  close(fd);
}

int
main(int argc, char* argv[])
{
  char* exec_cmd = 0;
  char* exec_arg = 0;

  if (argc < 3) {
    fprintf(2, "Usage: find <path> <filename> [-exec <cmd> <arg>]\n");
    exit(1);
  }

  if (argc > 3 && strcmp(argv[3], "-exec") == 0) {
    if (argc < 5) {
      fprintf(2, "find: -exec requires <cmd> <arg>\n");
      exit(1);
    }
    exec_cmd = argv[4];
    exec_arg = argv[5];
  }

  find(argv[1], argv[2], exec_cmd, exec_arg);
  exit(0);
}
