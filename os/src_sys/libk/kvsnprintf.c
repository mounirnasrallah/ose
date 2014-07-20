#include <kstdio.h>
#include <stdarg.h>

int kvsnprintf(char *buffer, unsigned size, const char *restrict fmt, va_list ap)
{
    register char *tmp;
    int val, i, count = size - 1;
    char arg[16], *xdigit;
    char *buf = buffer;

    while (*fmt) {
        while ((*fmt != '%') && (*fmt)) {
            *buf++ = *fmt++;
            if (--count == 0) goto abort;
        }
        if (*fmt) {
            fmt++;
            switch (*fmt) {
            case '%':
                *buf++ = '%';
                if (--count == 0) goto abort;
                goto next;
            case 'c':
                *buf++ = va_arg(ap, int);
                if (--count == 0) goto abort;
                goto next;
            case 's':
                tmp = va_arg(ap, char *);
                tmp = (tmp) ? tmp : "(null)";
                break;
            case 'd':
            case 'i':
                val = va_arg(ap, int);
                i = ((val != 0) && (val == -val)) ? 1 : 0;
                if (val < 0) {
                    val = -(val+i);
                    *buf++ = '-';
                    if (--count == 0) goto abort;
                }
                tmp = arg + sizeof(arg);
                *--tmp = '\0';
                do {
                    *--tmp = (val % 10) + '0' + i;
                    i = 0;
                    val = val / 10;
                } while (val);
                break;
            case 'u':
                val = va_arg(ap, unsigned int);
                tmp = arg + sizeof(arg);
                *--tmp = '\0';
                do {
                    *--tmp = (val % 10) + '0';
                    val = val / 10;
                } while (val);
                break;
            case 'p':
            case 'x':
            case 'X':
                val = va_arg(ap, int);
                tmp = arg + sizeof(arg);
                *--tmp = '\0';
                xdigit = (*fmt == 'X') ? "0123456789ABCDEF":"0123456789abcdef";
                i = 0;
                do {
                    *--tmp = xdigit[val & 15];
                    val = (unsigned int) val >> 4;
                    i++;
                } while (val);
                if (*fmt == 'p')
                    while (i < 8) {
                        *--tmp = xdigit[0];
                        i++;
                    }
                break;
            default:
                *buf++ = *fmt;
                if (--count == 0) goto abort;
                goto next;
            }
            while (*tmp) {
                *buf++ = *tmp++;
                if (--count == 0) goto abort;
            }
          next:
            fmt++;
        }
    }
abort:
    *buf = (char) 0;
    return (int)((unsigned)buf-(unsigned)buffer);
}
