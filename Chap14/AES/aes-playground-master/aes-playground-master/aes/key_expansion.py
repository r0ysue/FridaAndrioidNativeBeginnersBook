from .finite_field import *
from .transformations import *
from .utils import *

"""
	See section 5 of FIPS 197.
	
	This file implements key expansion and related functions
"""


def rcon(i):
	"""
		Section 5.2: Key Expansion
		
		Rcon[i], contains the values given by [x^(i-1), {00}, {00} ,{00}],
		with x^(i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
		
		Although Rcon[0] is never used, I rearranged x^(i-1) to be (x^(-1))^i so that
		it can still be calculated.
		
		x^(-1) == 2^(-1) == ff_multiplicative_inverse(2) == 0x8d
		
	"""

	return [ff_mulx(0x8d, 1 << i), 0x00, 0x00, 0x00]


def rot_word(word):
	return word[1:] + word[:1]


def sub_word(word):
	return bytearray(map(sub_byte, word))


def key_expansion(key):
	"""
		Section 5.2: Key Expansion
		
		AES-128 only, for now. See Figure 11
	"""

	round_keys = [key.copy()]
	for i in range(1, 10+1):
		rkey = bytearray()
		temp = xor_bytes(sub_word(rot_word(round_keys[-1][-4:])), rcon(i))
		for j in range(4):
			temp = xor_bytes(temp, round_keys[-1][j * 4 : j * 4 + 4])
			rkey += temp
		round_keys.append(rkey)

	return round_keys
