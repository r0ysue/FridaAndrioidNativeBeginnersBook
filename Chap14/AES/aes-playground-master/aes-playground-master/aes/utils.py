def bitn(i, n):
	return (i >> n) & 1


def blen(n):
	return n.bit_length()


def masknbits(n):
	return ((1<<n)-1)


def rotl(i, n, order=8):
	return ((i << n) | (i >> order-n)) & masknbits(order)


# Apply a function column-wise
def map_columns(func, state):
	s2 = bytearray()
	for i in range(4):
		s2 += func(state[i * 4 : i * 4 + 4])
	return s2


def xor_bytes(a, b):
	return bytearray([ai^bi for ai, bi in zip(a, b)])


def transpose(state):
	s2 = []
	
	for x in range(4):
		for y in range(4):
			s2.append(state[y*4+x])
	
	return bytearray(s2)
