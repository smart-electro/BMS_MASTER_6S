/*********************************************************/
/*              auxiliary functions                      */
/*********************************************************/
/*int hexCharToInt(BYTE digit)
{
	if('0'<=digit && digit<='9')
		return digit-'0';
	if('A'<=digit && digit<='F')
		return digit-'A'+10;
	if('a'<=digit && digit<='f')
		return digit-'a'+10;
	return -1;
}

int hexToInt(BYTE *h)
{
	int i, result=0;
	int tmp;
	
	for(i=0;h[i] && i<8;i++)
    {
		tmp = hexCharToInt(h[i]);
		if(tmp == -1) return result;
		result = result*0x10+tmp;
	}
	return result;
} */