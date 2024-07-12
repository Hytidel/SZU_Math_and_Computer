using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using Unity.VisualScripting;

public class Logic_Basket : MonoBehaviour {
    [Header("Set in Inspector")]
    public float xRange = 20.0f;
    GameObject soundEffectObject;
    GameObject appleTreeObject;

    void Start() {
        soundEffectObject = GameObject.Find("SoundEffectLogicObject");
        appleTreeObject = GameObject.Find("AppleTree");
    }

    void Update() {
        Vector3 mousePosition2D = Input.mousePosition;  // 鼠标的屏幕坐标

        // 摄像机的 z 坐标决定在三维空间中将鼠标沿 z 轴向前移动多远
        mousePosition2D.z = -Camera.main.transform.position.z;

        // 将该点转化为三维的世界坐标
        Vector3 mousePosition3D = Camera.main.ScreenToWorldPoint(mousePosition2D);

        // 修改篮筐的 x 坐标
        Vector3 position = this.transform.position;
        position.x = mousePosition3D.x;
        position.x = Mathf.Max(position.x, -xRange);
        position.x = Mathf.Min(position.x, xRange);
        this.transform.position = position;
    }

    void OnCollisionEnter(Collision collision) {
        // 若碰撞的是苹果, 则销毁苹果
        GameObject collidedWith = collision.gameObject;
        if (collidedWith.tag == "Apple") {
            Destroy(collidedWith);

            // 得 1 分
            Logic_Score.UpdateCurrentScore(Logic_Score.GetCurrentScore() + 1);

            // 播放音效
            soundEffectObject.GetComponent<AudioSource>().Play();

            // 苹果掉落加速
            float timeInterval2DropApples = appleTreeObject.GetComponent<Logic_AppleTree>().timeInterval2DropApples;
            appleTreeObject.GetComponent<Logic_AppleTree>().timeInterval2DropApples = Mathf.Max(timeInterval2DropApples - 0.01f, 0.2f);
        }
    }
}
