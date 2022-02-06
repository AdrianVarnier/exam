#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	color;
}				t_zone;

typedef struct	s_rectangle
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}				t_rectangle;

static int ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

static int	ft_err(char *s)
{
	ft_putstr(s);
	return (1);
}

static void	free_draw(char	**draw)
{
	int	i;

	i = 0;
	while (draw[i] != NULL)
	{
		free(draw[i]);
		i++;
	}
	free(draw);
}

static int	get_zone(FILE *file, t_zone	*zone)
{
	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->color) != 3)
		return (1);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (1);
	return (0);
}

static char	**draw_zone(t_zone zone)
{
	int		i;
	int		j;
	char	**new;

	if ((new = malloc(sizeof(char *) * (zone.height + 1))) == NULL)
		return (NULL);
	j = 0;
	i = 0;
	while (j < zone.height)
	{
		if ((new[j] = malloc(sizeof(char) * (zone.width + 1))) == NULL)
		{
			free_draw(new);
			return (NULL);
		}
		while (i < zone.width)
		{
			new[j][i] = zone.color;
			i++;
		}
		new[j][i] = '\0';
		i = 0;
		j++;
	}
	new[j] = NULL;
	return (new);
}

static int	is_in_rectangle(int x, int y, t_rectangle rectangle)
{
	if (x < rectangle.x || x > rectangle.x + rectangle.width || y < rectangle.y || y > rectangle.y + rectangle.height)
		return (0);
	if (rectangle.type == 'R')
		return (1);
	if (x - rectangle.x < 1 || y - rectangle.y < 1 || rectangle.x + rectangle.width - x < 1 || rectangle.y + rectangle.height - y < 1)
		return (1);
	return (0);
}

static int	draw_rectangle(FILE *file, t_zone zone, char **draw)
{
	int			x;
	int			y;
	int			ret;
	t_rectangle	rectangle;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rectangle.type, &rectangle.x, &rectangle.y, &rectangle.width, &rectangle.height, &rectangle.color)) == 6)
	{
		x = 0;
		y = 0;
		if (rectangle.width <= 0 || rectangle.height <= 0 || (rectangle.type != 'r' && rectangle.type != 'R'))
			return (1);
		while (y < zone.height)
		{
			while (x < zone.width)
			{
				if (is_in_rectangle(x, y, rectangle) == 1)
					draw[y][x] = rectangle.color;
				x++;
			}
			x = 0;
			y++;
		}
	}
	if (ret == -1)
		return (0);
	return (1);
}

static void	print_draw(char **draw)
{
	int	i;

	i = 0;
	while (draw[i] != NULL)
	{
		ft_putstr(draw[i]);
		ft_putstr("\n");
		i++;
	}
}

int	main(int argc, char **argv)
{
	FILE	*file;
	t_zone	zone;
	char	**draw;

	if (argc != 2)
		return(ft_err("Error: argument\n"));
	if ((file = fopen(argv[1], "r")) == NULL)
		return (ft_err("Error: Operation file corrupted\n"));
	if (get_zone(file, &zone) == 1)
		return (ft_err("Error: Operation file corrupted\n"));
	if ((draw = draw_zone(zone)) == NULL)
	{
		fclose(file);
		return (ft_err("Error: Operation file corrupted\n"));
	}
	if (draw_rectangle(file, zone, draw) == 1)
	{
		fclose(file);
		free_draw(draw);
		return (ft_err("Error: Operation file corrupted\n"));
	}
	print_draw(draw);
	fclose(file);
	free_draw(draw);
}
