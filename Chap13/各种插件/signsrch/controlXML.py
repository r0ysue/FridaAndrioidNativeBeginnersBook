import codecs

def load_signatures():
    import xml.etree.ElementTree as ET

    db = "hashsrch.xml"
    root = ET.parse(db).getroot()

    # There were different constant characteristics between different types of cryptographic algorithms.
    # For example, most hash functions would use one or several specific constant values, and most block
    # ciphers and serial ciphers would useone or several constant tables.
    # 这就是Findcrypt/Signsrch以及大部分静态检测加密工具的原理
    signature = []
    for p in root:
        # <p t="name [64.le rev.64&amp;]">
        name, data = p.attrib['t'].split(" [")
        bits, endian, size = data[:-1].split(".")

        if "&" in size:
            if bits == "float":
                bits = 32
            elif bits == "double":
                bits = 64
            else:
                bits = int(bits)

        signature.append({
            "name": name,
            "bits": bits,
            "endian": endian,
            "size": size,
            "data": codecs.decode(p.text, ('hex')),
        })

    return signature


print(load_signatures())