"""
Description: Transform bmp file into c header file.

Author: Cheng Shu
Date: 2021-10-04 12:45:11
LastEditTime: 2021-10-04 14:15:50
LastEditors: Cheng Shu
@Copyright © 2020 Cheng Shu
License: MIT License
"""


# -*- coding: UTF-8 -*-
import os
from struct import unpack

# 读取并存储 bmp 文件
class BMPFileReader :
    def __init__(self, filePath) :
        file = open(filePath, "rb")
        # 读取 bmp 文件的文件头    14 字节
        self.bfType = unpack("<h", file.read(2))[0]       # 0x4d42 对应BM 表示这是Windows支持的位图格式
        self.bfSize = unpack("<i", file.read(4))[0]       # 位图文件大小
        self.bfReserved1 = unpack("<h", file.read(2))[0]  # 保留字段 必须设为 0 
        self.bfReserved2 = unpack("<h", file.read(2))[0]  # 保留字段 必须设为 0 
        self.bfOffBits = unpack("<i", file.read(4))[0]    # 偏移量 从文件头到位图数据需偏移多少字节（位图信息头、调色板长度等不是固定的，这时就需要这个参数了）
        # 读取 bmp 文件的位图信息头 40 字节
        self.biSize = unpack("<i", file.read(4))[0]       # 所需要的字节数
        self.biWidth = unpack("<i", file.read(4))[0]      # 图像的宽度 单位 像素
        self.biHeight = unpack("<i", file.read(4))[0]     # 图像的高度 单位 像素
        self.biPlanes = unpack("<h", file.read(2))[0]     # 说明颜色平面数 总设为 1
        self.biBitCount = unpack("<h", file.read(2))[0]   # 说明比特数
        
        self.biCompression = unpack("<i", file.read(4))[0]  # 图像压缩的数据类型
        self.biSizeImage = unpack("<i", file.read(4))[0]    # 图像大小
        self.biXPelsPerMeter = unpack("<i", file.read(4))[0]# 水平分辨率
        self.biYPelsPerMeter = unpack("<i", file.read(4))[0]# 垂直分辨率
        self.biClrUsed = unpack("<i", file.read(4))[0]      # 实际使用的彩色表中的颜色索引数
        self.biClrImportant = unpack("<i", file.read(4))[0] # 对图像显示有重要影响的颜色索引的数目
        self.bmp_data = []

        # if self.biBitCount != 24 :
        #     print("输入的图片比特值为 ：" + str(self.biBitCount) + "\t 与程序不匹配")

        file.seek(self.bfOffBits)
        for height in range(self.biHeight) :
            bmp_data_row = []
            # 四字节填充位检测
            count = 0
            for width in range(self.biWidth):
                if self.biBitCount == 32:
                    bmp_data_row.append([
                        unpack("<B", file.read(1))[0],
                        unpack("<B", file.read(1))[0],
                        unpack("<B", file.read(1))[0],
                        unpack("<B", file.read(1))[0]])
                else:
                    bmp_data_row.append([
                        unpack("<B", file.read(1))[0],
                        unpack("<B", file.read(1))[0],
                        unpack("<B", file.read(1))[0]])
                count = count + 3
            # bmp 四字节对齐原则
            while count % 4 != 0 :
                file.read(1)
                count = count + 1
            self.bmp_data.append(bmp_data_row)
        self.bmp_data.reverse()
        file.close()
        # R, G, B 三个通道
        self.R = []
        self.G = []
        self.B = []

        for row in range(self.biHeight) :
            R_row = []
            G_row = []
            B_row = []
            for col in range(self.biWidth) :
                B_row.append(self.bmp_data[row][col][0])
                G_row.append(self.bmp_data[row][col][1])
                R_row.append(self.bmp_data[row][col][2])
            self.B.append(B_row)
            self.G.append(G_row)
            self.R.append(R_row)

    def get_bit_arr(self):
        bit_arr = []
        for i in range(self.biHeight):
            row = []
            for j in range(self.biWidth):
                t = self.R[i][j] + self.G[i][j] + self.B[i][j]
                row.append(1 if not (t) else 0)
            bit_arr.append(row)
        return bit_arr
            
            
# br = BMPFileReader("doc/icons/bmp2/400.bmp")

# bits = br.get_bit_arr()
# for line in bits:
#     for bit in line:
#         if bit:
#             print('* ', end='')
#         else:
#             print('  ', end='')
#     print()

def bits2hex(arr):
    ans = []
    for i in range(32):
        data = 0
        for j in range(32):
            t = arr[i][j]
            if j and j % 8 == 0:
                ans.append(data)
                data = 0
            data = (data << 1) | t
        ans.append(data)
    return ans

def get_file_list():
    file_list = os.listdir("doc/icons/bmp2")
    file_list.sort()
    return file_list

def bmp2data(file):
    br = BMPFileReader(file)
    bits = br.get_bit_arr()
    return bits2hex(bits)



img_date = []

fl = get_file_list()
for file in fl[:]:
    res = bmp2data("doc/icons/bmp2" + '/' + file)
    img_date.extend(res)

with open('src/libs/weatherIcons.h', 'w') as f:
    f.write("#ifndef WEATHER_ICON_CDC\n#define WEATHER_ICON_CDC\n\n")
    f.write("const uint8_t weatherIcons[] PROGMEM = {\n")
    for i in range(len(img_date)):
        if i and i % 8 == 0:
            f.write("\n")
        if i % 8 == 0:
            f.write("    ")
        f.write(f"0x{img_date[i]:02x}, ")
    f.write("\n};\n\n#endif")
print("Finished.")
