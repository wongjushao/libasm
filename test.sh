#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIBFT="${ROOT}/libft"
BUILD="${ROOT}/build/test"
TESTS="${ROOT}/tests"

need() {
	if ! command -v "$1" >/dev/null 2>&1; then
		echo "Missing $1. Install it and re-run this script." >&2
		case "$1" in
			nasm) echo "  sudo apt install nasm" >&2 ;;
			gcc)  echo "  sudo apt install build-essential" >&2 ;;
		esac
		exit 1
	fi
}

ref_c_from_libft() {
	local src="$1"
	local sym="$2"
	local dest="$3"

	sed -E \
		-e 's/#include "libft.h"/#include <stddef.h>/' \
		-e "s/\\b${sym}\\b/ref_${sym}/g" \
		"${src}" > "${dest}"
}

run_one() {
	local name="$1"
	local asm="${ROOT}/${name}.s"
	local c_ref="${LIBFT}/${name}.c"
	local test_c="${TESTS}/test_${name}.c"
	local out="${BUILD}/${name}"
	local libc_ref=0
	local extra_asm=()
	local extra_ref=()
	local link_objs=()
	local dep

	if [[ ! -f "${asm}" ]] || [[ ! -s "${asm}" ]]; then
		echo "SKIP ${name}: missing or empty ${name}.s"
		return 0
	fi
	if [[ ! -f "${test_c}" ]]; then
		echo "SKIP ${name}: no tests/test_${name}.c"
		return 0
	fi
	if [[ ! -f "${c_ref}" ]]; then
		libc_ref=1
	fi

	case "${name}" in
	ft_strdup)
		extra_asm=(ft_strlen ft_strcpy)
		extra_ref=(ft_strlen)
		;;
	esac

	mkdir -p "${out}"

	echo "==> Assemble ${asm}"
	nasm -f elf64 "${asm}" -o "${out}/${name}.o"
	for dep in "${extra_asm[@]}"; do
		echo "==> Assemble ${ROOT}/${dep}.s (dependency)"
		nasm -f elf64 "${ROOT}/${dep}.s" -o "${out}/${dep}.o"
		link_objs+=("${out}/${dep}.o")
	done

	if ! nm "${out}/${name}.o" | grep -q " T ${name}\$"; then
		echo "Link error: ${asm} must export a global symbol ${name}." >&2
		echo "Add: global ${name}  and label ${name}: in the .text section." >&2
		return 1
	fi

	gcc -Wall -Wextra -Werror -c "${test_c}" -o "${out}/test_${name}.o"
	if [[ "${libc_ref}" -eq 1 ]]; then
		echo "==> Compare asm (${name}.s) vs libc (${name#ft_}())"
		gcc -Wall -Wextra -Werror -no-pie \
			"${out}/test_${name}.o" \
			"${out}/${name}.o" \
			-o "${out}/test_${name}"
	else
		for dep in "${extra_ref[@]}"; do
			echo "==> C reference from libft/${dep}.c (as ref_${dep})"
			ref_c_from_libft "${LIBFT}/${dep}.c" "${dep}" "${out}/ref_${dep}.c"
			gcc -Wall -Wextra -Werror -c "${out}/ref_${dep}.c" -o "${out}/ref_${dep}.o"
			link_objs+=("${out}/ref_${dep}.o")
		done

		echo "==> C reference from libft/${name}.c (as ref_${name})"
		if [[ "${name}" == "ft_strdup" ]]; then
			{
				cat <<'EOF'
#include <stdlib.h>
#include <stddef.h>

size_t ref_ft_strlen(const char *s);
EOF
				sed -E \
					-e '/#include "libft.h"/d' \
					-e "s/\\b${name}\\b/ref_${name}/g" \
					-e 's/\bft_strlen\b/ref_ft_strlen/g' \
					"${c_ref}"
			} > "${out}/ref_${name}.c"
		else
			ref_c_from_libft "${c_ref}" "${name}" "${out}/ref_${name}.c"
		fi

		gcc -Wall -Wextra -Werror -c "${out}/ref_${name}.c" -o "${out}/ref_${name}.o"
		gcc -Wall -Wextra -Werror -no-pie \
			"${out}/test_${name}.o" \
			"${out}/${name}.o" \
			"${out}/ref_${name}.o" \
			"${link_objs[@]}" \
			-o "${out}/test_${name}"
		echo "==> Compare asm (${name}.s) vs libft (${name}.c)"
	fi
	"${out}/test_${name}"
	echo
}

main() {
	local failed=0
	local name

	need nasm
	need gcc
	mkdir -p "${BUILD}"

	if [[ $# -gt 0 ]]; then
		for name in "$@"; do
			run_one "${name}" || failed=1
		done
	else
		for asm in "${ROOT}"/ft_*.s; do
			[[ -e "${asm}" ]] || continue
			name="$(basename "${asm}" .s)"
			run_one "${name}" || failed=1
		done
	fi

	if [[ "${failed}" -ne 0 ]]; then
		exit 1
	fi
}

main "$@"
