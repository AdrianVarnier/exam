#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <stdio.h>

typedef struct	s_cmd
{
	int				start;
	int				end;
	int				input;
	int				output;
	int				type;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}				t_cmd;

//utils

static int	ft_strlen(char *s)
{
	if (s == NULL)
		return (0);
	int	i = 0;
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

static void	free_tab(char **s)
{
	if (s == NULL)
		return ;
	for (int i = 0; s[i] != NULL; i++)
		free(s[i]);
	free(s);
}

static void	free_cmd(t_cmd *cmd)
{
	if (cmd == NULL)
		return ;
	for (t_cmd *tmp = cmd->next; cmd != NULL; cmd = tmp)
	{
		tmp = cmd->next;
		free(cmd);
	}
}

static char	*ft_strdup(char *s)
{
	int 	i;
	char	*new;

	if ((new = malloc(sizeof(char) * (ft_strlen(s) + 1))) == NULL)
		fatal();
	i = 0;
	while (s[i] != '\0')
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

static int	ft_strlen_extract(char *s)
{
	int	i;

	i = ft_strlen(s);
	while (s[i] != '/' && s[i] != '\0')
		i--;
	return (ft_strlen(s) - i);
}

static char	*ft_strdup_extract(char *s)
{
	int 	i;
	char	*new;

	if ((new = malloc(sizeof(char) * (ft_strlen_extract(s) + 1))) == NULL)
		fatal();
	i = ft_strlen(s);
	new[i] = '\0';
	while (i != ft_strlen(s) - ft_strlen_extract(s))
	{
		new[i] = s[i];
		i--;
	}
	return (new);
}

static char	**tab_dup(char **s, int start, int end)
{
	int		i;
	char	**new;

	if ((new = malloc(sizeof(char *) * (end - start + 1))) == NULL)
		fatal();
	i = 0;
	while (i < end - start)
	{
		if (i == 0)
			new[i] = ft_strdup_extract(s[start + i]);
		else
			new[i] = ft_strdup(s[start + i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}

//cmd

static void	add_cmd(t_cmd **cmd, int i, int j, char **argv, int type)
{
	t_cmd	*new;
	t_cmd	*index;

	index = *cmd;
	if ((new = malloc(sizeof(t_cmd))) == NULL)
		fatal();
	new->next = NULL;
	new->prev = NULL;
	new->start = j;
	new->end = i;
	new->input = 0;
	new->output = 1;
	new->type = type;
	if (*cmd == NULL)
		*cmd = new;
	else
	{
		while (index->next != NULL)
			index = index->next;
		index->next = new;
		new->prev = index;
	}
}

static void	parse_cmd(t_cmd **cmd, char **argv)
{
	int		i;
	int		j;

	if (argv == NULL)
		return ;
	i = 1;
	j = 1;
	while (argv[i] != NULL)
	{
		if (strcmp(argv[i], "|") == 0)
		{
			add_cmd(cmd, i, j, argv, 1);
			j = i;
		}
		if (strcmp(argv[i], ";") == 0)
		{
			add_cmd(cmd, i, j, argv, 2);
			j = i;
		}
		i++;
	}
	add_cmd(cmd, i, j, argv, 0);
}

static int exec_cd(t_cmd *cmd, char **argv)
{
	if (cmd->end - cmd->start != 2)
	{
		ft_putstr("error: cd: bad arguments\n");
		return (1);
	}
	if (chdir(argv[cmd->start + 1]) == -1)
	{
		ft_putstr("error: cd: cannot change directory to ");
		ft_putstr(argv[cmd->start + 1]);
		ft_putstr("\n");
		return (1);
	}
	return (0);
}

static void	redir(t_cmd *cmd)
{
	if (cmd->type == 1)
		close(cmd->next->input);
	if (cmd->prev != NULL)
		if (cmd->prev->type == 1)
			close(cmd->prev->output);
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
	int		ret;
	char	**tmp;

	redir(cmd);
	tmp = tab_dup(argv, cmd->start, cmd->end - 1);
	while (cmd->prev != NULL)
		cmd = cmd->prev;
//	free_cmd(cmd);
	int i = 0;
	while (tmp[i] != NULL)
		printf("%s\n", tmp[i]);
	if (tmp[i] == NULL)
		printf("null\n");
	ret = execve(argv[cmd->start], tmp, envp);
	ft_putstr("error: cannot execute ");
	ft_putstr(tmp[0]);
	ft_putstr("\n");
	free_tab(tmp);
	exit(ret);
}

static int is_pipe(t_cmd *cmd)
{
	if (cmd->prev != NULL)
		if (cmd->prev->type == 1)
			return (1);
	return (0);
}

static int	exec_all_cmd(t_cmd *cmd, char **argv, char **envp)
{
	int		ret;
	int		fd[2];
	pid_t	pid;
	t_cmd	*tmp = cmd;

	while (cmd != NULL)
	{
		if (cmd->type == 1)
		{
			pipe(fd);
			cmd->output = fd[0];
			cmd->next->input = fd[1];
		}
		if (strcmp(argv[cmd->start], "cd") == 0)
			ret = exec_cd(cmd, argv);
		else
		{
			pid = fork();
			if (pid == 0)
				exec_cmd(cmd, argv, envp);
			else if (pid > 0)
				waitpid(pid, &ret, 0);
		}
		if (is_pipe(cmd) == 1)
		{
			close(cmd->input);
			close(cmd->prev->output);
		}
		cmd = cmd->next;
	}
	free_cmd(tmp);
	return (ret);
}

//main

int main(int argc, char **argv, char **envp)
{
	t_cmd	*cmd = NULL;

	if (argc == 1)
		return (0);
	parse_cmd(&cmd, argv);
	return (exec_all_cmd(cmd, argv, envp));
}
