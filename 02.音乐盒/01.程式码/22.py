import time

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
            print(f'Playing note: {note}, duration: {duration}, octave: {octave}, volume: {volume}')
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

mml_data = "l16>b>cv9gv6c-v8cgv6c-v8cv9g2v6c-v7cv9gv6c-v8cgv6c-v7cv8g2v6c-v7cv9gv6c-v8cv9gv6c-v7cv9g4rv7<gv8>cde4v7d4v6c4<g+4rv7ggv6ev8ev7ddcv8d8ev7er<ggv8av9>d8cd8e8cv7c4r8<gv8av9>d8v7cv8d8e8v7c4&cr8<gv8av9>d8v7cv8d8e8v7e8d8.v6<gv8ab>crv7ggv6ev7eddcv8d8ev7er<ggv8av9>d8v7cv8d8g8v6cv7c4r8<gv8av9>d8v7cv8d8e8v6c4&cr8v7<g8v8av9>d8v7cv8d8e8v6c4&cr8<gv8av9>d8v7cv8d8e8v6c4&v6<av7av8>cv9av8bv7a8g8r8ev8gv7fev8fv7ev8fv9>c8v7c8.&c32r32v6<av8>c8dv6<a8.v7g2&grv8gab+v7bv10a.g8&g32v11gv12ab+v10ba.g8&g32v12gab+v11bv10a.g.g.v10dv12e8.v11ev10dv11ev12gv10edc8<av11av12>cv13av12bv11a8v10g8r8ev12gv11f8v10ev11f8v13b+8v8cv11d4v13gv12ab+v10ba.g8&g32v11gv12ab+v10ba.g8&g32v11gv12ab+v10ba.g.g.v11dv12e8&e32r32ev10degedc8<av11av12>cv14av12bv11a8v10g8r8ev12gv10fev11fv10ev12fv13b+8v8cv11d8&d32r32v9<gv13>e8v12gav9d8l32&drv10c2."
play_mml(mml_data)
print(new_song)
print(new_duration)
print(new_octave)
for num in range(len(new_song)):
    # 将当前数字添加到新数组中
    if(new_octave[num]==6):
        new_song[num]=str(new_song[num])+'1'
    if(new_octave[num]==4):
        new_song[num]=str(new_song[num])+'0'
print(new_song)

for num in range(len(new_song)):
    if(new_song[num]=="do"): new_song[num]=100
print(new_song)
