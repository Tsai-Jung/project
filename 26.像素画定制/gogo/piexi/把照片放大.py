from PIL import Image

# 打开照片文件
image = Image.open('pixelate_output.png')

# 获取原始尺寸
original_width, original_height = image.size

# 放大的倍数
scale_factor = 8

# 计算放大后的尺寸
new_width = original_width * scale_factor
new_height = original_height * scale_factor

# 放大照片
scaled_image = image.resize((new_width, new_height))

# 保存放大后的照片
scaled_image.save('scaled_image3333.png')
