from PIL import Image,ImageEnhance ,ImageFilter
 
def reduce_colors(image_path, output_path, num_colors=9):
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
 
# 使用函数转换图像并保存结果
reduce_colors('me2.jpg', 'new_image.png', num_colors=5)
reduce_colors('new_image.png', 'new_image2.png', num_colors=5)
reduce_colors('new_image2.png', 'new_image3.png', num_colors=5)

