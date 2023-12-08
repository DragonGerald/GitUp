import tkinter as tk
from PIL import ImageTk,Image
import pyttsx3
import tkinter.messagebox as messagebox
import csv
import cv2
import time
import os
import numpy as np


class FaceAttendanceSystem:
    def __init__(self, window):
        self.engine = pyttsx3.init()
        self.engine.setProperty('rate', 150)  # 设置语速
        self.window = window
        self.window.title("人脸识别考勤系统")
        self.is_capturing = False  # 用于跟踪是否正在进行人脸信息采集

        # 创建一个框架，放置画布和标签
        self.frame = tk.Frame(window)
        self.frame.pack(side=tk.LEFT)

        # 创建画布
        self.canvas = tk.Canvas(self.frame, width=640, height=480)
        self.canvas.pack()

        # 创建标签用于显示检测结果
        self.label1= tk.Label(self.frame, text="未检测到人脸", font=("Arial", 16))
        self.label1.pack(pady=10)

        # 创建一个框架，放置按钮
        self.button_frame = tk.Frame(window)
        self.button_frame.pack(side=tk.RIGHT)

        # 创建按钮用于开始采集人脸信息
        self.start_button = tk.Button(self.button_frame, text="开始采集", command=self.start_face_collection)
        self.start_button.pack(pady=5)

        self.label2 = tk.Label(window, text="请输入需要添加的成员姓名：")
        self.label2.pack()

        self.entry = tk.Entry(window)
        self.entry.pack()

        self.progress_label = tk.Label(window, text="")
        self.progress_label.pack()
        # 创建按钮用于开始签到
        self.check_in_button = tk.Button(self.button_frame, text="开始签到", command=self.check_in)
        self.check_in_button.pack(pady=5)

        # 创建用于显示签到信息的标签
        self.attendance_label = tk.Label(window, text="签到信息：", font=("Arial", 16))
        self.attendance_label.pack(pady=10)

        # 创建按钮用于停止考勤
        self.stop_attendance_button = tk.Button(self.button_frame, text="停止考勤", command=self.stop_attendance)
        self.stop_attendance_button.pack(pady=5)

        # 创建按钮用于显示考勤信息
        self.attendance_info_button = tk.Button(self.button_frame, text="考勤信息", command=self.show_attendance_info)
        self.attendance_info_button.pack(pady=5)

        # 创建一个显示框用于显示考勤信息
        self.attendance_info_frame = tk.Frame(window)
        self.attendance_info_frame.pack(side="bottom", padx=10, pady=10)

        self.attendance_info_text = tk.Text(self.attendance_info_frame, font=("Arial", 12), height=10, width=40)
        self.attendance_info_text.pack()

        self.attendance_data = [["name", "time"]]  # 列表中的第一个元素是表头


        # 打开摄像头
        self.video_capture = cv2.VideoCapture(0)

        # 开始显示摄像头画面
        self.show_frame()

        # 初始化语音提示引擎
        self.engine = pyttsx3.init()

    def start_face_collection(self):
        # 释放摄像头资源
        self.video_capture.release()

        # 由用户输入文件夹名称
        folder_name = self.entry.get()
        if not folder_name:
            messagebox.showerror("错误", "请输入需要添加的成员姓名")
            self.engine.say("错误！请输入需要添加的成员姓名")
            self.engine.runAndWait()
            return # 结束当前函数的执行
        else:
            self.engine.say("准备开始采集人脸数据，请在采集过程中缓慢摇头以便收集信息")
            self.engine.runAndWait()

        # 创建保存图片的文件夹
        folder_path = os.path.join('data1', folder_name)
        if not os.path.exists(folder_path):
            os.makedirs(folder_path)

        # 打开摄像头
        cap = cv2.VideoCapture(0)

        # 设置人脸识别器
        face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

        count = 0
        last_capture_time = time.time() - 0.2
        capture_interval = 0.2
        max_photos = 20

        while True:
            ret, frame = cap.read()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

            for (x, y, w, h) in faces:
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

                current_time = time.time()
                if current_time - last_capture_time >= capture_interval and count < max_photos:
                    face_img = gray[y:y + h, x:x + w]
                    file_name = f'{folder_path}/face{count}.pgm'
                    cv2.imwrite(file_name, face_img)

                    count += 1
                    last_capture_time = current_time

            cv2.imshow('Face_Collection', frame)

            if cv2.waitKey(1) & 0xFF == ord('c') and count < max_photos:
                face_img = gray[y:y + h, x:x + w]
                file_name = f'{folder_path}/face_{count}.pgm'
                cv2.imwrite(file_name, face_img)

                count += 1

            if count >= max_photos:
                break

        # 播放提示音
        self.engine.say("照片信息已采集")
        self.engine.runAndWait()
        cap.release()
        cv2.destroyAllWindows()

    def stop_capture(self):
        # 打开摄像头
        self.video_capture = cv2.VideoCapture(0)
        if self.is_capturing:
            self.is_capturing = True
            self.label1.config(text="未检测到人脸")

    def show_frame(self):
        # 读取摄像头画面
        ret, frame = self.video_capture.read()
        if not ret:
            # 如果未能读取到图像帧，则直接返回
            return
        # 将画面转换为RGB格式
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # 检测人脸
        face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
        faces = face_cascade.detectMultiScale(frame_rgb, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        # 如果检测到人脸
        if len(faces) > 0:
            # 在画面上绘制人脸框
            for (x, y, w, h) in faces:
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

            # 更新标签显示
            self.label1.config(text="检测到人脸")

        # 如果未检测到人脸
        else:
            # 更新标签显示
            self.label1.config(text="未检测到人脸")

        # 将OpenCV画面转换为PIL图像
        image = Image.fromarray(frame)

        # 缩放图像以适应画布大小
        image = image.resize((640, 480), Image.ANTIALIAS)

        # 在画布上显示图像
        self.photo = ImageTk.PhotoImage(image=image)
        self.canvas.create_image(0, 0, image=self.photo, anchor=tk.NW)

        # 每隔10毫秒更新画面
        self.window.after(10, self.show_frame)

    def quit(self):
        # 释放摄像头资源
        self.video_capture.release()
        self.window.destroy()

    def check_in(self):
        # 释放摄像头资源
        self.video_capture.release()

        # 加载训练集数据
        faces_dir = 'data1'  # 存储人脸照片的文件夹
        labels = []  # 存储标签的数组
        faces = []  # 存储人脸图像的数组

        label_dict = {}  # 存储文件夹名称和标签的映射字典
        label_count = 0  # 标签计数器

        # 遍历data1文件夹及其子文件夹，读取图像和标签
        for root, dirs, files in os.walk(faces_dir):
            for dir in dirs:
                dir_path = os.path.join(root, dir)
                if dir not in label_dict:
                    label_dict[dir] = label_count
                    label_count += 1
                label = label_dict[dir]  # 使用映射字典获取标签
                for filename in os.listdir(dir_path):
                    if filename.endswith('.pgm'):
                        img_path = os.path.join(dir_path, filename)
                        image = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)  # 以灰度图像方式读取图像
                        faces.append(image)
                        labels.append(label)

        # 创建LBPH人脸识别器
        face_recognizer = cv2.face.LBPHFaceRecognizer_create()

        # 训练模型
        face_recognizer.train(faces, np.array(labels))

        # 保存训练结果
        face_recognizer.save('face_recognizer.xml')

        # 加载训练好的LBPH人脸识别器
        face_recognizer = cv2.face.LBPHFaceRecognizer_create()
        face_recognizer.read('face_recognizer.xml')

        # 加载人脸检测器
        face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

        # 打开摄像头
        cap = cv2.VideoCapture(0)

        while True:
            # 读取一帧图像
            ret, frame = cap.read()

            # 将图像转换为灰度图像
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # 检测人脸
            faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30),
                                                  flags=cv2.CASCADE_SCALE_IMAGE)

            # 遍历每个检测到的人脸
            for (x, y, w, h) in faces:
                # 提取人脸区域
                face_roi = gray[y:y + h, x:x + w]

                # 进行人脸识别
                label, confidence = face_recognizer.predict(face_roi)
                if confidence > 70:
                    text = 'Unknown'
                else:
                    for folder, label_id in label_dict.items():
                        if label_id == label:
                            text = folder
                            break

                # 设置颜色
                if text == 'Unknown':
                    color = (0, 0, 255)  # 设置颜色为红色
                else:
                    color = (0, 255, 0)  # 设置颜色为绿色

                # 在图像上绘制边框和标签
                cv2.rectangle(frame, (x, y), (x + w, y + h), color, 2)
                cv2.putText(frame, text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, color, 2)

            # 显示图像
            cv2.imshow('Face_Recognition', frame)

            # 检查键盘输入,按下 'q' 键退出循环
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        # 释放摄像头并关闭窗口
        cap.release()
        cv2.destroyAllWindows()

        # 弹出签到成功提示窗口
        self.speak("签到成功")
        self.show_check_in_success_message()

        # 获取当前时间
        current_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())

        # 弹出签名窗口
        self.speak("请签名：")
        self.show_signature_window(current_time,text)


    def show_check_in_success_message(self):
        messagebox.showinfo("签到成功", "签到成功！")

    def show_signature_window(self, current_time,text):
        # 创建签名窗口
        signature_window = tk.Toplevel(self.window)
        signature_window.title("签名")

        # 创建签名标签
        signature_label = tk.Label(signature_window, text="请在下方输入您的名字")
        signature_label.pack(pady=10)

        # 创建签名输入框
        signature_entry = tk.Entry(signature_window)
        signature_entry.pack(pady=5)

        # 创建确认按钮
        confirm_button = tk.Button(signature_window, text="确认",
                                   command=lambda: self.check_signature(signature_entry.get(), text, current_time,signature_window))
        confirm_button.pack(pady=5)

    def check_signature(self, signature, text, current_time, signature_window):
        if signature == text:
            self.save_signature(signature, current_time, signature_window)
            signature_window.destroy()
        else:
            messagebox.showerror("错误", "输入的名字与签到时的名字不一致，请重新输入！")

    def save_signature(self, name, current_time, signature_window):
        # 创建签名成功提示窗口
        messagebox.showinfo("签名成功", "签名成功！")

        # 关闭签名窗口
        signature_window.destroy()

        # 在控制台打印签到者的名字和签到时间
        print("签到者:", name)
        print("签到时间:", current_time)

        # 更新界面的签到信息标签
        attendance_info = f"签到者：{name}\n签到时间：{current_time}"
        self.attendance_label.config(text=attendance_info)

        # 将签到者和签到时间添加到二维列表中
        self.attendance_data.append([name, current_time])

    def speak(self, message):
        self.engine.say(message)
        self.engine.runAndWait()

    def save_attendance_data(self):
        with open('attendance.csv', mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerows(self.attendance_data)

    def show_attendance_info(self):
        # 保存签到数据
        self.save_attendance_data()
        # 读取保存在CSV表格中的考勤数据
        with open('attendance.csv', mode='r') as file:
            reader = csv.reader(file)
            attendance_data = list(reader)

        # 清空考勤信息文本框
        self.attendance_info_text.delete(1.0, tk.END)

        # 将考勤数据显示在考勤信息文本框中
        for row in attendance_data:
            self.attendance_info_text.insert(tk.END, f"姓名：{row[0]}   时间：{row[1]}\n")

    def stop_attendance(self):
        # 停止摄像头读取
        self.stop_capture()
        # 关闭窗口
        self.window.destroy()
        # 退出程序
        exit()

window = tk.Tk()
app = FaceAttendanceSystem(window)
window.mainloop()
