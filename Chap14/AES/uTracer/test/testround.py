page_size = 0x1000
end = 0x0
ret = int(end // page_size) * page_size
print(hex(ret))