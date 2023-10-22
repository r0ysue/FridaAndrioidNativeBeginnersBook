1.Java层Hook观察特征

objection -g com.sankuai.meituan explore

android hooking watch class_method com.meituan.android.common.datacollectio
n.DataCollectionJni.packData --dump-args --dump-backtrace --dump-return



2.进入native层，开始使用脚本

使用trace natives 崩溃



3.观察发现使用base64，hook base64，打印调用栈

called from:
0xf37048b7 libc.so!je_malloc+0x36
0xbd45b4a7 libmtguard.so!0x44a7
0xbd45b57d libmtguard.so!0x457d
0xbd45b8f5 libmtguard.so!0x48f5
0xbd464e47 libmtguard.so!0xde47
0xbd463d4d libmtguard.so!0xcd4d
0xbd45ecf9 libmtguard.so!0x7cf9
0xc3d0eec7 base.odex!0x400ec7

called from:
0xf37048b7 libc.so!je_malloc+0x36
0xbd463d4d libmtguard.so!0xcd4d
0xbd45ecf9 libmtguard.so!0x7cf9
0xc3d0eec7 base.odex!0x400ec7

有点迷糊

先看这个函数参数吧，结合cyberchef确认

参数1是入参，参数2是长度，参数3是buffer，出来的结果。



4.我怎么不用自己教学的东西呢？

先重放攻击鸭

```powershell
[MIX 2S::美团]-> testsiua("0123456789abcd")
base64 result:y7YbniFhlQOy9KxsS96efQ==
cbb61b9e21619503b2f4ac6c4bde9e7d
[MIX 2S::美团]-> testsiua("0123456789abcdef")
base64 result:MCmRKYFf8UlT558PNSNNGYrn/LNDGxjFo+kUCkxRC+k=
30299129815ff14953e79f0f35234d198ae7fcb3431b18c5a3e9140a4c510be9
```

大概率是aes ，padding也大概率是pkcs#7

顺着findcrypt，找到36ac，需要注意的是，T0-T3的查表法aes实现必须掌握，非常重要



修改subkeys后还是对不上

开始字节流验证

第一个点是IV后

hook结果是

`48 3e 3f 3d 3f 3c 3f 3b 3f 3a 3f 39 3f 38 3f 37`

clion结果

`48 3e 3f 3d 3f 3c 3f 3b 3f 3a 3f 39 3f 38 3f 37`

参数2 buffer

参数3 是subkeys

hook结果

```powershell
b3ff9960  74 69 65 6d 30 6e 61 75 6b 6e 61 73 31 69 61 75  tiem0nauknas1iau
b3ff9970  e9 ae 9c 83 d9 c0 fd f6 b2 ae 9c 85 83 c7 fd f0  ................
b3ff9980  65 42 5a d5 bc 82 a7 23 0e 2c 3b a6 8d eb c6 56  eBZ....#.,;....V
b3ff9990  d4 1f b3 65 68 9d 14 46 66 b1 2f e0 eb 5a e9 b6  ...eh..Ff./..Z..
b3ff99a0  9a f6 0d 73 f2 6b 19 35 94 da 36 d5 7f 80 df 63  ...s.k.5..6....c
b3ff99b0  61 24 c0 fd 93 4f d9 c8 07 95 ef 1d 78 15 30 7e  a$...O......x.0~
b3ff99c0  92 98 99 d9 01 d7 40 11 06 42 af 0c 7e 57 9f 72  ......@..B..~W.r
b3ff99d0  d2 6b c2 42 d3 bc 82 53 d5 fe 2d 5f ab a9 b2 2d  .k.B...S..-_...-
b3ff99e0  0a 09 11 f5 d9 b5 93 a6 0c 4b be f9 a7 e2 0c d4  .........K......
b3ff99f0  42 55 89 10 9b e0 1a b6 97 ab a4 4f 30 49 a8 9b  BU.........O0I..
b3ff9a00  56 51 b2 e4 cd b1 a8 52 5a 1a 0c 1d 6a 53 a4 86  VQ.....RZ...jS..
```

clion

```powershell
   74 69 65 6d   30 6e 61 75   6b 6e 61 73   31 69 61 75   │ tiem0nauknas1iau │
   e9 ae 9c 83   d9 c0 fd f6   b2 ae 9c 85   83 c7 fd f0   │ ················ │
   65 42 5a d5   bc 82 a7 23   0e 2c 3b a6   8d eb c6 56   │ eBZ····#·,;····V │
   d4 1f b3 65   68 9d 14 46   66 b1 2f e0   eb 5a e9 b6   │ ···eh··Ff·/··Z·· │
   9a f6 0d 73   f2 6b 19 35   94 da 36 d5   7f 80 df 63   │ ···s·k·5··6····c │
   61 24 c0 fd   93 4f d9 c8   07 95 ef 1d   78 15 30 7e   │ a$···O······x·0~ │
   92 98 99 d9   01 d7 40 11   06 42 af 0c   7e 57 9f 72   │ ······@··B··~W·r │
   d2 6b c2 42   d3 bc 82 53   d5 fe 2d 5f   ab a9 b2 2d   │ ·k·B···S··-_···- │
   0a 09 11 f5   d9 b5 93 a6   0c 4b be f9   a7 e2 0c d4   │ ·········K······ │
   42 55 89 10   9b e0 1a b6   97 ab a4 4f   30 49 a8 9b   │ BU·········O0I·· │
   56 51 b2 e4   cd b1 a8 52   5a 1a 0c 1d   6a 53 a4 86   │ VQ·····RZ···jS·· │
```

下一个节点是addroundkeys

[From Hex, XOR, To Hex - CyberChef (gchq.github.io)](https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')XOR({'option':'Hex','string':'31 69 61 75'},'Standard',false)To_Hex('Space',0)&input=M2YgMzggM2YgMzc)



样本流程 75 61 69 31（key） xor  3f 38 3f 37（text）

`ef 0a a1 b7 60 2e a4 b7 52 51 5b 4c 06 56 59 4a`

```
0x4a595606
```

clion 的标准结果 31 69 61 75（key） xor  3f 38 3f 37（text）

`3c 57 5a 50   0f 52 5e 4e   54 54 5e 4a   0e 51 5e 42`





如果将密钥转成它那样子呢？

```
原
74 69 65 6d   30 6e 61 75   6b 6e 61 73   31 69 61 75
现
6d 65 69 74   75 61 6e 30   73 61 6e 6b   75 61 69 31
```

结果

```
25 5b 56 49   4a 5d 51 0b   4c 5b 51 52   4a 59 56 06 
```



