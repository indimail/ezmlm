/*
 * $Id: trycpp.c,v 1.1 2025-01-22 11:21:29+05:30 Cprogrammer Exp mbhangui $
 */
void main()
{
#ifdef NeXT
  printf("nextstep\n"); exit(0);
#endif
  printf("unknown\n"); exit(0);
}

/*
 * $Log: trycpp.c,v $
 * Revision 1.1  2025-01-22 11:21:29+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
