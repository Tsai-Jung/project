from PIL import Image, ImageDraw  ,ImageStat


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
resize_image('me.jpg', 'me2.jpg', (320, 480))


# 把原图跟本代码放在同一目录下
image = Image.open('me2.jpg') # 括号里填图片名称+图片格式
radius = 4  # 波点半径
offset = 2  # 波点间距
output_image = Image.new("RGB", image.size)
draw = ImageDraw.Draw(output_image)
# 绘制
for x in range(0, image.width, radius * 2 + offset):
    for y in range(0, image.height, radius * 2 + offset):
        box = (x, y, x + radius * 2, y + radius * 2)
        region = image.crop(box)
        stat = ImageStat.Stat(region)
        average_color = tuple(int(c) for c in stat.median)
        draw.ellipse(box, fill=average_color)
# 保存图片&图片格式类型可更改，这里为png
output_image.save('pixelate_output.png')
