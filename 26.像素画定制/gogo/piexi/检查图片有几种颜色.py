from PIL import Image
 
def count_colors_in_image(image_path):
    # 打开图片
    img = Image.open(image_path)
    
    # 将图片转换为RGB模式（如果需要的话）
    #img = img.convert('RGB')
    r, g, b = img.split()
    print(r.histogram())
    print(g.histogram())
    #用histogram方法获取颜色直方图，其中每个值代表一个颜色通道的像素数量
    histogram = img.histogram()
    print(set(histogram))
    # 计算不同颜色的数量（这里假设颜色直方图中的每个值都是唯一的）
    unique_colors = len(set(histogram))
    
    return unique_colors
 
# 使用函数检查图片中的颜色数量
image_path = 'pixelate_output.png'  # 替换为你的图片路径
print(f"The image contains {count_colors_in_image(image_path)} different colors.")
