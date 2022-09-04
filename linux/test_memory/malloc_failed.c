#include <stdio.h>
#include <malloc.h>


static int malloc_use_brk(void)
{
  printf("使用cat /proc/%d/maps查看内存分配\n", getpid());

  //申请1字节的内存
  void *addr = malloc(1);
  printf("此1字节的内存起始地址：%p\n", addr);
  printf("使用cat /proc/%d/maps查看内存分配\n", getpid());

  //将程序阻塞，当输入任意字符时才往下执行
  getchar();

  //释放内存
  free(addr);
  printf("释放了1字节的内存，但heap堆并不会释放\n");

  getchar();
  return 0;
}

static int malloc_use_mmap() {
  //申请1字节的内存
  void *addr = malloc(1280*1024);
  printf("此128KB字节的内存起始地址：%p\n", addr);
  printf("使用cat /proc/%d/maps查看内存分配\n",getpid());

  //将程序阻塞，当输入任意字符时才往下执行
  getchar();

  //释放内存
  free(addr);
  printf("释放了128KB字节的内存，内存也归还给了操作系统\n");

  getchar();
  return 0;
}

int main() {

    //return malloc_use_brk();
    return malloc_use_mmap();
}
