# 0xffffffff is used to make sure numbers dont go over 32

def chunks(messageLength, chunkSize):
    chunkValues = []
    for i in range(0, len(messageLength), chunkSize):
        chunkValues.append(messageLength[i:i + chunkSize])

    return chunkValues


def leftRotate(chunk, rotateLength):
    return ((chunk << rotateLength) | (chunk >> (32 - rotateLength))) & 0xffffffff


def sha1Function(message):
    # initial hash values
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    messageLength = ""

    # preprocessing
    for char in range(len(message)):
        messageLength += '{0:08b}'.format(ord(message[char]))

    temp = messageLength
    messageLength += '1'

    while (len(messageLength) % 512 != 448):
        messageLength += '0'

    messageLength += '{0:064b}'.format(len(temp))
    chunk = chunks(messageLength, 512)

    for eachChunk in chunk:
        words = chunks(eachChunk, 32)
        w = [0] * 80
        for n in range(0, 16):
            w[n] = int(words[n], 2)

        for i in range(16, 80):
            # sha1
            # w[i] = leftRotate((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1)
            # sha0
            w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16])

            # Initialize hash value for this chunk:
        a = h0
        b = h1
        c = h2
        d = h3
        e = h4

        # main loop:
        for i in range(0, 80):
            if 0 <= i <= 19:
                f = (b & c) | ((~b) & d)
                k = 0x5A827999

            elif 20 <= i <= 39:
                f = b ^ c ^ d
                k = 0x6ED9EBA1

            elif 40 <= i <= 59:
                f = (b & c) | (b & d) | (c & d)
                k = 0x8F1BBCDC

            elif 60 <= i <= 79:
                f = b ^ c ^ d
                k = 0xCA62C1D6

            a, b, c, d, e = ((leftRotate(a, 5) + f + e + k + w[i]) & 0xffffffff, a, leftRotate(b, 30), c, d)

        h0 = h0 + a & 0xffffffff
        h1 = h1 + b & 0xffffffff
        h2 = h2 + c & 0xffffffff
        h3 = h3 + d & 0xffffffff
        h4 = h4 + e & 0xffffffff

    return '%08x%08x%08x%08x%08x' % (h0, h1, h2, h3, h4)


plainText = "123456"
sha1Hash = sha1Function(plainText)
print(sha1Hash)