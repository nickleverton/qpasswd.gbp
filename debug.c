#include "qpasswd.h"
#include <stdarg.h>

#if WANT_DEBUG
void debug(const char *format, ...)
{
	char *printf_buf;
	ssize_t n;

	doname();
	va_list arg_ptr;
	va_start(arg_ptr, format);
	n=vsnprintf(0,BUFSIZE,format,arg_ptr);
	va_end (arg_ptr);
	va_start (arg_ptr, format);
	printf_buf=alloca(n+2);
	n=vsnprintf(printf_buf,n+1,format,arg_ptr);
	printf_buf[n]=0;
	log1(printf_buf);
	va_end(arg_ptr);

	return;
}
#endif
