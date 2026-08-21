#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char* argv[])
{
  char *mem;
  int i, j, k;
  int found = 0;

  // allocate page by page and scan each
  for (k = 0; k < 200; k++) {
    mem = sbrk(4096);
    if (mem == (char*)-1) break;
    // scan this page for "This may help."
    for (i = 0; i < 4096 - 20; i++) {
      if (mem[i] == 'T' && mem[i+1] == 'h' && mem[i+2] == 'i' && mem[i+3] == 's' &&
          mem[i+4] == ' ' && mem[i+5] == 'm' && mem[i+6] == 'a' && mem[i+7] == 'y' &&
          mem[i+8] == ' ' && mem[i+9] == 'h' && mem[i+10] == 'e' && mem[i+11] == 'l' &&
          mem[i+12] == 'p' && mem[i+13] == '.' && mem[i+14] == '\0') {
        j = i + 16;
        while ((mem[j] >= 'a' && mem[j] <= 'z') ||
               (mem[j] >= 'A' && mem[j] <= 'Z') ||
               (mem[j] >= '0' && mem[j] <= '9')) {
          printf("%c", mem[j]);
          j++;
        }
        printf("\n");
        found = 1;
        break;
      }
    }
    if (found) break;
  }
  if (!found) exit(1);
  exit(0);
}
