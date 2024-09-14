from PIL import Image, ImageDraw,ImageFont


def resize_image(input_image_path, output_image_path, size):
    """
    Resize an image to a specific size.
    
    :param input_image_path: Path to the input image file.
    :param output_image_path: Path to save the resized image.
    :param size: The size to resize the image to, in the format (width, height).
    """
    original_image = Image.open(input_image_path)
    width, height = size  # 确保size是一个包含宽度和高度的元组
    resized_image = original_image.resize(size)  # 使用resize方法改变图片尺寸
    resized_image.save(output_image_path)  # 保存改变尺寸后的图片
 
# 使用示例：将图片尺寸改为800x600并保存为新文件
resize_image('me.jpg', 'me2.jpg', (480, 480))




# 把原图跟本代码放在同一目录下
image = Image.open('new_image3.png') # 括号里填图片名称+图片格式
radius = 4  # 波点半径
offset = 2  # 波点间距
output_image = Image.new("RGB", image.size)
draw = ImageDraw.Draw(output_image)



# 假设有一个数值列表 num_list
num_list = [(255,255,255)]
listnum = [0,0,0,0,0,0,0,0,0,0]



# 绘制
for x in range(0, image.width, radius * 2 + offset):
    for y in range(0, image.height, radius * 2 + offset):
        box = (x, y, x + radius * 2, y + radius * 2)
        region = image.crop(box)
        # 获取像素颜色（RGB模式）
        pixel_color = region.getpixel((4, 4))
        for num in num_list:
            if pixel_color not in num_list:
                num_list.append(pixel_color)
        draw.ellipse(box, fill=pixel_color)
# 保存图片&图片格式类型可更改，这里为png
output_image.save('pixelate_output.png')
print(num_list)


def closest_color(rgb_color, colors_to_compare):
    # 假设colors_to_compare是一个包含RGB颜色值的列表
    closest_distance = float('inf')  # 初始化一个无穷大的距离值
    closest_color = None  # 初始化一个None值来存储最接近的颜色
 
    for compare_color in colors_to_compare:
        distance = calculate_distance(rgb_color, compare_color)
        if distance < closest_distance:
            closest_distance = distance
            closest_color = compare_color
 
    return closest_color, closest_distance




def get_pixel_color(image_path, x, y):
    # 打开图像文件
    image = Image.open(image_path)
    # 获取像素颜色（RGB模式）
    pixel_color = image.getpixel((x, y))
    return pixel_color


imageRRR = Image.open('pixelate_output.png')
draw = ImageDraw.Draw(image)
font = ImageFont.truetype('arial.ttf', 12)

for i in range(47):
    for j in range(47):
        color = get_pixel_color('pixelate_output.png', i*10+4, j*10+4)  # 替换为你的图像路径和坐标
        #print(color)  # 输出颜色，格式为RGB元组，例如(255, 0, 0)代表红色
        for index,num in enumerate(num_list):
            if( color == num ):
                listnum[index] = listnum[index]+1
                draw.text((i*10+4, j*10+4), str(index), font=font, fill='red')
                #print(listnum[index])

image.save('allgoood.png')
print(listnum)
