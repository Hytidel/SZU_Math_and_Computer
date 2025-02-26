import json
import os
import PyQt5
import numpy as np
import cv2
import time
import torch
from GUI.Utils.Logger import Logger
import threading
import queue
import net
import torch
import os
from PIL import Image
from face_alignment import align

lock = threading.Lock()

adaface_models = {
    'ir_50': "resource/adaface_ir50_ms1mv2.ckpt",
}


def to_input(pil_rgb_image):
    np_img = np.array(pil_rgb_image)
    brg_img = ((np_img[:, :, ::-1] / 255.) - 0.5) / 0.5
    tensor = torch.tensor([brg_img.transpose(2, 0, 1)]).float()
    return tensor


class FaceDetection:
    def __init__(self):
        self.cap = None
        self.haar_face_cascade = cv2.CascadeClassifier("resource/haarcascade_frontalface_alt.xml")
        self.alive = False
        self.recognizeAlive = False
        self.lastFrame = None
        self.lastFrameTime = 0
        self.logger = Logger()
        self.UISignals = None
        self.frameUpdated = False
        self.showFrameQueue = queue.Queue()
        self.FaceRectQueue = queue.Queue()
        self.recognizedQueue = queue.Queue()
        self.model = None
        self.face_feature = None
        self.face_name = None
        self.face_info = None
        self.init_model = {
            "finish": False,
            "fail": False
        }
        self.init_cap = {
            "finish": False,
            "fail": False
        }

    def load_pretrained_model(self, architecture='ir_50'):
        # load model and pretrained statedict
        if self.model is not None:
            return True
        try:
            self.logger.info(f'Loading Pretrained Model -- {architecture} -- from ' + adaface_models[architecture])
            if not os.path.exists(adaface_models[architecture]):
                self.logger.error('Pretrained Model Not Found Please Check Resource Folder')
                self.init_model['finish'] = True
                self.init_model['fail'] = True
                return False
            assert architecture in adaface_models.keys()
            self.model = net.build_model(architecture)
            statedict = torch.load(adaface_models[architecture])['state_dict']
            model_statedict = {key[6:]: val for key, val in statedict.items() if key.startswith('model.')}
            self.model.load_state_dict(model_statedict)
            self.model.eval()
            self.init_model['finish'] = True
            self.logger.info(f'Pretrained Model Loaded Successfully')
        except Exception as e:
            self.init_model['finish'] = True
            self.init_model['fail'] = True
            self.logger.error("Load Model Error : " + str(e))

    def loadFaces(self):
        if self.face_info is None:
            with open('Config/faceConfig.json', 'r') as f:
                self.face_info = json.load(f)['faces']

    def initCamera(self):
        if self.cap is None:
            self.cap = cv2.VideoCapture(0)
        if not self.cap.isOpened():
            self.logger.error('Video Capture Not Opened, Please Check Camera Connection')
            self.init_cap['fail'] = True
        else:
            self.logger.info('Camera Init Successfully')
        self.init_cap['finish'] = True
        return True

    def waitCameraInit(self):
        self.logger.info('Waiting for Camera Initialization')
        while not self.init_cap['finish']:
            time.sleep(0.01)

    def waitModelInit(self):
        self.logger.info('Waiting for Model Initialization')
        while not self.init_model['finish']:
            time.sleep(0.01)

    def getFeature(self, img):
        rgb_pil_image = Image.fromarray(img)
        aligned_rgb_img = align.get_aligned_face(None, rgb_pil_image)
        if aligned_rgb_img is None:
            return None
        bgr_tensor_input = to_input(aligned_rgb_img)
        feature, _ = self.model(bgr_tensor_input)
        return feature

    def saveFaces(self):
        with open('Config/faceConfig.json', 'w') as f:
            json.dump({'faces': self.face_info}, f, indent=4)

    def takeSnapshotAndAdd(self, name="Person"):
        self.logger.info('Name : [ ' + name + ' ]')
        self.initCamera()
        if not self.init_cap['finish']:
            self.waitCameraInit()
        self.logger.info('Taking Snapshot After 3 Seconds')
        for i in range(3):
            self.logger.info(f'Countdown : {3 - i}')
            time.sleep(1)
        face = self.lastFrame
        self.recognizedQueue.put(face)
        self.UISignals.update_signal.emit('r')
        if not self.init_model['finish']:
            self.waitModelInit()
        feature = self.getFeature(face)
        if feature is not None:
            impath = f"resource/Faces/{name}.png"
            cv2.imwrite(impath, face)
            self.face_info.append({'name': name, 'featureTensor': feature.tolist(), 'imgPath': impath})
            self.saveFaces()
            self.logger.info('Face Added Successfully : [ ' + name + ' ] image saved at : ' + impath)
        else:
            self.logger.error('No Face Detected in Snapshot')


    def addFace(self, info):
        self.face_info.append(info)
        self.saveFaces()

    def removeFace(self, index):
        self.face_info.pop(index)
        self.saveFaces()

    def loadFaceInfo(self):
        self.face_name = []
        self.face_feature = []
        if self.face_info is None:
            self.loadFaces()
        for face in self.face_info:
            self.face_name.append(face['name'])
            self.face_feature.append(torch.tensor(face['featureTensor']))

    def getFaceInfo(self):
        self.loadFaceInfo()
        return self.face_info

    def getLastFrame(self):
        if self.showFrameQueue.empty():
            return None
        with lock:
            faces = self.FaceRectQueue.get()
            image = self.showFrameQueue.get()
            if len(faces) > 0:
                for (x, y, w, h) in faces:
                    cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
            return image

    def getRecognized(self):
        if self.recognizedQueue.empty():
            return None
        return self.recognizedQueue.get()

    def getMostSimilar(self, feature):
        similarity_scores = torch.cat(self.face_feature) @ feature.T  # 计算相似度得分
        maxIndex = torch.argmax(similarity_scores)
        return similarity_scores[maxIndex], maxIndex

    def recognizeThread(self):
        self.waitModelInit()
        if self.init_model['fail']:
            self.logger.error('Init Model Failed')
            return
        if self.face_info is None:
            self.loadFaceInfo()
        self.recognizeAlive = True
        self.logger.info('--------------------------')
        self.logger.info('|Recognize Thread Started|')
        self.logger.info('--------------------------')
        while self.recognizeAlive:
            if self.FaceRectQueue.empty() or len(self.FaceRectQueue.queue[0]) <= 0:
                time.sleep(0.01)
                continue
            with lock:
                face = self.showFrameQueue.queue[0]
            feature = self.getFeature(face)
            if feature is None:
                continue
            similarity, index = self.getMostSimilar(feature)
            if similarity > 0.6:
                self.logger.info(f'Recognized \t: [ {self.face_name[index]} ] ')
                self.logger.info(f'Similarity \t: [ {similarity[0]} ]')
                self.recognizedQueue.put(face)

    def checkNameExist(self, name):
        for face in self.face_info:
            if face['name'] == name:
                return True
        return False

    def stop(self):
        self.alive = False

    def setUISignals(self, UI):
        self.UISignals = UI
        self.logger.setLoggerSignal(UI.logger_signal)

    def start(self):
        if not self.run():
            self.UISignals.button_signal.emit('START')

    def videoCap(self):
        self.logger.info('-------------------------')
        self.logger.info('| Video Capture Started |')
        self.logger.info('-------------------------')
        while self.alive:
            ret, self.lastFrame = self.cap.read()
            self.lastFrameTime = time.time()
            if not ret:
                self.logger.error('Video Capture Ended')
                return False
            gray = cv2.cvtColor(self.lastFrame, cv2.COLOR_BGR2GRAY)
            hist = cv2.equalizeHist(gray)
            faces = self.haar_face_cascade.detectMultiScale(hist, scaleFactor=1.1, minNeighbors=5)
            with lock:
                self.FaceRectQueue.put(faces)
                self.showFrameQueue.put(self.lastFrame)
        self.alive = False
        self.recognizeAlive = False
        self.cap.release()
        self.cap = None
        cv2.destroyAllWindows()
        self.logger.info('Face Detection Stopped Reason : Human Intervention')
        return True

    def run(self):
        self.showFrameQueue.queue.clear()
        self.FaceRectQueue.queue.clear()
        self.recognizedQueue.queue.clear()
        if self.cap is None:
            self.initCamera()
        if self.init_cap['fail']:
            self.logger.error('Init Camera Failed')
            self.alive = False
            self.recognizeAlive = False
            return False
        self.videoCap()
        return True

    def addFaceFromDir(self, dir):
        self.logger.info("------------------------------------------------------")
        self.logger.info("Adding Faces from Directory : [" + dir + "]")
        for fname in sorted(os.listdir(dir)):
            path = os.path.join(dir, fname)
            aligned_rgb_img = align.get_aligned_face(path)
            if aligned_rgb_img is None:
                self.logger.error(f'No Face Detected in image : [ {path} ]')
                continue
            bgr_tensor_input = to_input(aligned_rgb_img)
            self.logger.info("Extracting Feature from Image : [" + path + "]")
            feature, _ = self.model(bgr_tensor_input)
            self.face_info.append({'name': fname.split('.')[0], 'featureTensor': feature.tolist(), 'imgPath': path})
        self.saveFaces()
        self.logger.info("Faces Added Successfully : ")
        self.logger.info("------------------------------------------------------")

    def deleteFace(self, name):
        for i, face in enumerate(self.face_info):
            if face['name'] == name:
                image_path = face['imgPath']
                if os.path.exists(image_path):
                    os.remove(image_path)
                self.face_info.pop(i)
                self.saveFaces()
                self.logger.info('Face Deleted Successfully : [ ' + name + ' ]')
                return True
        self.logger.error('Face Not Found : [ ' + name + ' ]')
        return False