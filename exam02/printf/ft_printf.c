#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		ft_putchar(s[i]);
		i++;
	}
}

void	ft_putnbr(int x)
{
	if (x < 0)
	{
		ft_putchar('-');
		x = x * -1;
	}
	if (x >= 10)
		ft_putnbr(x / 10);
	ft_putchar(x % 10 + 48);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_intlen(int x)
{
	int	i;

	i = 0;
	if (x < 0)
	{
		x = x * -1;
		i++;
	}
	while (x >= 10)
	{
		x = x / 10;
		i++;
	}
	i++;
	return (i);
}

int	ft_intlen16(int x)
{
	int	i;

	i = 0;
	if (x < 0)
	{
		x = x * -1;
		i++;
	}
	while (x >= 16)
	{
		x = x / 16;
		i++;
	}
	i++;
	return (i);
}

int	ft_intlenu16(unsigned long x)
{
	int	i;

	i = 0;

	while (x >= 16)
	{
		x = x / 16;
		i++;
	}
	i++;
	return (i);
}

void	ft_putnbr16(int x, char *base)
{
	if (x < 0)
	{
		x = x * -1;
		ft_putchar('-');
	}
	if (x >= 16)
		ft_putnbr16(x / 16, base);
	ft_putchar(base[x % 16]);
}

void	ft_putnbru16(unsigned long x, char *base)
{
	if (x < 0)
	{
		x = x * -1;
		ft_putchar('-');
	}
	if (x >= 16)
		ft_putnbru16(x / 16, base);
	ft_putchar(base[x % 16]);
}

int ft_printf(const char *s, ...)
{
	int		i;
	int		ret;
	int		tmp_int;
	char	*tmp_str;
	unsigned long	tmp_uint;
	va_list	ap;

	i = 0;
	ret = 0;
	tmp_int = 0;
	tmp_str = NULL;
	va_start(ap, s);
	while (s[i] != '\0')
	{
		if (s[i] == '%' && s[i + 1] == '%')
		{
			ft_putchar('%');
			i = i + 2;
			ret++;
		}
		else if (s[i] == '%')
		{
			if (s[i + 1] == 'c')
			{
				ft_putchar(va_arg(ap, int));
				i = i + 2;
				ret++;
			}
			else if (s[i + 1] == 's')
			{
				tmp_str = va_arg(ap, char *);
				if (tmp_str == NULL)
				{
					ft_putstr("(null)");
					ret = ret + 6;
					break ;
				}
				ft_putstr(tmp_str);
				i = i + 2;
				ret = ret + ft_strlen(tmp_str);
			}
			else if (s[i + 1] == 'i' || s[i + 1] == 'd')
			{
				tmp_int = va_arg(ap, int);
				ft_putnbr(tmp_int);
				i = i + 2;
				ret = ret + ft_intlen(tmp_int);
			}
			else if (s[i + 1] == 'x')
			{
				tmp_int = va_arg(ap, int);
				ft_putnbr16(tmp_int, "0123456789abcdef");
				i = i + 2;
				ret = ret + ft_intlen16(tmp_int);
			}
			else if (s[i + 1] == 'X')
			{
				tmp_int = va_arg(ap, int);
				ft_putnbr16(tmp_int, "0123456789ABCDEF");
				i = i + 2;
				ret = ret + ft_intlen16(tmp_int);
			}
			else if (s[i + 1] == 'p')
			{
				ft_putstr("0x");
				ret = ret + 2;
				tmp_uint = va_arg(ap, unsigned long);
				ft_putnbru16(tmp_uint, "0123456789abcdef");
				ret = ret + ft_intlenu16(tmp_uint);
				i = i + 2;
			}
			else
				i++;
		}
		else
		{
			ft_putchar(s[i]);
			i++;
			ret++;
		}
	}
	va_end(ap);
	return (ret);
}

int main()
{
	int i;
	char *p;
//	p = malloc(sizeof(char) * 10);
	p = NULL;
	i = ft_printf("%s", p);
	printf("%i\n", i);
	i = printf("%s", p);
	printf("%i\n", i);
	return (0);
}
