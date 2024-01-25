import time
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


note_mapping = {
    'c': "do",
    'd': "re",
    'e': "mi",
    'f': "fa",
    'g': "so",
    'a': "la",
    'b': "si",
    'r': 0
}

def get_note_and_duration(mml, index):
    char = mml[index]
    note = None
    duration = None

    if char.isalpha():
        note = note_mapping.get(char.lower())
        if note is not None:
            duration = 4  # default quarter note duration

            # Check for duration specified after the note
            if index + 1 < len(mml) and mml[index + 1].isdigit():
                index += 1
                duration = int(mml[index])

            # Check for dot duration specified after the note
            if index + 2 < len(mml) and mml[index + 1] == '.' and mml[index + 2].isdigit():
                index += 2
                duration = duration + duration // int(mml[index])

            return note, duration, index

    elif char.isdigit():
        duration = int(char)
        return note, duration, index

    return note, duration, index

new_song = []
new_song2 = []
new_duration = []
new_octave = []
def play_mml(mml, tempo=120):
    beats_per_minute = tempo
    seconds_per_beat = 60.0 / beats_per_minute
    default_note_duration = 4  # quarter note by default
    octave = 4
    volume = 8

    index = 0
    while index < len(mml):
        char = mml[index]
        note, duration, index = get_note_and_duration(mml, index)

        if note is not None and duration is not None:
            #print(f'Playing note: {note}, duration: {duration}, octave: {octave}, volume: {volume}')
            # play_note()
            new_song.append(note)
            new_duration.append(duration)
            new_octave.append(octave)
            #time.sleep(seconds_per_beat / duration)
        
        elif char == '>':
            octave += 1
            #print('Increasing octave')

        elif char == '<':
            octave -= 1
            #print('Decreasing octave')

        elif char == 'v':
            if index + 1 < len(mml) and mml[index + 1].isdigit():
                index += 1
                volume = int(mml[index])
                #print(f'Setting volume: {volume}')
        
        elif char == 'r':
            print('Rest')
            # rest()

        index += 1

mml_data = "t120r16.[2]v4o3g+4.v3g+4..:v5b4.z64.á+16.r64g+32.:v6b16òr64[2]|[2]o3b16r64b4x+32v7b16β+k+32r64v4g+16:v5b8y16r32.v4b16r64v5b16.:v6g+16.т1ο8r64v5g+16:b4&b32μ+2ō8r64v6g+16.:v5b8.ο+ξ1v6g+16.:v5b16.u+16ā+v6g+16.:v8b8..бr64v7b4..:v5g+16ι+4u64r32v9o4c+8..θ+16λ+r64v8c+8..:v9o3b2.&b16θ+16r32v13o4b4..:o3b4..é16о+1r32v6o1e4&e16.:v5o2e8ε+8[2]r64[2]v3o1e8:v2o2e8τ1r64v4e16:v5o1e8.q8ι+15r64v7o3g+8..:v5o2e16.:o1e8..p+л2r64f+8..:v8o4c+2..j+4ψ+4[2]r32[2]v6o1g+2.&g+8.:v8o4c+2..:v7o2g+8k1.r16v8f+16.ъс+8.r64v7f+8:v10o3a+2.r16v8o2e16.с+r64v10o3a+2.:v7o2e8:o1e8..h+64r16v8o2f+16.ō+16[2]r64[2]f+8:v11o3a+2&a+8r64v9o4b1з+á+2[2]r16.[2]o2f+16.ā+4r64f+8:v13o3a+2&a+8.:v8o1f+8χr32.v6o2g+1:v8o4d+1ы32ο+4[2]r32|r2.&r16..[2]v6o2b1&b8:v8o5d+8..:o4d+1&d+8υ64r64[2]|r2..&r16[2]o5d+4d+8..:o3g+4..r64o5d+4:v7o3g+1ξ+16r8.&r64o5d+8..x+64ω+8d+8..r64d+8..пυ+r64d+8..κ+1r64v6d+8..φ+2ψ+32v8d+8..r64o4b4&b32:o5d+4&d+32п+1v7o4b1.ē+[2]|r4.&r64[2]v9o4d+4:v8o5d+8..:v6o2b1.&b16r32.[2]|r4..&r32.[2]v8o5d+4j2д+30d+8..:v7o4d+8..u+2.r64v5d+4..ξ+ν+16r32v8d+8..ц+v9o5d+8..г1r64v8d+4:v9o4c+4&c+16:v8d+4ě+9r8&r32.o5d+8.ё+r64d+4л+1v9d+8..r64v7o4d+4:o5d+4x32p64v5o4d+8..ъr64v8d+2:o5d+8..ε+16.r16&r64v9o3b1.&b8е52r64v10o4f+8χ2ч+32r64v4o1g+1&g+16:v5o2g+2.&g+8.p+p32[2]r32|r1|r64[2]v8o5d+8..ζ64r64v11o4g+8..:o5d+8..:v8o3e2r8.&r64v10o5d+8..r64d+8..:v11o4a+8..ф+κr64o5d+4:o4a+4&a+32r16v10g+1&g+8.y43r32.[2]|r2.&r16&r64[2]v6o2e8..e4:v10o4c+4..ā+r32v7o2f+4ν+64v8f+8..п+64q+16.v11o4f+8..:v8o2f+8..r64f+4&f+16q4h+16r8&r32.v9g+8..л+19ο+v12o4c+4..:v8o2g+8..ē32r32v9o4c+2.&c+16:v6o1f+1й+32r8.v8o3g+2.κ+5r8.o2f+8..л+2θr64v7f+8..:v10o4c+2&c+16ó64r8&r32v6o2g+4v7g+8.ā+32r64v9o5g+8..:v7o2g+8..:v9o4b8..σ+w+3r64v8b2..:v7o2g+8..r32.v10o4b4&b16.к32p8r64b8ι8v5o2e2:v9o4b8..д64r16&r64v13o5c+2&c+8.v12c+8.r16v10c+8..ι+v11c+4..ó8к+32r32.v12c+1&c+16v9o4b4&b16.:v8f+2.&f+8.:v7c+2.&c+8.ο+16v12b4&b16.:v10f+1:v11c+1:v8o1f+1:v9o2f+2у+ur32.v10o5c+8..z53[2]r64[2]v11c+4..м4.r64v13c+8.э+r8&r32.f+4..о+r32v12f+4&f+32:v11o4f+4&f+32:v10o2f+4&f+32:v12o4a+4&a+32р4r4&r16o5f+4..:v11o4f+4..kмr64o5f+8r64o4a+4..н+8[2][2]v12o5f+8..:o4f+8..:a+8..:v11o3f+2:v10o2f+8ы4r8.g+8.ω+1.v9g+8x+r32[2]|[2]o2g+16.π+16g+4&g+32ν+30r16.v8o3f+4&f+16:v11o5f+4&f+16:v10o4f+4&f+16ι+4v13o5f+8..:v12o4f+8..:a+2&a+16:v11o3f+8..:v10o2f+16.r8&r32.v12o5c+8..:v11o4c+8..υ32r64c+2:v12o5c+2:v10o4g+8..w+21r8.v12o5c+8..:o4c+8..к+8c+4..:o5c+4..ór32v10o4d+2.&d+8.:o2g+4&g+32αв+16[2]r8&r32.[2]v8f+2&f+8.фr16&r64[2]|r8&r32[2]v9o5c+8..r64v10o3a+4..:v11o5c+4..:v10o4c+4..:v8o2a+4..y+16r8&r32.[2]|r16&r64[2]v13o4c+8..:o5c+8..:v10o2a+8..:v13o3a+8..:v12o4a+8..пv13c+2:v14o5c+2:v13o4a+2:v12o3a+2:v10o2a+4&a+16.ár64v5b2.&b8.á+[2]r64[2]v8o5c+2..:v7d+16.кr64o3b2..&b4..:v8o5c+2.&c+8.β6л+16r32[2]|r4..&r64v5o2b2..&b4..[2]v8o5d+8:o4a+2&a+8.r32v6o3b1.h+1в20[2]r64|r4.&r32.[2]v5o1e4:v4o2e1.&e16:v6o4g+4&g+16к+2r64[2]|r2&r16..[2]v7o5d+1&d+4.щ4r64[2]|r1|r4&r64[2]o1f+4&f+16è4r32.o3c+2.е+64r32v8o5g+1&g+4.pцr32v9d+8..r64v10d+8.r16&r64v7o2e16.ъ64r64e4ě+2.v10o4g+4..:v8o3e8..:v7o2e8ж11ο4r32v8o4c+8..w[2]r16.[2]o3c+2&c+8:v11o5d+4ο+30h+56v9o4g+4..ι+4[2]r64[2]v12c+4..:v13o5c+4..:v11o4g+2:v8o3d+8..ιr8&r32.v10c+4&c+16.v9c+4.&c+32u49r32.[2]|r8&r64[2]v12o5g+8..:v11o4g+2.μò+1r16..[2]|r16&r64[2]v10o3c+2.&c+16yхr16..[2]|r32.[2]v9o2c+8..s+2r64c+4&c+16.:v10o3c+1&c+8:v11g+2:v12o4f1&f8:v11o3f1&f8r16..v10o4a+16.h50[2]r64v6o2e8:v8o4f+4..χ+r32f+4..ε+16[2]v10b4&b16.:v9f+4..:v8c+4..r64v10f+4..r8v12o5c+8.:v8o2g+16.r64v9g+8:v12o5c+4..ф+и+v8o2g+8.:v13o5c+8.r16v8o2f+16.у+r64f+8:o1f+2&f+8.:v12o5f+2&f+8.j1r16&r64v10o2e16.х2r64v13o4a+2:v8o2e8бr32.v12o4a+8.х+r32.v8o2f+16.:v12o5c+2&c+8h+4β+8r32v10o2f+8:v14o5c+8r64[2]|r16.[2]v9o2g+16.:v12o5c+8..ψ51c+4..:v9o2g+8:v8o1g+8к+v12o5c+8.δ16s+r32.v9o2f+8.v10f+8..s+r64v11f+8.r64v9f+8.u+2r8&r32.v10e4&e16sr64v9e4&e16.γ+2o5d+4&d+16:o4d+4&d+16:v10a+4.&a+32ā2[2][2]v13o5f+8..:o4f+8..:a+8..:v10o3c+8..q16.v14o4a+8..:v10o3c+8..:v13o4f+8..е+п2r64o5d+4.&d+32:o4b2..:v14d+2..:v12o2g+8.ē+54[2]v11o5d+4&d+16s+r64[2]v10o2f+4&f+16:v12o5d+2&d+16:o4d+2&d+16j14r64v11o5d+2.:g+4w4r64v13o6d+8:v14o5d+8w+r8..c+8..:v13o6c+8..нc+4.&c+32:v14o5c+4.&c+32:v12f+4.&f+32:a+4.&a+32:o3f+4.&f+32ж4r32.v9o1g+2..:o2g+8..:v11o5d+4&d+16y6θ4[2]r16|r8&r32[2]v9o2f+8..ā+f+2:v11o3f+2r8[2]|r16.[2]v9o5c+8..:v8o3a+8..:o4c+8..:v9a+8..:v7o2a+8..ο8r64v11o4c+4..:o3a+4..:o5c+4..:v8o2a+4..:v11o4a+4..yr8..&r64[2]|r64[2]v10o2a+4:v13o3a+4:v12o4c+4:o5c+4:o4a+4sv14o3a+2&a+16:v13o5c+2&c+16:o4c+2&c+16:v11a+2&a+16r8&r32v8o5g+4..r32.v15o6g+4:o5g+1&g+8.r16.v14o6g+2&g+8.:o5g+2.н+28у+r16&r64v10o2g+2&g+8τw+r8&r32v12o1e8..:o3g+2&g+16:o2e8..:v15o3b2&b16è+[2]r32.[2]v11b4..η+32v15b8.w+1ο+r64v11o6e8..:v14o4e4..:v8o3g+4..:v11b16u+1r16.v14o5g+1.&g+16:o6g+1.&g+16ц+4r32v6o2e8:v7b64:v8o4f+4.&f+32η8[2]r64[2]v10c+8..:f+4.&f+32α+r64b4&b16:v8o2f+8.v11o4b16.α+16r64[2]|r64[2]v12o4c+4..:v11b8..:f+4..ι5k4v10o5d+8.:v9o4b8.:v10g+8.аx+61[2]r64[2]v12o5d+8.:v10o4g+8.:v11b8.ó+r64v10o3g+4..r16&r64v8o1f+16r64f+16y8r64f+2&f+8.κè53r16.v12o6c+2.к+64r64v9o2f+8j[2]r32[2]f+16r32f+16s+8v8f+16r64v10f+16ж+64γ+r64v9f+16д2r64v8f+1&f+8ο+б+16v11o1f+32:v12o2f+2:v13o6d+4.&d+32:v12o5f+8..:v13a+8..п+16ǒ+[2]r32[2]o3f+4.:v12o2f+4&f+16.υ+64ъr8&r32v10o3e2&e8:v8g+16оr64v11o1f+32.:v12o2f+32.r16&r64v13o6f+4&f+16:o5f+4&f+16v15o6f+8.:v13o5f+8.h+8v12o2d+4:v14o5f+4:o6f+4:v12o5a+4ι+61r8&r32.v10o3g+16:v8d+16r32v10o2g+2&g+16:o3g+8..:v13o5f+4..:v14o6f+4..ёv13d+4&d+32:o5d+4&d+32:v11a+16.[2]r64[2]v13o6d+2ěr32v14o5d+2&d+8.:o6d+2&d+8.:v13o5b2&b8κ+τr32.o6d+8:v14o5d+2..ér32v9o1g+2..ж+17[2]r8..[2]o3f+8..:v11a+16:o4c+4&c+32r64v10c+4.&c+32:v8o3a+4.&a+32ēпr8.o2a+8..:o3a+8..:v10o5c+8..:v8o4c+8..r64v11o3a+4.&a+32:o5c+4.&c+32:v9o2a+4.&a+32:v11o4c+4.&c+32:a+4.&a+32r4v13o3a+8..:v10o2a+8..:v12o5c+8..:o4c+8..:a+8..χ+64ěr64v10o2a+4..:v12o3a+4..:v13o5c+4..:v11o4c+4..:v12a+4..r16v6o2b1..κ+[2]r64|r8&r64[2]v7o3f1.&f8щ1.r32o2e1&e8.к32r64[2]|r1|r4[2]v9o6d+16.wr64v8d+8н+v10o4g+2..:v9o6d+16.:v10o5g+16.v9o3g+1&g+4.:v12o5g+1&g+4.:v11o4g+1&g+4.η8ē64r4&r64[2]|r1|r1|r1|r1|r1|r1|"


play_mml(mml_data)
print(new_song)
print(new_duration)
print(len(new_song))
print(len(new_octave))
print(new_octave)
for num in range(len(new_song)):
    # 将当前数字添加到新数组中
    if(new_octave[num]==6 and new_song[num]!=0):
        new_song[num]=str(new_song[num])+'1'
    if(new_octave[num]==4 and new_song[num]!=0):
        new_song[num]=str(new_song[num])+'0'
    if(new_song[num]==0):
        new_song[num]=10
print(new_song)

for num in range(len(new_song)):
    if(new_song[num]=="do0"): new_song[num]=do0
    if(new_song[num]=="re0"): new_song[num]=re0
    if(new_song[num]=="mi0"): new_song[num]=mi0
    if(new_song[num]=="fa0"): new_song[num]=fa0
    if(new_song[num]=="so0"): new_song[num]=so0
    if(new_song[num]=="la0"): new_song[num]=la0
    if(new_song[num]=="si0"): new_song[num]=si0
    if(new_song[num]=="do"): new_song[num]=do
    if(new_song[num]=="re"): new_song[num]=re
    if(new_song[num]=="mi"): new_song[num]=mi
    if(new_song[num]=="fa"): new_song[num]=fa
    if(new_song[num]=="so"): new_song[num]=so
    if(new_song[num]=="la"): new_song[num]=la
    if(new_song[num]=="si"): new_song[num]=si
    if(new_song[num]=="do1"): new_song[num]=do1
    if(new_song[num]=="re1"): new_song[num]=re1
    if(new_song[num]=="mi1"): new_song[num]=mi1
    if(new_song[num]=="fa1"): new_song[num]=fa1
    if(new_song[num]=="so1"): new_song[num]=so1
    if(new_song[num]=="la1"): new_song[num]=la1
    if(new_song[num]=="si1"): new_song[num]=si1
    if(new_song[num]=="do2"): new_song[num]=do2
print(new_song)
print(len(new_song))
for num in range(len(new_duration)):
    # 将当前数字添加到新数组中
    new_duration[num]=new_duration[num]*100
print(new_duration)
print(len(new_duration))

# 创建一个新的数组来存储插入0后的结果
new_song1 = []
new_duration1 = []

for num in new_song:
    # 将当前数字添加到新数组中
    new_song1.append(num)
    # 在当前数字后插入0
    new_song1.append(zero)

for num in new_duration:
    # 将当前数字添加到新数组中
    new_duration1.append(num)
    # 在当前数字后插入0
    new_duration1.append(ms)


print(new_song1)
print("_______")
print(new_duration1)
