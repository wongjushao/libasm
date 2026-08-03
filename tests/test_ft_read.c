#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

ssize_t	ft_read(int fd, void *buf, size_t count);

static int	run_ok(const char *label, const char *input, size_t write_len,
		size_t read_count)
{
	int		p_asm[2];
	int		p_libc[2];
	char	got_buf[64];
	char	want_buf[64];
	ssize_t	got;
	ssize_t	want;

	if (pipe(p_asm) != 0 || pipe(p_libc) != 0)
	{
		perror("pipe");
		return (1);
	}
	if (write(p_asm[1], input, write_len) != (ssize_t)write_len
		|| write(p_libc[1], input, write_len) != (ssize_t)write_len)
	{
		perror("write");
		close(p_asm[0]);
		close(p_asm[1]);
		close(p_libc[0]);
		close(p_libc[1]);
		return (1);
	}
	close(p_asm[1]);
	close(p_libc[1]);
	memset(got_buf, 0, sizeof(got_buf));
	memset(want_buf, 0, sizeof(want_buf));
	got = ft_read(p_asm[0], got_buf, read_count);
	want = read(p_libc[0], want_buf, read_count);
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
	if (memcmp(got_buf, want_buf, (size_t)got) != 0)
	{
		fprintf(stderr, "FAIL [%s] buffer content mismatch\n", label);
		return (1);
	}
	printf("OK  [%s] read %zd bytes\n", label, got);
	return (0);
}

static int	run_err(const char *label, int fd, void *buf, size_t count)
{
	int		save;
	ssize_t	got;
	ssize_t	want;
	int		got_errno;
	int		want_errno;

	save = errno;
	errno = 0;
	got = ft_read(fd, buf, count);
	got_errno = errno;
	errno = 0;
	want = read(fd, buf, count);
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
	char		buf[8];
	int		err;

	msg = "hello world";
	err = 0;
	err |= run_ok("short", msg, 5, 5);
	err |= run_ok("partial", msg, 5, 3);
	err |= run_ok("zero_count", msg, 5, 0);
	err |= run_err("bad_fd", -1, buf, 1);
	err |= run_err("closed_fd", 99999, buf, 1);
	if (err)
		return (1);
	printf("ft_read.s matches read(2) on all cases.\n");
	return (0);
}
