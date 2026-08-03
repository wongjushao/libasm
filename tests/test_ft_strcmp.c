#include <stdio.h>

int	ft_strcmp(const char *s1, const char *s2);
int	ref_ft_strcmp(const char *s1, const char *s2);

static int	sign(int n)
{
	if (n < 0)
		return (-1);
	if (n > 0)
		return (1);
	return (0);
}

static int	run_case(const char *label, const char *s1, const char *s2)
{
	int	got;
	int	want;

	got = ft_strcmp(s1, s2);
	want = ref_ft_strcmp(s1, s2);
	if (got != want)
	{
		fprintf(stderr, "FAIL [%s] asm=%d c=%d (s1=\"%s\", s2=\"%s\")\n",
			label, got, want, s1, s2);
		return (1);
	}
	printf("OK  [%s] cmp=%d (sign=%d)\n", label, got, sign(got));
	return (0);
}

int	main(void)
{
	int	err;

	err = 0;
	err |= run_case("both_empty", "", "");
	err |= run_case("equal_one", "a", "a");
	err |= run_case("equal_word", "hello", "hello");
	err |= run_case("prefix", "hello", "hell");
	err |= run_case("shorter_first", "abc", "abcd");
	err |= run_case("longer_first", "abcd", "abc");
	err |= run_case("first_diff", "abc", "axc");
	err |= run_case("last_diff", "abz", "aby");
	err |= run_case("case_diff", "A", "a");
	err |= run_case("digits", "42", "43");
	err |= run_case("spaces", "hello world", "hello worle");
	err |= run_case("unsigned", "\xFF", "\xFE");
	if (err)
		return (1);
	printf("ft_strcmp.s matches libft/ft_strcmp.c on all cases.\n");
	return (0);
}
