import os

CMD = r'D:\ImageMagick-7.0.8-Q16\magick.exe'   #ImageMagick安装目录下convert.exe所在目录
SOURCE_PATH = r'E:\Qt_PRJ\goodPrj\SuperGame\img\chineseChess'                          #png图片所在目录

def doStrip(path):
  data = {};
  print(path)
  for root, dirs, files in os.walk(path):
    for file in files:
      name = file.lower();
      if name.find('.png') != -1:
        path = os.path.join(root, file)
        os.system('"{0}" {1} -strip {1}'.format(CMD, path, path));


doStrip(SOURCE_PATH)