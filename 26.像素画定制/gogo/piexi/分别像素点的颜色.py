
from PIL import Image
 
def get_pixel_color(image_path, x, y):
    # 打开图像文件
    image = Image.open(image_path)
    # 获取像素颜色（RGB模式）
    pixel_color = image.getpixel((x, y))
    return pixel_color


white = 0

for i in range(31):
    for j in range(47):
        color = get_pixel_color('pixelate_output.png', i*10+4, j*10+4)  # 替换为你的图像路径和坐标
        print(color)  # 输出颜色，格式为RGB元组，例如(255, 0, 0)代表红色
        if(color ==(255, 255, 255)):
            #print("白色")
            white = white+1
        
print("白色:"+str(white))
