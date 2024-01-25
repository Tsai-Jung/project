zero = 10
do0 = 523
csharp5_db5 = 554
re0 = 587
dsharp5_eb5 = 622
mi0 = 659
fa0 = 698
fsharp5_gb5 = 739
so0 = 783
gsharp5_ab5 = 830
la0 = 880
asharp5_bb5 = 932
si0 = 987
do = 1046
csharp6_db6 = 1108
re = 1174
dsharp6_eb6 = 1244
mi = 1318
fa = 1396
fsharp6_gb6 = 1479
so = 1567
gsharp6_ab6 = 1661
la = 1760
asharp6_bb6 = 1864
si = 1975
do1 = 2093
re1 = 2*re
mi1 = 2*mi
fa1 = 2*fa
so1 = 2*so
la1 = 2*la
si1 = 2*si
do2 = 2*do1


x=150

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


