from aes.transformations import *
from aes.key_expansion import *


def do_aes_128(_in, key):
	rkeys = key_expansion(key)
	state = add_round_key(_in, rkeys[0])

	for i in range(1, 10):
		state = sub_bytes(state)
		state = shift_rows(state)
		state = mix_columns(state)
		state = add_round_key(state, rkeys[i])

	state = sub_bytes(state)
	state = shift_rows(state)
	state = add_round_key(state, rkeys[-1])

	return state


TESTKEY = bytearray(range(16))
TESTDAT = bytes.fromhex("00112233 44556677 8899aabb ccddeeff")

print(do_aes_128(TESTDAT, TESTKEY).hex())

# output
# 69c4e0d86a7b0430d8cdb78070b4c55a
# cyberchef[0]
# 69c4e0d86a7b0430d8cdb78070b4c55a
