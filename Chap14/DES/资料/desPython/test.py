l = [1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0]
def showBits(s, n):#Split a list into sublists of size "n"
    return " ".join(["".join([str(i) for i in s[k:k+n]]) for k in range(0, len(s), n)])

print(showBits(l, 8))