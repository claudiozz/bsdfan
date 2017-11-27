PROG=	bsdfan
SRCS=	bsdfan.c common.c parser.c mystring.c system.c

BINDIR=	${LOCALBASE}/bin
MANDIR=	${LOCALBASE}/man/man

WARNS=	2

.include <bsd.prog.mk>
