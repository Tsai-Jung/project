from PIL import Image, ImageDraw  ,ImageStat ,ImageFont


def get_pixel_color(image_path, x, y):
    # 打开图像文件
    image = Image.open(image_path)
    # 获取像素颜色（RGB模式）
    pixel_color = image.getpixel((x, y))
    return pixel_color



def resize_image(input_image_path, output_image_path, size):
    original_image = Image.open(input_image_path)
    width, height = size  # 确保size是一个包含宽度和高度的元组
    resized_image = original_image.resize(size)  # 使用resize方法改变图片尺寸
    resized_image.save(output_image_path)  # 保存改变尺寸后的图片


def reduce_colors(image_path, output_path, num_colors=7):
    # 读取图像
    img = Image.open(image_path)
    
    # 使用quantize方法减少颜色数量到指定的数量（在这个例子中是9）
    # 可以调整palette参数来选择不同的调色板算法，例如'web'或'median-cut'等。
    img_reduced = img.quantize(colors=num_colors, palette=Image.WEB)
    img_reduced = img_reduced.convert('RGB')
    # 保存转换后的图像（可选）
    img_reduced.save(output_path)
    
    # 如果需要，也可以直接显示转换后的图像
    # img_reduced.show()

def split_image_into_nine_parts(image_path,widX,heiY):
    # 打开图片
    image = Image.open(image_path)
    width, height = image.size
    
    # 计算每个部分的尺寸
    part_width = width // widX
    part_height = height // heiY
    
    # 创建一个列表来存储分割后的部分
    parts = []
    
    # 从左上角开始分割图片
    for j in range(heiY):
        for i in range(widX):
            # 计算当前部分的起始和结束位置
            left = i * part_width
            top = j * part_height
            right = (i + 1) * part_width
            bottom = (j + 1) * part_height
            
            # 裁剪图片并添加到列表中
            part = image.crop((left, top, right, bottom))
            parts.append(part)
    
    return parts


#开始
me = input("放进图片")
XX = int(input("宽像素块"))
YY = int(input("长像素块"))

# 使用示例：将图片尺寸改为800x600并保存为新文件
resize_image(me, 'me2.png', (XX*160,YY*160))

# 使用函数转换图像并保存结果
reduce_colors('me2.png', 'me3.png', num_colors=9)


# 把原图跟本代码放在同一目录下
image = Image.open('me3.png') # 括号里填图片名称+图片格式
radius = 4  # 波点半径
offset = 2  # 波点间距
output_image = Image.new("RGB", image.size)
draw = ImageDraw.Draw(output_image)
# 绘制

# 假设有一个数值列表 num_list
num_list = [(255,255,255)]
listnum = [0,0,0,0,0,0,0,0,0,0]

for x in range(0, image.width, radius * 2 + offset):
    for y in range(0, image.height, radius * 2 + offset):
        box = (x, y, x + radius * 2, y + radius * 2)
        region = image.crop(box)
        pixel_color = region.getpixel((4, 4))
        for num in num_list:
            if pixel_color not in num_list:
                num_list.append(pixel_color)
        draw.ellipse(box, fill=pixel_color)
# 保存图片&图片格式类型可更改，这里为png
output_image.save('pixelate_output.png')
print(num_list)

for i in range(XX*16-1):
    for j in range(YY*16-1):
        color = get_pixel_color('pixelate_output.png', i*10+4, j*10+4)  # 替换为你的图像路径和坐标
        #print(color)  # 输出颜色，格式为RGB元组，例如(255, 0, 0)代表红色
        for index,num in enumerate(num_list):
            if( color == num ):
                listnum[index] = listnum[index]+1
                #print(listnum[index])
        
print(listnum)



# 使用函数分割图片并保存结果（可选）
image_path = 'pixelate_output.png'  # 替换为你的图片路径
parts = split_image_into_nine_parts(image_path,XX,YY)
for i, part in enumerate(parts):
    part.save(f'part_{i}.png')  # 保存每个部分为单独的文件

for i, part in enumerate(parts):
    image = Image.open(f'part_{i}.png')
    original_width, original_height = image.size
    scale_factor = 8
    new_width = original_width * scale_factor
    new_height = original_height * scale_factor
    scaled_image = image.resize((new_width, new_height))
    
    draw = ImageDraw.Draw(scaled_image)
    font = ImageFont.truetype('arial.ttf', 36)
    for k in range(16):
        for j in range(16):
            color = scaled_image.getpixel((k*80+40, j*80+40))
            #print(color)  # 输出颜色，格式为RGB元组，例如(255, 0, 0)代表红色
            for index,num in enumerate(num_list):
                if( color == num ):
                    listnum[index] = listnum[index]+1
                    draw.text((k*80+28, j*80+18), str(index), font=font, fill='red')
                    #print(listnum[index])
    scaled_image.save(f'Big/BigPart_{i}.png')
print(listnum)





