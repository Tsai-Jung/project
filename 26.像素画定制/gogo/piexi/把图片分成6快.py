from PIL import Image
 
def split_image_into_nine_parts(image_path):
    # 打开图片
    image = Image.open(image_path)
    width, height = image.size
    
    # 计算每个部分的尺寸
    part_width = width // 2
    part_height = height // 3
    
    # 创建一个列表来存储分割后的部分
    parts = []
    
    # 从左上角开始分割图片
    for i in range(2):
        for j in range(3):
            # 计算当前部分的起始和结束位置
            left = j * part_width
            top = i * part_height
            right = (j + 1) * part_width
            bottom = (i + 1) * part_height
            
            # 裁剪图片并添加到列表中
            part = image.crop((left, top, right, bottom))
            parts.append(part)
    
    return parts
 
# 使用函数分割图片并保存结果（可选）
image_path = 'pixelate_output.png'  # 替换为你的图片路径
parts = split_image_into_nine_parts(image_path)
for i, part in enumerate(parts):
    part.save(f'part_{i}.jpg')  # 保存每个部分为单独的文件
