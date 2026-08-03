#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char	*ft_strdup(const char *s);
char	*ref_ft_strdup(const char *s);

static int	run_case(const char *label, const char *s)
{
	char	*got;
	char	*want;

	got = ft_strdup(s);
	want = ref_ft_strdup(s);
	if (got == NULL && want == NULL)
	{
		printf("OK  [%s] NULL\n", label);
		return (0);
	}
	if (got == NULL || want == NULL)
	{
		fprintf(stderr, "FAIL [%s] null mismatch asm=%p c=%p\n", label,
			(void *)got, (void *)want);
		return (1);
	}
	if (strcmp(got, want) != 0)
	{
		fprintf(stderr, "FAIL [%s] asm=\"%s\" c=\"%s\"\n", label, got, want);
		free(got);
		free(want);
		return (1);
	}
	printf("OK  [%s] \"%s\"\n", label, got);
	free(got);
	free(want);
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
	err |= run_case("null", NULL);
	if (err)
		return (1);
	printf("ft_strdup.s matches libft/ft_strdup.c on all cases.\n");
	return (0);
}
