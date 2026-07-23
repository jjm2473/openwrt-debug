# SPDX-License-Identifier: GPL-2.0
# Usage: awk -v name=module -f extract_struct.awk <preprocessed.i>
# Prints the first matching `struct <name> { ... };` block.

BEGIN {
	in_struct = 0
	brace_depth = 0
	found = 0
}

function count_char(s, c,    t, n) {
	t = s
	n = gsub(c, "", t)
	return n
}

{
	if (!in_struct) {
		if ($0 ~ "^struct[[:space:]]+" name "[[:space:]]*\\{") {
			in_struct = 1
			found = 1
			print $0
			brace_depth += count_char($0, "\\{")
			brace_depth -= count_char($0, "\\}")
			if (brace_depth == 0)
				exit
		}
		next
	}

	print $0
	brace_depth += count_char($0, "\\{")
	brace_depth -= count_char($0, "\\}")

	if (brace_depth == 0)
		exit
}

END {
	if (!found)
		exit 1
}
