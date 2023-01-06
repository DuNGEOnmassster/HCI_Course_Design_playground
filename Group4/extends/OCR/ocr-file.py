import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import argparse
import os
import paddlehub as hub
import cv2

argument_parser = argparse.ArgumentParser()
# dataset目录，存放输入图像
argument_parser.add_argument('-d', '--dataset', required=True, help='path of images')
# output目录，输出结果的目录
argument_parser.add_argument('-o', '--output', required=True, help='path of result')
args = vars(argument_parser.parse_args())
path_list = [args['dataset'] + '/' + path for path in os.listdir(args['dataset'])]
print(path_list)

# 加载移动端预训练模型
ocr = hub.Module(name="chinese_ocr_db_crnn_mobile")

np_images =[cv2.imread(image_path) for image_path in path_list]

results = ocr.recognize_text(
                    images=np_images,         # 图片数据，ndarray.shape 为 [H, W, C]，BGR格式；
                    use_gpu=False,            # 是否使用 GPU；若使用GPU，请先设置CUDA_VISIBLE_DEVICES环境变量
                    output_dir=args['output'],  # 图片的保存路径，默认设为 ocr_result；
                    visualization=True,       # 是否将识别结果保存为图片文件；
                    box_thresh=0.5,           # 检测文本框置信度的阈值；
                    text_thresh=0.5)          # 识别中文文本置信度的阈值；

for result in results:
    data = result['data']
    save_path = result['save_path']
    for infomation in data:
        print('text: ', infomation['text'], '\nconfidence: ', infomation['confidence'], '\ntext_box_position: ', infomation['text_box_position'])

