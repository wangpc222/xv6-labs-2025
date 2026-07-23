#include "kernel/types.h"
  #include "kernel/stat.h"
  #include "user/user.h"

void sixfive(int fd)
{
  char buf[128];
  int n, i, num, in_num;

  num = 0;
  in_num = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] >= '0' && buf[i] <= '9') {
        num = num * 10 + (buf[i] - '0');
        in_num = 1;
      }
      else {
        if (in_num && (num % 5 == 0 || num % 6 == 0)) {
          printf("%d\n", num);
        }
        num = 0;
        in_num = 0;
      }
    }
  }

  if (in_num && (num % 5 == 0 || num % 6 == 0)) {
    printf("%d\n", num);
  }
}

int
main(int argc, char* argv[])
{
  int fd, i;

  if (argc < 2) {
    fprintf(2, "Usage: sixfive <file>...\n");
    exit(1);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }

  exit(0);
}
