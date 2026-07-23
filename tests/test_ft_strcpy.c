#include <stdio.h>
#include <string.h>

char	*ft_strcpy(char *dest, const char *src);
char	*ref_ft_strcpy(char *dest, const char *src);

static int	run_copy(const char *label, const char *src)
{
	char	dest_asm[64];
	char	dest_ref[64];
	char	*got;
	char	*want;

	memset(dest_asm, 0xAA, sizeof(dest_asm));
	memset(dest_ref, 0xBB, sizeof(dest_ref));
	got = ft_strcpy(dest_asm, src);
	want = ref_ft_strcpy(dest_ref, src);
	if (got != dest_asm || want != dest_ref)
	{
		fprintf(stderr, "FAIL [%s] return pointer mismatch\n", label);
		return (1);
	}
	if (strcmp(dest_asm, dest_ref) != 0)
	{
		fprintf(stderr, "FAIL [%s] dest content mismatch\n", label);
		return (1);
	}
	printf("OK  [%s] dest=\"%s\"\n", label, dest_asm);
	return (0);
}

static int	run_null(const char *label, char *dest, const char *src)
{
	char	*got;
	char	*want;

	got = ft_strcpy(dest, src);
	want = ref_ft_strcpy(dest, src);
	if (got != want)
	{
		fprintf(stderr, "FAIL [%s] asm=%p c=%p\n", label,
			(void *)got, (void *)want);
		return (1);
	}
	printf("OK  [%s]\n", label);
	return (0);
}

int	main(void)
{
	char	buf[8];
	int	err;

	err = 0;
	err |= run_copy("empty", "");
	err |= run_copy("one_char", "a");
	err |= run_copy("word", "hello");
	err |= run_copy("digits", "42kl");
	memset(buf, 'x', sizeof(buf));
	err |= run_copy("into_buf", "ok");
	err |= run_null("null_dest", NULL, "x");
	err |= run_null("null_src", buf, NULL);
	if (err)
		return (1);
	printf("ft_strcpy.s matches libft/ft_strcpy.c on all cases.\n");
	return (0);
}
