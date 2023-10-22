from .finite_field import *
from .utils import *

"""
	See section 5 of FIPS 197.
	
	This file implements the state tranformation functions
"""


# Section 5.1.1: SubBytes() Transformation
def sub_bytes(state):
	# apply sub_byte() to each element of the state
	return bytearray(map(sub_byte, state))


def sub_byte(x):
	x = ff_multiplicative_inverse(x)

	# affine transform
	x ^= rotl(x, 1) ^ rotl(x, 2) ^ rotl(x, 3) ^ rotl(x, 4) ^ 0x63

	return x


# Section 5.3.2: InvSubBytes() Transformation
def inv_sub_bytes(state):
	# apply inv_sub_byte() to each element of the state
	return bytearray(map(inv_sub_byte, state))


def inv_sub_byte(x):
	# inverse affine transform
	x = rotl(x, 1) ^ rotl(x, 3) ^ rotl(x, 6) ^ 0x05

	return ff_multiplicative_inverse(x)


def shift_rows(state):
	"""
		Section 5.1.2: ShiftRows() Transformation
		
		!!! FUN FACT ALERT !!!
		Rotating a row by one position to the left is equivalent to polynomial
		multiplication by {01}x^3 + {00}x^2 + {00}x^1 + {00}x^0, modulo x^4 + 1
		
		>>> shift_rows(b"ABCDefghIJKLmnop")
		bytearray(b'AfKpeJoDInChmBgL')
		
	"""

	s2 = bytearray(4 * 4)
	for i in range(4):
		row = state[i : 4 * 4 : 4]
		s2[i : 4 * 4 : 4] = row[i:] + row[:i]
	return s2


def inv_shift_rows(state):
	"""
		Section 5.3.1: InvShiftRows() Transformation
		
		>>> inv_shift_rows(b"AfKpeJoDInChmBgL")
		bytearray(b'ABCDefghIJKLmnop')
		
	"""

	return shift_rows(shift_rows(shift_rows(state)))  # -1 % 4 == 3. don't @ me.


# Section 5.1.3: MixColumns() Transformation
def mix_columns(state):
	return map_columns(mix_column, state)


def mix_column(col):
	"""
		multiplication by mat_a is equivalent to polynomial multiplication by
		{03}x^3 + {01}x^2 + {01}x^1 + {02}x^0, modulo x^4 + 1
		
		See Section 4.3 (Polynomials with Coefficients in GF(2^8))
		
	"""

	mat_a = [[0x02, 0x03, 0x01, 0x01],
	         [0x01, 0x02, 0x03, 0x01],
	         [0x01, 0x01, 0x02, 0x03],
	         [0x03, 0x01, 0x01, 0x02]]

	# matrix multiplication
	c2 = bytearray(4)
	for y in range(4):
		for x in range(4):
			c2[y] ^= ff_mulx(col[x], mat_a[y][x])

	return c2


# Section 5.1.3: MixColumns() Transformation
def inv_mix_columns(state):
	return map_columns(inv_mix_column, state)


def inv_mix_column(col):
	"""
		mat_ai represents the multiplcative inverse of mat_a, modulo x^4 + 1, i.e.:
		{0b}x^3 + {0d}x^2 + {09}x^1 + {0e}x^0
		
		>>> inv_mix_columns(mix_columns(b"ABCDEFGHIJKLMNOP"))
		bytearray(b'ABCDEFGHIJKLMNOP')
		
	"""

	mat_ai = [[0x0e, 0x0b, 0x0d, 0x09],
	          [0x09, 0x0e, 0x0b, 0x0d],
	          [0x0d, 0x09, 0x0e, 0x0b],
	          [0x0b, 0x0d, 0x09, 0x0e]]

	# matrix multiplication
	c2 = bytearray(4)
	for y in range(4):
		for x in range(4):
			c2[y] ^= ff_mulx(col[x], mat_ai[y][x])

	return c2


# Section 5.1.4: AddRoundKey() Transformation
def add_round_key(state, rkey):
	return xor_bytes(state, rkey)
