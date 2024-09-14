
from PIL import Image ,ImageDraw, ImageFont

# RGB 值列表
rgb_list = [(255, 255, 255), (200, 212, 227), (160, 167, 157),
            (165, 165, 80), (148, 155, 38), (96, 100, 11),
            (120, 134, 10), (57, 72, 2), (186, 194, 87), (242, 206, 155),
            (148, 99, 46), (210, 200, 109), (177, 195, 53),
            (187, 200, 216), (92, 77, 73), (38, 29, 25), (75, 53, 41),
            (124, 138, 80), (165, 176, 202), (42, 47, 75),
            (122, 135, 162), (223, 182, 140), (156, 113, 75),
            (75, 61, 66), (220, 163, 102), (199, 119, 80),
            (104, 113, 143), (139, 54, 20), (213, 132, 59),
            (202, 119, 51), (182, 193, 178)]  # RGB 值对应红色、绿色和蓝色

numdodo = [0, 2680, 95, 1169, 2289, 2838,
           604, 133, 28, 320, 267, 73, 1,
           1910, 139, 2060, 631, 3, 64, 3,
           4, 128, 161, 11, 475, 10, 3, 3,
           8, 15, 4, 0, 0, 0, 0, 0, 0, 0,
           0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

# 获取图像的宽度和高度
width =  1500
height =  500
print(len(rgb_list))
# 创建一个更大的图像
image = Image.new('RGB', (width, height)) 
for i in range(10):
    for k in range(150):
        for l in range(100):
            
            
            image.putpixel((150*i+k, l), rgb_list[i])
            image.putpixel((150*i+k, l+100), rgb_list[i+10])
            image.putpixel((150*i+k, l+200), rgb_list[i+20])
            try:
                image.putpixel((100*i+k, l+200), rgb_list[i+30])
            except:
                pass

    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype('arial.ttf', 36)

    text = str(numdodo[i])
    draw.text((150*i+28, 20), text, font=font, fill='red')
    text = str(numdodo[i+10])
    draw.text((150*i+28, 120), text, font=font, fill='red')
    text = str(numdodo[i+20])
    draw.text((150*i+28, 220), text, font=font, fill='red')


             
            
            
image.show()
image.save('scaled_image55555.png')
