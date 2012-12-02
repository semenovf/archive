/*
 * Copyright (c) 2012, Fedor Semenov
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * snprintf.c
 *
 *  Created on: 02.12.2012
 *      Author: Fedor Semenov
 */


#include <cwt/logger.h>
#include <cwt/stdio.h>
#include <cwt/test.h>

int main(int argc, char *argv[])
{
	CwtStdioNS *stdio_ns = cwt_stdio_ns();
	CWT_CHAR holder[100];
	int i;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	stdio_ns->printf(_T("abc%%n\n"));
	stdio_ns->printf(_T("abc%n"), &i);
	stdio_ns->printf(_T("%d\n"), i);
	stdio_ns->snprintf(holder, sizeof(holder), _T("abc%n"), &i);
	stdio_ns->printf(_T("%s"), holder);
	stdio_ns->printf(_T("%d\n\n"), i);


	/* Checking the field widths */
	stdio_ns->printf(_T("/%%d/, 336\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%d/\n"), 336);
	stdio_ns->printf(_T("/%d/\n"), 336);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%2d/, 336\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%2d/\n"), 336);
	stdio_ns->printf(_T("/%2d/\n"), 336);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%10d/, 336\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%10d/\n"), 336);
	stdio_ns->printf(_T("/%10d/\n"), 336);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%-10d/, 336\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%-10d/\n"), 336);
	stdio_ns->printf(_T("/%-10d/\n"), 336);
	stdio_ns->printf(_T("%s\n"), holder);


	/* floating points */
	stdio_ns->printf(_T("/%%f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%f/\n"), 1234.56);
	stdio_ns->printf(_T("/%f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%e/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%e/\n"), 1234.56);
	stdio_ns->printf(_T("/%e/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%4.2f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%4.2f/\n"), 1234.56);
	stdio_ns->printf(_T("/%4.2f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%3.1f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%3.1f/\n"), 1234.56);
	stdio_ns->printf(_T("/%3.1f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%10.3f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%10.3f/\n"), 1234.56);
	stdio_ns->printf(_T("/%10.3f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%10.3e/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%10.3e/\n"), 1234.56);
	stdio_ns->printf(_T("/%10.3e/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%+4.2f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%+4.2f/\n"), 1234.56);
	stdio_ns->printf(_T("/%+4.2f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%010.2f/, 1234.56\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%010.2f/\n"), 1234.56);
	stdio_ns->printf(_T("/%010.2f/\n"), 1234.56);
	stdio_ns->printf(_T("%s\n"), holder);

#define BLURB _T("Outstanding acting !")
/* strings precisions */

	stdio_ns->printf(_T("/%%2s/, \"%s\"\n"), BLURB);
	stdio_ns->snprintf(holder, sizeof holder, _T("/%2s/\n"), BLURB);
	stdio_ns->printf(_T("/%2s/\n"), BLURB);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%22s/ %s\n"), BLURB);
	stdio_ns->snprintf(holder, sizeof holder, _T("/%22s/\n"), BLURB);
	stdio_ns->printf(_T("/%22s/\n"), BLURB);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%22.5s/ %s\n"), BLURB);
	stdio_ns->snprintf(holder, sizeof holder, _T("/%22.5s/\n"), BLURB);
	stdio_ns->printf(_T("/%22.5s/\n"), BLURB);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%-22.5s/ %s\n"), BLURB);
	stdio_ns->snprintf(holder, sizeof holder, _T("/%-22.5s/\n"), BLURB);
	stdio_ns->printf(_T("/%-22.5s/\n"), BLURB);
	stdio_ns->printf(_T("%s\n"), holder);

	/* see some flags */

	stdio_ns->printf(_T("%%x %%X %%#x, 31, 31, 31\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("%x %X %#x\n"), 31, 31, 31);
	stdio_ns->printf(_T("%x %X %#x\n"), 31, 31, 31);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("**%%d**%% d**%% d**, 42, 42, -42\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("**%d**% d**% d**\n"), 42, 42, -42);
	stdio_ns->printf(_T("**%d**% d**% d**\n"), 42, 42, -42);
	stdio_ns->printf(_T("%s\n"), holder);

	/* other flags */
	stdio_ns->printf(_T("/%%g/, 31.4\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%g/\n"), 31.4);
	stdio_ns->printf(_T("/%g/\n"), 31.4);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%.6g/, 31.4\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%.6g/\n"), 31.4);
	stdio_ns->printf(_T("/%.6g/\n"), 31.4);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("/%%.1G/, 31.4\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("/%.1G/\n"), 31.4);
	stdio_ns->printf(_T("/%.1G/\n"), 31.4);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("abc%%n\n"));
	stdio_ns->printf(_T("abc%n"), &i); stdio_ns->printf(_T("%d\n"), i);
	stdio_ns->snprintf(holder, sizeof holder, _T("abc%n"), &i);
	stdio_ns->printf(_T("%s"), holder); stdio_ns->printf(_T("%d\n\n"), i);

	stdio_ns->printf(_T("%%*.*s --> 10.10\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("%*.*s\n"), 10, 10, BLURB);
	stdio_ns->printf(_T("%*.*s\n"), 10, 10, BLURB);
	stdio_ns->printf(_T("%s\n"), holder);

	stdio_ns->printf(_T("%%%%%%%%\n"));
	stdio_ns->snprintf(holder, sizeof holder, _T("%%%%\n"));
	stdio_ns->printf(_T("%%%%\n"));
	stdio_ns->printf(_T("%s\n"), holder);

#define BIG "Hello this is a too big string for the buffer"
	/*  stdio_ns->printf(_T("A buffer to small of 10, trying to put this:\n");*/
	stdio_ns->printf(_T("<%%>, %s\n"), BIG);
	i = stdio_ns->snprintf(holder, 10, _T("%s\n"), BIG);
	stdio_ns->printf(_T("<%s>\n"), BIG);
	stdio_ns->printf(_T("<%s>\n"), holder);

  CWT_END_TESTS;
}

