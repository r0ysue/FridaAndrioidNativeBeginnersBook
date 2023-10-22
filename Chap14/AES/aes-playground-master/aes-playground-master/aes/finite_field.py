from .utils import bitn, blen

"""
	See section 4 of FIPS 197.
	
	Implements arithmetic over GF(2^8)
"""


def ff_add(a, b):
	"""
		Section 4.1: Addition
		
		>>> hex(ff_add(0x57, 0x83))
		'0xd4'
		
	"""

	return a ^ b


def ff_divmod(a, b):
	"""
		Works like "long division"
		
		https://en.wikipedia.org/wiki/Finite_field_arithmetic#Rijndael.27s_finite_field
		
		        11111101111110 (mod) 100011011
		       ^100011011     
		       ---------------
		        01110000011110
		        ^100011011    
		        --------------
		         0110110101110
		         ^100011011   
		         -------------
		          010101110110
		          ^100011011  
		          ------------
		           00100011010
		            ^100011011
		            ----------
		             000000001
		
		>>> ff_divmod(0b11111101111110, 0x11b)
		(61, 1)
		
	"""

	q = 0
	r = a

	while blen(r) >= blen(b): # XXX: I don't like this implementation
		q ^= 1 << (blen(r) - blen(b))
		r ^= b << (blen(r) - blen(b))

	return q, r


def ff_multiply(a, b, modulus=0x11b):
	"""
		Section 4.2: Multiplication
		
		>>> hex(ff_multiply(0x57, 0x83))
		'0xc1'
		
		>>> hex(ff_multiply(0x53, 0xca))
		'0x1'
		
	"""

	# polynomial product via "long multiplication"
	# similar to calculating 132*456 as:
	# 123*6 + 1230*5 + 12300*4
	result = 0
	while b:
		result ^= a * (b & 1)
		a <<= 1
		b >>= 1

	# calculate residue
	_, r = ff_divmod(result, modulus)

	return r


def ff_multiplicative_inverse(a, modulus=0x11b):
	"""
		Based on extended Euclidean algorithm
		
		>>> hex(ff_multiplicative_inverse(0x53))
		'0xca'
		
	"""

	b = modulus
	x0, x1 = 0, 1
	while a:
		(q, a), b = ff_divmod(b, a), a
		x0, x1 = x1, x0 ^ ff_multiply(q, x1, modulus)

	_, r = ff_divmod(x0, modulus)

	return r


def ff_mulx(a, x):
	"""
		Section 4.2.1: Multiplication by x
		
		FIPS-197 describes an optimised method of doubling a polynomial, however
		for simplicity we can just use ff_multiply(a, 2)
		
		>>> hex(ff_mulx(0x57, 0x13))
		'0xfe'
		
	"""

	result = 0
	while x:
		result ^= a * (x & 1)
		a = ff_multiply(a, 2)
		x >>= 1

	return result
