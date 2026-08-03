#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

ssize_t	ft_write(int fd, const void *buf, size_t count);

static int	read_pipe(int fd, char *out, size_t outlen)
{
	size_t	total;
	ssize_t	n;

	total = 0;
	while (total < outlen)
	{
		n = read(fd, out + total, outlen - total);
		if (n < 0)
			return (-1);
		if (n == 0)
			break ;
		total += (size_t)n;
	}
	out[total] = '\0';
	return ((int)total);
}

static int	run_ok(const char *label, const void *buf, size_t count)
{
	int		p_asm[2];
	int		p_libc[2];
	char	got_buf[64];
	char	want_buf[64];
	ssize_t	got;
	ssize_t	want;
	int		got_read;
	int		want_read;

	if (pipe(p_asm) != 0 || pipe(p_libc) != 0)
	{
		perror("pipe");
		return (1);
	}
	got = ft_write(p_asm[1], buf, count);
	want = write(p_libc[1], buf, count);
	close(p_asm[1]);
	close(p_libc[1]);
	got_read = read_pipe(p_asm[0], got_buf, sizeof(got_buf) - 1);
	want_read = read_pipe(p_libc[0], want_buf, sizeof(want_buf) - 1);
	close(p_asm[0]);
	close(p_libc[0]);
	if (got != want)
	{
		fprintf(stderr, "FAIL [%s] return asm=%zd libc=%zd\n",
			label, got, want);
		return (1);
	}
	if (got < 0)
		return (0);
	if (got_read != want_read || memcmp(got_buf, want_buf, (size_t)got) != 0)
	{
		fprintf(stderr, "FAIL [%s] pipe content mismatch\n", label);
		return (1);
	}
	printf("OK  [%s] wrote %zd bytes\n", label, got);
	return (0);
}

static int	run_err(const char *label, int fd, const void *buf, size_t count)
{
	int		save;
	ssize_t	got;
	ssize_t	want;
	int		got_errno;
	int		want_errno;

	save = errno;
	errno = 0;
	got = ft_write(fd, buf, count);
	got_errno = errno;
	errno = 0;
	want = write(fd, buf, count);
	want_errno = errno;
	errno = save;
	if (got != want || got_errno != want_errno)
	{
		fprintf(stderr,
			"FAIL [%s] asm=(%zd, errno=%d) libc=(%zd, errno=%d)\n",
			label, got, got_errno, want, want_errno);
		return (1);
	}
	printf("OK  [%s] ret=%zd errno=%d\n", label, got, got_errno);
	return (0);
}

int	main(void)
{
	const char	*msg;
	int		err;

	msg = "hello";
	err = 0;
	err |= run_ok("short", msg, 5);
	err |= run_ok("partial", msg, 3);
	err |= run_ok("zero_count", msg, 0);
	err |= run_ok("empty", "", 0);
	err |= run_err("bad_fd", -1, msg, 1);
	err |= run_err("closed_fd", 99999, msg, 1);
	if (err)
		return (1);
	printf("ft_write.s matches write(2) on all cases.\n");
	return (0);
}
