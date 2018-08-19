void kernel_main(void)
{
	const char* Str = "Massara Kernel";
	char* VideoPtr = (char *)0xb8000;
	unsigned int i = 0;
	unsigned int j = 0;

	while(j < 80 * 25 * 2)
	{
		VideoPtr[j] = ' ';
		VideoPtr[j + 1] = 0x07;
		j = j + 2;
	}

	j = 0;

	while(Str[j] != '\0')
	{
		VideoPtr[i] = Str[j];
		VideoPtr[i + 1] = 0x07;
		++j;
		i = i + 2;
	}

	return;
}
