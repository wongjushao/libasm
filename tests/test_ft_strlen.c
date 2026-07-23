#include <stdio.h>
#include <stddef.h>

size_t	ft_strlen(const char *s);
size_t	ref_ft_strlen(const char *s);

static int	run_case(const char *label, const char *s)
{
	size_t	got;
	size_t	want;

	got = ft_strlen(s);
	want = ref_ft_strlen(s);
	if (got != want)
	{
		fprintf(stderr, "FAIL [%s] asm=%zu c=%zu\n", label, got, want);
		return (1);
	}
	printf("OK  [%s] len=%zu\n", label, got);
	return (0);
}

int	main(void)
{
	int	err;

	err = 0;
	err |= run_case("empty", "");
	err |= run_case("one_char", "a");
	err |= run_case("word", "hello");
	err |= run_case("spaces", "hello world");
	err |= run_case("digits", "42kl");
	err |= run_case("long",
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789");
	if (err)
		return (1);
	printf("ft_strlen.s matches libft/ft_strlen.c on all cases.\n");
	return (0);
}
