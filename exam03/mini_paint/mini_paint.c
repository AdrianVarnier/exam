#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	color;
}				t_zone;

typedef struct	s_circle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}				t_circle;

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

static int	is_in_circle(int x, int y, t_circle circle)
{
	float	distance;

	distance = sqrtf(powf(x - circle.x, 2) + powf(y - circle.y, 2));
	if (distance > circle.radius)
		return (0);
	if (circle.type == 'C')
		return (1);
	if (circle.radius - distance < 1)
		return (1);
	return (0);
}

static int	draw_circle(FILE *file, t_zone zone, char **draw)
{
	int			x;
	int			y;
	int			ret;
	t_circle	circle;

	while ((ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color)) == 5)
	{
		x = 0;
		y = 0;
		if (circle.radius <= 0 || (circle.type != 'c' && circle.type != 'C'))
			return (1);
		while (y < zone.height)
		{
			while (x < zone.width)
			{
				if (is_in_circle(x, y, circle) == 1)
					draw[y][x] = circle.color;
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
	if (draw_circle(file, zone, draw) == 1)
	{
		fclose(file);
		free_draw(draw);
		return (ft_err("Error: Operation file corrupted\n"));
	}
	print_draw(draw);
	fclose(file);
	free_draw(draw);
}
