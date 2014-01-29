void *memcpy(void *dest, const void *src, int count)
{
    /* code to copy 'count' bytes of data from 'src' to
    *  'dest', finally return 'dest' */
    int i;
    const char *sp = (const char *)src;
    char *dp = (char *)dest;

    for(i=0; i<count; i++)
        dp[i] = sp[i];
    return dest;
}

void *memset(void *dest, char val, int count)
{
    /* code here to set 'count' bytes in 'dest' to 'val'.
    *  return 'dest' */
    int i;
    char *temp = (char *)dest;
    for(i=0; i<count; i++)
        temp[i] = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    /* Same as above, but this time, we're working with a 16-bit
    *  'val' and dest pointer. Your code can be an exact copy of
    *  the above, provided that your local variables if any, are
    *  unsigned short */
    int i;
    for(i=0; i<count; i++)
        dest[i] = val;
    return dest;
}
