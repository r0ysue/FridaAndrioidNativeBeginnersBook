def convert_hex_to_array(subkeys):
    subkeys = [i for i in subkeys.split(" ")]
    return [subkeys[i:i + 4][::-1] for i in range(0, len(subkeys), 4)]

keys = "74 69 65 6d 30 6e 61 75 6b 6e 61 73 31 69 61 75 e9 ae 9c 83 d9 c0 fd f6 b2 ae 9c 85 83 c7 fd f0 65 42 5a d5 bc 82 a7 23 0e 2c 3b a6 8d eb c6 56 d4 1f b3 65 68 9d 14 46 66 b1 2f e0 eb 5a e9 b6 9a f6 0d 73 f2 6b 19 35 94 da 36 d5 7f 80 df 63 61 24 c0 fd 93 4f d9 c8 07 95 ef 1d 78 15 30 7e 92 98 99 d9 01 d7 40 11 06 42 af 0c 7e 57 9f 72 d2 6b c2 42 d3 bc 82 53 d5 fe 2d 5f ab a9 b2 2d 0a 09 11 f5 d9 b5 93 a6 0c 4b be f9 a7 e2 0c d4 42 55 89 10 9b e0 1a b6 97 ab a4 4f 30 49 a8 9b 56 51 b2 e4 cd b1 a8 52 5a 1a 0c 1d 6a 53 a4 86"
print(convert_hex_to_array(keys))