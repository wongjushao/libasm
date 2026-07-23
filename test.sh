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

run_one() {
	local name="$1"
	local asm="${ROOT}/${name}.s"
	local c_ref="${LIBFT}/${name}.c"
	local test_c="${TESTS}/test_${name}.c"
	local out="${BUILD}/${name}"

	if [[ ! -f "${asm}" ]] || [[ ! -s "${asm}" ]]; then
		echo "SKIP ${name}: missing or empty ${name}.s"
		return 0
	fi
	if [[ ! -f "${c_ref}" ]]; then
		echo "SKIP ${name}: no libft/${name}.c"
		return 0
	fi
	if [[ ! -f "${test_c}" ]]; then
		echo "SKIP ${name}: no tests/test_${name}.c"
		return 0
	fi

	mkdir -p "${out}"

	echo "==> Assemble ${asm}"
	nasm -f elf64 "${asm}" -o "${out}/${name}.o"

	if ! nm "${out}/${name}.o" | grep -q " T ${name}\$"; then
		echo "Link error: ${asm} must export a global symbol ${name}." >&2
		echo "Add: global ${name}  and label ${name}: in the .text section." >&2
		return 1
	fi

	echo "==> C reference from libft/${name}.c (as ref_${name})"
	sed -E \
		-e 's/#include "libft.h"/#include <stddef.h>/' \
		-e "s/\\b${name}\\b/ref_${name}/g" \
		"${c_ref}" > "${out}/ref_${name}.c"

	gcc -Wall -Wextra -Werror -c "${out}/ref_${name}.c" -o "${out}/ref_${name}.o"
	gcc -Wall -Wextra -Werror -c "${test_c}" -o "${out}/test_${name}.o"
	gcc -Wall -Wextra -Werror \
		"${out}/test_${name}.o" \
		"${out}/${name}.o" \
		"${out}/ref_${name}.o" \
		-o "${out}/test_${name}"

	echo "==> Compare asm (${name}.s) vs libft (${name}.c)"
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
