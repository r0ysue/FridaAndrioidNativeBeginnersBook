from aes.transformations import *
from aes.key_expansion import *


"""
See "A Tutorial on White-box AES", page 6
"""


def do_aes_128(_in, key):
	rkeys = key_expansion(key)
	state = _in

	for i in range(1, 10):
		state = shift_rows(state)
		state = add_round_key(state, shift_rows(rkeys[i-1]))
		state = sub_bytes(state)
		state = mix_columns(state)

	state = shift_rows(state)
	state = add_round_key(state, shift_rows(rkeys[-2]))
	state = sub_bytes(state)
	state = add_round_key(state, rkeys[-1])

	return state


TESTKEY = bytearray(range(16))
TESTDAT = bytes.fromhex("00112233 44556677 8899aabb ccddeeff")

print(do_aes_128(TESTDAT, TESTKEY).hex())

