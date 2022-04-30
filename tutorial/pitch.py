import wave
import struct
import numpy as np
import matplotlib.pyplot as plt

voice_data = './01.male.wav'
wr = wave.open(voice_data, 'r')

frame_rate = wr.getframerate()
frames = wr.getnframes()
width = wr.getsampwidth()
ch = wr.getnchannels()
buf = wr.readframes(frames)

data = np.frombuffer(buf, dtype='int16')
data = data / 32768.0

# fft
c = np.fft.fft(data[0:frame_rate*3])
c = abs(c)

shift = 30 # ここを動かすと影響度が変わる
moved = np.zeros((shift))
temp = c # cをそのまま使うと編集されてしまうかもしれないので一旦コピーする
tempwave = np.concatenate((temp[:2], moved, temp[2:]), 0)
    # 低周波数を動かすとハウリングするしいので

# plt.subplot(2,1,1)
# plt.title('freq1')
# plt.plot(c, linestyle='-')
# plt.subplot(2,1,2)
# plt.title('freq2')
# plt.plot(tempwave, linestyle='-')
# plt.tight_layout()
# plt.show()

# ifft
half = tempwave[:frame_rate*3//2]
tempi = temp[::-1]
#tempi.imag = -temp.imag[::-1]
processed = np.concatenate([temp, tempi], 0)
invF = np.fft.ifft(processed).real
resWave = np.reshape(invF, (-1)).astype(np.int16)

outd = struct.pack("h"*len(resWave), *resWave)
outf = './test.wav'
ww = wave.open(outf, 'w')
ww.setnchannels(ch)
ww.setframerate(frame_rate)
ww.setsampwidth(width)
ww.writeframes(outd)
ww.close()
