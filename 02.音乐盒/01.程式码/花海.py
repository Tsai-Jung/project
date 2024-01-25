zero = 10
do0 = 261
csharp4_db4 = 277
re0 = 293
dsharp4_eb4 = 311
mi0 = 329
fa0 = 349
fsharp4_gb4 = 369
so0 = 392
gsharp4_ab4 = 415
la0 = 440
asharp4_bb4 = 466
si0 = 493
do = 523
csharp5_db5 = 554
re = 587
dsharp5_eb5 = 622
mi = 659
fa = 698
fsharp5_gb5 = 739
so = 783
gsharp5_ab5 = 830
la = 880
asharp5_bb5 = 932
si = 987
do1 = 1046
csharp6_db6 = 1108
re1 = 1174
dsharp6_eb6 = 1244
mi1 = 1318
fa1 = 1396
fsharp6_gb6 = 1479
so1 = 1567
gsharp6_ab6 = 1661
re1 = 1760
asharp6_bb6 = 1864
si1 = 1975
do2 = 2093

x=140

ms = 50
mh4 = x
mh2 = 2*mh4
m1 = 2*mh2
m15 = 1.5*m1
m2 = 2*m1
m3 = 3*m1
m35 = 3.5*m1

songtest = [[do, mh2], [mi, mh2], [mi, m2],[mi,mh2],[re,mh2],[do,mh2],[re,m1],[mi,m15],[zero,m1],
            [do,mh2],[mi,mh2],[mi,m2],[fa,mh2],[mi,mh2],[so,mh2],[do,m35],
            [do,mh2],[fa,mh2],[fa,m2],[fa,mh2],[mi,mh2],[do,mh2],[so0,m1],[mi,m15],
            [do,mh2],[mi,mh2],[re,m1],[do,mh2],[mi,mh2],[re,m1],[do,mh2],[mi,mh2],[re,m3],
            [do,mh2],[mi,mh2],[mi,m2],[mi,mh2],[re,mh2],[do,mh2],[re,m1],[so,mh2],[mi,mh2],
            [do,mh2]]  # Sample data with arbitrary number of sets

songtest2 =[[do, mh2],[re, mh2],[mi, mh2],[so, mh2],[so, m15],[zero, mh4],
            [mi, mh2],[re, mh2],[do, mh2],[re, mh2],[mi, m2],
            [do, mh2],[re, mh2],[mi, mh2],[si, mh2],[do1, m15],
            [do1, mh2],[si, mh2],[la, mh2],[so, mh2],[la, mh2],[so, m15],[zero, mh4],
            [do, mh2],[re, mh2],[mi, mh2],[so, mh2],[so, m15],[zero, mh4],
            [mi, mh2],[re, mh2],[do, mh2],[re, mh2],[do, m15],[zero, mh4],
            [do, mh2],[re, mh2],[mi, mh2],[so, mh2],[so, mh2],[fa, mh2],[fa, mh2],[mi, mh2],
            [do1, m1],[mi, mh2],[re, m15],[zero, mh4]]


song1 = []
song2 = []

for data in songtest2:
    song1.append(data[0])
    song2.append(data[1])

#print(song1)  # Output: ['a', 'c', 'e', ...]
#print(song2)  # Output: ['b', 'd', 'f', ...]

# 创建一个新的数组来存储插入0后的结果
new_song = []
new_duration = []

# 遍历原始数组
for num in song1:
    # 将当前数字添加到新数组中
    new_song.append(num)
    # 在当前数字后插入0
    new_song.append(zero)

for num in song2:
    # 将当前数字添加到新数组中
    new_duration.append(num)
    # 在当前数字后插入0
    new_duration.append(ms)

# 输出结果
print(len(song1))
print(new_song)
print("_______")
print(len(song2))
print(new_duration)


