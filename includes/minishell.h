/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 13:40:36 by dimendon          #+#    #+#             */
/*   Updated: 2025/08/26 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard C Library
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

// POSIX System Calls
# include <fcntl.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

// Directory operations
# include <dirent.h>

// Termcap Library
# include <termcap.h>

// Terminal attributes
# include <termios.h>

// GNU Readline Library
# include <readline/history.h>
# include <readline/readline.h>

extern int		g_exit_code;

typedef struct s_token
{
	char		*str;
	int			quoted;
	int			type;
}				t_token;

typedef struct s_command
{
	t_token		**tokens;
	int			in_fd;
	int			out_fd;
	int			cmd_type;
	int			has_heredoc;
}				t_command;

typedef struct s_pipeline
{
	char		**envp;
	char		**segments;
	pid_t		*pids;
	int			nbr_segments;
	t_command	*cmds;
}				t_pipeline;

typedef struct s_pipe_context
{
	int			in_fd;
	int			*fd;
	int			last;
}				t_pipe_context;

typedef struct s_redir_ctx
{
	char		**envp;
	int			*in_fd;
	int			*out_fd;
}				t_redir_ctx;

typedef struct s_redir_iter
{
	int			i;
	int			j;
	int			count;
}				t_redir_iter;

typedef struct s_span
{
	int			start;
	int			end;
}				t_span;

typedef struct s_pipe_state
{
	size_t		start;
	size_t		seg;
	char		**arr;
}				t_pipe_state;

typedef struct s_split_ctx
{
	int			j;
	int			start;
	char		quote;
}				t_split_ctx;

typedef struct s_expand_ctx
{
	char		*str;
	char		**envp;
	char		quote;
	char		*result;
	int			i;
	int			start;
}				t_expand_ctx;

// ==================== BUILTINS ====================
short int		custom_cd(char ***envp, t_token **args);
short int		custom_echo(t_token **args);
short int		custom_env(char **envp, t_token **args);
short int		custom_exit(t_token **args);
short int		custom_export(char ***env, t_token **args);
short int		custom_pwd(void);
short int		custom_unset(char ***envp, t_token **args);
void			print_env(char **env, int *index, int size);
void			sort_index(char **env, int *index, int size);
void			init_export_index(int *index, int size);
int				is_valid_name(const char *name);
int				update_or_add_env(char ***env, char *arg);
char			*normalize_token(const char *str);
int				is_valid_n_flag(const char *s);
int				skip_quoted_n_flags(t_token **arg, int i);
int				skip_n_flags(t_token **arg, int start);

// ==================== EXECUTION ====================
int				trim_command_name(t_token **cmd);
int				dispatch_builtin(char *name, char ***envp, t_token **cmd);
int				run_builtin(char ***envp, t_token **cmd);
void			process_command(char ***envp, char *line);
int				is_folder(char *arg);
t_token			**prepare_command(char *segment, int *in_fd, int *out_fd,
					char ***envp);
void			setup_redirections(int in_fd, int out_fd, int *save_in,
					int *save_out);
void			restore_redirections(int save_in, int save_out);
short int		is_builtin(const char *cmd);
int				execute_command(char *path, t_token **tokens, char **envp);
char			**prepare_argv_from_tokens(t_token **tokens);

// ==================== ENVIRONMENT ====================
char			*get_env_value(char **envp, const char *name);
char			*get_path(char **envp, char **cmd);
char			**copy_envp(char **envp);
int				env_size(char **env);
char			**env_realloc_add(char **env);
int				env_add(char ***env_ptr, const char *new_var);

// ==================== PIPING ====================
void			execute_pipeline(char **envp, char **segments);
void			execute_cmd(char **envp, t_token **cmd);
int				exit_code_from_errno(void);
char			**tokens_to_argv(t_token **cmd);
void			close_pipe(int *fd);
void			parent_cleanup(int *in_fd, int *fd, int i, int num);
void			wait_for_all(pid_t *pids, int count);
void			cleanup_commands(t_pipeline *pipeline, int count);
int				build_command(t_pipeline *p, int i, char **envp);
t_token			**handle_redirections(t_token **cmd, char **envp, int *in_fd,
					int *out_fd);
int				handle_heredoc(const char *delim, int quoted, char **envp,
					int *in_fd);
void			pipeline_loop(t_pipeline *pipeline);
void			handle_input_redirection(int redir_in, int *in_fd);
void			handle_output_redirection(int redir_out, int *fd, int last);
void			child_process(char **envp, t_command *cmd,
					t_pipe_context pipe_ctx);
void			spawn_child(t_pipeline *p, t_pipe_context ctx, int i);
int				open_infile(char *file, int *in_fd);
int				open_outfile(char *file, int *out_fd);
int				open_appendfile(char *file, int *out_fd);
void			free_token(t_token *tok);
void			free_token_array(t_token **arr, int count);
int				count_tokens(t_token **arr);
t_token			**alloc_clean_array(int count);
int				apply_infile(char *filename, int *in_fd);
int				apply_heredoc(char *filename, int quoted, char **envp,
					int *in_fd);
int				apply_outfile(char *filename, int *out_fd);
int				apply_append(char *filename, int *out_fd);
int				fetch_target(t_token **cmd, int i, char **filename,
					int *quoted);
int				dispatch_redirection(int type, char *filename, int quoted,
					t_redir_ctx *ctx);
void			remove_consumed_tokens(t_token **cmd, int *i, int has_filename);
t_token			**init_clean_array(t_token **cmd, int *count);
t_token			**finalize_clean_array(t_token **cmd, t_token **clean, int j);

// ==================== PARSING ====================

// Tokenization
t_token			*new_token(const char *str, int quoted, int type);
t_token			**tokenize_command(char const *s, char c, char **envp);
char			**split_pipes(const char *line);
t_token			**alloc_token_array(const char *s, char c, int *token_num);
int				populate_tokens(t_token **arr, const char *s, char c);
int				expand_all(t_token **arr, char **envp);
t_token			**post_process_tokens(t_token **arr);

// Expansion
void			mark_quoted_spaces(char *str);
void			remove_quotes(char *str);
void			restore_marked_spaces(char *str);
char			*append_literal(char *result, char *str, int start, int i);
char			*expand_var(char *str, int *var_len);
char			*append_expanded_var(char *result, char *str, int *i,
					char **envp);
char			*collect_var_name(char *str, int *i);
int				process_dollar(t_expand_ctx *ctx);
char			*build_expanded_str(char *str, char **envp);
t_token			**split_expanded_tokens(t_token **arr);
int				expand_token(t_token **out, t_token *tok, int *k);
t_token			**allocate_output(t_token **arr, int *total);
char			**split_whitespace(const char *s);

// Redirections
t_token			**split_redirs(t_token **arr);
int				total_parts(t_token **arr);

// ==================== UTILS ====================
void			free_cmd(char **cmd);
void			free_tokens(t_token **arr);
int				count_strings(char **arr);
void			error_msg(const char *prefix, const char *msg);
void			error_perror(const char *context);

// ==================== TOKEN UTILS ====================
int				fully_quoted(const char *s);
int				is_delim(char ch, char delim);
size_t			skip_token(const char *s, size_t i, char c);
size_t			token_count(const char *s, char c);
size_t			next_c(const char *s, char c);
int				has_unclosed_quotes(const char *s);
void			handle_quote_state(char c, char *quote);

// ==================== TOKEN BUILD UTILS ====================
char			*next_substring(const char **s, char c, int *error);
void			assign_token_fields(char *substr, int *quoted, int *type);
int				store_token(t_token **slot, char *substr, int quoted, int type);

#endif
