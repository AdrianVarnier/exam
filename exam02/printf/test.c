#include <stdio.h>

int main()
{
	int i;
	int	x;
	int	n;

	i = 0;
	x = 10236598;
	n = x;
	while (x >= 16)
	{
		x = x / 16;
		i++;
	}
	i++;
	printf("%i %x\n", i, n);
	return (0);
}
