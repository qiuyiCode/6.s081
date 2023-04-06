#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), '\0', DIRSIZ - strlen(p));
  return buf;
}

void find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // path肯定是文件夹,直接读文件夹即可,参考ls.c
  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
  {
    printf("find: path too long\n");
    exit(1);
  }
  // 复制文件夹的目录名称
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0)
    {
      continue;
    }
    memmove(p, de.name, DIRSIZ);
    if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
    {
      continue;
    }
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0)
    {
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    // 类型为文件,检查名字是否为要find的
    if (st.type == T_FILE)
    {
      if (!strcmp(fmtname(buf), name))
      {
        printf("%s\n", buf);
      }
    }
    else if (st.type == T_DIR)
    { // 类型为文件夹
      // 不需要搜索. 和 ..
      // printf("%s\n",buf);
      if (!strcmp(fmtname(buf), name)) // 文件夹本身为find的,输出一次,递归搜索
      {
        printf("%s\n", buf);
      }
      else
      {
        if (de.inum == 0)
        {
          return;
        }
        find(buf, name);
      }
      /*
      if (de.inum == 0)
      {
        return;
      }
      else
      {
        find(buf, name);
      }
      */
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf(2, "find should be have three args\n");
    exit(1);
  }

  char *s1 = argv[1];
  char *s2 = argv[2];
  find(s1, s2);
  exit(0);
}