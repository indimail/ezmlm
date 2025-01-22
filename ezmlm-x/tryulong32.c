/*
 * $Id: tryulong32.c,v 1.1 2025-01-22 11:21:29+05:30 Cprogrammer Exp mbhangui $
 */
void main()
{
  unsigned long u;
  u = 1;
  u += u; u += u; u += u; u += u; u += u; u += u; u += u; u += u;
  u += u; u += u; u += u; u += u; u += u; u += u; u += u; u += u;
  u += u; u += u; u += u; u += u; u += u; u += u; u += u; u += u;
  u += u; u += u; u += u; u += u; u += u; u += u; u += u; u += u;
  if (!u) _exit(0);
  _exit(1);
}

/*
 * $Log: tryulong32.c,v $
 * Revision 1.1  2025-01-22 11:21:29+05:30  Cprogrammer
 * Fixes for gcc14
 *
 */
