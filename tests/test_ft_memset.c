#include <stdio.h>
#include <string.h>

void	*ft_memset(void *s, int c, size_t n);
void	*ref_ft_memset(void *s, int c, size_t n);

static void	print_buf(const unsigned char *buf, size_t len)
{
	size_t	i;

	printf("  [");
	i = 0;
	while (i < len)
	{
		if (i > 0)
			printf(" ");
		printf("0x%02x", buf[i]);
		i++;
	}
	printf("]\n");
}

static int	run_case(const char *label, int c, size_t n, size_t buf_len)
{
	unsigned char	buf_asm[16];
	unsigned char	buf_ref[16];
	void			*got;
	void			*want;
	size_t			i;

	memset(buf_asm, 0xAA, buf_len);
	memset(buf_ref, 0xBB, buf_len);
	got = ft_memset(buf_asm, c, n);
	want = ref_ft_memset(buf_ref, c, n);
	if (got != buf_asm || want != buf_ref)
	{
		fprintf(stderr, "FAIL [%s] return pointer mismatch\n", label);
		return (1);
	}
	i = 0;
	while (i < n)
	{
		if (buf_asm[i] != buf_ref[i])
		{
			fprintf(stderr,
				"FAIL [%s] filled byte %zu: asm=0x%02x c=0x%02x\n",
				label, i, buf_asm[i], buf_ref[i]);
			return (1);
		}
		i++;
	}
	while (i < buf_len)
	{
		if (buf_asm[i] != 0xAA || buf_ref[i] != 0xBB)
		{
			fprintf(stderr,
				"FAIL [%s] tail byte %zu changed: asm=0x%02x ref=0x%02x\n",
				label, i, buf_asm[i], buf_ref[i]);
			return (1);
		}
		i++;
	}
	printf("OK  [%s] c=0x%02x n=%zu\n", label, (unsigned char)c, n);
	print_buf(buf_asm, buf_len);
	return (0);
}

int	main(void)
{
	int	err;

	err = 0;
	err |= run_case("zero_len", 'A', 0, 8);
	err |= run_case("one_byte", 'Z', 1, 8);
	err |= run_case("partial", 'x', 4, 8);
	err |= run_case("full", 'q', 8, 8);
	err |= run_case("zero_fill", 0, 6, 8);
	err |= run_case("negative_char", -1, 5, 8);
	err |= run_case("high_char", 255, 5, 8);
	if (err)
		return (1);
	printf("ft_memset matches libft/ft_memset.c on all cases.\n");
	return (0);
}
