#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if (s == NULL) return 0;
  int i = 0, len = 0;
  while (s[i] != '\0') {
    len++;
    i++;
  }
  return len;
}


char *strcpy(char *dst, const char *src) {
  size_t len = strlen(src);
  int i = 0, j = 0;
  while (j <= len) {
    dst[i++] = src[j++];
  }
  return dst;  
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  size_t len1 = strlen(dst);
  size_t len2 = strlen(src);
  if (len2 == 0) return dst;
  int i = len1, j = 0;
  while (j <= len2) {
    dst[i++] = src[j++];
  }
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  int i = 0, j = 0;
  char c1, c2;
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  while (i <= len1 && j <= len2) {
    c1 = s1[i++];  
    c2 = s2[j++];
    if (c1 == c2) continue;
    else if (c1 < c2) return -1;
    else return 1;
  }
  if (len1 < len2) return -1;
  else if (len1 > len2) return 1;
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  // char * default
  char *byte = (char *)s;
  for (int i = 0; i < n; i++) {
    byte[i] = c;
  }
  byte[n] = '\0';
  return s;
}


void *memmove(void *dst, const void *src, size_t n) {
    char *d = dst;
    const char *s = src;

    if (d < s) {
        // 如果 dest 在 src 之前，则从后向前复制
        for (size_t i = n; i > 0; --i) {
            *d = *s;
            ++d;
            ++s;
        }
    } else {
        // 如果 dest 在 src 之后，则从前向后复制
        for (size_t i = 0; i < n; ++i) {
            *d = *s;
            ++d;
            ++s;
        }
    }

    return dst; // 返回目标指针  
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
    int reval;
    while(n && !(reval = (*(unsigned char *)s1) - (*(unsigned char *)s2)))
    {
        s1 = (unsigned char *)s1 + 1;
        s2 = (unsigned char *)s2 + 1;
        --n;
    }
    return reval;
}
#endif
