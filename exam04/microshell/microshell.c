#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef	struct	s_cmd
{
	int				start;
	int				end;
	int				input;
	int				output;
	int				type;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}				t_cmd;

static int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

static void	ft_putstr(char *s)
{
	write(2, s, ft_strlen(s));
}

static void	fatal(void)
{
	ft_putstr("error: fatal\n");
	exit(1);
}

static char	*get_name(char *s)
{
	int		i;
	int		j;
	int		k;
	char	*new;

	i = ft_strlen(s) - 1;
	j = i;
	while (i >= 0 && s[i] != '/')
		i--;
	if (i == -1)
		i = 0;
	else
		i++;
	new = malloc(sizeof(char) * (j - i + 1 + 1));
	if (new == NULL)
		fatal();
	k = 0;
	while (i <= j)
	{
		new[k] = s[i];
		k++;
		i++;
	}
	new[k] = '\0';
	return (new);
}

static char	*ft_strdup(char *s)
{
	int		i;
	char	*new;

	new = malloc(sizeof(char) * (ft_strlen(s) + 1));
	i = 0;
	while (i < ft_strlen(s))
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

static char	**char2dup(t_cmd *cmd, char **s)
{
	int		i;
	char	**new;

	new = malloc(sizeof(char *) * (cmd->end - cmd->start + 1 + 1));
	if (new == NULL)
		fatal();
	i = 0;
	while (i < cmd->end - cmd->start + 1)
	{
		if (i == 0)
			new[i] = get_name(s[cmd->start]);
		else
			new[i] = ft_strdup(s[cmd->start + i]);
		if (new == NULL)
			fatal();
		i++;
	}
	new[i] = NULL;
	return (new);
}

static void	add_cmd(t_cmd **cmd, int start, int end, int type)
{
	t_cmd	*new;
	t_cmd	*index;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
		fatal();
	new->start = start;
	new->end = end;
	new->input = 0;
	new->output = 1;
	new->type = type;
	new->next = NULL;
	if (*cmd == NULL)
	{
		new->prev = NULL;
		*cmd = new;
	}
	else
	{
		index = *cmd;
		while (index->next != NULL)
			index = index->next;
		new->prev = index;
		index->next = new;
	}
}

static void	parse_cmd(t_cmd **cmd, char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i], "|") == 0)
		{
			add_cmd(cmd, j, i - 1, 1);
			j = i + 1;
		}
		if (strcmp(argv[i], ";") == 0)
		{
			if (strcmp(argv[i - 1], ";") != 0 && i != 1)
				add_cmd(cmd, j, i - 1, 2);
			j = i + 1;
		}
		i++;
	}
	if (strcmp(argv[i - 1], "|") != 0 && strcmp(argv[i - 1], ";") != 0)
		add_cmd(cmd, j , i - 1, 0);
}

static void	exec_cd(t_cmd *cmd, char **argv)
{
	if (cmd->end - cmd->start != 1)
	{
		ft_putstr("error: cd: bad arguments\n");
		exit (1);
	}
	if (chdir(argv[cmd->end]) == -1)
	{
		ft_putstr("error: cd: cannot change directory to ");
		ft_putstr(argv[cmd->end]);
		ft_putstr("\n");
		exit (1);
	}
}

static void	redir(t_cmd *cmd)
{
	if (cmd->prev != NULL)
	{
		if (cmd->prev->type == 1)
			close(cmd->prev->output);
	}
	if (cmd->type == 1)
		close(cmd->next->input);
	if (cmd->input != 0)
	{
		dup2(cmd->input, 0);
		close(cmd->input);
	}
	if (cmd->output != 1)
	{
		dup2(cmd->output, 1);
		close(cmd->output);
	}
}

static void	exec_cmd(t_cmd *cmd, char **argv, char **envp)
{
	char	**arg_tmp;

	redir(cmd);
	arg_tmp = char2dup(cmd, argv);
	if (execve(argv[cmd->start], arg_tmp, envp) == -1)
	{
		ft_putstr("error: cannot execute ");
		ft_putstr(argv[cmd->start]);
		ft_putstr("\n");
		exit (1);
	}
}

static int	exec_all_cmd(t_cmd *cmd, char **argv, char **envp)
{
	int		fd[2];
	int		status;
	pid_t	pid;

	while (cmd != NULL)
	{
		if (cmd->type == 1)
		{
			pipe(fd);
			cmd->output = fd[1];
			cmd->next->input = fd[0];
		}
		if (strcmp(argv[cmd->start], "cd") == 0)
			exec_cd(cmd, argv);
		else
		{
			pid = fork();
			if (pid == 0)
				exec_cmd(cmd, argv, envp);
			if (cmd->prev != NULL)
			{
				if (cmd->prev->type == 1)
				{
					close(cmd->input);
					close(cmd->prev->output);
				}
			}
			waitpid(pid, &status, 0);
		}
		cmd = cmd->next;
	}
	return (status);
}

static void	free_all(t_cmd *cmd)
{
	t_cmd	*index;

	while (cmd != NULL)
	{
		index = cmd->next;
		free(cmd);
		cmd = index;
	}
}

int main(int argc, char **argv, char **envp)
{
	int		ret;
	t_cmd	*cmd;

	if (argc < 2)
		return (0);
	cmd = NULL;
	parse_cmd(&cmd, argv);
	ret = exec_all_cmd(cmd, argv, envp);
	free_all(cmd);
	return (ret);
}
