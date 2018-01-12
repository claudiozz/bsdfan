PROG=	bsdfan
SRCS=	bsdfan.c parser.c mystring.c system.c

BINDIR=	${LOCALBASE}/bin
MANDIR=	${LOCALBASE}/man/man

WARNS=	9

MANFILTER=	sed 's|/usr/local|${LOCALBASE}|g'
CFLAGS+=	-DLOCALBASE='"${LOCALBASE}"'

.include <bsd.prog.mk>
