import streamlit as st
from scipy.spatial.distance import cdist

import numpy as np
from PIL import Image


rootPath = "./images/"


@st.cache_data
def read_data():
    fileNames = np.load("fileNames.npy")
    featureVectors = np.load("featureVectors.npy") 
    return fileNames, featureVectors


fileNames, featureVectors = read_data()


pictureName, picture, _ = st.columns(3)  # 图片展示在第 1 行的第 2 列
buttonCol1, buttonCol2 = st.columns(2)  # 第 2 行的两列展示两个按钮
buttonRandom, buttonSearch = buttonCol1.button("Random"), buttonCol2.button("Search")


# 随机一张查询图片
if buttonRandom:
    fileName = fileNames[np.random.randint(len(fileNames))]
    picture.image(Image.open(rootPath + fileName))
    st.session_state["display_image"] = fileName
    pictureName.header(st.session_state["display_image"])


# 检索前 3 个相似图片
if buttonSearch:
    # 注意点击按钮后会刷新, 需重新显示
    picture.image(Image.open(rootPath + st.session_state["display_image"]))
    pictureName.header(st.session_state["display_image"])
    
    # 当前选中的图片对应的下标和特征向量
    targetIndex = int(np.argwhere(fileNames == st.session_state["display_image"]))
    targetFeatureVector = featureVectors[targetIndex]
    
    # 按特征向量与 targetFeatureVector 的 Euclid 距离排序,相似度前 3 高的图片
    top3 = cdist(targetFeatureVector[None , ...] , featureVectors).squeeze().argsort()[1:4]  # 原图片在下标 0 处
    
    # 展示相似度前 3 高的图片, 用两行展示结果
    # 第 1 行展示检索结果的文件名
    pictureNameCol1, pictureNameCol2, pictureNameCol3 = st.columns(3)  # 用 3 列展示文件名
    pictureNameCol1.header(fileNames[top3[0]])
    pictureNameCol2.header(fileNames[top3[1]])
    pictureNameCol3.header(fileNames[top3[2]])
    # 第 2 行展示检索结果的图片
    pictureCol1, pictureCol2, pictureCol3 = st.columns(3)  # 用 3 列展示图片
    pictureCol1.image(Image.open(rootPath + fileNames[top3[0]]))
    pictureCol2.image(Image.open(rootPath + fileNames[top3[1]]))
    pictureCol3.image(Image.open(rootPath + fileNames[top3[2]]))