from PIL import Image, ImageDraw, ImageFont

 


# 打开图片文件
image = Image.open('scaled_image3333.png')
# 创建绘图对象
draw = ImageDraw.Draw(image)
# 加载字体
font = ImageFont.truetype('arial.ttf', 36)  # 替换为你的字体文件和字号
# 要添加的数字
text = '3'
# 添加数字到图片上
draw.text((28, 20), text, font=font, fill='red')  # 设置添加数字的位置、内容、字体和颜色
# 保存修改后的图片
image.save('scaled_image55555.png')
